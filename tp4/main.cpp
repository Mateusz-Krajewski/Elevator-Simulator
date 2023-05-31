#include "frameworks.h"

struct pos_t {
    int x;
    int y;
};
enum direction_t {
    up,
    down,
    wait
};

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class Floor {
private:
    pos_t position[2];
    int floorNum;
public:
    Floor(pos_t position[2], int floorNum) :floorNum(floorNum) {
        this->position[0] = position[0];
        this->position[1] = position[1];
    };
    void draw(Graphics &graphics) {
        Pen pen(Color(255,0,0));
        graphics.DrawLine(&pen,this->position[0].x,this->position[0].y,this->position[1].x,this->position[1].y);
    }
    int getY() {
        return this->position[0].y;
    }
    bool needStop() {
        return true;
    }
};


class Elevator {
private:
    pos_t position;
    int speed;
    direction_t direction;
    int stopTimer;


public:
    Elevator(pos_t position):position(position) {
        this->speed = config::elevatorStandardSpeed;
        this->direction = up;
        this->stopTimer = -1;
    };
    void draw(Graphics &graphics) {
        Image image(config::elevatorImagePath);
        graphics.DrawImage(&image, this->position.x,this->position.y, config::elevatorImageHeight, config::elevatorImageWidth);
    }
    void updatePos(RECT &rect,std::vector<Floor> floors) {
        if (this->stopTimer != -1) {
            this->stopTimer++;
        }
        for (auto f : floors) {
            if ((f.getY() == this->position.y+config::elevatorImageHeight+config::elevatorMargines)&&(this->direction!=wait)) {
                this->direction = wait;
                this->stopTimer = 0;
            }
        }
        if (this->stopTimer >= 10) {
            this->direction = down;
            this->stopTimer = -1;
       }


        //poruszanie winda w zaleznosci od direction
        if (this->direction == up) {
            this->position.y -= this->speed;
        }else if (this->direction == down) {
            this->position.y += this->speed;
        }
        //zmiana kierunku w skrajnych przypadkach
        if ((this->position.y + config::elevatorImageHeight + config::elevatorMargines >= rect.bottom)) {
            this->direction = up;
        }else if(this->position.y <= 0){
            this->direction = down;
        }
    }
};

pos_t p[2] = { { 0,200 },{100,200} };
pos_t p2[2] = { {0,400},{100,400} };
std::vector<Floor> floors = { {p,0},{ p2,1 } };
Elevator elevator({100,100});

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    elevator.draw(graphics);
    for (auto f : floors) {
        f.draw(graphics);
    }
}

VOID OnTimer(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    elevator.updatePos(rect,floors); //update elevator position


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