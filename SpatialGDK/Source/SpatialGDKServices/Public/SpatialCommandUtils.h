// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpatialCommandUtils, Log, All);

class SpatialCommandUtils
{
public:
	SPATIALGDKSERVICES_API static bool SpatialVersion(bool bIsRunningInChina, const FString& DirectoryToRun, FString& OutResult, int32& OutExitCode);
	SPATIALGDKSERVICES_API static bool AttemptSpatialAuth(bool bIsRunningInChina);
	SPATIALGDKSERVICES_API static bool StartSpatialService(const FString& Version, const FString& RuntimeIP, bool bIsRunningInChina, const FString& DirectoryToRun, FString& OutResult, int32& OutExitCode);
	SPATIALGDKSERVICES_API static bool StopSpatialService(bool bIsRunningInChina, const FString& DirectoryToRun, FString& OutResult, int32& OutExitCode);
	SPATIALGDKSERVICES_API static bool BuildWorkerConfig(bool bIsRunningInChina, const FString& DirectoryToRun, FString& OutResult, int32& OutExitCode);
	SPATIALGDKSERVICES_API static FProcHandle LocalWorkerReplace(const FString& ServicePort, const FString& OldWorker, const FString& NewWorker, bool bIsRunningInChina, uint32* OutProcessID);
	SPATIALGDKSERVICES_API static bool GenerateDevAuthToken(bool bIsRunningInChina, FString& OutTokenSecret, FString& OutErrorMessage);
	SPATIALGDKSERVICES_API static bool HasDevLoginTag(const FString& DeploymentName, bool bIsRunningInChinat, FText& OutErrorMessage);
	SPATIALGDKSERVICES_API static FProcHandle StartLocalReceptionistProxyServer(bool bIsRunningInChina, const FString& CloudDeploymentName, FString &OutResult, int32 &ExitCode, bool &bProcessSucceeded);
	SPATIALGDKSERVICES_API static void StopLocalReceptionistProxyServer(FProcHandle& ProcessHandle);

};
