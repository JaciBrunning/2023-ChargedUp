#include <gtest/gtest.h>

#include "bilya/Bilya.h"
#include "bilya/BilyaFixtures.h"

#include <memory>

using namespace bilya;

TEST(Bilya, Spline) {
  auto spline = std::make_shared<Spline<2>>(SplineConfig<2> {
    std::vector<SplineControlPoint<2>> {
      { Vec<2> { 3, 3 }, Vec<2> { 1, 1 } },
      { Vec<2> { 6, 6 }, Vec<2> { 0, 5 } }
    },
    0, 
    0.005
  }, EmitterConfig {
    4, 0.1
  });

  std::vector<std::shared_ptr<Emitter<2>>> emitters;
  emitters.emplace_back(spline);
  
  writeVectorField("bilya_field.csv", emitters.cbegin(), emitters.cend());

  std::ofstream out("bilya_spline.csv");
  out << "x,y" << std::endl;

  auto fullSpline = spline->GetSpline();

  for (auto it = fullSpline.cbegin(); it != fullSpline.cend(); it++) {
    auto f = *it;
    out << f.position.x() << "," << f.position.y() << std::endl;
  }
}