// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "LoadBalancing/LayeredLBStrategy.h"

#include "CoreMinimal.h"
#include "Utils/LayerInfo.h"

#include "SpatialMultiWorkerSettings.generated.h"

class UAbstractLBStrategy;
class UAbstractLockingPolicy;

UCLASS(Blueprintable)
class SPATIALGDK_API USpatialMultiWorkerSettings : public UObject
{
	GENERATED_BODY()

public:
	USpatialMultiWorkerSettings() {};

	/** Enable running different server worker types to split the simulation. */
	UPROPERTY(EditAnywhere, Category = "Multi-Worker")
	bool bEnableMultiWorker;

	UPROPERTY(EditAnywhere, Category = "Multi-Worker")
	TSubclassOf<UAbstractLBStrategy> DefaultLayerLoadBalanceStrategy;

	UPROPERTY(EditAnywhere, Category = "Multi-Worker")
	TSubclassOf<UAbstractLockingPolicy> DefaultLayerLockingPolicy;

	/** Layer configuration. */
	UPROPERTY(EditAnywhere, Category = "Multi-Worker")
	TMap<FName, FLayerInfo> WorkerLayers;
};
