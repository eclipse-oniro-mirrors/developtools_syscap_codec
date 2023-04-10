/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _SYSCAP_DEFINE_H
#define _SYSCAP_DEFINE_H

#include <stdint.h>

#define SINGLE_SYSCAP_LEN (256 + 17)
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
typedef struct SystemCapabilityWithNum {
    char str[SINGLE_SYSCAP_LEN];
    uint16_t num;
} SyscapWithNum;

/*
 * New SyscapNum must be added last and
 * don't delete anyone, just comment after it.
 */
typedef enum SystemCapabilityNum {
    ACCOUNT_APPACCOUNT,
    ACCOUNT_OSACCOUNT,
    AI_AIENGINE,
    APPLICATIONS_CONTACTSDATA,
    BARRIERFREE_ACCESSIBILITY_CORE,
    BUNDLEMANAGER_BUNDLEFRAMEWORK,
    BUNDLEMANAGER_BUNDLETOOL,
    BUNDLEMANAGER_DISTRIBUTEDBUNDLEFRAMEWORK,
    BUNDLEMANAGER_ZLIB,
    COMMUNICATION_BLUETOOTH_CORE,
    COMMUNICATION_CONNECTEDTAG,
    COMMUNICATION_NETMANAGER_CORE,
    COMMUNICATION_NETMANAGER_EXTENSION, // abandoned
    COMMUNICATION_NETSTACK,
    COMMUNICATION_NFC_CORE,
    COMMUNICATION_SOFTBUS_CORE,
    COMMUNICATION_WIFI_CORE,
    COMMUNICATION_WIFI_P2P,
    COMMUNICATION_WIFI_STA,
    CUSTOMIZATION_CONFIGPOLICY,
    CUSTOMIZATION_ENTERPRISEDEVICEMANAGER,
    DISTRIBUTEDDATAMANAGER_DATAOBJECT_DISTRIBUTEDOBJECT,
    DISTRIBUTEDDATAMANAGER_DATASHARE_CONSUMER,
    DISTRIBUTEDDATAMANAGER_DATASHARE_CORE,
    DISTRIBUTEDDATAMANAGER_DATASHARE_PROVIDER,
    DISTRIBUTEDDATAMANAGER_KVSTORE_CORE,
    DISTRIBUTEDDATAMANAGER_KVSTORE_DISTRIBUTEDKVSTORE,
    DISTRIBUTEDDATAMANAGER_KVSTORE_LITE,
    DISTRIBUTEDDATAMANAGER_PREFERENCES_CORE,
    DISTRIBUTEDDATAMANAGER_RELATIONALSTORE_CORE,
    DISTRIBUTEDHARDWARE_DEVICEMANAGER,
    DISTRIBUTEDHARDWARE_DISTRIBUTEDCAMERA,
    DISTRIBUTEDHARDWARE_DISTRIBUTEDAUDIO,
    DISTRIBUTEDHARDWARE_DISTRIBUTEDHARDWAREFWK,
    DISTRIBUTEDHARDWARE_DISTRIBUTEDSCREEN,
    DISTRIBUTEDHARDWARE_DISTRIBUTEDINPUT,
    FILEMANAGEMENT_FILEMANAGERSERVICE,
    FILEMANAGEMENT_REMOTEFILESHARE,
    FILEMANAGEMENT_STORAGESERVICE_BACKUP,
    GLOBAL_RESOURCEMANAGER,
    GRAPHIC_GRAPHIC2D_EGL,
    GRAPHIC_GRAPHIC2D_GLES3,
    GRAPHIC_GRAPHIC2D_NATIVEDRAWING,
    GRAPHIC_GRAPHIC2D_NATIVEWINDOW,
    GRAPHIC_GRAPHIC2D_WEBGL,
    GRAPHIC_GRAPHIC2D_WEBGL2,
    GRAPHIC_SURFACE,
    GRAPHIC_UI,
    GRAPHIC_UTILS,
    GRAPHIC_WMS,
    HIVIEWDFX_HIAPPEVENT,
    HIVIEWDFX_HICHECKER,
    HIVIEWDFX_HIDUMPER,
    HIEVENTKLITE,
    HIVIEWDFX_HILOG,
    HIVIEWDFX_HILOGLITE,
    HIVIEWDFX_HIPROFILER_HIDEBUG,
    HIVIEWDFX_HISYSEVENT,
    HIVIEWDFX_HITRACE,
    HIVIEWDFX_HIVIEW,
    HIVIEWDFX_HIVIEW_FAULTLOGGER,
    HIVIEWDFX_HIVIEWLITE,
    GLOBAL_I18N,
    KERNEL_LINUX,
    KERNEL_LITEOS_A,
    KERNEL_LITEOS_M,
    MISCSERVICES_DOWNLOAD,
    MISCSERVICES_PASTEBOARD,
    MISCSERVICES_SCREENLOCK,
    MISCSERVICES_TIME,
    MISCSERVICES_UPLOAD,
    MISCSERVICES_WALLPAPER,
    MSDP_GEOFENCE,
    MSDP_MOTION,
    MSDP_MOVEMENT,
    MSDP_SPATIALAWARENESS,
    MSDP_TIMELINE,
    MULTIMEDIA_MEDIA_AUDIOPLAYER,
    MULTIMEDIA_MEDIA_AUDIORECORDER,
    MULTIMEDIA_MEDIA_AUDIODECODER,
    MULTIMEDIA_MEDIA_AUDIOENCODER,
    MULTIMEDIA_MEDIA_VIDEODECODER,
    MULTIMEDIA_MEDIA_VIDEOENCODER,
    MULTIMEDIA_MEDIA_CODECBASE,
    MULTIMEDIA_MEDIA_CORE,
    MULTIMEDIA_MEDIA_MUXER,
    MULTIMEDIA_IMAGE,
    MULTIMEDIA_IMAGE_CORE,
    MULTIMEDIA_IMAGE_IMAGESOURCE,
    MULTIMEDIA_IMAGE_IMAGEPACKER,
    MULTIMEDIA_IMAGE_IMAGERECEIVER,
    MULTIMEDIA_IMAGE_IMAGECREATOR,
    MULTIMEDIA_MEDIA_SPLITER,
    MULTIMEDIA_MEDIA_VIDEOPLAYER,
    MULTIMEDIA_MEDIA_VIDEORECORDER,
    MULTIMEDIA_AV_SESSION,
    MULTIMODALINPUT_INPUT, // abandoned
    NOTIFICATION_COMMONEVENT,
    NOTIFICATION_EMITTER,
    NOTIFICATION_NOTIFICATION,
    NOTIFICATION_REMINDERAGENT,
    POWERMANAGER_BATTERYMANAGER_CORE,
    POWERMANAGER_BATTERYMANAGER_EXTENSION,
    POWERMANAGER_BATTERYMANAGER_LITE,
    POWERMANAGER_BATTERYSTATISTICS,
    POWERMANAGER_DISPLAYPOWERMANAGER,
    POWERMANAGER_POWERMANAGER_CORE,
    POWERMANAGER_POWERMANAGER_EXTENSION,
    POWERMANAGER_POWERMANAGER_LITE,
    POWERMANAGER_THERMALMANAGER,
    RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_CONTINUOUSTASK,
    RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_TRANSIENTTASK,
    RESOURCESCHEDULE_USAGESTATISTICS_APP,
    RESOURCESCHEDULE_USAGESTATISTICS_APPGROUP,
    RESOURCESCHEDULE_WORKSCHEDULER,
    SECURITY_ACCESSTOKEN,
    SECURITY_DLPPERMISSIONSERVICE,
    SECURITY_APPVERIFY,
    SECURITY_DATATRANSITMANAGER,
    SECURITY_DEVICEAUTH,
    SECURITY_DEVICESECURITYLEVEL,
    SECURITY_HUKS,
    SENSORS_MEDICAL_SENSOR,
    SENSORS_MISCDEVICE,
    SENSORS_SENSOR,
    SENSORS_SENSOR_LITE,
    TELEPHONY_CALLMANAGER,
    TELEPHONY_CELLULARCALL,
    TELEPHONY_CELLULARDATA,
    TELEPHONY_CORESERVICE,
    TELEPHONY_DATASTORAGE,
    TELEPHONY_DCALL,
    TELEPHONY_SMSMMS,
    TELEPHONY_STATEREGISTRY,
    TEST_UITEST,
    TEST_WUKONG,
    UPDATE_DUPDATE_ENGINE,
    UPDATER_RAW,
    UPDATE_UPDATESERVICE,
    USB_USBMANAGER,
    USERIAM_USERAUTH_CORE,
    USERIAM_USERAUTH_FACEAUTH,
    USERIAM_USERAUTH_FINGERPRINTAUTH,
    USERIAM_USERAUTH_PINAUTH,
    ARKUI_UIAPPEARANCE,
    SECURITY_CERTIFICATE_MANAGER,
    GRAPHIC_GRAPHIC2D_VULKAN,
    LOCATION_LOCATION_CORE,
    LOCATION_LOCATION_GNSS,
    LOCATION_LOCATION_GEOFENCE,
    LOCATION_LOCATION_GEOCODER,
    LOCATION_LOCATION_LITE,
    DEVELOPTOOLS_SYSCAP,
    COMMUNICATION_BLUETOOTH_LITE,
    MULTIMEDIA_AUDIO_CORE,
    MULTIMEDIA_AUDIO_RENDERER,
    MULTIMEDIA_AUDIO_CAPTURER,
    MULTIMEDIA_AUDIO_DEVICE,
    MULTIMEDIA_AUDIO_VOLUME,
    MULTIMEDIA_AUDIO_COMMUNICATION,
    MULTIMEDIA_CAMERA_CORE,
    BARRIERFREE_ACCESSIBILITY_HEARING,
    BARRIERFREE_ACCESSIBILITY_VISION,
    ABILITY_ABILITYBASE,
    ABILITY_ABILITYRUNTIME_CORE,
    ABILITY_ABILITYRUNTIME_FAMODEL,
    ABILITY_ABILITYRUNTIME_ABILITYCORE,
    ABILITY_ABILITYRUNTIME_MISSION,
    ABILITY_ABILITYTOOLS_ABILITYASSISTANT,
    ABILITY_DISTRIBUTEDABILITYMANAGER,
    ABILITY_FORM,
    COMMUNICATION_WIFI_AP_CORE,
    COMMUNICATION_WIFI_AP_EXTENSION,
    ARKUI_ARKUI_LITE,
    ARKUI_ARKUI_FULL,
    ARKUI_ARKUI_LIBUV,
    ARKUI_ARKUI_NAPI,
    MISCSERVICES_INPUTMETHOD_FRAMEWORK,
    UTILS_LANG,
    WEB_WEBVIEW_CORE,
    GRAPHIC_GRAPHIC2D_COLOR_MANAGER_CORE,
    WINDOWMANAGER_WINDOWMANAGER_CORE,
    MULTIMODALINPUT_INPUT_INPUTCONSUMER,
    MULTIMODALINPUT_INPUT_INPUTDEVICE,
    MULTIMODALINPUT_INPUT_CORE,
    MULTIMODALINPUT_INPUT_INPUTSIMULATOR,
    MULTIMODALINPUT_INPUT_INPUTMONITOR,
    MULTIMODALINPUT_INPUT_POINTER,
    COMMUNICATION_IPC_CORE,
    STARTUP_SYSTEMINFO,
    AI_MINDSPORE,
    FILEMANAGEMENT_STORAGESERVICE_SPATIALSTATISTICS,
    FILEMANAGEMENT_STORAGESERVICE_VOLUME,
    FILEMANAGEMENT_STORAGESERVICE_ENCRYPTION,
    FILEMANAGEMENT_FILE_FILEIO,
    FILEMANAGEMENT_FILE_ENVIRONMENT,
    FILEMANAGEMENT_FILE_DISTRIBUTEDFILE,
    FILEMANAGEMENT_APPFILESERVICE,
    FILEMANAGEMENT_USERFILESERVICE,
    MULTIMEDIA_MEDIALIBRARY_CORE,
    MULTIMEDIA_MEDIALIBRARY_DISTRIBUTEDCORE,
    APPLICATIONS_CONTACTS,
    SECURITY_CIPHER,
    APPLICATIONS_SETTINGS_CORE,
    FILEMANAGEMENT_USERFILEMANAGER_CORE,
    FILEMANAGEMENT_USERFILEMANAGER_DISTRIBUTEDCORE,
    RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_EFFICIENCYRESOURCESAPPLY,
    SECURITY_SECURITYGUARD,
    SECURITY_CRYPTOFRAMEWORK,
    MULTIMODALINPUT_INPUT_COOPERATOR,
    ABILITY_ABILITYRUNTIME_QUICKFIX,
    SECURITY_DLPCREDENTIALSERVICE,
    BUNDLEMANAGER_BUNDLEFRAMEWORK_APPCONTROL,
    BUNDLEMANAGER_BUNDLEFRAMEWORK_CORE,
    BUNDLEMANAGER_BUNDLEFRAMEWORK_FREEINSTALL,
    BUNDLEMANAGER_BUNDLEFRAMEWORK_LAUNCHER,
    BUNDLEMANAGER_BUNDLEFRAMEWORK_DEFAULTAPP,
    BUNDLEMANAGER_BUNDLEFRAMEWORK_RESOURCE,
    MULTIMEDIA_AVSESSION_CORE,
    MULTIMEDIA_AVSESSION_MANAGER,
    MULTIMEDIA_AUDIO_TONE,
    MULTIMEDIA_AUDIO_INTERRUPT,
    CLOUD_AAID,
    MSDP_DEVICESTATUS_STATIONARY,
    COMMUNICATION_NETMANAGER_ETHERNET,
    COMMUNICATION_NETMANAGER_NETSHARING,
    COMMUNICATION_NETMANAGER_MDNS,
    COMMUNICATION_NETMANAGER_VPN,
    COMMUNICATION_NFC_TAG,
    COMMUNICATION_NFC_CARDEMULATION,
    SECURITY_CERT,
    CLOUD_OAID,
    XTS_DEVICEATTEST,
    XTS_DEVICEATTESTLITE,
    MULTIMEDIA_MEDIA_AVPLAYER,
    MULTIMEDIA_MEDIA_AVRECORDER,
    PRINT_PRINT_FRAMEWORK,
    BUNDLEMANAGER_BUNDLEFRAMEWORK_OVERLAY,
    CLOUD_PUSH,
    MULTIMEDIA_SYSTEMSOUND_CORE,
    COMMUNICATION_SECURE_ELEMENT,
    DISTRIBUTEDDATAMANAGER_UDMF_CORE,
    FILEMANAGEMENT_DISTRIBUTEDFILESERVICE_CLOUDSYNCMANAGER,
    MULTIMODALINPUT_INPUT_SHORTKEY,
    // Add before here
    SYSCAP_NUM_MAX = 960
} SyscapNum;


/* Sort by SyscapNum */
const static SyscapWithNum g_arraySyscap[] = {
    {"SystemCapability.Account.AppAccount", ACCOUNT_APPACCOUNT},
    {"SystemCapability.Account.OsAccount", ACCOUNT_OSACCOUNT},
    {"SystemCapability.Ai.AiEngine", AI_AIENGINE},
    {"SystemCapability.Applications.ContactsData", APPLICATIONS_CONTACTSDATA},
    {"SystemCapability.BarrierFree.Accessibility.Core", BARRIERFREE_ACCESSIBILITY_CORE},
    {"SystemCapability.BundleManager.BundleFramework", BUNDLEMANAGER_BUNDLEFRAMEWORK},
    {"SystemCapability.BundleManager.BundleTool", BUNDLEMANAGER_BUNDLETOOL},
    {"SystemCapability.BundleManager.DistributedBundleFramework", BUNDLEMANAGER_DISTRIBUTEDBUNDLEFRAMEWORK},
    {"SystemCapability.BundleManager.Zlib", BUNDLEMANAGER_ZLIB},
    {"SystemCapability.Communication.Bluetooth.Core", COMMUNICATION_BLUETOOTH_CORE},
    {"SystemCapability.Communication.ConnectedTag", COMMUNICATION_CONNECTEDTAG},
    {"SystemCapability.Communication.NetManager.Core", COMMUNICATION_NETMANAGER_CORE},
    {"SystemCapability.Communication.NetManager.Extension", COMMUNICATION_NETMANAGER_EXTENSION},
    {"SystemCapability.Communication.NetStack", COMMUNICATION_NETSTACK},
    {"SystemCapability.Communication.NFC.Core", COMMUNICATION_NFC_CORE},
    {"SystemCapability.Communication.SoftBus.Core", COMMUNICATION_SOFTBUS_CORE},
    {"SystemCapability.Communication.WiFi.Core", COMMUNICATION_WIFI_CORE},
    {"SystemCapability.Communication.WiFi.P2P", COMMUNICATION_WIFI_P2P},
    {"SystemCapability.Communication.WiFi.STA", COMMUNICATION_WIFI_STA},
    {"SystemCapability.Customization.ConfigPolicy", CUSTOMIZATION_CONFIGPOLICY},
    {"SystemCapability.Customization.EnterpriseDeviceManager", CUSTOMIZATION_ENTERPRISEDEVICEMANAGER},
    {"SystemCapability.DistributedDataManager.DataObject.DistributedObject",
        DISTRIBUTEDDATAMANAGER_DATAOBJECT_DISTRIBUTEDOBJECT},
    {"SystemCapability.DistributedDataManager.DataShare.Consumer", DISTRIBUTEDDATAMANAGER_DATASHARE_CONSUMER},
    {"SystemCapability.DistributedDataManager.DataShare.Core", DISTRIBUTEDDATAMANAGER_DATASHARE_CORE},
    {"SystemCapability.DistributedDataManager.DataShare.Provider", DISTRIBUTEDDATAMANAGER_DATASHARE_PROVIDER},
    {"SystemCapability.DistributedDataManager.KVStore.Core", DISTRIBUTEDDATAMANAGER_KVSTORE_CORE},
    {"SystemCapability.DistributedDataManager.KVStore.DistributedKVStore",
        DISTRIBUTEDDATAMANAGER_KVSTORE_DISTRIBUTEDKVSTORE},
    {"SystemCapability.DistributedDataManager.KVStore.Lite", DISTRIBUTEDDATAMANAGER_KVSTORE_LITE},
    {"SystemCapability.DistributedDataManager.Preferences.Core", DISTRIBUTEDDATAMANAGER_PREFERENCES_CORE},
    {"SystemCapability.DistributedDataManager.RelationalStore.Core", DISTRIBUTEDDATAMANAGER_RELATIONALSTORE_CORE},
    {"SystemCapability.DistributedHardware.DeviceManager", DISTRIBUTEDHARDWARE_DEVICEMANAGER},
    {"SystemCapability.DistributedHardware.DistributedCamera", DISTRIBUTEDHARDWARE_DISTRIBUTEDCAMERA},
    {"SystemCapability.DistributedHardware.DistributedAudio", DISTRIBUTEDHARDWARE_DISTRIBUTEDAUDIO},
    {"SystemCapability.DistributedHardware.DistributedHardwareFWK", DISTRIBUTEDHARDWARE_DISTRIBUTEDHARDWAREFWK},
    {"SystemCapability.DistributedHardware.DistributedScreen", DISTRIBUTEDHARDWARE_DISTRIBUTEDSCREEN},
    {"SystemCapability.DistributedHardware.DistributedInput", DISTRIBUTEDHARDWARE_DISTRIBUTEDINPUT},
    {"SystemCapability.FileManagement.FileManagerService", FILEMANAGEMENT_FILEMANAGERSERVICE},
    {"SystemCapability.FileManagement.RemoteFileShare", FILEMANAGEMENT_REMOTEFILESHARE},
    {"SystemCapability.FileManagement.StorageService.Backup", FILEMANAGEMENT_STORAGESERVICE_BACKUP},
    {"SystemCapability.Global.ResourceManager", GLOBAL_RESOURCEMANAGER},
    {"SystemCapability.Graphic.Graphic2D.EGL", GRAPHIC_GRAPHIC2D_EGL},
    {"SystemCapability.Graphic.Graphic2D.GLES3", GRAPHIC_GRAPHIC2D_GLES3},
    {"SystemCapability.Graphic.Graphic2D.NativeDrawing", GRAPHIC_GRAPHIC2D_NATIVEDRAWING},
    {"SystemCapability.Graphic.Graphic2D.NativeWindow", GRAPHIC_GRAPHIC2D_NATIVEWINDOW},
    {"SystemCapability.Graphic.Graphic2D.WebGL", GRAPHIC_GRAPHIC2D_WEBGL},
    {"SystemCapability.Graphic.Graphic2D.WebGL2", GRAPHIC_GRAPHIC2D_WEBGL2},
    {"SystemCapability.Graphic.Surface", GRAPHIC_SURFACE},
    {"SystemCapability.Graphic.UI", GRAPHIC_UI},
    {"SystemCapability.Graphic.Utils", GRAPHIC_UTILS},
    {"SystemCapability.Graphic.Wms", GRAPHIC_WMS},
    {"SystemCapability.HiviewDFX.HiAppEvent", HIVIEWDFX_HIAPPEVENT},
    {"SystemCapability.HiviewDFX.HiChecker", HIVIEWDFX_HICHECKER},
    {"SystemCapability.HiviewDFX.HiDumper", HIVIEWDFX_HIDUMPER},
    {"SystemCapability.HiviewDFX.HiEventLite", HIEVENTKLITE},
    {"SystemCapability.HiviewDFX.HiLog", HIVIEWDFX_HILOG},
    {"SystemCapability.HiviewDFX.HiLogLite", HIVIEWDFX_HILOGLITE},
    {"SystemCapability.HiviewDFX.HiProfiler.HiDebug", HIVIEWDFX_HIPROFILER_HIDEBUG},
    {"SystemCapability.HiviewDFX.HiSysEvent", HIVIEWDFX_HISYSEVENT},
    {"SystemCapability.HiviewDFX.HiTrace", HIVIEWDFX_HITRACE},
    {"SystemCapability.HiviewDFX.Hiview", HIVIEWDFX_HIVIEW},
    {"SystemCapability.HiviewDFX.Hiview.FaultLogger", HIVIEWDFX_HIVIEW_FAULTLOGGER},
    {"SystemCapability.HiviewDFX.HiviewLite", HIVIEWDFX_HIVIEWLITE},
    {"SystemCapability.Global.I18n", GLOBAL_I18N},
    {"SystemCapability.Kernel.Linux", KERNEL_LINUX},
    {"SystemCapability.Kernel.liteos-a", KERNEL_LITEOS_A},
    {"SystemCapability.Kernel.liteos-m", KERNEL_LITEOS_M},
    {"SystemCapability.MiscServices.Download", MISCSERVICES_DOWNLOAD},
    {"SystemCapability.MiscServices.Pasteboard", MISCSERVICES_PASTEBOARD},
    {"SystemCapability.MiscServices.ScreenLock", MISCSERVICES_SCREENLOCK},
    {"SystemCapability.MiscServices.Time", MISCSERVICES_TIME},
    {"SystemCapability.MiscServices.Upload", MISCSERVICES_UPLOAD},
    {"SystemCapability.MiscServices.Wallpaper", MISCSERVICES_WALLPAPER},
    {"SystemCapability.Msdp.Geofence", MSDP_GEOFENCE},
    {"SystemCapability.Msdp.Motion", MSDP_MOTION},
    {"SystemCapability.Msdp.Movement", MSDP_MOVEMENT},
    {"SystemCapability.Msdp.SpatialAwareness", MSDP_SPATIALAWARENESS},
    {"SystemCapability.Msdp.Timeline", MSDP_TIMELINE},
    {"SystemCapability.Multimedia.Media.AudioPlayer", MULTIMEDIA_MEDIA_AUDIOPLAYER},
    {"SystemCapability.Multimedia.Media.AudioRecorder", MULTIMEDIA_MEDIA_AUDIORECORDER},
    {"SystemCapability.Multimedia.Media.AudioDecoder", MULTIMEDIA_MEDIA_AUDIODECODER},
    {"SystemCapability.Multimedia.Media.AudioEncoder", MULTIMEDIA_MEDIA_AUDIOENCODER},
    {"SystemCapability.Multimedia.Media.VideoDecoder", MULTIMEDIA_MEDIA_VIDEODECODER},
    {"SystemCapability.Multimedia.Media.VideoEncoder", MULTIMEDIA_MEDIA_VIDEOENCODER},
    {"SystemCapability.Multimedia.Media.CodecBase", MULTIMEDIA_MEDIA_CODECBASE},
    {"SystemCapability.Multimedia.Media.Core", MULTIMEDIA_MEDIA_CORE},
    {"SystemCapability.Multimedia.Media.Muxer", MULTIMEDIA_MEDIA_MUXER},
    {"SystemCapability.Multimedia.Image", MULTIMEDIA_IMAGE},
    {"SystemCapability.Multimedia.Image.Core", MULTIMEDIA_IMAGE_CORE},
    {"SystemCapability.Multimedia.Image.ImageSource", MULTIMEDIA_IMAGE_IMAGESOURCE},
    {"SystemCapability.Multimedia.Image.ImagePacker", MULTIMEDIA_IMAGE_IMAGEPACKER},
    {"SystemCapability.Multimedia.Image.ImageReceiver", MULTIMEDIA_IMAGE_IMAGERECEIVER},
    {"SystemCapability.Multimedia.Image.ImageCreator", MULTIMEDIA_IMAGE_IMAGECREATOR},
    {"SystemCapability.Multimedia.Media.Spliter", MULTIMEDIA_MEDIA_SPLITER},
    {"SystemCapability.Multimedia.Media.VideoPlayer", MULTIMEDIA_MEDIA_VIDEOPLAYER},
    {"SystemCapability.Multimedia.Media.VideoRecorder", MULTIMEDIA_MEDIA_VIDEORECORDER},
    {"SystemCapability.Multimedia.AVSession", MULTIMEDIA_AV_SESSION},
    {"SystemCapability.multimodalinput.input", MULTIMODALINPUT_INPUT},
    {"SystemCapability.Notification.CommonEvent", NOTIFICATION_COMMONEVENT},
    {"SystemCapability.Notification.Emitter", NOTIFICATION_EMITTER},
    {"SystemCapability.Notification.Notification", NOTIFICATION_NOTIFICATION},
    {"SystemCapability.Notification.ReminderAgent", NOTIFICATION_REMINDERAGENT},
    {"SystemCapability.PowerManager.BatteryManager.Core", POWERMANAGER_BATTERYMANAGER_CORE},
    {"SystemCapability.PowerManager.BatteryManager.Extension", POWERMANAGER_BATTERYMANAGER_EXTENSION},
    {"SystemCapability.PowerManager.BatteryManager.Lite", POWERMANAGER_BATTERYMANAGER_LITE},
    {"SystemCapability.PowerManager.BatteryStatistics", POWERMANAGER_BATTERYSTATISTICS},
    {"SystemCapability.PowerManager.DisplayPowerManager", POWERMANAGER_DISPLAYPOWERMANAGER},
    {"SystemCapability.PowerManager.PowerManager.Core", POWERMANAGER_POWERMANAGER_CORE},
    {"SystemCapability.PowerManager.PowerManager.Extension", POWERMANAGER_POWERMANAGER_EXTENSION},
    {"SystemCapability.PowerManager.PowerManager.Lite", POWERMANAGER_POWERMANAGER_LITE},
    {"SystemCapability.PowerManager.ThermalManager", POWERMANAGER_THERMALMANAGER},
    {"SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask",
        RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_CONTINUOUSTASK},
    {"SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask",
        RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_TRANSIENTTASK},
    {"SystemCapability.ResourceSchedule.UsageStatistics.App", RESOURCESCHEDULE_USAGESTATISTICS_APP},
    {"SystemCapability.ResourceSchedule.UsageStatistics.AppGroup", RESOURCESCHEDULE_USAGESTATISTICS_APPGROUP},
    {"SystemCapability.ResourceSchedule.WorkScheduler", RESOURCESCHEDULE_WORKSCHEDULER},
    {"SystemCapability.Security.AccessToken", SECURITY_ACCESSTOKEN},
    {"SystemCapability.Security.DlpPermissionService", SECURITY_DLPPERMISSIONSERVICE},
    {"SystemCapability.Security.AppVerify", SECURITY_APPVERIFY},
    {"SystemCapability.Security.DataTransitManager", SECURITY_DATATRANSITMANAGER},
    {"SystemCapability.Security.DeviceAuth", SECURITY_DEVICEAUTH},
    {"SystemCapability.Security.DeviceSecurityLevel", SECURITY_DEVICESECURITYLEVEL},
    {"SystemCapability.Security.Huks", SECURITY_HUKS},
    {"SystemCapability.Sensors.Medical.Sensor", SENSORS_MEDICAL_SENSOR},
    {"SystemCapability.Sensors.MiscDevice", SENSORS_MISCDEVICE},
    {"SystemCapability.Sensors.Sensor", SENSORS_SENSOR},
    {"SystemCapability.Sensors.Sensor.Lite", SENSORS_SENSOR_LITE},
    {"SystemCapability.Telephony.CallManager", TELEPHONY_CALLMANAGER},
    {"SystemCapability.Telephony.CellularCall", TELEPHONY_CELLULARCALL},
    {"SystemCapability.Telephony.CellularData", TELEPHONY_CELLULARDATA},
    {"SystemCapability.Telephony.CoreService", TELEPHONY_CORESERVICE},
    {"SystemCapability.Telephony.DataStorage", TELEPHONY_DATASTORAGE},
    {"SystemCapability.Telephony.DCall", TELEPHONY_DCALL},
    {"SystemCapability.Telephony.SmsMms", TELEPHONY_SMSMMS},
    {"SystemCapability.Telephony.StateRegistry", TELEPHONY_STATEREGISTRY},
    {"SystemCapability.Test.UiTest", TEST_UITEST},
    {"SystemCapability.Test.WuKong", TEST_WUKONG},
    {"SystemCapability.Update.Dupdate_engine", UPDATE_DUPDATE_ENGINE},
    {"SystemCapability.Updater.Raw", UPDATER_RAW},
    {"SystemCapability.Update.UpdateService", UPDATE_UPDATESERVICE},
    {"SystemCapability.USB.USBManager", USB_USBMANAGER},
    {"SystemCapability.UserIAM.UserAuth.Core", USERIAM_USERAUTH_CORE},
    {"SystemCapability.UserIAM.UserAuth.FaceAuth", USERIAM_USERAUTH_FACEAUTH},
    {"SystemCapability.UserIAM.UserAuth.FingerprintAuth", USERIAM_USERAUTH_FINGERPRINTAUTH},
    {"SystemCapability.UserIAM.UserAuth.PinAuth", USERIAM_USERAUTH_PINAUTH},
    {"SystemCapability.ArkUI.UiAppearance", ARKUI_UIAPPEARANCE},
    {"SystemCapability.Security.CertificateManager", SECURITY_CERTIFICATE_MANAGER},
    {"SystemCapability.Graphic.Graphic2D.Vulkan", GRAPHIC_GRAPHIC2D_VULKAN},
    {"SystemCapability.Location.Location.Core", LOCATION_LOCATION_CORE},
    {"SystemCapability.Location.Location.Gnss", LOCATION_LOCATION_GNSS},
    {"SystemCapability.Location.Location.Geofence", LOCATION_LOCATION_GEOFENCE},
    {"SystemCapability.Location.Location.Geocoder", LOCATION_LOCATION_GEOCODER},
    {"SystemCapability.Location.Location.Lite", LOCATION_LOCATION_LITE},
    {"SystemCapability.Developtools.Syscap", DEVELOPTOOLS_SYSCAP},
    {"SystemCapability.Communication.Bluetooth.Lite", COMMUNICATION_BLUETOOTH_LITE},
    {"SystemCapability.Multimedia.Audio.Core", MULTIMEDIA_AUDIO_CORE},
    {"SystemCapability.Multimedia.Audio.Renderer", MULTIMEDIA_AUDIO_RENDERER},
    {"SystemCapability.Multimedia.Audio.Capturer", MULTIMEDIA_AUDIO_CAPTURER},
    {"SystemCapability.Multimedia.Audio.Device", MULTIMEDIA_AUDIO_DEVICE},
    {"SystemCapability.Multimedia.Audio.Volume", MULTIMEDIA_AUDIO_VOLUME},
    {"SystemCapability.Multimedia.Audio.Communication", MULTIMEDIA_AUDIO_COMMUNICATION},
    {"SystemCapability.Multimedia.Camera.Core", MULTIMEDIA_CAMERA_CORE},
    {"SystemCapability.BarrierFree.Accessibility.Hearing", BARRIERFREE_ACCESSIBILITY_HEARING},
    {"SystemCapability.BarrierFree.Accessibility.Vision", BARRIERFREE_ACCESSIBILITY_VISION},
    {"SystemCapability.Ability.AbilityBase", ABILITY_ABILITYBASE},
    {"SystemCapability.Ability.AbilityRuntime.Core", ABILITY_ABILITYRUNTIME_CORE},
    {"SystemCapability.Ability.AbilityRuntime.FAModel", ABILITY_ABILITYRUNTIME_FAMODEL},
    {"SystemCapability.Ability.AbilityRuntime.AbilityCore", ABILITY_ABILITYRUNTIME_ABILITYCORE},
    {"SystemCapability.Ability.AbilityRuntime.Mission", ABILITY_ABILITYRUNTIME_MISSION},
    {"SystemCapability.Ability.AbilityTools.AbilityAssistant", ABILITY_ABILITYTOOLS_ABILITYASSISTANT},
    {"SystemCapability.Ability.DistributedAbilityManager", ABILITY_DISTRIBUTEDABILITYMANAGER},
    {"SystemCapability.Ability.Form", ABILITY_FORM},
    {"SystemCapability.Communication.WiFi.AP.Core", COMMUNICATION_WIFI_AP_CORE},
    {"SystemCapability.Communication.WiFi.AP.Extension", COMMUNICATION_WIFI_AP_EXTENSION},
    {"SystemCapability.ArkUI.ArkUI.Lite", ARKUI_ARKUI_LITE},
    {"SystemCapability.ArkUI.ArkUI.Full", ARKUI_ARKUI_FULL},
    {"SystemCapability.ArkUI.ArkUI.Libuv", ARKUI_ARKUI_LIBUV},
    {"SystemCapability.ArkUI.ArkUI.Napi", ARKUI_ARKUI_NAPI},
    {"SystemCapability.MiscServices.InputMethodFramework", MISCSERVICES_INPUTMETHOD_FRAMEWORK},
    {"SystemCapability.Utils.Lang", UTILS_LANG},
    {"SystemCapability.Web.Webview.Core", WEB_WEBVIEW_CORE},
    {"SystemCapability.Graphic.Graphic2D.ColorManager.Core", GRAPHIC_GRAPHIC2D_COLOR_MANAGER_CORE},
    {"SystemCapability.WindowManager.WindowManager.Core", WINDOWMANAGER_WINDOWMANAGER_CORE},
    {"SystemCapability.MultimodalInput.Input.InputConsumer", MULTIMODALINPUT_INPUT_INPUTCONSUMER},
    {"SystemCapability.MultimodalInput.Input.InputDevice", MULTIMODALINPUT_INPUT_INPUTDEVICE},
    {"SystemCapability.MultimodalInput.Input.Core", MULTIMODALINPUT_INPUT_CORE},
    {"SystemCapability.MultimodalInput.Input.InputSimulator", MULTIMODALINPUT_INPUT_INPUTSIMULATOR},
    {"SystemCapability.MultimodalInput.Input.InputMonitor", MULTIMODALINPUT_INPUT_INPUTMONITOR},
    {"SystemCapability.MultimodalInput.Input.Pointer", MULTIMODALINPUT_INPUT_POINTER},
    {"SystemCapability.Communication.IPC.Core", COMMUNICATION_IPC_CORE},
    {"SystemCapability.Startup.SystemInfo", STARTUP_SYSTEMINFO},
    {"SystemCapability.Ai.MindSpore", AI_MINDSPORE},
    {"SystemCapability.FileManagement.StorageService.SpatialStatistics",
        FILEMANAGEMENT_STORAGESERVICE_SPATIALSTATISTICS},
    {"SystemCapability.FileManagement.StorageService.Volume", FILEMANAGEMENT_STORAGESERVICE_VOLUME},
    {"SystemCapability.FileManagement.StorageService.Encryption", FILEMANAGEMENT_STORAGESERVICE_ENCRYPTION},
    {"SystemCapability.FileManagement.File.FileIO", FILEMANAGEMENT_FILE_FILEIO},
    {"SystemCapability.FileManagement.File.Environment", FILEMANAGEMENT_FILE_ENVIRONMENT},
    {"SystemCapability.FileManagement.File.DistributedFile", FILEMANAGEMENT_FILE_DISTRIBUTEDFILE},
    {"SystemCapability.FileManagement.AppFileService", FILEMANAGEMENT_APPFILESERVICE},
    {"SystemCapability.FileManagement.UserFileService", FILEMANAGEMENT_USERFILESERVICE},
    {"SystemCapability.Multimedia.MediaLibrary.Core", MULTIMEDIA_MEDIALIBRARY_CORE},
    {"SystemCapability.Multimedia.MediaLibrary.DistributedCore", MULTIMEDIA_MEDIALIBRARY_DISTRIBUTEDCORE},
    {"SystemCapability.Applications.Contacts", APPLICATIONS_CONTACTS},
    {"SystemCapability.Security.Cipher", SECURITY_CIPHER},
    {"SystemCapability.Applications.settings.Core", APPLICATIONS_SETTINGS_CORE},
    {"SystemCapability.FileManagement.UserFileManager.Core", FILEMANAGEMENT_USERFILEMANAGER_CORE},
    {"SystemCapability.FileManagement.UserFileManager.DistributedCore", FILEMANAGEMENT_USERFILEMANAGER_DISTRIBUTEDCORE},
    {"SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply",
        RESOURCESCHEDULE_BACKGROUNDTASKMANAGER_EFFICIENCYRESOURCESAPPLY},
    {"SystemCapability.Security.SecurityGuard", SECURITY_SECURITYGUARD},
    {"SystemCapability.Security.CryptoFramework", SECURITY_CRYPTOFRAMEWORK},
    {"SystemCapability.MultimodalInput.Input.Cooperator", MULTIMODALINPUT_INPUT_COOPERATOR},
    {"SystemCapability.Ability.AbilityRuntime.QuickFix", ABILITY_ABILITYRUNTIME_QUICKFIX},
    {"SystemCapability.Security.DlpCredentialService", SECURITY_DLPCREDENTIALSERVICE},
    {"SystemCapability.BundleManager.BundleFramework.AppControl", BUNDLEMANAGER_BUNDLEFRAMEWORK_APPCONTROL},
    {"SystemCapability.BundleManager.BundleFramework.Core", BUNDLEMANAGER_BUNDLEFRAMEWORK_CORE},
    {"SystemCapability.BundleManager.BundleFramework.FreeInstall", BUNDLEMANAGER_BUNDLEFRAMEWORK_FREEINSTALL},
    {"SystemCapability.BundleManager.BundleFramework.Launcher", BUNDLEMANAGER_BUNDLEFRAMEWORK_LAUNCHER},
    {"SystemCapability.BundleManager.BundleFramework.DefaultApp",
        BUNDLEMANAGER_BUNDLEFRAMEWORK_DEFAULTAPP},
    {"SystemCapability.BundleManager.BundleFramework.Resource", BUNDLEMANAGER_BUNDLEFRAMEWORK_RESOURCE},
    {"SystemCapability.Multimedia.AVSession.Core", MULTIMEDIA_AVSESSION_CORE},
    {"SystemCapability.Multimedia.AVSession.Manager", MULTIMEDIA_AVSESSION_MANAGER},
    {"SystemCapability.Multimedia.Audio.Tone", MULTIMEDIA_AUDIO_TONE},
    {"SystemCapability.Multimedia.Audio.Interrupt", MULTIMEDIA_AUDIO_INTERRUPT},
    {"SystemCapability.Cloud.AAID", CLOUD_AAID},
    {"SystemCapability.Msdp.DeviceStatus.Stationary", MSDP_DEVICESTATUS_STATIONARY},
    {"SystemCapability.Communication.NetManager.Ethernet", COMMUNICATION_NETMANAGER_ETHERNET},
    {"SystemCapability.Communication.NetManager.NetSharing", COMMUNICATION_NETMANAGER_NETSHARING},
    {"SystemCapability.Communication.NetManager.MDNS", COMMUNICATION_NETMANAGER_MDNS},
    {"SystemCapability.Communication.NetManager.Vpn", COMMUNICATION_NETMANAGER_VPN},
    {"SystemCapability.Communication.NFC.Tag", COMMUNICATION_NFC_TAG},
    {"SystemCapability.Communication.NFC.CardEmulation", COMMUNICATION_NFC_CARDEMULATION},
    {"SystemCapability.Security.Cert", SECURITY_CERT},
    {"SystemCapability.Cloud.OAID", CLOUD_OAID},
    {"SystemCapability.XTS.DeviceAttest", XTS_DEVICEATTEST},
    {"SystemCapability.XTS.DeviceAttestLite", XTS_DEVICEATTESTLITE},
    {"SystemCapability.Multimedia.Media.AVPlayer", MULTIMEDIA_MEDIA_AVPLAYER},
    {"SystemCapability.Multimedia.Media.AVRecorder", MULTIMEDIA_MEDIA_AVRECORDER},
    {"SystemCapability.Print.PrintFramework", PRINT_PRINT_FRAMEWORK},
    {"SystemCapability.BundleManager.BundleFramework.Overlay", BUNDLEMANAGER_BUNDLEFRAMEWORK_OVERLAY},
    {"SystemCapability.Cloud.Push", CLOUD_PUSH},
    {"SystemCapability.Multimedia.SystemSound.Core", MULTIMEDIA_SYSTEMSOUND_CORE},
    {"SystemCapability.Communication.SecureElement", COMMUNICATION_SECURE_ELEMENT},
    {"SystemCapability.DistributedDataManager.UDMF.Core", DISTRIBUTEDDATAMANAGER_UDMF_CORE},
    {"SystemCapability.FileManagement.DistributedFileService.CloudSyncManager",
        FILEMANAGEMENT_DISTRIBUTEDFILESERVICE_CLOUDSYNCMANAGER},
    {"SystemCapability.MultimodalInput.Input.ShortKey", MULTIMODALINPUT_INPUT_SHORTKEY},
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif  // _SYSCAP_DEFINE_H
