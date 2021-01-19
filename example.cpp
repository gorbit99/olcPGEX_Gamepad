#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

#define OLC_PGE_GAMEPAD

#include "olcPGEX_Gamepad.h"

class Test : public olc::PixelGameEngine {
    bool OnUserCreate() override {
        olc::GamePad::init();
        return true;
    }

    olc::GamePad *gamepad = nullptr;

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);
        if (gamepad == nullptr || !gamepad->stillConnected) {
            DrawString(10, 10, "Press the select button on a gamepad!");
            gamepad = olc::GamePad::selectWithButton(olc::GPButtons::SELECT);
            return true;
        }

        if (!gamepad->getButton(olc::GPButtons::L2).bHeld) {
            DrawRect(10, 10, 30, 10);
        } else {
            FillRect(10, 10, 30, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::L1).bHeld) {
            DrawRect(10, 30, 30, 10);
        } else {
            FillRect(10, 30, 30, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::R2).bHeld) {
            DrawRect(360, 10, 30, 10);
        } else {
            FillRect(360, 10, 30, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::R1).bHeld) {
            DrawRect(360, 30, 30, 10);
        } else {
            FillRect(360, 30, 30, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::FACE_U).bHeld) {
            DrawCircle(360, 70, 10);
        } else {
            FillCircle(360, 70, 10);
        }

        if (gamepad->getButton(olc::GPButtons::FACE_U).bPressed) {
            gamepad->startVibration(1);
        } else if (gamepad->getButton(olc::GPButtons::FACE_U).bReleased) {
            gamepad->stopVibration();
        }

        if (!gamepad->getButton(olc::GPButtons::FACE_L).bHeld) {
            DrawCircle(345, 85, 10);
        } else {
            FillCircle(345, 85, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::FACE_R).bHeld) {
            DrawCircle(375, 85, 10);
        } else {
            FillCircle(375, 85, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::FACE_D).bHeld) {
            DrawCircle(360, 100, 10);
        } else {
            FillCircle(360, 100, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::DPAD_U).bHeld) {
            DrawRect(50, 100, 10, 10);
        } else {
            FillRect(50, 100, 10, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::DPAD_D).bHeld) {
            DrawRect(50, 120, 10, 10);
        } else {
            FillRect(50, 120, 10, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::DPAD_L).bHeld) {
            DrawRect(40, 110, 10, 10);
        } else {
            FillRect(40, 110, 10, 10);
        }

        if (!gamepad->getButton(olc::GPButtons::DPAD_R).bHeld) {
            DrawRect(60, 110, 10, 10);
        } else {
            FillRect(60, 110, 10, 10);
        }

        FillCircle(20 + gamepad->getAxis(olc::GPAxes::LX) * 10, 70 + gamepad->getAxis(olc::GPAxes::LY) * 10, 10);

        FillCircle(320 + gamepad->getAxis(olc::GPAxes::RX) * 10, 100 + gamepad->getAxis(olc::GPAxes::RY) * 10, 10);
        return true;
    }
};

int main() {
    Test test;
    test.Construct(640, 480, 1, 1, false, true);
    test.Start();
}
