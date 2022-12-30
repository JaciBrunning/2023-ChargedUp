#include "bilya/emitters/Wall.h"
using namespace bilya;

Wall::Wall(WallConfig cfg, EmitterConfig emit) : Emitter(emit), _wallConfig(cfg) {}

PointGradient Wall::Calculate(const Position2d &position) const {
  auto v = _wallConfig.to - _wallConfig.from;
  auto u = _wallConfig.from - position;

  auto t = (-(v.X() * u.X() + v.Y() * u.Y()) / (v.X() * v.X() + v.Y() * v.Y())).value();
  auto d = (v.X() * u.Y() - v.Y() * u.X()).value();

  PointGradient result;
  if ((_wallConfig.side == WallSide::POSITIVE && d < 0) || (_wallConfig.side == WallSide::NEGATIVE && d > 0))
    return result;
  
  if (t < 0) {
    if (_wallConfig.overflow)
      return CalculateEmitter(_wallConfig.from, position);
  } else if (t > 1) {
    if (_wallConfig.overflow)
      return CalculateEmitter(_wallConfig.to, position);
  } else {
    return CalculateEmitter(_wallConfig.from + v*t, position);
  }

  return result;
}