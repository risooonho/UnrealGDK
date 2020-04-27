// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "Containers/Queue.h"
#include "EngineClasses/SpatialVirtualWorkerTranslator.h"
#include "SpatialCommonTypes.h"
#include "SpatialConstants.h"

#include <WorkerSDK/improbable/c_worker.h>
#include <WorkerSDK/improbable/c_schema.h>

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpatialVirtualWorkerTranslationManager, Log, All)

class SpatialVirtualWorkerTranslator;
class SpatialOSDispatcherInterface;
class SpatialOSWorkerInterface;

//
// The Translation Manager is responsible for querying SpatialOS for all UnrealWorker worker
// entities and querying the LBStrategy for the number of Virtual Workers needed for load
// balancing. Then the Translation manager creates a mapping from physical worker name
// to virtual worker ID, and writes that to the single Translation entity.
//
// One UnrealWorker is arbitrarily chosen by SpatialOS to be authoritative for the Translation
// entity. This class will execute on that worker and will be idle on all other workers.
//
// This class is currently implemented in the UnrealWorker, but none of the logic must be in
// Unreal. It could be moved to an independent worker in the future in cloud deployments. It
// lives here now for convenience and for fast iteration on local deployments.
//

class SPATIALGDK_API SpatialVirtualWorkerTranslationManager
{
public:
	struct PartitionInfo
	{
		Worker_EntityId PartitionEntityId;
		VirtualWorkerId VirtualWorker;
	};

	SpatialVirtualWorkerTranslationManager(SpatialOSDispatcherInterface* InReceiver,
		SpatialOSWorkerInterface* InConnection,
		SpatialVirtualWorkerTranslator* InTranslator);

	void AddVirtualWorkerIds(const TSet<VirtualWorkerId>& InVirtualWorkerIds);
	void AuthorityChanged(const Worker_AuthorityChangeOp& AuthOp);
	void SpawnPartitionEntitiesForVirtualWorkerIds();
	const TArray<PartitionInfo>& GetAllPartitions() { return Partitions; };

private:
	SpatialOSDispatcherInterface* Receiver;
	SpatialOSWorkerInterface* Connection;

	SpatialVirtualWorkerTranslator* Translator;

	TArray<VirtualWorkerId> VirtualWorkersToAssign;
	TArray<PartitionInfo> Partitions;
	TMap<VirtualWorkerId, SpatialVirtualWorkerTranslator::WorkerInformation> VirtualToPhysicalWorkerMapping;

	bool bWorkerEntityQueryInFlight;

	// Serialization and deserialization of the mapping.
	void WriteMappingToSchema(Schema_Object* Object) const;

	// The following methods are used to query the Runtime for all worker entities and update the mapping
	// based on the response.
	void QueryForServerWorkerEntities();
	void ServerWorkerEntityQueryDelegate(const Worker_EntityQueryResponseOp& Op);
	bool AllServerWorkersAreReady(const Worker_EntityQueryResponseOp& Op, uint32& ServerWorkersNotReady);
	void AssignPartitionsToEachServerWorkerFromQueryResponse(const Worker_EntityQueryResponseOp& Op);
	void SendVirtualWorkerMappingUpdate();

	void AssignPartitionToWorker(const PhysicalWorkerName& WorkerName, const Worker_EntityId& ServerWorkerEntityId, PartitionInfo Partition);

	void SpawnPartitionEntity(VirtualWorkerId VirtualWorker);
	void OnPartitionEntityCreation(Worker_EntityId PartitionEntityId, VirtualWorkerId VirtualWorker);
};

