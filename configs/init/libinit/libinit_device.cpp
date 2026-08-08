/*
 * Copyright (C) The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <libinit_device.h>
#include <libinit_utils.h>

using android::base::GetProperty;

#define CODENAME_PROP "ro.product.vendor.device"

void search_device(const std::vector<device_info_t> devices) {
  std::string codename_prop = GetProperty(CODENAME_PROP, "");

  for (const auto &device : devices) {
    if ((device.codename == "" || device.codename == codename_prop)) {
      set_device_props(device);
      break;
    }
  }
}

void set_device_props(const device_info_t device) {
  property_override("bluetooth.device.default_name", device.full_name);

  set_ro_build_prop("device", device.codename, true);

  if (device.adaptive_rr) {
    property_override("ro.surface_flinger.set_display_power_timer_ms", "200");
    property_override("ro.surface_flinger.set_idle_timer_ms", "1000");
    property_override("ro.surface_flinger.set_touch_timer_ms", "1000");
    property_override("ro.surface_flinger.use_content_detection_for_refresh_rate", "true");
  }
  if (device.side_fp) {
    property_override("ro.vendor.fingerprint.type", "side");
    property_override("ro.vendor.fingerprint.supports_gestures", "false");
    property_override("ro.vendor.fingerprint.sensor_location", "1080|820|350|local:4633128672291735932");
  }
  if (device.udfps) {
    property_override("ro.vendor.fingerprint.type", "udfps_optical");
    property_override("ro.vendor.fingerprint.sensor_location", "540|2194|119");
    property_override("ro.vendor.fingerprint.rectangular_sensor_location", "421,2075,659,2313");
    property_override("ro.vendor.fingerprint.force_calibrate", "true");
    property_override("ro.vendor.fingerprint.request_touch_event", "true");
  }

  if (!device.nfc_chip.empty()) {
    property_override("ro.vendor.nfc.feature.chipname", device.nfc_chip);
  }
}
