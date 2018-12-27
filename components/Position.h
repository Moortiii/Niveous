#ifndef NIVEOUS_POSITION_H
#define NIVEOUS_POSITION_H

struct Position {
    float x;
    float y;

    explicit Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

#endif //NIVEOUS_POSITION_H
