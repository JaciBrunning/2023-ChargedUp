#pragma once

#include "bilya/emitters/BaseEmitter.h"

namespace bilya {
  template<size_t N>
  class Point : public Emitter<N> {
   public:
    Point(Vec<N> source, EmitterConfig emitter) : Emitter<N>(emitter), _source(source) {}

    PointGradient<N> Calculate(const Vec<N> &position) const override {
      return this->CalculateEmitter(_source, position);
    }
   private:
    Vec<N> _source;
  };
}