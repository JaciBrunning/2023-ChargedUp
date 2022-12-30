#include "bilya/emitters/BaseEmitter.h"
using namespace bilya;

Emitter::Emitter(const EmitterConfig &config) : _config(config) {}

const EmitterConfig &Emitter::GetConfig() const {
  return _config;
}

PointGradient Emitter::CalculateEmitter(const Position2d &emit_src, const Position2d &position) const {
  auto delta = emit_src - position;
  auto dist = delta.Norm();

  auto config = GetConfig();

  double potential = -config.alpha * exp( -(config.gamma / 2 * dist * dist).value() );
  auto gradient = delta * -config.gamma * potential;

  return PointGradient {
    position,
    Gradient2d{ units::meters_per_second_t{gradient.X().value()}, units::meters_per_second_t{gradient.Y().value()} },
    potential
  };
}
