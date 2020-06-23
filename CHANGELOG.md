# Changelog
All notable changes to the SpatialOS Game Development Kit for Unreal will be documented in this file.

The format of this Changelog is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

**Note**: Since GDK for Unreal v0.8.0, the changelog is published in both English and Chinese. The Chinese version of each changelog is shown after its English version.<br>
**注意**：自虚幻引擎开发套件 v0.8.0 版本起，其日志提供中英文两个版本。每个日志的中文版本都置于英文版本之后。

## [`x.y.z`] - Unreleased
- Replaced the **Generate From Current Map** button  from the **Cloud Deployment Configuration** window by **Automatically Generate Launch Configuration** checkbox. If ticked, it generates an up to date configuration from the current map when selecting the **Start Deployment** button.


### Features:
- You can how change the GDK Editor Setting "Stop local deployment on stop play in editor" in order to automatically stop deployment when you stop playing in editor.

## [`0.10.0`] - 2020-06-15

### New Known Issues:

### Breaking Changes:
- Singletons have been removed as a class specifier and you will need to remove your usages of it. Replicating the behavior of former singletons is achievable through ensuring your Actor is spawned once by a single server-side worker in your deployment.
- `OnConnected` and `OnConnectionFailed` on `SpatialGameInstance` have been renamed to `OnSpatialConnected` and `OnSpatialConnectionFailed`. They are now also blueprint-assignable.
- The GenerateSchema and GenerateSchemaAndSnapshots commandlet will not generate Schema anymore and has been deprecated in favor of CookAndGenerateSchemaCommandlet (GenerateSchemaAndSnapshots still works with the -SkipSchema option).
- Settings for Offloading and Load Balancing have been combined and moved from the Editor and Runtime settings to instead be per map in the SpatialWorldSettings. For a detailed explanation please see the Load Balancing documentation.
- Command line arguments `OverrideSpatialOffloading` and `OverrideLoadBalancer` have been removed and UnrealGDK Load balancing is always enabled. To override a map's load balancing config "EnableMultiWorker" setting, use the command line flag `OverrideMultiWorker`.
- Running with result types (previously default enabled) is now mandatory. The Runtime setting `bEnableResultTypes` has been removed to reflect this.
- Offloading lookup by Actor returns based on the root owner of the Actor.
- Removed `QueuedOutgoingRPCWaitTime`, all RPC failure cases are now correctly queued or dropped.
- Removed `Max connection capacity limit` and `Login rate limit` from generated worker configurations as no longer supported.
- Secure worker connections are no longer supported for Editor builds. They are still supported for packaged builds.

### Features:
- You can now generate valid schema for classes that start with a leading digit. The generated schema class will be prefixed with `ZZ` internally.
- Handover properties will be automatically replicated when required for load balancing. `bEnableHandover` is off by default.
- Added `OnSpatialPlayerSpawnFailed` delegate to `SpatialGameInstance`. This is helpful if you have established a successful connection but the server worker crashed.
- The GDK now uses SpatialOS 14.6.1.
- Added `bWorkerFlushAfterOutgoingNetworkOp` (defaulted false) which publishes changes to the GDK worker queue after RPCs and property replication to allow for lower latencies. Can be used in conjunction with `bRunSpatialWorkerConnectionOnGameThread` to get the lowest available latency at a trade-off with bandwidth.
- You can now edit the project name field in the `Cloud Deployment Configuration` window.
- Worker types are now defined in the runtime settings.
- Local deployment will now use the map's load balancing strategy to get the launch configuration settings. The launch configuration file is saved per-map in the Intermediate/Improbable folder.
- A launch configuration editor has been added under the `Configure` toolbar button.
- The cloud deployment window can now generate a launch configuration from the current map or use the launch configuration editor.
- Worker load can be specified by game logic via `SpatialMetrics::SetWorkerLoadDelegate`
- You can now specify deployment tags in the `Cloud Deployment Configuration` window.
- RPCs declared in a UINTERFACE can now be executed. Previously, this would lead to a runtime assertion.
- Full Schema generation now uses the CookAndGenerateSchema commandlet, which will result in faster and more stable schema generation for big projects.
- Added `Open Deployment Page` button to the `Cloud Deployment Configuration` window.
- The `Start Deployment` button in the `Cloud Deployment Configuration` dialog can now generate schema, generate a snapshot, build all selected workers, and upload the assembly before starting the deployment. There are checkboxes to toggle the generation of schema and snapshots as well as whether to build the client and simulated player workers.
- When starting a cloud deployment via the Unreal Editor, it will now automatically add the `dev_login` tag to the deployment.
- Renamed `enableProtocolLogging` command line parameter to `enableWorkerSDKProtocolLogging` and added `enableWorkerSDKOpLogging` parameter that allows to log user-level ops. Renamed `protocolLoggingPrefix` parameter to `workerSDKLogPrefix`. This prefix is used for both protocol and op logging. Added `workerSDKLogLevel` parameter that takes "debug", "info", "warning" or "error". Added `workerSDKLogFileSize` to control the maximum file size of the worker SDK log file.
- Changed the icon of the `Start Deployment` toolbar button based on the selected connection flow.
- Created a new dropdown in the Spatial toolbar. This dropdown menu allows you to configure how to connect your PIE client or your Launch on Device client:
  - You can choose between `Connect to a local deployment` and `Connect to a cloud deployment` to specify the flow the client should automatically take upon clicking the `Play` or the `Launch` button.
  - Added the `Local Deployment IP` field to specify which local deployment you want to connect to. By default, this will be `127.0.0.1`.
  - Added the `Cloud deployment name` field to specify which cloud deployment you want to connect to. If no cloud deployment is specified and you select `Connect to cloud deployment`, it will try to connect to the first running deployment that has the `dev_login` deployment tag.
  - Added the `Editor Settings` field to allow you to quickly get to the **SpatialOS Editor Settings**
- Added `Build Client Worker` and `Build SimulatedPlayer` checkbox to the Connection dropdown to quickly enable/disable building and including the client worker or simulated player worker in the assembly.
- Added new icons for the toolbar.
- The port is now respected when travelling via URL, translating to the receptionist port. The `-receptionistPort` command-line argument will still be used for the first connection.
- Running BuildWorker.bat with <game-name>Client will build the Client target of your project.
- When changing the project name via the `Cloud Deployment Configuration` window the development authentication token will automatically be regenerated.
- Changed the names of the following toolbar buttons:
  - `Start` is now called `Start Deployment`
  - `Deploy` is now called `Configure`
- Required fields in the Cloud Deployment Configuration window are now marked with an asterisk.
- When changing the project name via the `Cloud Deployment` dialog the development authentication token will automatically be regenerated.
- The SpatialOS project name can now be modified via the **SpatialOS Editor Settings**.
- Added support for the new SpatialOS Runtime. 
- Added a new dropdown setting in SpatialGDK Editor Settings to choose Runtime variant. There is currently Standard and Compatibility Mode. Standard is default, Compatibility Mode can be used if any networking issues arise when updating to the latest GDK version.
- Added new default deployment templates. The default template changes based on which Runtime variant you have selected and your current primary deployment region is.
- Inspector V2 is now supported. Inspector V2 is used by default for the Standard Runtime variant. Inspector V1 remains the default for the Compatibility Mode Runtime variant.
-Added the `Connect local server worker to the cloud deployment` checkbox in **SpatialOS Editor Settings**, that enables/disables the option to start and connect a local server-worker to a cloud deployment when `Connect to cloud deployment` is enabled.

## Bug fixes:
- Fix problem where load balanced cloud deploys could fail to start while under heavy load.
- Fix to avoid using packages still being processed in the async loading thread.
- Fixed a bug when running GDK setup scripts fail to unzip dependencies sometimes.
- Fixed a bug where RPCs called before the CreateEntityRequest were not being processed as early as possible in the RPC Ring Buffer system, resulting in startup delays on the client.
- Fixed a crash when running with nullrhi and using SpatialDebugger.
- When using a URL with options in the command line, receptionist parameters will be parsed correctly, making use of the URL if necessary.
- Fixed a bug when creating multiple dynamic subobjects at the same time, when they would fail to be created on clients.
- OwnerOnly components are now properly replicated when gaining authority over an actor. Previously, they were sometimes only replicated when a value on them changed after already being authoritative.
- Fixed a rare server crash that could occur when closing an actor channel right after attaching a dynamic subobject to that actor.

### Internal:
Features listed in this section are not ready to use. However, in the spirit of open development, we record every change that we make to the GDK.

- The SpatialOS GDK for Unreal is now released automatically using Buildkite CI. This should result in more frequent releases.

## [`0.9.0`] - 2020-05-05

### New Known Issues:
- After you upgrade to Unreal Engine `4.24.3` using `git pull`, you might be left in a state where several `.isph` and `.ispc` files are missing. This state produces [compile errors](https://forums.unrealengine.com/unreal-engine/announcements-and-releases/1695917-unreal-engine-4-24-released?p=1715142#post1715142) when you build the engine. You can fix this by running `git restore .` in the root of your `UnrealEngine` repository.

### Breaking Changes:
- Simulated player worker configurations now require a development authentication token and deployment flag instead of a login token and player identity token. See the Example Project for an example of how to set this up.

### Features:
- We now support Unreal Engine `4.24.3`. You can find the `4.24.3` version of our engine fork [here](https://github.com/improbableio/UnrealEngine/tree/4.24-SpatialOSUnrealGDK-release).
- Added a new variable: `QueuedOutgoingRPCWaitTime`. Outgoing RPCs are now dropped in the following three scenarios: more than `QueuedOutgoingRPCWaitTime` time has passed since the RPC was sent; the worker instance is never expected to have the authority required to receive the RPC (if you're using offloading or zoning); or the Actor that the RPC is sent to is being destroyed.
- In local deployments of the Example Project, you can now launch simulated players with one click. To launch a single simulated player client, run `LaunchSimPlayerClient.bat`. To launch ten simulated player clients, run `Launch10SimPlayerClients.bat`.
- Added support for the UE4 Network Profiler to measure relative size of RPC and Actor replication data.
- Added a `SpatialToggleMetricsDisplay` console command. You must enable `bEnableMetricsDisplay` in order for the metrics display to be available. You must then must call `SpatialToggleMetricsDisplay` on each client that you want the metrics display to be visible for.
- Enabled compression in the Modular UDP networking stack.
- Switched off default RPC-packing. You can re-enable this in `SpatialGDKSettings`.
- When you start a local deployment, we now check to see if the required Runtime port is blocked. If it is, we display a dialog box that asks whether you want to kill the process that is blocking the port.
- A configurable Actor component `SpatialPingComponent` is now available. This enables PlayerControllers to measure the ping time to the server-worker instances that have authority over them. You can access the latest raw ping value via `GetPing()`, or access the rolling average, which is stored in `PlayerState`.
- You can invoke the `GenerateSchema`, `GenerateSchemaAndSnapshots`, and `CookAndGenerateSchema` commandlets with the `-AdditionalSchemaCompilerArguments="..."` command line switch to output additional compiled schema formats. If you don't provide this switch, the output contains only the schema descriptor. This switch's value should be a subset of the arguments that you can pass to the schema compiler directly (for example `--bundle_out="path/to/bundle.sb"`). You can see a full list of possible values in the [schema compiler documentation](https://docs.improbable.io/reference/14.2/shared/schema/introduction#schema-compiler-cli-reference)
- Added the `AllowUnresolvedParameters` function flag. This flag disables warnings that occur during processing of RPCs that have unresolved parameters. To enable this flag, use Blueprints, or add a tag to the `UFUNCTION` macro.
- There is now a warning if you launch a cloud deployment with the `manual_worker_connection_only` flag set to `true`.
- We now support server travel for single-server game worlds. We don't support server travel for game worlds that use zoning or offloading.
- Improved the workflow relating to schema generation issues when you launch local deployments. There is now a warning if you try to launch a local deployment after a schema error.
- `DeploymentLauncher` can now launch a simulated player deployment independently from the target deployment.
Usage: `DeploymentLauncher createsim <project-name> <assembly-name> <target-deployment-name> <sim-deployment-name> <sim-deployment-json> <sim-deployment-region> <num-sim-players> <auto-connect>`
- We now use `HeartbeatTimeoutWithEditorSeconds` if `WITH_EDITOR` is defined. This prevents worker instances from disconnecting when you're running them from the Unreal Editor for debugging.
- Added the `bAsyncLoadNewClassesOnEntityCheckout` setting to `SpatialGDKSettings`. This allows worker instances to load new classes asynchronously when they are receiving the initial updates about an entity. It is `false` by default.
- Added `IndexYFromSchema` functions for the `Coordinates`, `WorkerRequirementSet`, `FRotator`, and `FVector` classes. We've remapped the `GetYFromSchema` functions for the same classes to invoke `IndexYFromSchema` internally, in line with other implementations of the pattern.
- Clients now validate their schema files against the schema files on the server, and log a warning if the files do not match.
- Entries in the schema database are now sorted to improve efficiency when searching for assets in the Unreal Editor. (DW-Sebastien)
- `BatchSpatialPositionUpdates` in `SpatialGDKSettings` now defaults to false.
- Added `bEnableNetCullDistanceInterest` (default `true`) to enable client interest to be exposed through component tagging. This functionality has closer parity to native Unreal client interest.
- Added `bEnableNetCullDistanceFrequency` (default `false`) to enable client interest queries to use frequency. You can configure this functionality using `InterestRangeFrequencyPairs` and `FullFrequencyNetCullDistanceRatio`.
- Introduced the feature flag `bEnableResultTypes` (default `true`). This configures interest queries to include only the set of components required for the queries to run. Depending on your game, this might save bandwidth.
- If you set the `bEnableResultTypes` flag to `true`, this disables dynamic interest overrides.
- Moved the development authentication settings from the Runtime Settings panel to the Editor Settings panel.
- Added the option to use the development authentication flow with the command line.
- Added a button to generate a development authentication token inside the Unreal Editor. To use it, navigate to **Edit** > **Project Setting** > **SpatialOS GDK for Unreal** > **Editor Settings** > **Cloud Connection**.
- Added a new section where you can configure the launch arguments for running a client on a mobile device. To use it, navigate to **Edit** > **Project Setting** > **SpatialOS GDK for Unreal** > **Editor Settings** > **Mobile**.
- You can now choose which Runtime version to use (in the Runtime Settings) when you launch a local or cloud deployment.
- If you set the `--OverrideResultTypes` flag to `true`, server-worker instances no longer receive updates about server RPC components on Actors that they do not own. This should decrease bandwidth for server-worker instances in offloaded and zoned games.
- The `InstallGDK` scripts now `git clone` the correct version of the `UnrealGDK` and `UnrealGDKExampleProject` for the `UnrealEngine` branch that you have checked out. They read `UnrealGDKVersion.txt` and `UnrealGDKExampleProjectVersion.txt` to determine what the correct branches are.
- Removed the `bEnableServerQBI` property and the `--OverrideServerInterest` flag.
- Added custom warning timeouts for each RPC failure condition.
- `SpatialPingComponent` can now also report average ping measurements over a specified number of recent pings. You can use `PingMeasurementsWindowSize` to specify how many measurements you want to record, and call `GetAverageData` to get the measurement data. There is also a delegate `OnRecordPing` that is broadcast whenever a new ping measurement is recorded.
- The Spatial Output Log window now displays deployment startup errors.
- Added `bEnableClientQueriesOnServer` (default false) which makes the same queries on the server as it makes on clients, if the GDK for Unreal's load balancer is enabled. Enable `bEnableClientQueriesOnServer` to avoid a situation in which clients receive updates about entities that the server doesn't receive updates about (which happens if the server's interest query is configured incorrectly).
- We now log a warning when `AddPendingRPC` fails due to `ControllerChannelNotListening`.
- When offloading is enabled, Actors have local authority (`ROLE_Authority`) on servers for longer periods of time, to allow more native Unreal functionality to work without problems.
- When offloading is enabled, if you try to spawn Actors on a server that will not be the Actor Group owner for them, we now log an error and delete the Actors.
- The GDK now uses SpatialOS Runtime version 14.5.1 by default.
- Renamed the configuration setting `bPreventAutoConnectWithLocator` to `bPreventClientCloudDeploymentAutoConnect` and moved it to `SpatialGDKSettings`. To use this feature, enable the setting in `SpatialGDKSettings`.
- Made `USpatialMetrics::WorkerMetricsRecieved` static.
- You can now connect to a local deployment by selecting **Connect to a local deployment** and specifying the local IP address of your computer in the Launch drop-down menu.
- Enabled RPC ring buffers by default. We'll remove the legacy RPC mode in a future release.
- Removed the `bPackRPCs` property and the `--OverrideRPCPacking` flag.
- Added `OnClientOwnershipGained` and `OnClientOwnershipLost` events on Actors and Actor Components. These events trigger when an Actor is added to or removed from the ownership hierarchy of a client's PlayerController.
- Automatically remove UE4CommandLine.txt after finishing a Launch on device session on an Android device (only UnrealEngine 4.24 or above). This is done to prevent the launch session command line from overriding the one built into the APK.

## Bug fixes:
- Queued RPCs no longer spam logs when an entity is deleted.
- We now take the `OverrideSpatialNetworking` command line argument into account as early as possible (previously, `LocalDeploymentManager` queried `bSpatialNetworking` before the command line was parsed).
- Servers now maintain interest in `AlwaysRelevant` Actors.
- `GetActorSpatialPosition` now returns the last spectator sync location while the player is spectating.
- The default cloud launch configuration is now empty.
- Fixed a crash that happened when the GDK attempted to read schema from an unloaded class.
- We now properly handle (and eventually resolve) unresolved object references in replicated arrays of structs.
- Fixed a tombstone-related assert that could fire and bring down the Unreal Editor.
- If an Actor that is placed in the level with `bNetLoadOnClient=false` goes out of a worker instance's view, it is now reloaded if it comes back into view.
- Fixed a crash in `SpatialDebugger` that was caused by the dereference of an invalid weak pointer.
- Fixed a connection error that occurred when using `spatial cloud connect external`.
- The command line argument `receptionistHost <URL>` no longer overrides connections to `127.0.0.1`.
- If you connect a worker instance to a deployment using the Locator, and you initiate a `ClientTravel` using a URL that requires the Receptionist, this now works correctly.
- You can now access the worker flags via `USpatialStatics::GetWorkerFlag` instead of `USpatialWorkerFlags::GetWorkerFlag`.
- Fixed a crash in `SpatialDebugger` that occurs when GDK-space load balancing is disabled.
- The schema database no longer fails to load previous saved state when working in the Unreal Editor.
- If you attempt to launch a cloud deployment, this now runs the `spatial auth` process as required. Previously the deployment would fail.
- Made a minor spelling fix to the connection log message.
- The debug strings in `GlobalStateManager` now display the Actor class name in log files.
- The server no longer crashes when received RPCs are processed recursively.
- The GDK no longer crashes when `SoftObjectPointers` are not yet resolved, but instead serializes them as expected after they are resolved.
- Fixed an issue that occurred when replicating a property for a class that was part of an asynchronously-loaded package, when the package had not finished loading.
- Fixed component interest constraints that are constructed from schema.
- The GDK now tracks properties that contain references to replicated Actors, so that it can resolve them again if the Actor that they reference moves out of and back into relevance.
- PIE sessions no longer occasionally fail to start due to missing schema for the `SpatialDebugger` Blueprint.
- Fixed an issue where a newly-created subobject had empty state when `RepNotify` was called for a property pointing to that subobject.
- Fixed an issue where deleted, initially dormant startup Actors would still be present on other worker instances.
- We now force-activate the RPC ring buffer when load balancing is enabled, to allow RPC handover when authority changes.
- Fixed a race condition where a client that was leaving the deployment could leave its Actor behind on the server, to be cleaned up after a long timeout.
- Fixed a crash that was caused by state in `SpatialGameInstance` persisting across a transition from one deployment to another.
- The GDK no longer crashes when you start and stop PIE clients multiple times.
- The GDK no longer crashes when shadow data is uninitialized when resolving unresolved objects.
- Fixed an occasional issue when sending component RPCs on a recently-created Actor.

### Internal:
Features listed in this section are not ready to use. However, in the spirit of open development, we record every change that we make to the GDK.

- Enabled the SpatialOS toolbar for MacOS.
- Added support for Android.
- `SpatialDebugger` worker regions are now cuboids rather than planes, and can have their `WorkerRegionVerticalScale` adjusted via a setting in the `SpatialDebugger`.
- Added an `AuthorityIntent` component, a `VirtualWorkerTranslation` component, and a partial framework. We'll use these in the future to control load balancing.
- Load balancing strategies and locking strategies can be set per-level using `SpatialWorldSettings`.
- Added a new Runtime Settings flag to enable the GDK for Unreal load balancer. This is a feature that is in development and not yet ready for general use. Enabling the GDK for Unreal load balancer now creates a single query per server-worker instance, depending on the defined load balancing strategy.
- Extracted the logic responsible for taking an Actor and generating the array of SpatialOS components that represents it as an entity in SpatialOS. This logic is now in `EntityFactory`.
- `DeploymentLauncher` can now parse a .pb.json launch configuration.

### External contributors:
@DW-Sebastien


## [`0.8.1`] - 2020-03-17

### English version
### Adapted from 0.8.1-preview
### Features:
- **SpatialOS GDK for Unreal** > **Editor Settings** > **Region Settings** has been moved to **SpatialOS GDK for Unreal** > **Runtime Settings** > **Region Settings**.
- You can now choose which SpatialOS service region you want to use by adjusting the **Region where services are located** setting. You must use the service region that you're geographically located in.
- Deployments can now be launched in China, when the **Region where services are located** is set to `CN`.
- Updated the version of the local API service used by the UnrealGDK.
- The Spatial output log will now be open by default.
- The GDK now uses SpatialOS 14.5.0.

### Bug fixes:
- Replicated references to newly created dynamic subobjects will now be resolved correctly.
- Fixed a bug that caused the local API service to memory leak.
- Cloud deployment flow will now correctly report errors when a deployment fails to launch due to a missing assembly.
- Errors are now correctly reported when you try to launch a cloud deployment without an assembly.
- The Start deployment button will no longer become greyed out when a `spatial auth login` process times out.
------
### 中文版本
### 引用 0.8.1-preview 版本
### 功能介绍:
- **UI 路径变更**：**SpatialOS GDK for Unreal** > **Editor Settings** > **Region Settings** 移动到 **SpatialOS GDK for Unreal** > **Runtime Settings** > **Region Settings** 路径。
- **新增 SpatialOS 服务地区选项**：通过调整 **Region where services are located** 设置，您可以选择想要使用的 SpatialOS 服务地区。您选择的服务地区必须是您所处的地理位置。
- **新增中国地区部署**：通过将 **Region where services are located** 设置为 `CN`，您可以在中国启动游戏部署。
- **本地 API 服务版本**：虚幻引擎开发套件使用的本地 API 服务版本更新。
- **SpatialOS 输出日志**：SpatialOS 输出日志默认开启。
- 虚幻引擎开发套件现已使用 **SpatialOS 14.5.0** 版本。

### 问题修复:

- 修复对新创建的动态子对象的重复引用问题。
- 修复导致本地 API 服务内存泄漏的问题。
- 在没有程序集的情况下启动云部署时发生的错误，现已正确上报。
- 当 `spatial auth login` 进程超时，启动部署的按钮 (Start) 不再显示为灰色。


## [`0.8.1-preview`] - 2020-03-17

### Internal:
### Adapted from 0.6.5
- **SpatialOS GDK for Unreal** > **Editor Settings** > **Region Settings** has been moved to **SpatialOS GDK for Unreal** > **Runtime Settings** > **Region Settings**.
- You can now choose which SpatialOS service region you want to use by adjusting the **Region where services are located** setting. You must use the service region that you're geographically located in.
- Deployments can now be launched in China, when the **Region where services are located** is set to `CN`.

### Features:
- Updated the version of the local API service used by the UnrealGDK.
- The Spatial output log will now be open by default.
- The GDK now uses SpatialOS 14.5.0.

### Bug fixes:
- Replicated references to newly created dynamic subobjects will now be resolved correctly.
- Fixed a bug that caused the local API service to memory leak.
- Cloud deployment flow will now correctly report errors when a deployment fails to launch due to a missing assembly.
- Errors are now correctly reported when you try to launch a cloud deployment without an assembly.
- The Start deployment button will no longer become greyed out when a `spatial auth login` process times out.

## [`0.8.0-preview`] - 2019-12-17

### English version

### Breaking Changes:
- This is the last GDK version to support Unreal Engine 4.22. You will need to upgrade your project to use Unreal Engine 4.23 (`4.23-SpatialOSUnrealGDK-preview`) in order to continue receiving GDK releases and support.
- When upgrading to Unreal Engine 4.23 you must:
1. `git checkout 4.23-SpatialOSUnrealGDK-preview`
1. `git pull`
1. Download and install the `-v15 clang-8.0.1-based` toolchain from this [Unreal Engine Documentation page](https://docs.unrealengine.com/en-US/Platforms/Linux/GettingStarted/index.html).
1. Navigate to the root of GDK repo and run `Setup.bat`.
1. Run `Setup.bat`, which is located in the root directory of the `UnrealEngine` repository.
1. Run `GenerateProjectFiles.bat`, which is in the same root directory.

For more information, check the [Keep your GDK up to date](https://documentation.improbable.io/gdk-for-unreal/docs/keep-your-gdk-up-to-date) SpatialOS documentation.

### Features:
- You can now call `SpatialToggleMetricsDisplay` from the console in your Unreal clients in order to view metrics. `bEnableMetricsDisplay` must be enabled on clients where you want to use this feature.
- The modular-udp networking stack now uses compression by default.
- Reduced network latency by switching off default rpc-packing. If you need this on by default, you can re-enable it by editing `SpatialGDKSettings.ini`
- When you start a local deployment, the GDK now checks the port required by the runtime and, if it's in use, prompts you to kill that process.
- You can now measure round-trip ping from a player controller to the server-worker that's currently authoritative over it using the configurable actor component 'SpatialPingComponent'. The latest ping value can be accessed through the component via 'GetPing()' or via the rolling average stored in 'PlayerState'.
- You can disable the warnings that trigger when RPCs are processed with unresolved parameters using the `AllowUnresolvedParameters` function flag. This flag can be enabled through Blueprints or by adding a tag to the `UFUNCTION` macro.
- Improved logging around entity creation.
- Unreal Engine `4.23.1` is now supported. You can find the `4.23.1` version of our engine fork [here](https://github.com/improbableio/UnrealEngine/tree/4.23-SpatialOSUnrealGDK-preview).
- In Example Project, the default session duration has increased from 5 minutes to 120 minutes so you don't have to re-deploy while playtesting.
- In Example Project, the default lobby timer has decreased from 15 seconds to 3 seconds so you don't have to wait for your playtest to start.
- Added in-editor support for exposing a local runtime at a particular IP address. This offers the same functionality as the `--runtime_ip` option in the SpatialOS CLI.
- Spatial networking is now always enabled in built assemblies.

### Bug fixes:
- Fixed a bug that could cause name collisions in schema generated for sublevels.
- Downgraded name collisions during schema generation from Warning to Display.
- Replicating a static subobject after it has been deleted on a client no longer results in client attaching a new dynamic subobject.
- Fixed a bug that caused entity pool reservations to cease after a request times out.
- Running `BuildWorker.bat` for `SimulatedPlayer` no longer fails if the project path has a space in it.
- Fixed a crash when starting PIE with out-of-date schema.
- Fixed an issue where launching a cloud deployment with an invalid assembly name or deployment name wouldn't show a helpful error message.

### Internal:
Features listed in the internal section are not ready to use but, in the spirit of open development, we detail every change we make to the GDK.
- We've added a partial loadbalancing framework. When this is completed in a future release, you will be able to control loadbalancing using server-workers.
------

### 中文版本
### 重大变化:
- 这是最后支持虚幻引擎 4.22 版本的 GDK 版本。您需要升级您的项目来使用虚幻引擎 4.23 (`4.23-SpatialOSUnrealGDK-preview`)以便继续获取 GDK 发布和支持。
- 要升级到虚幻引擎 4.23 版本，您必须完成以下步骤：<br>
        1. `git checkout 4.23-SpatialOSUnrealGDK-preview`<br>
        2. `git pull`<br>
        3. 下载并安装 `-v15 clang-8.0.1-based` 工具链。详情参见 [虚幻引擎文档](https://docs.unrealengine.com/zh-CN/Platforms/Linux/GettingStarted/index.html)。<br>
        4. 打开 GDK 的根目录，运行 `Setup.bat`。<br>
        5. 运行 `Setup.bat`, 该程序位于 `UnrealEngine` 仓库的根目录。<br>
        6. 在同样的根目录中，运行 `GenerateProjectFiles.bat`。

更多信息，查看 [使 GDK 保持更新](https://documentation.improbable.io/gdk-for-unreal/lang-zh_CN/docs/keep-your-gdk-up-to-date) 文档。


### 功能介绍:
- 您可以在虚幻引擎客户端的控制台调用 `SpatialToggleMetricsDisplay` 来查看指标。您必须在客户端开启 `bEnableMetricsDisplay` 来使用该功能。
- 默认情况下，模块化udp网络栈现在使用压缩。
- 通过关闭默认 rpc 打包来减少网络延迟。如果需要默认情况下启用它，则可以通过编辑`SpatialGDKSettings.ini`来重新启用它。
- 当您开始本地部署时，GDK 会检查运行时所需的端口。如果该端口正在被使用，则会提示您终止该进程。
- 您可以使用可配置的 Actor 组件 `SpatialPingComponent` 来测量从玩家控制器到当前对其具有管辖权的服务端 worker 的往返 ping。您可以通过 `GetPing()`或存储在 `PlayerState` 中的移动平均值来访问组件获取最新的 ping 值。
- 通过 `AllowUnresolvedParameters` 函数标记，您可以禁用因使用未解析的参数处理 RPC 时触发的警告。您可以使用蓝图或通过向 `UFUNCTION` 宏添加标签来启用此标志。
- 改进关于实体创建的日志输出。
- 现已支持虚幻引擎 `4.23.1`。您可以在 [此处](https://github.com/improbableio/UnrealEngine/tree/4.23-SpatialOSUnrealGDK-preview) 找到虚幻引擎分支的 `4.23.1` 版本。
- 在示例项目中，默认的副本持续时间从5分钟增加到120分钟，因此您不必在进行游戏测试时重新部署。
- 在示例项目中，默认的游戏大厅计时器从15秒减少到3秒，因此您不必等待游戏测试开始。
- 添加编辑器内公开特定 IP 地址的本地运行时支持。它提供的功能与 SpatialOS CLI 中的 `--runtime_ip` 选项相同。
- SpatialOS 网络在构建的程序集中始终开启。

### 问题修复:
- 修复可能导致为关卡分段生成的模式语言中名称冲突的错误。
- 降低模式语言生成过程中的名称冲突级别：由 Warning (警告) 到 Display (显示)。
- 在客户端上删除静态子对象后对其进行复制不再导致客户端附加新的动态子对象。
- 修复请求超时后实体池预留停止的问题。
- 如果项目路径中有空格，为 `SimulatedPlayer` 运行 `BuildWorker.bat` 不再失败。
- 修复使用过期模式语言启动 PIE 时发生崩溃的问题。
- 修复使用无效的程序集名称或部署名称启动云部署不会显示有用的错误消息的问题。

### 内部:
内部部分中列出的功能尚未准备就绪，但本着开放式开发的精神，我们详细介绍了对 GDK 所做的所有更改。
- 我们添加了部分负载均衡框架。在未来的版本中完成此框架后，您将可以使用服务端 worker 来控制负载均衡。


## [`0.7.1-preview`] - 2019-12-06

### Adapted from 0.6.3
### Bug fixes:
- The C Worker SDK now communicates on port 443 instead of 444. This change is intended to protect your cloud deployments from DDoS attacks.

### Internal:
Features listed in the internal section are not ready to use but, in the spirit of open development, we detail every change we make to the GDK.
- The GDK is now compatible with the `CN` launch region. When Improbable's online services are fully working in China, they will work with this version of the GDK. You will be able to create SpatialOS Deployments in China by specifying the `CN` region in the Deployment Launcher.
- `Setup.bat` and `Setup.sh` both accept the `--china` flag, which will be required in order to run SpatialOS CLI commands in the `CN` region.
- **SpatialOS GDK for Unreal** > **Editor Settings** now contains a **Region Settings** section. You will be required to set **Region where services are located** to `CN` in order to create SpatialOS Deployments in China.

## [`0.7.0-preview`] - 2019-10-11

### New Known Issues:
- MSVC v14.23 removes `typeinfo.h` and replaces it with `typeinfo`. This change causes errors when building the Unreal Engine. This issue **only affects Visual Studio 2019** users. You can work around the issue by:
1. Open Visual Studio Installer.
1. Select "Modify" on your Visual Studio 2019 installation.
1. In the Installation details section uncheck all workloads and components until only **Visual Studio code editor** remains.
1. Select the following items in the Workloads tab:
* **Universal Windows Platform development**
* **.NET desktop development**
* You must also select the **.NET Framework 4.6.2 development tools** component in the Installation details section.
* **Desktop development with C++**
* You must then deselect **MSVC v142 - VS 2019 C++ x64/x86 build tools (v14.23)**, which was added as part of the **Desktop development with C++** Workload. You will be notified that: "If you continue, we'll remove the componenet and any items liseted above that depend on it." Select remove to confirm your decision.
* Lastly, add **MSVC v142 - VS 2019 C++ x64/x86 build tools (v14.22)** from the Individual components tab.
5. Select "Modify" to confirm your changes.

### Breaking Changes:
- If your project uses replicated subobjects that do not inherit from ActorComponent or GameplayAbility, you now need to enable generating schema for them using `SpatialType` UCLASS specifier, or by checking the Spatial Type checkbox on blueprints.
- Chunk based interest is no longer supported. All interest is resolved using query-based interest (QBI). You should remove streaming query and chunk based interest options from worker and launch config files to avoid unnecessary streaming queries being generated.
- If you already have a project that you are upgrading to this version of the GDK, it is encouraged to follow the upgrade process to SpatialOS `14.1.0`:
1. Open the `spatialos.json` file in the `spatial/` directory of your project.
1. Replace the `sdk_version` value and the version value of all dependencies with `14.1.0`.
1. Replace all other instances of the version number in the file.

### Features:
- The GDK now uses SpatialOS `14.1.0`.
- Visual Studio 2019 is now supported.
- You can now delete your schema database using options in the GDK toolbar and the commandlet.
- The GDK now checks that schema and a snapshot are present before attempting to start a local deployment. If either are missing then an error message is displayed.
- Added optional net relevancy check in replication prioritization. If enabled, an actor will only be replicated if IsNetRelevantFor is true for one of the connected client's views.
- You can now specify which actors should not persist as entities in your Snapshot. You do this by adding the flag `SPATIALCLASS_NotPersistent` to a class or by entering `NotPersistent` in the `Class Defaults` > `Spatial Description` field on blueprints.
- Deleted startup actors are now tracked.
- Added a user bindable delegate to `SpatialMetrics` which triggers when worker metrics have been received.
- Local deployments now create a new log file known as `launch.log` which will contain logs relating to starting and running a deployment. Additionally it will contain worker logs which are forwarded to the SpatialOS runtime.
- Added a new setting to SpatialOS Runtime Settings `Worker Log Level` which allows configuration of which verbosity of worker logs gets forwarded to the SpatialOS runtime.
- Added a new developer tool called 'Spatial Output Log' which will show local deployment logs from the `launch.log` file.
- Added logging for queued RPCs.
- Added several new STAT annotations into the ServerReplicateActors call chain.
- The GDK no longer generates schema for all UObject subclasses. Schema generation for Actor, ActorComponent and GameplayAbility subclasses is enabled by default, other classes can be enabled using `SpatialType` UCLASS specifier, or by checking the Spatial Type checkbox on blueprints.
- Added new experimental CookAndGenerateSchemaCommandlet that generates required schema during a regular cook.
- Added the `OverrideSpatialOffloading` command line flag. This allows you to toggle offloading at launch time.
- The initial connection from a worker will attempt to use relevant command line arguments (receptionistHost, locatorHost) to inform the connection. If these are not provided the standard connection flow will be followed. Subsequent connections will not use command line arguments.
- The command "Open 0.0.0.0" can be used to connect a worker using its command line arguments, simulating initial connection.
- The command "ConnectToLocator <login> <playerToken>" has been added to allow for explicit connections to deployments.
- Add SpatialDebugger and associated content.  This tool can be enabled via the SpatialToggleDebugger console command.  Documentation will be added for this soon.

### Bug fixes:
- Fixed a bug where the spatial daemon started even with spatial networking disabled.
- Fixed an issue that could cause multiple Channels to be created for an Actor.
- PlayerControllers on non-auth servers now have BeginPlay called with correct authority.
- Attempting to replicate unsupported types (such as TMap) results in an error rather than crashing the game.
- Generating schema when the schema database is locked by another process will no longer crash the editor.
- When the schema compiler fails, schema generation now displays an error.
- Fixed crash during initialization when running GenerateSchemaCommandlet.
- Generating schema after deleting the schema database now correctly triggers an initial schema generation.
- Streaming levels with query-based interest (QBI) enabled no longer produces errors if the player connection owns unreplicated actors.
- Fixed an issue that prevented player movement in a zoned deployment.
- Fixed an issue that caused queued incoming RPCs with unresolved references to never be processed.
- Muticast RPCs that are sent shortly after an actor is created are now correctly processed by all clients.
- When replicating an actor, the owner's Spatial position will no longer be used if it isn't replicated.
- Fixed a crash upon checking out an actor with a deleted static subobject.
- Fixed an issue where launching a cloud deployment with an invalid assembly name or deployment name wouldn't show a helpful error message.

## [`0.6.5`] - 2020-01-13
### Internal:
Features listed in the internal section are not ready to use but, in the spirit of open development, we detail every change we make to the GDK.
- **SpatialOS GDK for Unreal** > **Editor Settings** > **Region Settings** has been moved to **SpatialOS GDK for Unreal** > **Runtime Settings** > **Region Settings**.
- Local deployments can now be launched in China, when the **Region where services are located** is set to `CN`.

## [`0.6.4`] - 2019-12-13
### Bug fixes:
- The Inspector button in the SpatialOS GDK for Unreal toolbar now opens the correct URL.

## [`0.6.3`] - 2019-12-05
### Bug fixes:
- The C Worker SDK now communicates on port 443 instead of 444. This change is intended to protect your cloud deployments from DDoS attacks.

### Internal:
Features listed in the internal section are not ready to use but, in the spirit of open development, we detail every change we make to the GDK.
- The GDK is now compatible with the `CN` launch region. When Improbable's online services are fully working in China, they will work with this version of the GDK. You will be able to create SpatialOS Deployments in China by specifying the `CN` region in the Deployment Launcher.
- `Setup.bat` and `Setup.sh` both accept the `--china` flag, which will be required in order to run SpatialOS CLI commands in the `CN` region.
- **SpatialOS GDK for Unreal** > **Editor Settings** now contains a **Region Settings** section. You will be required to set **Region where services are located** to `CN` in order to create SpatialOS Deployments in China.

## [`0.6.2`] - 2019-10-10

- The GDK no longer relies on an ordering of entity and interest queries that is not guaranteed by the SpatialOS runtime.
- The multiserver offloading tutorial has been simplified and re-factored.

## [`0.6.1`] - 2019-08-15

### Features:
- The [Multiserver zoning shooter tutorial](https://docs.improbable.io/unreal/alpha/content/tutorials/multiserver-shooter/tutorial-multiserver-intro) has been updated to use the Example Project.

### Bug fixes:
- Simulated player launch configurations are no longer invalid when the GDK is installed as an Engine Plugin.
- RPCs that have been queued for execution for more than 1 second (the default value in `SpatialGDKSettings QueuedIncomingRPCWaitTime`) are now executed even if there are unresolved parameters. This stops unresolved parameters from blocking the execution queue.
- Offloading is no longer enabled by default in the Example Project. You can toggle offloading on using [these steps](https://docs.improbable.io/unreal/alpha/content/tutorials/offloading-tutorial/offloading-setup#step-4-enable-offloading).
- Guns no longer intermittently detatch from simulated players in the Example Project.
- Default cloud deployment settings are now correctly set. This means you don't need to manually reset them before doing a cloud deployment.

## [`0.6.0`] - 2019-07-31

### Breaking Changes:
* You must [re-build](https://docs.improbable.io/unreal/alpha/content/get-started/example-project/exampleproject-setup#step-4-build-the-dependencies-and-launch-the-project) your [Example Project](https://github.com/spatialos/UnrealGDKExampleProject) if you're upgrading it to `0.6.0`.
* This is the last GDK version to support Unreal Engine 4.20. You will need to upgrade your project to use Unreal Engine 4.22 (`4.22-SpatialOSUnrealGDK-release`) in order to continue receiving GDK releases and support.

### New Known Issues:
- Workers will sometimes not gain authority when quickly reconnecting to an existing deployment, resulting in a failure to spawn or simulate. When using the editor if you Play - Stop - Play in quick succession you can sometimes fail to launch correctly.

### Features:
- The GDK now uses SpatialOS `13.8.1`.
- Dynamic components are now supported. You can now dynamically attach and remove replicated subobjects to Actors.
- Local deployment startup time has been significantly reduced.
- Local deployments now start automatically when you select `Play`. This means you no longer need to select `Start` in the GDK toolbar before you select `Play` in the Unreal toolbar.
- If your schema has changed during a local deployment, the next time you select `Play` the deployment will automatically restart.
- Local deployments no longer run in a seperate Command Prompt. Logs from these deployments are now found in the Unreal Editor's Output Log.
- SpatialOS Runtime logs can now be found at `<GameRoot>\spatial\logs\localdeployment\<timestamp>\runtime.log`.
- An option to `Show spatial service button` has been added to the SpatialOS Settings menu. This button can be useful when debugging.
- Every time you open a GDK project in the Unreal Editor, 'spatial service' will be restarted. This ensures the service is always running in the correct SpatialOS project. You can disable this auto start feature via the new SpatialOS setting `Auto-start local deployment`.
- Added external schema code-generation tool for [non-Unreal server-worker types]({{urlRoot}}/content/non-unreal-server-worker-types). If you create non-Unreal server-worker types using the [SpatialOS Worker SDK](https://docs.improbable.io/reference/13.8/shared/sdks-and-data-overview) outside of the GDK and your Unreal Engine, you manually create [schema]({{urlRoot}/content/glossary#schema). Use the new [helper-script]({{urlRoot}}/content/helper-scripts) to generate Unreal code from manually-created schema; it enables your Unreal game code to interoperate with non-Unreal server-worker types.
- Added simulated player tools, which will allow you to create logic to simulate the behavior of real players. Simulated players can be used to scale test your game to hundreds of players. Simulated players can be launched locally as part of your development flow for quick iteration, as well as part of a separate "simulated player deployment" to connect a configurable amount of simulated players to your running game deployment.
- Factored out writing of Linux worker start scripts into a library, and added a standalone `WriteLinuxScript.exe` to _just_ write the launch script (for use in custom build pipelines).
- Added temporary MaxNetCullDistanceSquared to SpatialGDKSettings to prevent excessive net cull distances impacting runtime performance. Set to 0 to disable.
- Added `OnWorkerFlagsUpdated`, a delegate that can be directly bound to in C++. To bind via blueprints you can use the blueprint callable functions `BindToOnWorkerFlagsUpdated` and `UnbindToOnWorkerFlagsUpdated`. You can use `OnWorkerFlagsUpdated` to register when worker flag updates are received, which allows you to tweak values at deployment runtime.
- RPC frequency and payload size can now be tracked using console commands: `SpatialStartRPCMetrics` to start recording RPCs and `SpatialStopRPCMetrics` to stop recording and log the collected information. Using these commands will also start/stop RPC tracking on the server.
- Spatial now respects `bAlwaysRelevant` and clients will always checkout Actors that have `bAlwaysRelevant` set to true.

### Bug fixes:
- The `improbable` namespace has been renamed to `SpatialGDK`. This prevents namespace conflicts with the C++ SDK.
- Disconnected players no longer remain on the server until they time out if the client was shut down manually.
- Fixed support for relative paths as the engine association in your games .uproject file.
- RPCs on `NotSpatial` types are no longer queued forever and are now dropped instead.
- Fixed issue where an Actor's Spatial position was not updated if it had an owner that was not replicated.
- BeginPlay is now only called with authority on startup actors once per deployment.
- Fixed null pointer dereference crash when trying to initiate a Spatial connection without an existing one.
- URL options are now properly sent through to the server when doing a ClientTravel.
- The correct error message is now printed when the SchemaDatabase is missing.
- `StartEditor.bat` is now generated correctly when you build a server worker outside of editor.
- Fixed an issue with logging errored blueprints after garbage collection which caused an invalid pointer crash.
- Removed the ability to configure snapshot save folder. Snapshots should always be saved to `<ProjectRoot>/spatial/snapshots`. This prevents an issue with absolute paths being checked in which can break snapshot generation.
- Introduced a new module, `SpatialGDKServices`, on which `SpatialGDK` and `SpatilGDKEditorToolbar` now depend. This resolves a previously cyclic dependency.
- RPCs called before entity creation are now queued in case they cannot yet be executed. Previously they were simply dropped. These RPCs are also included in RPC metrics.
- RPCs are now guaranteed to arrive in the same order for a given actor and all of its subobjects on single-server deployments. This matches native Unreal behavior.

## [`0.5.0-preview`](https://github.com/spatialos/UnrealGDK/releases/tag/0.5.0-preview) - 2019-06-25
- Prevented `Spatial GDK Content` from appearing under Content Browser in the editor, as the GDK plugin does not contain any game content.

### Breaking Changes:
- If you are using Unreal Engine 4.22, the AutomationTool and UnrealBuildTool now require [.NET 4.6.2](https://dotnet.microsoft.com/download/dotnet-framework/net462).

### New Known Issues:

### Features:
- Unreal Engine 4.22 is now supported. You can find the 4.22 verson of our engine fork [here](https://github.com/improbableio/UnrealEngine/tree/4.22-SpatialOSUnrealGDK-release).
- Setup.bat can now take a project path as an argument. This allows the UnrealGDK to be installed as an Engine Plugin, pass the project path as the first variable if you are running Setup.bat from UnrealEngine/Engine/Plugins.
- Removed the need for setting the `UNREAL_HOME` environment variable. The build and setup scripts will now use your project's engine association to find the Unreal Engine directory. If an association is not set they will search parent directories looking for the 'Engine' folder.
- Added the `ASpatialMetricsDisplay` class, which you can use to view UnrealWorker stats as an overlay on the client.
- Added the runtime option `bEnableHandover`, which you can use to toggle property handover when running in non-zoned deployments.
- Added the runtime option `bEnableMetricsDisplay`, which you can use to auto spawn `ASpatialMetricsDisplay`, which is used to remote debug server metrics.
- Added the runtime option `bBatchSpatialPositionUpdates`, which you can use to batch spatial position updates to the runtime.
- Started using the [schema_compiler tool](https://docs.improbable.io/reference/13.8/shared/schema/introduction#using-the-schema-compiler-directly) to generate [schema descriptors](https://docs.improbable.io/reference/13.8/shared/flexible-project-layout/build-process/schema-descriptor-build-process#schema-descriptor-introduction) rather than relying on 'spatial local launch' to do this.
- Changed Interest so that NetCullDistanceSquared is used to define the distance from a player that the actor type is *interesting to* the player. This replaces CheckoutRadius which defined the distance that an actor is *interested in* other types. Requires engine update to remove the CheckoutRadius property which is no longer used.
- Added ActorInterestComponent that can be used to define interest queries that are more complex than a radius around the player position.
- Enabled new Development Authentication Flow
- Added new "worker" entities which are created for each server worker in a deployment so they correctly receive interest in the global state manager.
- Added support for spawning actors with ACLs configured for offloading using actor groups.
- Removed the references to the `Number of servers` slider in the Play in editor drop-down menu. The number of each server worker type to launch in PIE is now specified within the launch configuration in the `Spatial GDK Editor Settings` settings tab.
- Added `SpatialWorkerId` which is set to the worker ID when the worker associated to the `UGameInstance` connects.
- Added `USpatialStatics` helper blueprint library exposing functions for checking if SpatialOS networking is enabled, whether offloading is enabled, and more SpatialOS related checks.


### Bug fixes:
- BeginPlay is not called with authority when checking out entities from Spatial.
- Launching SpatialOS would fail if there was a space in the full directory path.
- GenerateSchemaAndSnapshots commandlet no longer runs a full schema generation for each map.
- Reliable RPC checking no longer breaks compatibility between development and shipping builds.
- Fixed an issue with schema name collisions.
- Running Schema (Full Scan) now clears generated schema files first.
- [Singleton actor's](https://docs.improbable.io/unreal/latest/content/singleton-actors#singleton-actors) authority and state now resumes correctly when reconnecting servers to snapshot.
- Retrying reliable RPCs with `UObject` arguments that were destroyed before the RPC was retried no longer causes a crash.
- Fixed path naming issues in setup.sh
- Fixed an assert/crash in `SpatialMetricsDisplay` that occurred when reloading a snapshot.
- Added Singleton and SingletonManager to query-based interest (QBI) constraints to fix issue preventing Test configuration builds from functioning correctly.
- Failing to `NetSerialize` a struct in spatial no longer causes a crash, it now prints a warning. This matches native Unreal behavior.
- Query response delegates now execute even if response status shows failure. This allows handlers to implement custom retry logic such as clients querying for the GSM.
- Fixed a crash where processing unreliable RPCs made assumption that the worker had authority over all entities in the SpatialOS op
- Ordering and reliability for single server RPCs on the same Actor are now guaranteed.

### External contributors:

In addition to all of the updates from Improbable, this release includes x improvements submitted by the incredible community of SpatialOS developers on GitHub! Thanks to these contributors:

* @cyberbibby

## [`0.4.2`](https://github.com/spatialos/UnrealGDK/releases/tag/0.4.2) - 2019-05-20

### New Known Issues:
- `BeginPlay()` is not called on all `WorldSettings` actors [#937](https://github.com/spatialos/UnrealGDK/issues/937)
- Replicated properties within `DEBUG` or `WITH_EDITORONLY_DATA` macros are not supported [#939](https://github.com/spatialos/UnrealGDK/issues/939)
- Client connections will be closed by the `ServerWorker` when using blueprint or C++ breakpoints during play-in-editor sessions [#940](https://github.com/spatialos/UnrealGDK/issues/940)
- Clients that connect after a Startup Actor (with `bNetLoadOnClient = true`) will not delete the Actor [#941](https://github.com/spatialos/UnrealGDK/issues/941)
- Generating schema while asset manager is asynchronously loading causes editor to crash [#944](https://github.com/spatialos/UnrealGDK/issues/944)

### Bug fixes:
- Adjusted dispatcher tickrate to reduce latency
- GenerateSchemaAndSnapshots commandlet no longer runs a full schema generation for each map.
- Launching SpatialOS would fail if there was a space in the full directory path.
- Fixed an issue with schema name collisions.
- Schema generation now respects "Directories to never cook".
- The editor no longer crashes during schema generation when the database is readonly.
- Replicating `UInterfaceProperty` no longer causes crashes.

## [`0.4.1`](https://github.com/spatialos/UnrealGDK/releases/tag/0.4.1) - 2019-05-01

### Bug fixes:
- Fixed an issue where schema components were sometimes generated with incorrect component IDs.

## [`0.4.0`](https://github.com/spatialos/UnrealGDK/releases/tag/0.4.0) - 2019-04-30

### Features:
- The GDK now uses SpatialOS `13.6.2`.
- Added this Changelog
- Added an error when unsupported replicated gameplay abilities are found in schema generation.
- Demoted various logs to Verbose in SpatialSender and SpatialReceiver
- You can now use the Project Settings window to pass [command line flags](https://docs.improbable.io/reference/latest/shared/spatial-cli/spatial-local-launch#spatial-local-launch) to local deployments launched from the GDK toolbar.
- You can now adjust the SpatialOS update frequency and the distance an action must move before we update its SpatialOS position.

### Bug fixes:
- The worker disconnection flow is now handled by `UEngine::OnNetworkFailure` rather than the existing `OnDisconnection` callback, which has been removed.
- Fix duplicated log messages in `spatial CLI` output when running in PIE.
- Fixed deserialization of strings from schema.
- Ensure that components added in blueprints are replicated.
- Fixed potential loading issue when attempting to load the SchemaDatabase asset.
- Add pragma once directive to header file.
- Schema files are now generated correctly for subobjects of the blueprint classes.
- Fixed being unable to launch SpatialOS if project path had spaces in it.
- Editor no longer crashes when setting LogSpatialSender to Verbose.
- Server-workers quickly restarted in the editor will connect to runtime correctly.
- Game no longer crashes when connecting to Spatial with async loading thread suspended.

## [`0.3.0`](https://github.com/spatialos/UnrealGDK/releases/tag/0.3.0) - 2019-04-04

### New Known Issues:
- Enabling Query Based Interest is needed for level streaming support, but this might affect performance in certain scenarios and is currently being investigated.
- Replicated `TimelineComponents` are not supported.

For current known issues, please visit [this](https://docs.improbable.io/unreal/alpha/known-issues) docs page

### Features:
- The default connection protocol is now TCP.
- Query Based Interest is now supported as an opt-in feature.
- Level streaming is now supported. You must enable Query Based Interest checkbox in the Runtime Settings to use level streaming.
- The GDK Toolbar now recognises when a local deployment is running, and contextually displays start and stop buttons. - (@DW-Sebastien)
- Added interface support for Unreal Engine 4.21 `UNetConnection`. - (@GeorgeR)
- Unreliable RPCs are now implemented using events instead of commands. This resolves associated performance issues.
- The `delete dynamic entities` setting now works when used in conjunction with multiple processes.
- You can now determine the type of a SpatialOS worker from within the game instance.
- Entity IDs are now reserved in batches instead of individually. This accelerates the creation of SpatialOS entities.
- You can now serialize and deserialize component data defined in external schema (schema that is not-generated by the Unreal GDK). You can use this to send and receive data, and edit snapshots.
- Improved logging during RPCs.

### Bug fixes:
- The GDK now automatically compiles all dirty blueprints before generating schema.
- Attempting to load a class which is not present in the schema database now causes the game to quit instead of crashing the entire editor.
- `Actor::ReplicateSubobjects` is now called in the replication flow. This means that Subobjects are now replicated correctly.
- Schema generation is no longer fatally halted when blueprints fail to compile.
- `AActor::TornOff` is now called when a `TearOff` event is received. This is in-line with the native implementation.
- References to objects within streaming levels, that are resolved before the level has streamed in, no longer cause defective behavior on the client.
- Attempting to replicate a `NonSpatial` actor no longer causes a crash.
- The SpatialOS Launcher now launches the correct game client, even when `UnrealCEFSubProcess.exe` is present in the assembly.
- Duplicate startup-actors are no longer created when a server-worker reconnects to a deployment.
- `BeginPlay` is no-longer called authoritatively when a server-worker reconnects to a deployment.
- Fast Array Serialization now operates correctly in conjunction with `GameplayAbilitySystem`.
- Reference parameters for RPCs are now correctly supported.
- Clients now load the map specified by the global state manager, rather than loading the `GameDefaultMap` _before_ querying the global state manager.
- Automatically generated launch configurations for deployments with a prime numbers of server-workers are now generated with the correct number of rows and columns.
- Generating schema for a level blueprint no longer deletes schema that has been generated for other levels.
- Deleting recently created actors no longer causes crashes.
- Having multiple EventGraphs no longer causes incorrect RPCs to be called.
- `TimerManager`, which is used by SpatialOS networking, is no longer subject to time dilation in the `World` instance.
- Clients no longer crash after being assigned multiple players.
- `GetWorkerFlag` can now be called from C++ classes.
- Pathless mapname arguments are now supported by the GDK commandlet.
- When `NotifyBeginPlay` is called, `BeginPlay` is no longer called on actors before their `Role` is correctly set.
- Deployments containing multiple server-workers no longer fails to initialize properly when launched through PIE with the `use single process` option unchecked.

### External contributors:

In addition to all of the updates from Improbable, this release includes 2 improvements submitted by the incredible community of SpatialOS developers on GitHub! Thanks to these contributors:

* @DW-Sebastien
* @GeorgeR

## [`0.2.0`](https://github.com/spatialos/UnrealGDK/releases/tag/0.2.0) - 2019-02-26

Startup actors revamp is merged! Snapshots are now simpler. Many bugfixes.

### New Known Issues:
- A warning about an out of date net driver is printed at startup of clients and server.

For current known issues, please visit [this](https://docs.improbable.io/unreal/alpha/known-issues) docs page

### Features:
- Actors placed in the level are no longer saved to the snapshot. They are instead spawned dynamically at the start of the game. This should fix quite a few issues such as missing references, and non-replicated instanced data being incorrectly set
- Pass player name and login options in the login URL
- Server will identify clients that have been disconnected from Spatial and trigger the cleanup on their NetConnection
- Exposed SpatialOS connection events in `USpatialNetDriver`
- Dynamic Component Ids now start from 10000, Gdk Components will now use 9999 - 0 to avoid future clashes
- Report an error during schema generation if a blueprint RPC has a "by reference" argument
- Launch configs can now be auto-generated to match the selected number of servers to launch from within the PIE editor
- Placeholder entities placed into the generated snapshot are now optional with a UI switch in the SpatialOS Settings
- Implemented updated functionality for UnrealGDKEditorCommandlet: Whenever loading a map for schema/snapshot generation, all sublevels will also be loaded before generation is started
	1. Will now loop through maps (skipping duplicates) during schema generation, to leverage the "iterative schema generation" feature
	2. Accepts an additional argument -MapPaths that can specify a collection of specific maps and/or directories (recursive) containing maps, delimited by semicolons. If not provided, defaults to "All maps in project"
	3. The paths passed in via -MapPaths are flexible

### Bug fixes:
- StartPlayInEditorGameInstance() now correctly call OnStart() on PIE_Client - (@DW-Sebastien)
- Redirect logging in the cloud to output to the correct file
- Changed type of key in `TMap` so Linux build will not give errors
- Disabled loopback of component updates
- Fix hanging on shutdown for PIE when disconnected from SpatialOS
- Fixed an issue which caused a character controller to not be destroyed when leaving the view of an observing client
- Fixed crash on multiserver PIE shutdown
- Fixed single-worker shutdown issues when launching SpatialOS through Unreal Engine 4 with Use - - Single Process unchecked in Play Options
- Fixed crash on closing client from cloud deployment
- Fix `DeleteDynamicEntities` not getting used correctly in shutdown
- Only call `BeginPlay()` on Actors if the World has begun play
- Fixed an issue with schema generation for the default GameMode
- Deleting the schema database reset the starting component ID
- Report invalid name errors during schema generation instead of when launching a deployment.
- `SchemaDatabase` can now be deleted and component ids will reset.
- `COND_InitialOnly` are only replicated once at the start
- Fixed a bug where standalone clients run locally would not connect to spatial
- Missing classes when connecting via a non-editor client
- Schema is now generated for classes that only have RPCs
- Fixed issue where properties won’t be replicated at the start of the game sometimes
- Fixed path bug when specifying snapshot output file in the settings
- Fixed up default connection flows
- Fixed issue will stale shadow data when crossing worker boundaries.
- Removed actors from replication consider list if Unreal server-worker is not authoritative over said actor
- Remove legacy flag "qos_max_unacked_pings_rate" in generated default config - (@DW-Sebastien)

### External contributors:
@DW-Sebastien

## [`0.1.0`](https://github.com/spatialos/UnrealGDK/releases/tag/0.1.0) - 2019-02-08

## Release Notes 0.1.0

Support for the new Player Auth APIs has been added and general stability improvements.

### New Known Issues:
Level streaming is currently not supported.
For other current known issues, please visit [this docs page](https://docs.improbable.io/unreal/alpha/known-issues).

### Features:
* Support for the new Player Auth APIs
* FUniqueNetId support
* Support for the new network protocol KCP
* Lazy loading of FClassInfo
* Augmented BuildWorker.bat to support additional UBT parameters
* Add IsValid() to FUnrealObjRef

### Bug fixes:
* Fixed critical errors related to Unresolved Objects
* Fixed a bug with Player State appearing to be null
* Fixed a bug related to Create Entity responses coming off the wire after a corresponding actor has been deleted
* Fixed a bug with activating actor components. We now check Initial Data for Actor components and only apply updates if `bReplicates` is true
* Fixed a bug when replicating a null list / array
* Fixed a crash with unresolved handover properties
* Changed RakNet to default network protocol temporarily to avoid performance issues with KCP
* Fixed a bug where cloud logging would not work correctly
