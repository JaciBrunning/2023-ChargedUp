#pragma once

#include "Encoder.h"
#include "VoltageController.h"

namespace wom {
namespace test {
  class FakeEncoder : public Encoder {
   public:
    using Encoder::Encoder;

    double GetEncoderRawTicks() override {
      return _ticks;
    }

    double GetEncoderTickVelocity() override {
      return _ticks_per_second;
    }

    void UpdatePos(units::turn_t turns, units::second_t dt) {
      _ticks += (turns.value() * GetEncoderTicksPerRotation());
      _ticks_per_second = ((turns.value() * GetEncoderTicksPerRotation()) / dt).value();
    }

    void UpdateVel(units::turns_per_second_t turns_per_second, units::second_t dt) {
      _ticks += (turns_per_second * GetEncoderTicksPerRotation() * dt).value();
      _ticks_per_second = (turns_per_second * GetEncoderTicksPerRotation()).value();
    }
   private:
    double _ticks_per_second = 0;
    double _ticks = 0;
  };

  class FakeVoltageController : public VoltageController {
   public:
    void SetVoltageMax(units::volt_t v) {
      vmax = v;
    }

    void SetVoltage(units::volt_t v) override {
      voltage = v;
    }

    units::volt_t GetVoltage() override {
      return units::math::min(units::math::max(voltage, -vmax), vmax);
    }

    void SetInverted(bool invert) override {
      inverted = invert;
    }

    bool GetInverted() override {
      return inverted;
    }
   private:
    bool inverted = false;
    units::volt_t voltage{0}, vmax{12};
  };
}
}