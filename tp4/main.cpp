#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include "Config.h"

struct pos {
    int x;
    int y;
    int speed;
};

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class Elevator {
private:
    Image* image=nullptr; // Use a pointer to Image
    int x;
    int y;
    int speed;

public:
    Elevator(Image *image, int initialX, int initialY, int initialSpeed)
        : x(initialX), y(initialY), speed(initialSpeed)
    {
        this->image = image;
    }

    ~Elevator()
    {
        delete image;
    }

    void Draw(Graphics* graphics) {
        graphics->DrawImage(image, x, y, config::elevatorImageHeight, config::elevatorImageWidth);
    }

    void UpdatePosition(HWND hWnd) {
        RECT rect;
        GetClientRect(hWnd, &rect);

        // Update the position of the image
        y += speed;

        // Check if the image has reached the bottom or top of the window
        if ((y + config::elevatorImageHeight >= rect.bottom) || (y <= 0))
        {
            // Reverse the direction of the image
            speed *= (-1);
        }

        // Request a repaint of the window
        InvalidateRect(hWnd, NULL, FALSE);
    }
};


Elevator elevator(new Image(L"C:/Users/matik/OneDrive/Pulpit/tp4/tp4/img/elevator.png"), 200,200,2);



VOID OnPaint(HDC hdc)
{
    Graphics *graphics=new Graphics(hdc);
    elevator.Draw(graphics);
}
VOID OnTimer(HWND hWnd) {
    elevator.UpdatePosition(hWnd);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Register the window class
    const wchar_t CLASS_NAME[] = L"MyWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window
    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Window Title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hWnd == NULL) {
        return 0;
    }

    // Set the timer for the OnTimer event
    SetTimer(hWnd, 1, 16, NULL);

    ShowWindow(hWnd, nCmdShow);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        Graphics graphics(hdc);
        elevator.Draw(&graphics);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
    {
        //elevator.UpdatePosition(hWnd);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
