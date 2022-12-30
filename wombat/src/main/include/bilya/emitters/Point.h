#pragma once

#include "bilya/emitters/BaseEmitter.h"

namespace bilya {
  class Point : public Emitter {
   public:
    Point(Position2d source, EmitterConfig emitter);

    PointGradient Calculate(const Position2d &position) const override;
   private:
    Position2d _source;
  };
}