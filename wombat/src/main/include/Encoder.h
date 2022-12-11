#pragma once

#include <frc/Encoder.h>
#include <rev/CANSparkMax.h>
#include <units/angle.h>
#include <units/angular_velocity.h>

#include "Util.h"

namespace wom {
class Encoder {
 public:
  Encoder(double encoderTicksPerRotation)
      : _encoderTicksPerRotation(encoderTicksPerRotation){};
  virtual double GetEncoderRawTicks()     = 0;
  virtual double GetEncoderTickVelocity() = 0;  // ticks/s
  virtual void   ZeroEncoder();

  double GetEncoderTicks();
  double GetEncoderTicksPerRotation();

  units::radian_t             GetEncoderPosition();
  units::radians_per_second_t GetEncoderAngularVelocity();  // rad/s

 private:
  double _encoderTicksPerRotation;
  double _offset = 0;
};

class DigitalEncoder : public Encoder {
 public:
  DigitalEncoder(int channelA, int channelB, double ticksPerRotation)
      : Encoder(ticksPerRotation),
        _nativeEncoder(new frc::Encoder(channelA, channelB)){};

  DigitalEncoder(frc::Encoder *nativeEncoder, double ticksPerRotation)
      : Encoder(ticksPerRotation), _nativeEncoder(nativeEncoder) {}

  double GetEncoderRawTicks() override;
  double GetEncoderTickVelocity() override;

 private:
  frc::Encoder *_nativeEncoder;
};

class CANSparkMaxEncoder : public Encoder {
 public:
  CANSparkMaxEncoder(rev::CANSparkMax *controller);

  double GetEncoderRawTicks() override;
  double GetEncoderTickVelocity() override;

 private:
  rev::SparkMaxRelativeEncoder _encoder;
};
}  // namespace wom
