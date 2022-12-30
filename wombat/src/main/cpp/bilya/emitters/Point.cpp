#include "bilya/emitters/Point.h"
using namespace bilya;

Point::Point(Position2d source, EmitterConfig emitter) : Emitter(emitter), _source(source) {}

PointGradient Point::Calculate(const Position2d &position) const {
  return CalculateEmitter(_source, position);
}