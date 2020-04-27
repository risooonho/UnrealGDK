// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#include "EngineClasses/SpatialVirtualWorkerTranslationManager.h"

#include "EngineClasses/SpatialVirtualWorkerTranslator.h"
#include "Interop/Connection/SpatialWorkerConnection.h"
#include "Interop/SpatialOSDispatcherInterface.h"
#include "SpatialConstants.h"
#include "Utils/EntityFactory.h"
#include "Utils/SchemaUtils.h"

DEFINE_LOG_CATEGORY(LogSpatialVirtualWorkerTranslationManager);

SpatialVirtualWorkerTranslationManager::SpatialVirtualWorkerTranslationManager(
	SpatialOSDispatcherInterface* InReceiver,
	SpatialOSWorkerInterface* InConnection,
	SpatialVirtualWorkerTranslator* InTranslator)
	: Receiver(InReceiver)
	, Connection(InConnection)
	, Translator(InTranslator)
	, bWorkerEntityQueryInFlight(false)
{}

void SpatialVirtualWorkerTranslationManager::AddVirtualWorkerIds(const TSet<VirtualWorkerId>& InVirtualWorkerIds)
{
	VirtualWorkersToAssign = InVirtualWorkerIds.Array();
}

void SpatialVirtualWorkerTranslationManager::AuthorityChanged(const Worker_AuthorityChangeOp& AuthOp)
{
	check(AuthOp.component_id == SpatialConstants::VIRTUAL_WORKER_TRANSLATION_COMPONENT_ID);

	const bool bAuthoritative = AuthOp.authority == WORKER_AUTHORITY_AUTHORITATIVE;

	if (!bAuthoritative)
	{
		UE_LOG(LogSpatialVirtualWorkerTranslationManager, Error, TEXT("Lost authority over the translation mapping. This is not supported."));
		return;
	}

	UE_LOG(LogSpatialVirtualWorkerTranslationManager, Log, TEXT("This worker now has authority over the VirtualWorker translation."));

	SpawnPartitionEntitiesForVirtualWorkerIds();
}

void SpatialVirtualWorkerTranslationManager::SpawnPartitionEntitiesForVirtualWorkerIds()
{
	for (VirtualWorkerId VirtualWorkerId : VirtualWorkersToAssign)
	{
		SpawnPartitionEntity(VirtualWorkerId);
	}
}

// For each entry in the map, write a VirtualWorkerMapping type object to the Schema object.
void SpatialVirtualWorkerTranslationManager::WriteMappingToSchema(Schema_Object* Object) const
{
	for (const auto& Entry : VirtualToPhysicalWorkerMapping)
	{
		Schema_Object* EntryObject = Schema_AddObject(Object, SpatialConstants::VIRTUAL_WORKER_TRANSLATION_MAPPING_ID);
		Schema_AddUint32(EntryObject, SpatialConstants::MAPPING_VIRTUAL_WORKER_ID, Entry.Key);
		SpatialGDK::AddStringToSchema(EntryObject, SpatialConstants::MAPPING_PHYSICAL_WORKER_NAME_ID, Entry.Value.WorkerName);
		Schema_AddEntityId(EntryObject, SpatialConstants::MAPPING_SERVER_WORKER_ENTITY_ID, Entry.Value.ServerWorkerEntityId);
		Schema_AddEntityId(EntryObject, SpatialConstants::MAPPING_PARTITION_ID, Entry.Value.PartitionEntityId);
	}
}

// This method is called on the worker who is authoritative over the translation mapping. Based on the results of the
// system entity query, assign the VirtualWorkerIds to the workers represented by the system entities.
bool SpatialVirtualWorkerTranslationManager::AllServerWorkersAreReady(const Worker_EntityQueryResponseOp& Op, uint32& ServerWorkersNotReady)
{
	ServerWorkersNotReady = 0;

	// The query response is an array of entities. Each of these represents a worker.
	for (uint32_t i = 0; i < Op.result_count; ++i)
	{
		const Worker_Entity& Entity = Op.results[i];
		for (uint32_t j = 0; j < Entity.component_count; j++)
		{
			const Worker_ComponentData& Data = Entity.components[j];
			// Server worker entities which represent workers have a component on them which specifies the SpatialOS worker ID,
			// which is the string we use to refer to them as a physical worker ID.
			if (Data.component_id == SpatialConstants::SERVER_WORKER_COMPONENT_ID)
			{
				const Schema_Object* ComponentObject = Schema_GetComponentDataFields(Data.schema_type);

				// The translator should only acknowledge workers that are ready to begin play. This means we can make
				// guarantees based on where non-GSM-authoritative servers canBeginPlay=true as an AddComponent
				// or ComponentUpdate op. This affects how startup Actors are treated in a zoned environment.
				const bool bWorkerIsReadyToBeginPlay = SpatialGDK::GetBoolFromSchema(ComponentObject, SpatialConstants::SERVER_WORKER_READY_TO_BEGIN_PLAY_ID);
				if (!bWorkerIsReadyToBeginPlay)
				{
					ServerWorkersNotReady++;
				}
			}
		}
	}

	return ServerWorkersNotReady == 0;
}

// This method is called on the worker who is authoritative over the translation mapping. Based on the results of the
// system entity query, assign the VirtualWorkerIds to the workers represented by the system entities.
void SpatialVirtualWorkerTranslationManager::AssignPartitionsToEachServerWorkerFromQueryResponse(const Worker_EntityQueryResponseOp& Op)
{
	// The query response is an array of entities. Each of these represents a worker.
	for (uint32_t i = 0; i < Op.result_count; ++i)
	{
		const Worker_Entity& Entity = Op.results[i];
		for (uint32_t j = 0; j < Entity.component_count; j++)
		{
			const Worker_ComponentData& Data = Entity.components[j];

			// Server worker entities which represent workers have a component on them which specifies the SpatialOS worker ID,
			// which is the string we use to refer to them as a physical worker ID.
			if (Data.component_id == SpatialConstants::SERVER_WORKER_COMPONENT_ID)
			{
				const Schema_Object* ComponentObject = Schema_GetComponentDataFields(Data.schema_type);

				PartitionInfo Partition = Partitions[i];

				// TODO(zoning): Currently, this only works if server workers never die. Once we want to support replacing
				// workers, this will need to process UnassignWorker before processing AssignWorker.
				PhysicalWorkerName WorkerName  = SpatialGDK::GetStringFromSchema(ComponentObject, SpatialConstants::SERVER_WORKER_NAME_ID);
				AssignPartitionToWorker(WorkerName, Entity.entity_id, Partition);
			}
		}
	}
}

// This will be called on the worker authoritative for the translation mapping to push the new version of the map
// to the SpatialOS storage.
void SpatialVirtualWorkerTranslationManager::SendVirtualWorkerMappingUpdate()
{
	// Construct the mapping update based on the local virtual worker to physical worker mapping.
	FWorkerComponentUpdate Update = {};
	Update.component_id = SpatialConstants::VIRTUAL_WORKER_TRANSLATION_COMPONENT_ID;
	Update.schema_type = Schema_CreateComponentUpdate();
	Schema_Object* UpdateObject = Schema_GetComponentUpdateFields(Update.schema_type);

	WriteMappingToSchema(UpdateObject);

	check(Connection != nullptr);
	Connection->SendComponentUpdate(SpatialConstants::INITIAL_VIRTUAL_WORKER_TRANSLATOR_ENTITY_ID, &Update);

	// The Translator on the worker which hosts the manager won't get the component update notification,
	// so send it across directly.
	check(Translator != nullptr);
	Translator->ApplyVirtualWorkerManagerData(UpdateObject);
}

void SpatialVirtualWorkerTranslationManager::SpawnPartitionEntity(VirtualWorkerId VirtualWorkerId)
{
	UE_LOG(LogSpatialVirtualWorkerTranslationManager, Log, TEXT("Spawning partition for virtual worker %d"), VirtualWorkerId);

	TArray<FWorkerComponentData> Components = SpatialGDK::EntityFactory::CreatePartitionEntityComponents(VirtualWorkerId);

	const Worker_RequestId RequestId = Connection->SendCreateEntityRequest(MoveTemp(Components), nullptr);

	CreateEntityDelegate OnCreateWorkerEntityResponse;
	OnCreateWorkerEntityResponse.BindLambda([this, VirtualWorkerId](const Worker_CreateEntityResponseOp& Op)
    {
        if (Op.status_code == WORKER_STATUS_CODE_SUCCESS)
        {
            this->OnPartitionEntityCreation(Op.entity_id, VirtualWorkerId);
            return;
        }

        if (Op.status_code != WORKER_STATUS_CODE_TIMEOUT)
        {
            UE_LOG(LogSpatialVirtualWorkerTranslationManager, Error, TEXT("Partition entity creation request failed: \"%s\""),
                UTF8_TO_TCHAR(Op.message));
            return;
        }

        UE_LOG(LogSpatialVirtualWorkerTranslationManager, Error, TEXT("Partition entity creation request timed out. Ally write some retry logic"));
    });

	this->Receiver->AddCreateEntityDelegate(RequestId, MoveTemp(OnCreateWorkerEntityResponse));
}

void SpatialVirtualWorkerTranslationManager::OnPartitionEntityCreation(Worker_EntityId PartitionEntityId, VirtualWorkerId VirtualWorker)
{
	Partitions.Emplace(PartitionInfo{ PartitionEntityId, VirtualWorker});

	if (Partitions.Num() == VirtualWorkersToAssign.Num())
	{
		QueryForServerWorkerEntities();
	}
}

void SpatialVirtualWorkerTranslationManager::QueryForServerWorkerEntities()
{
	UE_LOG(LogSpatialVirtualWorkerTranslationManager, Log, TEXT("Sending query for server worker entities"));

	if (bWorkerEntityQueryInFlight)
	{
		UE_LOG(LogSpatialVirtualWorkerTranslationManager, Warning, TEXT("Trying to query for worker entities while a previous query is still in flight!"));
		return;
	}

	// Create a query for all the server worker entities. This will be used
	// to find physical workers which the virtual workers will map to.
	Worker_ComponentConstraint WorkerEntityComponentConstraint{};
	WorkerEntityComponentConstraint.component_id = SpatialConstants::SERVER_WORKER_COMPONENT_ID;

	Worker_Constraint WorkerEntityConstraint{};
	WorkerEntityConstraint.constraint_type = WORKER_CONSTRAINT_TYPE_COMPONENT;
	WorkerEntityConstraint.constraint.component_constraint = WorkerEntityComponentConstraint;

	Worker_EntityQuery WorkerEntityQuery{};
	WorkerEntityQuery.constraint = WorkerEntityConstraint;
	WorkerEntityQuery.result_type = WORKER_RESULT_TYPE_SNAPSHOT;

	// Make the query.
	check(Connection != nullptr);
	Worker_RequestId RequestID = Connection->SendEntityQueryRequest(&WorkerEntityQuery);
	bWorkerEntityQueryInFlight = true;

	// Register a method to handle the query response.
	EntityQueryDelegate ServerWorkerEntityQueryDelegate;
	ServerWorkerEntityQueryDelegate.BindRaw(this, &SpatialVirtualWorkerTranslationManager::ServerWorkerEntityQueryDelegate);
	check(Receiver != nullptr);
	Receiver->AddEntityQueryDelegate(RequestID, ServerWorkerEntityQueryDelegate);
}

// This method allows the translation manager to deal with the returned list of server worker entities when they are received.
// Note that this worker may have lost authority for the translation mapping in the meantime, so it's possible the
// returned information will be thrown away.
void SpatialVirtualWorkerTranslationManager::ServerWorkerEntityQueryDelegate(const Worker_EntityQueryResponseOp& Op)
{
	bWorkerEntityQueryInFlight = false;

	if (Op.status_code != WORKER_STATUS_CODE_SUCCESS)
	{
		UE_LOG(LogSpatialVirtualWorkerTranslationManager, Warning, TEXT("Could not find ServerWorker Entities via entity query: %s, retrying."), UTF8_TO_TCHAR(Op.message));
		QueryForServerWorkerEntities();
		return;
	}

	if (Op.result_count != Partitions.Num())
	{
		UE_LOG(LogSpatialVirtualWorkerTranslationManager, Warning, TEXT("Didn't find correct ServerWorker entity count. Found %d. Expected %d. Retrying."), Op.result_count, Partitions.Num());
		QueryForServerWorkerEntities();
		return;
	}

	uint32 ServerWorkersNotReady;
	if (!AllServerWorkersAreReady(Op, ServerWorkersNotReady))
	{
		UE_LOG(LogSpatialVirtualWorkerTranslationManager, Warning, TEXT("Query found correct number of server workers but %d were not ready."), ServerWorkersNotReady);
		QueryForServerWorkerEntities();
		return;
	}

	UE_LOG(LogSpatialVirtualWorkerTranslationManager, Log, TEXT("Found all required server worker entities ready to play."));
	AssignPartitionsToEachServerWorkerFromQueryResponse(Op);

	SendVirtualWorkerMappingUpdate();
}

void SpatialVirtualWorkerTranslationManager::AssignPartitionToWorker(const PhysicalWorkerName& WorkerName, const Worker_EntityId& ServerWorkerEntityId, PartitionInfo Partition)
{
	VirtualToPhysicalWorkerMapping.Add(Partition.VirtualWorker, SpatialVirtualWorkerTranslator::WorkerInformation{ WorkerName, ServerWorkerEntityId, Partition.PartitionEntityId });
	UE_LOG(LogSpatialVirtualWorkerTranslationManager, Log, TEXT("Assigned VirtualWorker %d with partition ID %lld to simulate on Worker %s"), Partition.VirtualWorker, Partition.PartitionEntityId, *WorkerName);
}
