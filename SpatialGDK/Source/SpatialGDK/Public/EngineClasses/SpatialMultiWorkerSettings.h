// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "LoadBalancing/LayeredLBStrategy.h"

#include "CoreMinimal.h"
#include "Utils/LayerInfo.h"

#include "SpatialMultiWorkerSettings.generated.h"

class UAbstractLBStrategy;
class UAbstractLockingPolicy;

USTRUCT()
struct SPATIALGDK_API FSpatialMultiWorkerSettings
{
	GENERATED_BODY()

public:
	/** Enable running different server worker types to split the simulation. */
	UPROPERTY(EditAnywhere, Config, Category = "Multi-Worker")
	bool bEnableMultiWorker;

	UPROPERTY(EditAnywhere, Config, Category = "Multi-Worker", meta = (EditCondition = "bEnableMultiWorker"))
	TSubclassOf<UAbstractLBStrategy> DefaultLayerLoadBalanceStrategy;

	UPROPERTY(EditAnywhere, Config, Category = "Multi-Worker", meta = (EditCondition = "bEnableMultiWorker"))
	TSubclassOf<UAbstractLockingPolicy> DefaultLayerLockingPolicy;

	/** Layer configuration. */
	UPROPERTY(EditAnywhere, Config, Category = "Multi-Worker", meta = (EditCondition = "bEnableMultiWorker"))
	TMap<FName, FLayerInfo> WorkerLayers;
};
