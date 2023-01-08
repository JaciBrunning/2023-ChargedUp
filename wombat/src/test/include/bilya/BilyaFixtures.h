#include "bilya/Bilya.h"

#include <units/length.h>

#include <fstream>

template<typename It>
void writeVectorField(std::string filename, It start, It end, units::meter_t xmax = 54_ft, units::meter_t ymax = 27_ft, units::meter_t dxy = 0.75_ft) {
  std::ofstream out{filename};

  out << "x,y,dx,dy,potential" << std::endl;

  for (units::meter_t x = 0_ft; x <= xmax; x += dxy) {
    for (units::meter_t y = 0_ft; y <= ymax; y += dxy) {
      auto value = bilya::CalculateNet<2>(bilya::Vec<2> { x.value(), y.value() }, start, end);

      double max_l = 0.35;
      auto gradient = value.gradient;

      if (gradient.norm() > max_l)
        gradient = gradient / gradient.norm() * max_l;

      out << value.position[0] << "," << value.position[1] << ","
          << gradient[0] << "," << gradient[1] << ","
          << value.potential << std::endl;
    }
  }
}