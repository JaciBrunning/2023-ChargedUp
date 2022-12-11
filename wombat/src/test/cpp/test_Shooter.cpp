#include <gtest/gtest.h>
#include <frc/simulation/FlywheelSim.h>
#include <frc/simulation/BatterySim.h>

#include <fstream>

#include "Shooter.h"

#include "FakeDevices.h"

using namespace wom;

class ShooterTest : public testing::Test {
 protected:
  test::FakeVoltageController motor;
  test::FakeEncoder encoder{1024};
  Gearbox gearbox{&motor, &encoder, DCMotor::NEO(3)};

  ShooterParams params{
    gearbox,
    {
      12_V / 3500_rpm,            /* kP*/
      0.02_V / (2000_rpm * 1_s),  /* kI*/
      0_V / (5000_rpm / 1_s),     /* kD */

      50_rpm,                     /* Pos Threshold*/
      25_rpm / 1_s                /* Vel Threshold*/
    },
    50_A
  };
  Shooter shooter{params};

  frc::sim::FlywheelSim sim{
    frc::DCMotor::NEO(3), 1.0,
    0.5 * 3_kg * 2_in * 2_in,
    {0.02}
  };

  frc::sim::BatterySim simBattery;
};

TEST_F(ShooterTest, PIDSpinUp) {
  shooter.SetPID(3500_rpm);

  std::ofstream output("shooter_spinup_pid.csv");
  output << "time,setpoint,speed,voltage,current" << std::endl;

  for (units::second_t t = 0_s; t < 5_s; t += 20_ms) {
    encoder.UpdateVel(sim.GetAngularVelocity(), 20_ms);
    shooter.OnUpdate(20_ms);
    motor.SetVoltageMax(simBattery.Calculate({ sim.GetCurrentDraw() }));
    sim.SetInputVoltage(motor.GetVoltage());
    sim.Update(20_ms);

    output << t.value() << "," << 3500 << "," << sim.GetAngularVelocity().convert<units::rpm>().value()
          << "," << motor.GetVoltage().value() << "," << sim.GetCurrentDraw().value() << std::endl;
  }
}
