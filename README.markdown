# olcPGEX_Gamepad
Cross platform (Windows + Linux) Gamepad API for the Pixel Game Engine (http://onelonecoder.com/)

# Compiling

#### Windows
With Visual Studio installed, you don't need to do anything besides putting the file inside your solution folder. If you're using a different compiler that doesn't support `#pragma comment(lib, ...)`, then you have to link against the following:<br>
```
dinput8.lib
dxguid.lib
xinput9_1_0.lib
```
#### Linux
On linux, compiling shouldn't change, just include the .h file.

# Usage
When using the code, you have to define `OLC_PGE_GAMEPAD` in the same manner as you would with `OLC_PGE_APPLICATION`.
To use the PGEX, first you have to call `olc::GamePad::init()`, this only matters on windows, but it makes the code cross-platform.<br><br>
You can get a vector of gamepads using `olc::GamePad::getGamepads()`, this will return an `std::vector<olc::GamePad>` object. You can of course index into this vector, but this isn't advisable, as the order isn't defined, and can change, and some controllers can appear multiple times in the vector. Instead use `olc::GamePad::selectWithButton(std::vector<olc::GamePad> gamepads, olc::GPBUTTONS button)`. This takes in a button that will be queried, and returns the first controller in the list, that has this button down. If no controllers are found, an invalid one will be returned. The following code is an example:
```cpp
//Variable declarations
olc::GamePad player1;
std::vector<olc::GamePad> gamepads;
//OnUserCreate
olc::GamePad::init();
gamepads = olc::GamePad::getGamepads();
//OnUserUpdate
if (!player1.valid) {
  player1 = olc::GamePad::selectWithButton(gamepads, olc::GPButton::FACE_D);
  return true;
}
```
This returns the first controller with the bottom face button pressed (A on XBOX controllers, X on PS controllers).<br><br
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
These can be queried using `getButton(olc::GPButton)` and `getAxis(olc::GPAxes)` respectively. `getButton` returns an `HWButton` struct that works exactly the same as getting a key in the PixelGameEngine. A deadzone has been built into `getAxis`, which works on a per axis basis, instead of acting on the axis pairs. If you want to alter the behaviour, define `OLC_GAMEPAD_DEADZONE` as a floating point value.<br><br>
You can get some information about the device using the following functions:
```c++
std::string getName();    //Returns the name of the gamepad
int getAxisCount();       //Returns the number of axes (always 8)
int getButtonCount();     //Returns the number of buttons (always 16)
```
Vibrations are also supported, use `startVibration(float strength)` to start them and `stopVibration()` to stop them.
#### Controller support:
The code supports Xbox controllers and the DualShock 4 controller out of the box. If you have a problem with the support for a game controller, please open an issue and I'll try to resolve it. Alternatively there exists many programs to turn any controller into an xinput compatible controller, those should fix the issue.
