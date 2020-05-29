// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "Interop/Connection/OutgoingMessages.h"
#include "Interop/Connection/SpatialOSWorkerInterface.h"
#include "Interop/Connection/WorkerConnectionCoordinator.h"
#include "SpatialCommonTypes.h"
#include "SpatialView/OpList/OpList.h"

#include "Containers/Queue.h"
#include "HAL/Event.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "UObject/WeakObjectPtr.h"

#include <improbable/c_schema.h>
#include <improbable/c_worker.h>

#include "SpatialWorkerConnection.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpatialWorkerConnection, Log, All);

UCLASS()
class SPATIALGDK_API USpatialWorkerConnection : public UObject, public FRunnable, public SpatialOSWorkerInterface
{
	GENERATED_BODY()

public:
	virtual void SetConnection(Worker_Connection* WorkerConnectionIn);
	virtual void FinishDestroy() override;
	virtual void DestroyConnection();

	// Worker Connection Interface
	virtual TArray<SpatialGDK::OpList> GetOpList() override;
	virtual Worker_RequestId SendReserveEntityIdsRequest(uint32_t NumOfEntities) override;
	virtual Worker_RequestId SendCreateEntityRequest(TArray<FWorkerComponentData>&& Components, const Worker_EntityId* EntityId) override;
	virtual Worker_RequestId SendDeleteEntityRequest(Worker_EntityId EntityId) override;
	virtual void SendAddComponent(Worker_EntityId EntityId, FWorkerComponentData* ComponentData) override;
	virtual void SendRemoveComponent(Worker_EntityId EntityId, Worker_ComponentId ComponentId) override;
	virtual void SendComponentUpdate(Worker_EntityId EntityId, FWorkerComponentUpdate* ComponentUpdate) override;
	virtual Worker_RequestId SendCommandRequest(Worker_EntityId EntityId, Worker_CommandRequest* Request, uint32_t CommandId) override;
	virtual void SendCommandResponse(Worker_RequestId RequestId, Worker_CommandResponse* Response) override;
	virtual void SendCommandFailure(Worker_RequestId RequestId, const FString& Message) override;
	virtual void SendLogMessage(uint8_t Level, const FName& LoggerName, const TCHAR* Message) override;
	virtual void SendComponentInterest(Worker_EntityId EntityId, TArray<Worker_InterestOverride>&& ComponentInterest) override;
	virtual Worker_RequestId SendEntityQueryRequest(const Worker_EntityQuery* EntityQuery) override;
	virtual void SendMetrics(const SpatialGDK::SpatialMetrics& Metrics) override;

	virtual PhysicalWorkerName GetWorkerId() const;
	virtual const TArray<FString>& GetWorkerAttributes() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnqueueMessage, const SpatialGDK::FOutgoingMessage*);
	FOnEnqueueMessage OnEnqueueMessage;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDequeueMessage, const SpatialGDK::FOutgoingMessage*);
	FOnDequeueMessage OnDequeueMessage;

	virtual void ProcessOutgoingMessages();
	virtual void MaybeFlush();

private:
	void QueueLatestOpList();
	void Flush();
	void CacheWorkerAttributes();

	// Begin FRunnable Interface
	virtual uint32 Run() override;
	virtual void Stop() override;
	// End FRunnable Interface

	void InitializeOpsProcessingThread();

	template <typename T, typename... ArgsType>
	void QueueOutgoingMessage(ArgsType&&... Args);

	Worker_Connection* WorkerConnection;

	TArray<FString> CachedWorkerAttributes;

	FRunnableThread* OpsProcessingThread;
	FThreadSafeBool KeepRunning = true;

	TQueue<SpatialGDK::OpList> OpListQueue;
	TQueue<TUniquePtr<SpatialGDK::FOutgoingMessage>> OutgoingMessagesQueue;

	// RequestIds per worker connection start at 0 and incrementally go up each command sent.
	Worker_RequestId NextRequestId = 0;

	// Coordinates the async worker ops thread.
	TOptional<WorkerConnectionCoordinator> ThreadWaitCondition;
};
