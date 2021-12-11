#pragma once

#include "./BasicShapes.h"
#include "../WndMng.h"
#include "../lib/list.h"
#include <Windows.h>

BITMAP;

#include <queue>  // May use self defined queue later
using std::queue;

// Animation:
//   Create a timer and refresh background periodically.


// May create a timer for all objects, or save the period for each object to support different fps.

#define FPS 24

class Animation : public Figure {
private:
    static list<WndID> AnimationList;
    static HANDLE Mutex;
    static HANDLE Timer;
    static HANDLE Thread;
    static DWORD WINAPI AnimationThread(LPVOID lpParameter) {
        queue<WndID> wndQueue;
        while (1) {
            WaitForSingleObject(Timer, INFINITE);

            WaitForSingleObject(Mutex, INFINITE);    // Acquire mutex to access list.
            auto it = AnimationList.begin();
            if (!it) { return 0; } // Finishes if the list is empty.
            for (; it; ++it) {
                WndID wndID = it.Item();
                wndQueue.push(wndID);
            }
            ReleaseMutex(Mutex);   // Release the mutex.

            // Uses a queue to avoid deadlock.
            while (!wndQueue.empty()) {
                WndID wndID = wndQueue.front(); wndQueue.pop();
                SendMsg(wndID, MSG_WND_CHANGEBG, nullptr);
            }
        }
    }
    // For managing all animaitons, register on constructing.
    static void AddAnimation(WndID wndID) {
        if (AnimationList.size() == 0) {
            // Create a mutex, a timer and a new thread.
            Mutex = CreateMutex(NULL, true, NULL);
            if (Mutex == NULL) {
                return;
            }
            Timer = CreateWaitableTimer(NULL, false, NULL);
            if (Timer == NULL) {
                CloseHandle(Mutex); Mutex = NULL; 
                return;
            }
            LARGE_INTEGER time; time.QuadPart = -400;
            if (!SetWaitableTimer(Timer, &time, 1000 / FPS, NULL, NULL, false)) {
                CloseHandle(Timer); Timer = NULL;
                CloseHandle(Mutex); Mutex = NULL;
                return;
            }
            Thread = CreateThread(NULL, 0, Animation::AnimationThread, NULL, 0, NULL);
            if (Thread == NULL) {
                CloseHandle(Timer); Timer = NULL;
                CloseHandle(Mutex); Mutex = NULL;
                return;
            }
        }
        else {
            WaitForSingleObject(Mutex, INFINITE);
        }
        AnimationList.push_back(wndID);

        // Attention! The wnd class may have not been initialized!

        // May wait for the wnd to initialize and then begin.

        ReleaseMutex(Mutex);
    }
    static void RemoveAnimation(WndID wndID) {
        WaitForSingleObject(Mutex, INFINITE);
        auto it = AnimationList.begin();
        for (; it; ++it) { if (it.Item() == wndID) { break; } }
        if (!it) { return; }
        AnimationList.remove(it);
        ReleaseMutex(Mutex);

        if (AnimationList.size() == 0) {
            // Destroy the mutex, timer and the new thread.
            WaitForSingleObject(Thread, INFINITE);
            CloseHandle(Thread); Thread = NULL;
            CloseHandle(Timer); Timer = NULL;
            CloseHandle(Mutex); Mutex = NULL;
        }
    } 

protected:
    WndID wndID;

public:
    Animation():wndID(MASTER_WND){}
    Animation(WndID wndID) :wndID(wndID) { if (wndID != MASTER_WND) { AddAnimation(wndID); } }
    virtual ~Animation() { if (wndID != MASTER_WND) { RemoveAnimation(wndID); } }

    void Begin();
    void Pause();
    void Resume();
};

// Initialize these static members.
HANDLE Animation::Mutex = NULL;
HANDLE Animation::Timer = NULL;
HANDLE Animation::Thread = NULL;
list<WndID> Animation::AnimationList;


class SpinningBall : public Animation {
    // Current state.
    uint angle = 0; // Starting angle

public:
    SpinningBall() : Animation(){}
    SpinningBall(WndID wndID) :Animation(wndID) {}
    virtual Figure* Clone(void* para) const{ return new SpinningBall(*(WndID*)para); }
    virtual ~SpinningBall() {}
    virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) {
        Size& size = rect.size;
        Pos& offset = rect.pos;
        if ((size.height || size.width) == 0) {
            size = bitmap.GetSize();
        }
        uint radius = min(size.height, size.width) / 2;
        if (radius == 0) { return; }  // Too small!
        int cx = offset.x + size.width / 2;
        int cy = offset.y + size.height / 2;

        // Clear previous state.
        double t = angle / 180.0 * M_PI;
        int x = radius * cos(t) + cx;
        int y = radius * sin(t) + cy;
        pen.SetColor(COLOR_WHITE);
        pen.SetThickness(5);
        pen.SetStyle(Solid);
        pen.PaintPoint(bitmap, { x,y });

        // Draw next state.
        angle = (angle + 6) % 360;

        t = angle / 180.0 * M_PI;
        x = radius * cos(t) + cx;
        y = radius * sin(t) + cy;
        pen.SetColor(COLOR_RED);
        pen.PaintPoint(bitmap, { x,y });
    }
};

class Fade : public Animation {
    // Current state.
    Color32 color = 0x0000FF00; 
    enum { red2green, green2blue, blue2red }state = red2green;

public:
    Fade() : Animation() {}
    Fade(WndID wndID) :Animation(wndID) {}
    virtual Figure* Clone(void* para) const { return new Fade(*(WndID*)para); }
    virtual ~Fade() {}
    virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) {
        Color color = this->color;
        color.PaintOn(bitmap);
        switch (state)
        {
        case Fade::red2green: this->color.red--; this->color.green++; if (this->color.red == 0) { state = green2blue; } break;
        case Fade::green2blue: this->color.green--; this->color.blue++; if (this->color.green == 0) { state = blue2red; }break;
        case Fade::blue2red: this->color.blue--; this->color.red++; if (this->color.blue == 0) { state = red2green; }break;
        }
    }
};