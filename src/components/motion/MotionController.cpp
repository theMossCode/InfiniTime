#include "components/motion/MotionController.h"
#include "os/os_cputime.h"
using namespace Pinetime::Controllers;

void MotionController::Update(uint32_t nbSteps, int16_t x, int16_t y, int16_t z, int16_t* samples, uint16_t samples_length) {
  if (this->nbSteps != nbSteps && service != nullptr) {
    service->OnNewStepCountValue(nbSteps);
  }

  if (service != nullptr && (this->x != x || this->y != y || this->z != z)) {
    service->OnNewMotionValues(samples, samples_length);
  }

  this->x = x;
  this->y = y;
  this->z = z;
  int32_t deltaSteps = nbSteps - this->nbSteps;
  this->nbSteps = nbSteps;
  if (deltaSteps > 0) {
    currentTripSteps += deltaSteps;
  }
}

bool MotionController::Should_RaiseWake(bool isSleeping) {
  if ((x + 335) <= 670 && z < 0) {
    if (not isSleeping) {
      if (y <= 0) {
        return false;
      } else {
        lastYForWakeUp = 0;
        return false;
      }
    }

    if (y >= 0) {
      lastYForWakeUp = 0;
      return false;
    }
    if (y + 230 < lastYForWakeUp) {
      lastYForWakeUp = y;
      return true;
    }
  }
  return false;
}

bool MotionController::Should_ShakeWake(uint16_t thresh) {
  bool wake = false;
  auto diff = xTaskGetTickCount() - lastShakeTime;
  lastShakeTime = xTaskGetTickCount();
  /* Currently Polling at 10hz, If this ever goes faster scalar and EMA might need adjusting */
  int32_t speed = std::abs(z + (y / 2) + (x / 4) - lastYForShake - lastZForShake) / diff * 100;
  //(.2 * speed) + ((1 - .2) * accumulatedspeed);
  // implemented without floats as .25Alpha
  accumulatedspeed = (speed / 5) + ((accumulatedspeed / 5) * 4);

  if (accumulatedspeed > thresh) {
    wake = true;
  }
  lastXForShake = x / 4;
  lastYForShake = y / 2;
  lastZForShake = z;
  return wake;
}

int32_t MotionController::currentShakeSpeed() {
  return accumulatedspeed;
}

void MotionController::IsSensorOk(bool isOk) {
  isSensorOk = isOk;
}

void MotionController::Init(Pinetime::Drivers::AccelerationDeviceTypes types) {
  this->deviceType = types;
}

void MotionController::SetService(Pinetime::Controllers::MotionService* service) {
  this->service = service;
}
