#pragma once

#include "Animation.h"

namespace WndDesign {

class Clock: public Animation {
    uint angle = 0;
public:
    Clock(): Animation(1000) {}
    virtual Figure* Clone() const { return new Clock(); }
    virtual void Paint();
};


class Waiting: public Animation {

};

} // namespace WndDesign