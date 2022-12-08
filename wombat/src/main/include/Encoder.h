#pragma once

#include <frc/Encoder.h>
#include <units/angle.h>
#include <units/angular_velocity.h>

#include <rev/CANSparkMax.h>

#include "Util.h"

namespace wom {
  class Encoder {
   public:
    Encoder(int encoderTicksPerRotation) : _encoderTicksPerRotation(encoderTicksPerRotation){};
    virtual int     GetEncoderRawTicks() = 0;
    virtual double  GetEncoderTickVelocity() = 0;  // ticks/s
    virtual void    ZeroEncoder();

    int    GetEncoderTicks();
    int    GetEncoderTicksPerRotation();

    units::radian_t GetEncoderPosition();
    units::radians_per_second_t GetEncoderAngularVelocity();   // rad/s

   private:
    int _encoderTicksPerRotation;
    int _offset = 0;
  };

  class DigitalEncoder : public Encoder {
   public:
    DigitalEncoder(int channelA, int channelB, int ticksPerRotation)
        : Encoder(ticksPerRotation),
          _nativeEncoder(new frc::Encoder(channelA, channelB)){};
    
    DigitalEncoder(frc::Encoder *nativeEncoder, int ticksPerRotation)
        : Encoder(ticksPerRotation),
          _nativeEncoder(nativeEncoder) {}

    int GetEncoderRawTicks() override;
    double GetEncoderTickVelocity() override;

   private:
    frc::Encoder *_nativeEncoder;
  };

  class CANSparkMaxEncoder : public Encoder {
   public:
    CANSparkMaxEncoder(rev::CANSparkMax *controller);

    int GetEncoderRawTicks() override;
    double GetEncoderTickVelocity() override;
   private:
    rev::SparkMaxRelativeEncoder _encoder;
  };
}  // namespace wom
