/*
This PGEX was made possible by the following people:
-Testers
My Girlfriend (Thx for letting me borrow your time)
Dragoneye
Javid
KrossX
MaGetzUb
Tutas
Zleapingbear

Thx for KrossX for giving me a link to his original work, which helped solve a few problems
*/

#include "olcPixelGameEngine.h"
#include <vector>
#include <string>

#pragma region Class Declaration

#ifndef OLC_PGEX_GAMEPAD
#define OLC_PGEX_GAMEPAD

#ifdef WIN32
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <wbemidl.h>
#include <oleauto.h>

#if _WIN32_WINNT == _WIN32_WINNT_NT4
#define CURVERSION _WIN32_WINNT_NT4
#elif _WIN32_WINNT == _WIN32_WINNT_WIN2K
#define CURVERSION _WIN32_WINNT_WIN2K
#elif _WIN32_WINNT == _WIN32_WINNT_WINXP
#define CURVERSION _WIN32_WINNT_WINXP
#elif _WIN32_WINNT == _WIN32_WINNT_WS03
#define CURVERSION _WIN32_WINNT_WS03
#elif _WIN32_WINNT == _WIN32_WINNT_WIN6
#define CURVERSION _WIN32_WINNT_WIN6
#elif _WIN32_WINNT == _WIN32_WINNT_WIN7
#define CURVERSION _WIN32_WINNT_WIN7
#elif _WIN32_WINNT == _WIN32_WINNT_WIN8
#define CURVERSION _WIN32_WINNT_WIN8
#elif _WIN32_WINNT == _WIN32_WINNT_WINBLUE
#define CURVERSION _WIN32_WINNT_WINBLUE
#elif _WIN32_WINNT == _WIN32_WINNT_WINXP
#define CURVERSION _WIN32_WINNT_WINXP
#elif _WIN32_WINNT == _WIN32_WINNT_WIN10
#define CURVERSION _WIN32_WINNT_WIN10
#endif

#undef _WIN32_WINNT
#define _WIN32_WINNT 1
#include <Xinput.h>
#undef _WIN32_WINNT
#define _WIN32_WINNT CURVERSION
#pragma comment(lib, "xinput9_1_0.lib")
#endif

#ifdef __linux__
#include <linux/input.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#endif

namespace olc
{
#pragma region Enums
	enum class GPButtons {
		FACE_D = 1,
		FACE_L = 0,
		FACE_R = 2,
		FACE_U = 3,
		L1 = 4,
		L2 = 6,
		L3 = 10,
		R1 = 5,
		R2 = 7,
		R3 = 11,
		SELECT = 8,
		START = 9,
		DPAD_L = 14,
		DPAD_R = 15,
		DPAD_U = 16,
		DPAD_D = 17,
	};

	enum class GPAxes {
		LX = 1,
		LY = 0,
		RX = 5,
		RY = 2,
		TL = 4,
		TR = 3,
		DX = 6,
		DY = 7,
	};
#pragma endregion

	class GamePad {
	public:
#ifdef WIN32
		GamePad(LPCDIDEVICEINSTANCE lpddi);
		GamePad(DWORD xId);
#endif
#ifdef __linux__
		GamePad(std::string path);
#endif
		GamePad();
		~GamePad();
		static void init();
		static std::vector<GamePad> getGamepads();
		bool valid = true;
		void poll();
		float getAxis(GPAxes a);
		HWButton getButton(GPButtons b);
		std::string getName();
		int getAxisCount();
		int getButtonCount();
		void startVibration(float strength = 1);
		void stopVibration();

		static GamePad selectWithButton(std::vector<GamePad> &pads, GPButtons b);
	private:
		std::string name = "";
		int axisCount = 8;
		int buttonCount = 16;
		float axes[8]{ 0 };
		olc::HWButton buttons[20];
		bool ff;

		void handleButton(int id, bool value);
#ifdef WIN32
		LPDIRECTINPUTDEVICE8 thisDevice;
		static IDirectInput8 *dev;
		std::vector<int> axisPositions;
		LPDIRECTINPUTEFFECT effect = nullptr;

		bool xInput = false;
		DWORD xId;
#endif
#ifdef __linux__
		std::vector<int> maxAbs;
		bool readEvent(input_event &event);
		int fd;
		ff_effect effect;
		std::string path;
#endif
	};
}

#endif

#pragma endregion

#ifdef OLC_PGE_GAMEPAD
#undef OLC_PGE_GAMEPAD

#pragma region Custom Dataformat
#ifdef WIN32

struct GamePadState {
	DWORD axes[6];
	DWORD povHat;
	BYTE buttons[20];
};

#ifndef DIDFT_OPTIONAL
constexpr DWORD DIDFT_OPTIONAL = 0x80000000;
#endif

DIOBJECTDATAFORMAT g_aObjectFormats[27] = {
	{0, (DWORD)FIELD_OFFSET(GamePadState, axes[1])    , DIDFT_AXIS | DIDFT_ANYINSTANCE                 , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, axes[0])    , DIDFT_AXIS | DIDFT_ANYINSTANCE                 , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, axes[3])    , DIDFT_AXIS | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, axes[2])    , DIDFT_AXIS | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, axes[4])    , DIDFT_AXIS | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, axes[5])    , DIDFT_AXIS | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, povHat)     , DIDFT_POV | DIDFT_ANYINSTANCE                  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 0]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 1]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 2]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 3]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 4]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 5]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 6]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 7]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 8]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[ 9]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[10]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[11]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[12]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[13]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[14]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[15]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[16]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[17]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[18]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
	{0, (DWORD)FIELD_OFFSET(GamePadState, buttons[19]), DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000  , 0},
};

DIDATAFORMAT gamepad = {
	sizeof(DIDATAFORMAT),
	sizeof(DIOBJECTDATAFORMAT),
	DIDF_ABSAXIS,
	sizeof(GamePadState),
	27,
	g_aObjectFormats
};

#endif
#pragma endregion

#pragma region Class Definition

#pragma region Platform Dependent

#pragma region Windows
#ifdef WIN32

IDirectInput8 *olc::GamePad::dev = nullptr;

void olc::GamePad::init() {
	HINSTANCE hinst = GetModuleHandle(NULL);
	DirectInput8Create(
		hinst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8A,
		(LPVOID *)&dev,
		NULL
	);
}

std::vector<olc::GamePad> olc::GamePad::getGamepads() {
	std::vector<olc::GamePad> result;

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
		XINPUT_STATE state;
		DWORD dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS) {
			GamePad xGamePad{ i };
			result.push_back(xGamePad);
		}
	}

	auto enumFunction = [](LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
		if (!(lpddi->dwDevType & DI8DEVTYPE_GAMEPAD))
			return DIENUM_CONTINUE;

		auto result = reinterpret_cast<std::vector<olc::GamePad> *>(pvRef);
		result->push_back(olc::GamePad{ lpddi });

		return DIENUM_CONTINUE;
	};

	dev->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		enumFunction,
		(LPVOID)&result,
		DIEDFL_ATTACHEDONLY
	);

	return result;
}

inline olc::GamePad::GamePad(LPCDIDEVICEINSTANCE lpddi)
{
	name = lpddi->tszInstanceName;

	dev->CreateDevice(lpddi->guidInstance, &thisDevice, NULL);

	DIDEVCAPS caps;
	caps.dwSize = sizeof(DIDEVCAPS);
	thisDevice->GetCapabilities(&caps);

	DIPROPDWORD bufferSize;
	bufferSize.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	bufferSize.diph.dwSize = sizeof(DIPROPDWORD);
	bufferSize.diph.dwHow = DIPH_DEVICE;
	bufferSize.diph.dwObj = 0;
	bufferSize.dwData = 128;
	thisDevice->SetProperty(DIPROP_BUFFERSIZE, &bufferSize.diph);
	ff = caps.dwFlags & DIDC_FORCEFEEDBACK;

	thisDevice->SetDataFormat(&gamepad);

	auto enumDOFunction = [](LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef) {

		IDirectInputDevice8A *thisDevice;
		std::vector<int> *axisPositions;

		std::tie(thisDevice, axisPositions) = *reinterpret_cast<std::tuple<IDirectInputDevice8A*, std::vector<int> *> *>(pvRef);

		//Axis
		if (lpddoi->dwType & DIDFT_AXIS) {
			DIPROPRANGE range;
			range.diph.dwSize = sizeof(DIPROPRANGE);
			range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			range.diph.dwHow = DIPH_BYID;
			range.diph.dwObj = lpddoi->dwType;
			range.lMin = -1000;
			range.lMax = 1000;
			thisDevice->SetProperty(DIPROP_RANGE, &range.diph);
			axisPositions->push_back((lpddoi->dwType >> 8) & 0xffff);
		}

		//Button
		if (lpddoi->dwType & DIDFT_BUTTON) {

		}

		//POVhat
		if (lpddoi->dwType & DIDFT_POV) {

		}

		return DIENUM_CONTINUE;
	};

	auto input = std::make_tuple(thisDevice, &axisPositions);

	thisDevice->EnumObjects(
		enumDOFunction,
		&input,
		DIDFT_ALL
	);

	int min = 0x7fffffff;
	for (auto p : axisPositions)
		min = std::min(min, p);

	for (auto &p : axisPositions)
		p -= min;

	for (int i = 0; i < 8; i++)
		axes[i] = 0;
	for (int i = 0; i < 20; i++) {
		buttons[i].bHeld = false;
		buttons[i].bPressed = false;
		buttons[i].bReleased = false;
	}

	if (ff) {
		DIPERIODIC periodic;
		periodic.dwMagnitude = DI_FFNOMINALMAX;
		periodic.lOffset = 0;
		periodic.dwPhase = 0;
		periodic.dwPeriod = DWORD(0.05f * DI_SECONDS);
		DIEFFECT eff;
		eff.dwSize = sizeof(DIEFFECT);
		eff.dwDuration = INFINITE;
		eff.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
		eff.dwSamplePeriod = 0;
		eff.dwGain = DI_FFNOMINALMAX;
		eff.cAxes = 2;
		DWORD axes[] = { DIJOFS_X, DIJOFS_Y };
		eff.rgdwAxes = axes;
		LONG lDirection[2] = { 0, 0 };
		eff.rglDirection = lDirection;
		eff.cbTypeSpecificParams = sizeof(periodic);
		eff.lpvTypeSpecificParams = &periodic;

		thisDevice->CreateEffect(GUID_Sine, &eff, &effect, NULL);
		if (effect) {
			effect->Download();
			effect->Stop();
		}
		else
			ff = false;
	}

	thisDevice->Acquire();
}

inline olc::GamePad::GamePad(DWORD xId) :xId(xId), xInput(true) {
	XINPUT_CAPABILITIES caps;
	XInputGetCapabilities(xId, 0, &caps);
	name = "XInput Controller";
}

void olc::GamePad::poll() {

	for (int i = 0; i < 20; i++) {
		buttons[i].bPressed = false;
		buttons[i].bReleased = false;
	}

	if (!xInput) {
		DIDEVICEOBJECTDATA data[128];

		if (FAILED(thisDevice->Poll())) {
			thisDevice->Acquire();
			return;
		}

		DWORD items = 128;
		thisDevice->GetDeviceData(
			sizeof(DIDEVICEOBJECTDATA),
			data,
			&items,
			0
		);

		for (size_t i = 0; i < items; i++) {
			//Button
			if (data[i].dwOfs >= (DWORD)FIELD_OFFSET(GamePadState, buttons[0]) && data[i].dwOfs <= (DWORD)FIELD_OFFSET(GamePadState, buttons[19])) {
				int id = data[i].dwOfs - FIELD_OFFSET(GamePadState, buttons[0]);
				bool pressed = data[i].dwData & 0x80;
				handleButton(id, pressed);
			}

			//POVhat
			if (data[i].dwOfs == FIELD_OFFSET(GamePadState, povHat)) {
				switch (data[i].dwData / DI_DEGREES / 45) {
				case 0:
					axes[6] = 0;
					axes[7] = -1;
					break;
				case 1:
					axes[6] = 1;
					axes[7] = -1;
					break;
				case 2:
					axes[6] = 1;
					axes[7] = 0;
					break;
				case 3:
					axes[6] = 1;
					axes[7] = 1;
					break;
				case 4:
					axes[6] = 0;
					axes[7] = 1;
					break;
				case 5:
					axes[6] = -1;
					axes[7] = 1;
					break;
				case 6:
					axes[6] = -1;
					axes[7] = 0;
					break;
				case 7:
					axes[6] = -1;
					axes[7] = -1;
					break;
				default:
					axes[6] = 0;
					axes[7] = 0;
				}

				int x = int(axes[6]), y = int(axes[7]);
				handleButton(14, x == -1);
				handleButton(15, x == 1);
				handleButton(16, y == -1);
				handleButton(17, y == 1);
			}

			//Axis
			if (data[i].dwOfs >= (DWORD)FIELD_OFFSET(GamePadState, axes[0]) && data[i].dwOfs <= (DWORD)FIELD_OFFSET(GamePadState, axes[5])) {
				int id = ((data[i].dwOfs - offsetof(GamePadState, axes[0])) / 4);
				int val = data[i].dwData;
				axes[axisPositions[id]] = val / 1000.0f;
			}
		}
	}
	else {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		XInputGetState(xId, &state);

		axes[2] = -state.Gamepad.sThumbRY / 32768.0f;
		axes[5] = state.Gamepad.sThumbRX / 32768.0f;
		axes[0] = -state.Gamepad.sThumbLY / 32768.0f;
		axes[1] = state.Gamepad.sThumbLX / 32768.0f;

		axes[4] = state.Gamepad.bLeftTrigger / 255.0f;
		axes[3] = state.Gamepad.bRightTrigger / 255.0f;

		bool u, d, l, r;
		u = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
		d = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		l = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
		r = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

		axes[6] = float(r - l);
		axes[7] = float(d - u);

		WORD buttonCodes[]{
			XINPUT_GAMEPAD_X,
			XINPUT_GAMEPAD_A,
			XINPUT_GAMEPAD_B,
			XINPUT_GAMEPAD_Y,
			XINPUT_GAMEPAD_LEFT_SHOULDER,
			XINPUT_GAMEPAD_RIGHT_SHOULDER,
			0,
			0,
			XINPUT_GAMEPAD_BACK,
			XINPUT_GAMEPAD_START,
			XINPUT_GAMEPAD_LEFT_THUMB,
			XINPUT_GAMEPAD_RIGHT_THUMB,
			0,
			0,
			XINPUT_GAMEPAD_DPAD_LEFT,
			XINPUT_GAMEPAD_DPAD_RIGHT,
			XINPUT_GAMEPAD_DPAD_UP,
			XINPUT_GAMEPAD_DPAD_DOWN,
		};

		for (size_t i = 0; i < sizeof(buttonCodes) / sizeof(WORD); i++) {
			bool pressed = state.Gamepad.wButtons & buttonCodes[i];
			handleButton(i, pressed);
		}

		handleButton(6, axes[4] > 0.75f);
		handleButton(7, axes[3] > 0.75f);
	}
}

void olc::GamePad::startVibration(float strength) {
	if (xInput) {
		if (strength < 0) strength = 0;
		if (strength > 1) strength = 1;

		XINPUT_VIBRATION vib;
		ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
		vib.wLeftMotorSpeed = short(65535 * strength);
		vib.wRightMotorSpeed = short(65535 * strength);
		XInputSetState(xId, &vib);
	}
	else if (ff) {
		DIEFFECT gainMod;
		gainMod.dwGain = DWORD(DI_FFNOMINALMAX * strength);
		effect->SetParameters(&gainMod, DIEP_GAIN);
		effect->Download();
		effect->Start(1, DIES_SOLO);
	}
}

void olc::GamePad::stopVibration() {
	if (xInput)
		startVibration(0);
	else {
		if (ff)
			effect->Stop();
	}
}

olc::GamePad::~GamePad() {

}

#endif
#pragma endregion

#pragma region Linux

#ifdef __linux__
#define ucharIndexForBit(bit) (bit/8)
#define bitOffsetInUchar(bit) (bit%8)
#define testBit(bit, array) ((array[bit/8] >> (bit%8)) & 1)
olc::GamePad::GamePad(std::string path) {
	fd = open(path.c_str(), O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		valid = false;
		return;
	}
	char name[256] = "";
	if (ioctl(fd, EVIOCGNAME(256), name) > 0)
		this->name = name;
	else this->name = "Undefined";

	const int32_t axisCodes[]{
		ABS_Y,
		ABS_X,
		ABS_RY,
		ABS_RZ,
		ABS_Z,
		ABS_RX,
		ABS_HAT0X,
		ABS_HAT0Y
	};

	input_absinfo info;

	for (int i = 0; i < sizeof(axisCodes) / sizeof(int32_t); i++) {
		ioctl(fd, EVIOCGABS(axisCodes[i]), &info);
		maxAbs.push_back(info.maximum);
	}

	unsigned char features[1 + FF_MAX / 8];
	memset(features, 0, sizeof(features));
	ioctl(fd, EVIOCGBIT(EV_FF, sizeof(features)), features);
	ff = testBit(FF_PERIODIC, features);

	if (ff) {
		memset(&effect, 0, sizeof(effect));
		ff_periodic_effect periodic;
		effect.u.periodic.waveform = FF_SINE;
		effect.u.periodic.period = 100;
		effect.u.periodic.magnitude = 0x7fff;
		effect.u.periodic.offset = 0;
		effect.u.periodic.phase = 0;
		effect.u.periodic.envelope.attack_length = 0;
		effect.u.periodic.envelope.attack_level = 0x7fff;
		effect.u.periodic.envelope.fade_length = 0;
		effect.u.periodic.envelope.fade_level = 0x7fff;
		effect.replay.delay = 0;
		effect.replay.length = 0xffff;
		effect.type = FF_PERIODIC;
		effect.id = -1;
		effect.direction = 0;

		if (ioctl(fd, EVIOCSFF, &effect) == -1)
			perror("Error:");
	}

	this->path = path;
}

void olc::GamePad::init() {

}

bool olc::GamePad::readEvent(input_event &event) {
	return read(fd, &event, sizeof(input_event)) == sizeof(input_event);
}

void olc::GamePad::poll() {

	for (int i = 0; i < 20; i++) {
		buttons[i].bPressed = false;
		buttons[i].bReleased = false;
	}

	input_event event;
	while (readEvent(event)) {
		if (event.type == EV_KEY && ((event.code & BTN_GAMEPAD) == BTN_GAMEPAD || (event.code & BTN_DPAD_UP) == BTN_DPAD_UP)) {
			const int32_t buttonCodes[]{
				BTN_X,
				BTN_A,
				BTN_B,
				BTN_Y,
				BTN_TL,
				BTN_TR,
				BTN_TL2,
				BTN_TR2,
				BTN_SELECT,
				BTN_START,
				BTN_THUMBL,
				BTN_THUMBR,
				0,
				0,
				BTN_DPAD_LEFT,
				BTN_DPAD_RIGHT,
				BTN_DPAD_UP,
				BTN_DPAD_DOWN
			};

			for (int i = 0; i < sizeof(buttonCodes) / sizeof(int32_t); i++) {
				std::cout << "Event code: " << event.code << '\n';
				if (event.code == buttonCodes[i]) {
					bool pressed = event.value;
					handleButton(i, pressed);

					std::cout << "Button id: " << i << "\n";

					if (i >= 14) {
						if (i == 14)
							axes[6] = -int(pressed);
						if (i == 15)
							axes[6] = int(pressed);
						if (i == 16)
							axes[7] = -int(pressed);
						if (i == 17)
							axes[7] = int(pressed);
					}
					if (i == 6) axes[4] = int(pressed);
					if (i == 7) axes[3] = int(pressed);
				}
			}
		}
		if (event.type == EV_ABS) {
			const int32_t axisCodes[]{
				ABS_Y,
				ABS_X,
				ABS_RY,
				ABS_RZ,
				ABS_Z,
				ABS_RX,
				ABS_HAT0X,
				ABS_HAT0Y
			};
			for (int i = 0; i < sizeof(axisCodes) / sizeof(int32_t); i++) {
				if (event.code == axisCodes[i]) {
					float value = float(event.value) / maxAbs[i];
					axes[i] = value;

					if (i == 6) {
						handleButton(14, event.value == -1);
						handleButton(15, event.value == 1);
					}
					if (i == 7) {
						handleButton(17, event.value == -1);
						handleButton(16, event.value == 1);
					}
					if (i == 4)
						handleButton(6, axes[4] >= 0.75f);
					if (i == 3)
						handleButton(7, axes[3] >= 0.75f);
				}
			}
		}
	}
}

std::vector<olc::GamePad> olc::GamePad::getGamepads() {
	std::vector<GamePad> result;

	dirent **list;

	int items = scandir(
		"/dev/input/",
		&list,
		[](const dirent *name) {return int(strncmp("event", name->d_name, 5) == 0); },
		alphasort);

	for (int i = 0; i < items; i++) {
		std::string path = "/dev/input/";
		GamePad gp{ path + list[i]->d_name };
		if (gp.valid)
			result.push_back(gp);
	}

	return result;
}

void olc::GamePad::startVibration(float strength) {
	if (!ff) return;
	if (strength < 0) strength = 0;
	if (strength > 1) strength = 1;
	int g = int(0xFFFF * strength);
	input_event gain;
	gain.type = EV_FF;
	gain.code = FF_GAIN;
	gain.value = g;
	int s = write(fd, &gain, sizeof(gain));

	input_event play;
	play.type = EV_FF;
	play.code = effect.id;
	play.value = 3;
	s = write(fd, (const void*)&play, sizeof(play));
}

void olc::GamePad::stopVibration() {
	if (!ff) return;
	input_event stop;
	stop.type = EV_FF;
	stop.code = effect.id;
	stop.value = 0;
	int s = write(fd, (const void*)&stop, sizeof(stop));
}

olc::GamePad::~GamePad() {
}
#endif

#pragma endregion

#pragma endregion

#pragma region Common

olc::GamePad::GamePad() {
	valid = false;
}

olc::GamePad olc::GamePad::selectWithButton(std::vector<olc::GamePad> &pads, olc::GPButtons b) {
	for (auto &gp : pads) {
		gp.poll();
		if (gp.getButton(b).bPressed)
			return gp;
	}
	GamePad nullPad;
	return nullPad;
}

#ifndef OLC_GAMEPAD_DEADZONE
#define OLC_GAMEPAD_DEADZONE 0.2f
#endif
float olc::GamePad::getAxis(olc::GPAxes a) {

	float axis = axes[static_cast<int>(a)];
	if (axis > 1) axis = 1;
	if (axis < -1) axis = -1;
	if (axis <= OLC_GAMEPAD_DEADZONE && axis >= -OLC_GAMEPAD_DEADZONE)
		axis = 0;

#ifdef WIN32
	if (!xInput && (a == GPAxes::TL || a == GPAxes::TR))
		return (axis + 1) / 2;
#endif
	return axis;
}

olc::HWButton olc::GamePad::getButton(olc::GPButtons b) {
	return buttons[static_cast<int>(b)];
}

std::string olc::GamePad::getName() {
	return name;
}

int olc::GamePad::getAxisCount() {
	return axisCount;
}

int olc::GamePad::getButtonCount() {
	return buttonCount;
}

void olc::GamePad::handleButton(int id, bool value) {
	if (value && !buttons[id].bHeld) {
		buttons[id].bHeld = true;
		buttons[id].bPressed = true;
	}
	if (!value && buttons[id].bHeld) {
		buttons[id].bHeld = false;
		buttons[id].bReleased = true;
	}
}

#pragma endregion

#endif //OLC_PGE_GAMEPAD

#pragma endregion
