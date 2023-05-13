/*  Copyright (C) 2020-2021 JF, Adam Pigg, Avamander

    This file is part of InfiniTime.

    InfiniTime is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    InfiniTime is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <cstdint>
#include <string>
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#include <atomic>
#undef max
#undef min

namespace Pinetime {
  namespace System {
    class SystemTask;
  }
  namespace Controllers {
    class MIDIService {
    public:
      explicit MIDIService(Pinetime::System::SystemTask& system);

      void Init();

      int OnBleUpdate(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt);

      void AppEvent(char cmd, char channel, char key);

      void OnTimerEnd();
      void ResetTimer();

      static const char NOTE_OFF = 0x80;
      static const char NOTE_ON =  0x90;

    private:
      Pinetime::System::SystemTask& midiSystemTask;

      //  03B80E5A-EDE8-4B33-A751-6CE34EC4C700
      static constexpr ble_uuid128_t midiServiceUuid {
        .u = {.type = BLE_UUID_TYPE_128},
        .value = {0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, 0x51, 0xa7, 0x33, 0x4b, 0xe8, 0xed, 0x5a, 0x0e, 0xb8, 0x03}};

      // 7772E5DB-3868-4112-A1A9-F2669D106BF3
      static constexpr ble_uuid128_t midiCharUuid {
        .u = {.type = BLE_UUID_TYPE_128},
        .value = {0xf3, 0x6b, 0x10, 0x9d, 0x66, 0xf2, 0xa9, 0xa1, 0x12, 0x41, 0x68, 0x38, 0xdb, 0xe5, 0x72, 0x77}};

      struct ble_gatt_chr_def characteristicDefinition[2];
      struct ble_gatt_svc_def serviceDefinition[2];
      uint16_t midiCharHandle;

      TimerHandle_t midiTimer;
      uint16_t currentTimerVal;

      static const char VELOCITY = 0x40;
    };
  }
}
