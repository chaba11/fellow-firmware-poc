#include "DeviceConfig.h"

#include <ArduinoJson.h>

const char *configFilePath = "/config.json";

bool saveConfig()
{
  Serial.println("[DEBUG] Enter save config");
  if (!SPIFFS.exists(configFilePath))
  {
    Serial.println("[DEBUG] Config file doesn't exists");
    return true;
  } else {
    File file = SPIFFS.open(configFilePath, FILE_WRITE);
    Serial.println(file);
    return true;
  }
  File file = SPIFFS.open(configFilePath, FILE_WRITE);
  Serial.println("[DEBUG] Finish save config");
  if (!file)
  {
    Serial.println("[CONFIG] Failed to create file to be saved");
    return true;
  }

  StaticJsonDocument<512> doc;

  doc["wifiSsid"] = globalConfig.wifiSsid;
  doc["wifiPass"] = globalConfig.wifiPass;
  doc["iotCorePrivateKey"] = globalConfig.iotCorePrivateKey;
  doc["iotCoreProjectId"] = globalConfig.iotCoreProjectId;
  doc["iotCoreRegistry"] = globalConfig.iotCoreRegistry;
  doc["iotCoreRegion"] = globalConfig.iotCoreRegion;

  if (serializeJson(doc, file) == 0)
  {
    Serial.println("[CONFIG] Failed to save config to file");
  }

  file.close();
  return false;
}

bool clearConfig()
{
  globalConfig = DeviceConfig{};
  return SPIFFS.remove(configFilePath);
}

bool loadConfig()
{
  if (!SPIFFS.exists(configFilePath))
  {
    Serial.println("[CONFIG] File not found, using default config");
    return true;
  }

  File file = SPIFFS.open(configFilePath, FILE_READ);
  if (!file)
  {
    Serial.println("[CONFIG] Failed to open file for reading, using default config");
    return true;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println("[CONFIG] Failed to parse file, using default config");
    file.close();
    return true;
  }

  strlcpy(globalConfig.wifiSsid, doc["wifiSsid"] | "", sizeof(globalConfig.wifiSsid));
  strlcpy(globalConfig.wifiPass, doc["wifiPass"] | "", sizeof(globalConfig.wifiPass));
  strlcpy(globalConfig.iotCorePrivateKey, doc["iotCorePrivateKey"] | "", sizeof(globalConfig.iotCorePrivateKey));
  strlcpy(globalConfig.iotCoreProjectId, doc["iotCoreProjectId"] | "", sizeof(globalConfig.iotCoreProjectId));
  strlcpy(globalConfig.iotCoreRegistry, doc["iotCoreRegistry"] | "", sizeof(globalConfig.iotCoreRegistry));
  strlcpy(globalConfig.iotCoreRegion, doc["iotCoreRegion"] | "", sizeof(globalConfig.iotCoreRegion));

  file.close();
  return false;
}

void setupConfig()
{
  uint64_t chipId = ESP.getEfuseMac();
  uint32_t highBytesChipId = (uint32_t)(chipId >> 16); // High 4 bytes
  //uint16_t lowBytesChipId = (uint16_t)chipId; // Low 2 bytes
  snprintf(globalConfig.deviceId, sizeof(globalConfig.deviceId), "%08X", highBytesChipId);
  snprintf(globalConfig.deviceName, sizeof(globalConfig.deviceName), "EKG_%08X", highBytesChipId);

  SPIFFS.begin(true);
}
