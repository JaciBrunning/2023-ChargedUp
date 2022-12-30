#include <gtest/gtest.h>

#include "bilya/BilyaFixtures.h"

#include "drivetrain/SwerveDrive.h"

#include "FakeVoltageController.h"
#include "FakeEncoder.h"
#include "FakeGyro.h"
#include "SwerveSim.h"

#include <memory>

using namespace wom;
using namespace bilya;

struct SwerveModuleTestVars {
  SwerveModuleTestVars(frc::Translation2d pos) : config{ pos, drive, turn, 4_in / 2 }, mod{ config, anglePID, velocityPID } {}

  FakeVoltageController driveMotor, turnMotor;
  FakeEncoder driveEncoder{1024}, turnEncoder{1024};

  Gearbox drive{ &driveMotor, &driveEncoder, DCMotor::NEO(1).WithReduction(8.14) };
  Gearbox turn{ &turnMotor, &turnEncoder, DCMotor::NEO(1).WithReduction(150 / 7.0) };

  SwerveModuleConfig config;

  SwerveModule::angle_pid_conf_t anglePID{
    12_V / 90_deg,
  };

  SwerveModule::velocity_pid_conf_t velocityPID{
    12_V / 1_mps
  };

  SwerveModule mod;
  SwerveModuleSim sim{ turn.motor, drive.motor, 54_kg / 4, 0.5 * 6_lb * 7.5_in * 7.5_in, 4_in / 2 };

  void update(units::second_t dt) {
    turnEncoder.SetTurns(sim.GetAngle());
    driveEncoder.SetTurnVelocity(sim.GetSpeed(), dt);
  }
};

class BilyaTest : public ::testing::Test {
 public:
  wpi::array<SwerveModuleTestVars *, 4> modules{ 
    new SwerveModuleTestVars(frc::Translation2d{1_m, 1_m} ),
    new SwerveModuleTestVars(frc::Translation2d{1_m, -1_m} ),
    new SwerveModuleTestVars(frc::Translation2d{-1_m, -1_m} ),
    new SwerveModuleTestVars(frc::Translation2d{-1_m, 1_m} ),
  };

  FakeGyro gyro;

  SwerveDriveConfig cfg{
    modules[0]->anglePID, modules[1]->velocityPID,
    { modules[0]->config, modules[1]->config, modules[2]->config, modules[3]->config },
    &gyro,
    {
      (180_deg / 1_s) / 45_deg
    },
    {
      4_mps / 1_m
    },
    { 0.1, 0.1, 0.1 },
    { 0.05 },
    { 0.0, 0.0, 0.0 }
  };

  SwerveDrive swerve{cfg, frc::Pose2d{ frc::Translation2d{0_m, 0_m}, frc::Rotation2d{ 0_deg } }};

  SwerveSim sim{
    frc::SwerveDriveKinematics(modules[0]->config.position, modules[1]->config.position, modules[2]->config.position, modules[3]->config.position),
    { &modules[0]->sim, &modules[1]->sim, &modules[2]->sim, &modules[3]->sim }
  };
};

TEST_F(BilyaTest, Bilya) {
  std::vector<std::shared_ptr<Emitter>> emitters;
  emitters.emplace_back(std::make_shared<Point>(
    Position2d{ 5_m, 5_m },
    EmitterConfig{ 2, 0.1 }
  ));
  emitters.emplace_back(std::make_shared<Point>(
    Position2d{ 15_m, 3_m },
    EmitterConfig{ 2, 0.1 }
  ));
  emitters.emplace_back(std::make_shared<Point>(
    Position2d{ 8_m, 5_m },
    EmitterConfig{ -3, 3 }
  ));
  emitters.emplace_back(std::make_shared<Wall>(
    WallConfig{
      { 0_m, 0_m }, { 54_ft, 0_m },   // from, to
      true, WallSide::BOTH            // overflow, side
    },
    EmitterConfig{ -1, 0.2 }
  ));

  writeVectorField("bilya_field.csv", emitters.cbegin(), emitters.cend());

  std::ofstream out{"bilya_robot.csv"};
  out << "t,x,y,heading" << std::endl;

  swerve.SetIdle();

  sim.x = 8_m;
  sim.y = 2_m;
  swerve.ResetPose(frc::Pose2d {
    frc::Translation2d{ sim.x, sim.y },
    frc::Rotation2d{ sim.theta }
  });

  int i = 0;

  for (units::second_t t = 0_s; t < 10_s; t += 20_ms) {
    i++;

    if (i % 5 == 0) {
      swerve.AddVisionMeasurement(frc::Pose2d {
        frc::Translation2d{ sim.x, sim.y },
        frc::Rotation2d{ sim.theta }
      }, units::microsecond_t{(double)wpi::Now()});
    }

    auto calc = CalculateNet(swerve.GetPose().Translation(), emitters.cbegin(), emitters.cend());
    swerve.SetFieldRelativeVelocity(FieldRelativeSpeeds {
      2 * calc.gradient.x, 2 * calc.gradient.y, 0_deg / 1_s
    });

    swerve.OnUpdate(20_ms);
    sim.Calculate({
      modules[0]->turnMotor.GetVoltage(),
      modules[1]->turnMotor.GetVoltage(),
      modules[2]->turnMotor.GetVoltage(),
      modules[3]->turnMotor.GetVoltage(),
    }, {
      modules[0]->driveMotor.GetVoltage(),
      modules[1]->driveMotor.GetVoltage(),
      modules[2]->driveMotor.GetVoltage(),
      modules[3]->driveMotor.GetVoltage(),
    }, 20_ms);

    gyro.SetAngle(sim.theta);
    modules[0]->update(20_ms);
    modules[1]->update(20_ms);
    modules[2]->update(20_ms);
    modules[3]->update(20_ms);

    out << t.value() << "," << sim.x.value() << "," 
        << sim.y.value() << "," << sim.theta.convert<units::degree>().value() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}