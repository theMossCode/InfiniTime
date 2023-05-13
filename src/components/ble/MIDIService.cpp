#include "components/ble/MIDIService.h"
#include <cstring>
#include "systemtask/SystemTask.h"

using namespace Pinetime::Controllers;

constexpr ble_uuid128_t MIDIService::midiServiceUuid;
constexpr ble_uuid128_t MIDIService::midiCharUuid;

int MidiServiceCallback(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt, void* arg) {
  return static_cast<MIDIService*>(arg)->OnBleUpdate(conn_handle, attr_handle, ctxt);
}

void MidiTimerCallback(TimerHandle_t xTimer) {
  auto controller = static_cast<MIDIService*>(pvTimerGetTimerID(xTimer)); 
  controller->OnTimerEnd();
}

MIDIService::MIDIService(Pinetime::System::SystemTask& system) 
  : midiSystemTask {system},
    characteristicDefinition {{
                                .uuid = &midiCharUuid.u,
                                .access_cb = MidiServiceCallback,
                                .arg = this,
                                .flags = BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE_NO_RSP,
                                .val_handle = &midiCharHandle
                              },
                              {0}
    },
    serviceDefinition {{
                        .type = BLE_GATT_SVC_TYPE_PRIMARY,
                        .uuid = &midiServiceUuid.u,
                        .characteristics = characteristicDefinition
                       },
                       {0}
    } {
  currentTimerVal = 0;
  midiTimer = xTimerCreate("Midi", pdMS_TO_TICKS(1), pdTRUE, this, MidiTimerCallback);
}

void MIDIService::Init() {
  uint8_t res = 0;
  res = ble_gatts_count_cfg(serviceDefinition);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(serviceDefinition);
  ASSERT(res == 0);

  xTimerStart(midiTimer, 0);
}

int MIDIService::OnBleUpdate(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt) {
  if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
    size_t notifSize = OS_MBUF_PKTLEN(ctxt->om);
    size_t bufferSize = notifSize;

    char data[bufferSize + 1];
    os_mbuf_copydata(ctxt->om, 0, bufferSize, data);
  }
  else if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
    // No payload in read
  }
  return 0;
}

void MIDIService::AppEvent(char cmd, char channel, char key) {
  // Header | Timestamp | Status | Key | Velocity
  char headerByte = 0x80 | ((currentTimerVal >> 7) & 0x3f);
  char timestampByte = 0x80 | (currentTimerVal & 0x7f);
  char statusByte = cmd | (channel & 0x0f);
  char midiMsg[]= {headerByte, timestampByte, statusByte, key, MIDIService::VELOCITY};
  auto* om = ble_hs_mbuf_from_flat(midiMsg, sizeof(midiMsg));

  uint16_t connectionHandle = midiSystemTask.nimble().connHandle();

  if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
    return;
  }

  ble_gattc_notify_custom(connectionHandle, midiCharHandle, om);
}

void MIDIService::OnTimerEnd() {
  currentTimerVal++;
}

void MIDIService::ResetTimer() {
  currentTimerVal = 0;
  xTimerReset(midiTimer, 0);
}

