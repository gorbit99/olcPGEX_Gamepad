# olcPGEX\_Gamepad
Cross platform (Windows + Linux) Gamepad API for the Pixel Game Engine (http://onelonecoder.com/)

# Compiling

#### Windows
With Visual Studio installed, you don't need to do anything besides putting the file inside your solution folder.

With MinGW the following command can be used to compile:

```
g++ main.cpp -o program.exe -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -lstdc++fs -static -std=c++17 -lxinput9_1_0 -lole32 -loleaut32 -ldinput8 -ldxguid -ldwmapi
```

#### Linux
On linux, compiling shouldn't change, just include the .h file in your project.

# Usage
When using the code, you have to define `OLC_PGE_GAMEPAD` in the same manner as you would with `OLC_PGE_APPLICATION`.

To initialize the extension, call `olc::GamePad::init()`. Afterwards it will automatically collect gamepads that are connected and will poll them for input.

To get a gamepad, you can either get a reference to the std::vector containing all of the controllers with `olc::GamePad::getGamepads()` and index into that, or if you only need one you can use
`olc::Gamepad::selectWithButton(button)`. Both of these methods return a pointer to a gamepad. SelectWithButton returns nullptr, if no such gamepad is found.

You can query the capabilities of a certain contoller by calling `myGamepad->hasButton(button)` and `myGamepad->hasAxis(axis)`. To get the state of a button or axis, you can do `myGamepad->getButton(button)`
and `myGamepad->getAxis(axis)`. Buttons use the usual structure known from the PixelGameEngine, axes return a float.

The following is the usual usecase of selectWithButton:
```cpp
#define OLC_PGE_GAMEPAD
#include "olcPGEX_Gamepad.h"

//Variable declarations
olc::GamePad *player;

//OnUserCreate
olc::GamePad::init();

//OnUserUpdate
if (player == nullptr){
  player = olc::GamePad::selectWithButton(olc::GPButtons::FACE_D);
  return true;
}
```
This returns the first controller with the bottom face button pressed (A on XBOX controllers, X on PS controllers).

To make the controller vibrate, you can use `myGamepad->startVibration(strength)` and `myGamepad->stopVibration()`. Strength is a value between 0 and 1.

To distinguish between multiple controllers, use the `myGamepad->getId()` function.

The following buttons are part of the `olc::GPButtons` enum:
```
Enum      XBOX/PS
-----------------
FACE_D    A/X
FACE_L    X/Square
FACE_R    B/Circle
FACE_U    Y/Triangle
L1        Left shoulder button
L2        Left trigger as a button
L3        Left stick button
R1        Right shoulder button
R2        Right trigger as a button
R3        Right stick button
SELECT    Back/Share
START     Start/Options
DPAD_L    Left DPAD direction as a button
DPAD_R    Right DPAD direction as a button
DPAD_U    Up DPAD direction as a button
DPAD_D    Down DPAD direction as a button
```
The following axes are part of the `olc::GPAxes` enum:
```
Enum    Range    Name
LX      -1..1    Left stick horizontal
LY      -1..1    Left stick vertical
RX      -1..1    Right stick horizontal
RY      -1..1    Right stick vertical
TL       0..1    Left trigger as an axis
TR       0..1    Right trigger as an axis
DX      -1..1    DPAD horizontal axis
DY      -1..1    DPAD vertical axis
```

An example file is provided to test the extension.

#### Controller support:
The code supports Xbox controllers and the DualShock 4 controller out of the box. If you have a problem with the support for a game controller, please open an issue and I'll try to resolve it. Alternatively there exists many programs to turn any controller into an xinput compatible controller, those should fix the issue.

# Migrating from the previous version
Version 0.2.0 changed a lot, so for previous users, the following has to be done to migrate to the new version:

- Now you get gamepads in pointer form, so you'll need to swap the types out.
- Getting the gamepad vector now gives you a reference to a vector of pointers, also you can index into it safely now.
- You don't need to (and can't) manually poll gamepads anymore.
