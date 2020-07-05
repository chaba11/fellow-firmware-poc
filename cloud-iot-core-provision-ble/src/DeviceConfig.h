#ifndef DEVICE_CONFIG_H_
#define DEVICE_CONFIG_H_

#include <FS.h>
#include <SPIFFS.h>
#include "DeviceState.h"

struct DeviceConfig
{

  // your network SSID (name)
  char wifiSsid[50] = "";
  // your network password (use for WPA)
  char wifiPass[50] = "";

  char iotCorePrivateKey[100] = "";
  char iotCoreRegion[50] = "us-central1";
  char iotCoreRegistry[50] = "arduino-iot-bootcamp";
  char iotCoreProjectId[50] = "iot-bootcamp-158521";

  /* Values not save on storage */
  char deviceId[24] = "";
  char deviceName[24] = "";
  const char *ntpPrimary = "time.google.com";
  const char *ntpSecondary = "pool.ntp.org";

  uint resetPin = 2;
  uint lightPin = 13;
  uint lightLedcChannel = 0;
  uint lightLedcFrequency = 5000;
};

extern DeviceConfig globalConfig;

void setupConfig();
bool loadConfig();
bool saveConfig();
bool clearConfig();

#endif // DEVICE_CONFIG_H_