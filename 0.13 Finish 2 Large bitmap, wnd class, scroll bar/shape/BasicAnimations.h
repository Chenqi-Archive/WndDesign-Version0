#pragma once

#include "Animation.h"

namespace WndDesign {

class Blinking : public Animation {
private:
    Color32 color1, color2;
    bool state;
private:
    inline void Init() override { state = false; }
    inline void Next() override { state = !state; }
    void PaintOn(Board& board) const override;
public:
    Blinking(Color32 color1 = COLOR_WHITE, Color32 color2 = COLOR_GREY) :color1(color1), color2(color2), state(false) {}
};

class SimpleClock: public Animation {
private:
    uint angle;
private:
    inline void Init() override { angle = 270; }
    inline void Next() override { angle += 6; angle %= 360; }
    void PaintOn(Board& board) const override;
public:
    SimpleClock(): Animation(1000), angle(270) {}
};


} // namespace WndDesign