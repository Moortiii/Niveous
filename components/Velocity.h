#ifndef NIVEOUS_VELOCITY_H
#define NIVEOUS_VELOCITY_H

struct Velocity {
    float x;
    float y;

    explicit Velocity(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

#endif //NIVEOUS_VELOCITY_H
