#pragma once

#include "Animation.h"

namespace WndDesign {

class Blinking : public Animation {
private:
    Color32 color1, color2;
    bool state;
private:
    void Init() override;
    void Next() override;
    void PaintOn(Board& board) const override;
public:
    Blinking(Color32 color1 = COLOR_WHITE, Color32 color2 = COLOR_GREY) :color1(color1), color2(color2), Animation(1000) {
        Init();
    }
};

class SimpleClock: public Animation {
private:
    uint angle;
private:
    void Init() override;
    void Next() override;
    void PaintOn(Board& board) const override;
public:
    SimpleClock() : Animation(1000) {
        Init();
        Play();
    }
};


} // namespace WndDesign