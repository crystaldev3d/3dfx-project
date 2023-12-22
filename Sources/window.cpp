#include "window.h"
#include "super_header.h"

#include "glide_demo.h"
#include "camera.h"

#include <hidusage.h>

HWND hwnd;

bool g_IsWindowActive = false;
int g_mouse_button = 0;
int g_mouse_delta_x = 0;
int g_mouse_delta_y = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			if (!Start(hwnd))
				return -1;
			return 0;
		case WM_DESTROY:
            done = true;
			return 0;
		case WM_SETCURSOR:
			SetCursor(FALSE);
			return TRUE;
		case WM_SETFOCUS:
		{
		
		}
		break;
		case WM_KILLFOCUS:
		{
		
		}
		break;
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				g_IsWindowActive = false;
			}
			else // WA_ACTIVE or WA_CLICKACTIVE
			{
				g_IsWindowActive = true;
			}
		}
		break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			if (wParam == VK_ESCAPE)
			{
				done = true;
				PostQuitMessage(0);
			}
			else
			{
				Input(uMsg, wParam);
			}
			
		}
		break;
		case WM_INPUT:
		{
			//UINT dwSize = 0;			
			//GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			//LPBYTE lpb = new BYTE[dwSize];

			UINT dwSize = sizeof(RAWINPUT);
			LPBYTE lpb = new BYTE[dwSize];

			if (lpb == NULL)
			{
				return 0;
			}
			
			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			//if (raw->header.dwType == RIM_TYPEKEYBOARD)
			//{
			//	if (raw->data.keyboard.VKey == VK_ESCAPE)
			//	{
			//		done = true;
			//		PostQuitMessage(0);
			//	}
			//	else
			//	{
			//		//printf("RawKeyboardInput: message %d key %d \n", raw->data.keyboard.Message, raw->data.keyboard.VKey);
			//		Input(raw->data.keyboard.Message, raw->data.keyboard.VKey);
			//	}
			//}
			//else 
			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				g_mouse_button = raw->data.mouse.ulButtons;
				g_mouse_delta_x = raw->data.mouse.lLastX;
				g_mouse_delta_y = raw->data.mouse.lLastY;

				Input(raw->data.keyboard.Message, raw->data.keyboard.VKey);
			}
			delete[] lpb;
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool InitRawInput(HWND hwnd)
{
	RAWINPUTDEVICE Rid[1] = {0};
	
	//Rid[0].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	//Rid[0].usUsage = 0x06;              // HID_USAGE_GENERIC_KEYBOARD
	//Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_APPKEYS;	// adds keyboard and also ignores legacy keyboard messages
	//Rid[0].hwndTarget = hwnd;

	Rid[0].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	Rid[0].usUsage = 0x02;              // HID_USAGE_GENERIC_MOUSE
	Rid[0].dwFlags = RIDEV_NOLEGACY;    // adds mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = hwnd;

	if (RegisterRawInputDevices(Rid, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		return false; //registration failed. Call GetLastError for the cause of the error
	}

	return true;
}

int main(int argc, char* argv[])
{
	static char name[] = "3Dfx GlideAPI Demo";
	WNDCLASS wc;

    HINSTANCE hInstance = GetModuleHandle(0);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = name;
	RegisterClass(&wc);

	hwnd = CreateWindow(
		name,
		name,
		WS_POPUP,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL)
		return FALSE;

	if (!InitRawInput(hwnd)) return FALSE;

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	
	MSG msg;
	while (!done) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}

        if (msg.message == WM_QUIT) 
			done = true;

        if (!Update()) 
			done = true;
	}

    End();
    DestroyWindow(hwnd);
    return 0;
}

int Run()
{
    return 0;
}

HWND GetHWND()
{
    return hwnd;
}
