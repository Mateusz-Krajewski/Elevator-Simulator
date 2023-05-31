#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include "Config.h"

#include <filesystem>

struct pos {
    int x;
    int y;
};

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class Elevator {
private:
    pos position;
    int speed;


public:
    Elevator(pos position):position(position) {
        this->speed = config::elevatorStandardSpeed;
    };
    void draw(Graphics &graphics) {
        Image image(config::elevatorImagePath);
        graphics.DrawImage(&image, this->position.x,this->position.y, config::elevatorImageHeight, config::elevatorImageWidth);
    }
    void updatePos(RECT &rect) {
        // Update the position of the image
        this->position.y += this->speed;

        // Check if the image has reached the bottom of the window
        if ((this->position.y + config::elevatorImageHeight +config::elevatorMargines >= rect.bottom) || (this->position.y <= 0))
        {
            // Reset the position of the image to the top
            this->speed *= (-1);
        }
    }
};

Elevator ele({100,100});

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    ele.draw(graphics);
}
VOID OnTimer(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    ele.updatePos(rect); //update elevator position


    // Request a repaint of the window
    InvalidateRect(hWnd, NULL, FALSE);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("Getting Started"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        config::windowWidth,            // initial x size
        config::windowHeight,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    SetTimer(hWnd, 1, 16, NULL);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_TIMER:
        OnTimer(hWnd);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc