#include "bilya/Bilya.h"

#include <fstream>

template<typename It>
void writeVectorField(std::string filename, It start, It end) {
  std::ofstream out{filename};

  out << "x,y,dx,dy,potential" << std::endl;

  for (units::foot_t x = 0_ft; x <= 54_ft; x += 1.5_ft) {
    for (units::foot_t y = 0_ft; y <= 27_ft; y += 1.5_ft) {
      auto value = bilya::CalculateNet(bilya::Position2d { x, y }, start, end);

      double max_l = 0.35;

      out << value.position.X().convert<units::meter>().value() << "," << value.position.Y().convert<units::meter>().value() << ","
          << std::max(std::min(value.gradient.x.value(), max_l), -max_l) << "," << std::max(std::min(value.gradient.y.value(), max_l), -max_l) << ","
          << value.potential << std::endl;
    }
  }
}