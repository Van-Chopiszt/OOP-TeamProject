#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Chip.h"
class Visualizer {
   private:
    /* data */
   public:
    Visualizer(/* args */);
    ~Visualizer();

    void draw(const Chip& chip);
};

Visualizer::Visualizer(/* args */) {}

Visualizer::~Visualizer() {}

#endif //VISUALIZER_H