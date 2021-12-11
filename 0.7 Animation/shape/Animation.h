#pragma once

#include "./BasicShapes.h"
#include "../WndMng.h"
#include "../lib/list.h"
#include <Windows.h>

class Animation : public Shape {
private:
    static list<WndID> AnimationList;
    static HANDLE Mutex;
    static HANDLE Timer;
    static HANDLE Thread;
    static DWORD WINAPI AnimationThread(LPVOID lpParameter) {
        while (1) {
            WaitForSingleObject(Timer, INFINITE);

            WaitForSingleObject(Mutex, INFINITE);    // Acquire mutex to access list.
            auto it = AnimationList.begin();
            if (!it) { return 0; } // Finishes if the list is empty.
            for (; it; ++it) {
                WndID wndID = it.Item();
                SendMsg(wndID, MSG_WND_CHANGEBG, nullptr);
            }
            ReleaseMutex(Mutex);   // Release the mutex.
        }
    }
    // For managing all animaitons, register on constructing.
    static void AddAnimation(WndID wndID) {
        if (AnimationList.size() == 0) {
            // Create a mutex, a timer and a new thread.
            Mutex = CreateMutex(NULL, true, NULL);
            if (Mutex == NULL) { return; }
            Timer = CreateWaitableTimer(NULL, false, NULL);
            if (Timer == NULL) { return; }  // Forgot to release the mutex.
            LARGE_INTEGER time; time.QuadPart = -400;
            if (!SetWaitableTimer(Timer, &time, 40, NULL, NULL, false))  // 20 fps
            {
                return;
            }
            Thread = CreateThread(NULL, 0, Animation::AnimationThread, NULL, 0, NULL);
            if (Thread == NULL) { return; }
        }
        else {
            WaitForSingleObject(Mutex, INFINITE);
        }
        AnimationList.push_back(wndID);

        // Attention! The wnd class may have not been initialized!
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
            CloseHandle(Thread);
            CloseHandle(Timer);
            CloseHandle(Mutex);
        }
    }

    // All animations are 25 fps. (Sleep 40 ms)
    //uint fps;

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
public:
    SpinningBall() : Animation(){}
    SpinningBall(WndID wndID) :Animation(wndID) {}
    virtual Shape* Clone(void* para) const{ return new SpinningBall(*(WndID*)para); }
    virtual ~SpinningBall() {}
    virtual void PaintOn(Bitmap& bitmap, Rect rect = {}, Pen pen = Pen(COLOR_BLACK, 1)) const {
        // Current state.
        static uint angle = 0; // Starting angle

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
        pen.SetThickness(10);
        pen.SetStyle(Brush);
        pen.PaintPoint(bitmap, { x,y });

        // Draw next state.
        angle = (angle + 20) % 360;

        t = angle / 180.0 * M_PI;
        x = radius * cos(t) + cx;
        y = radius * sin(t) + cy;
        pen.SetColor(COLOR_RED);
        pen.SetThickness(10);
        pen.SetStyle(Brush);
        pen.PaintPoint(bitmap, { x,y });
    }
};