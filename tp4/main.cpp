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

class People {
private:
    int targetFloor;
    int startFloor;
public:
    People(int startFloor, int targetFloor):startFloor(startFloor),targetFloor(targetFloor) {
    }
    int getTargetFloor() {
        return this->targetFloor;
    }
};

class Floor {
private:
    pos_t position[2];
    int floorNum;
    std::vector<People> people;
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
    bool needStop(direction_t direction) {
        /*
        for (auto p : this->people) {
            if((direction == up)&&(p.getTargetFloor() > this->getFloorNum())){
                return true;
            }
            else if ((direction == down) && (p.getTargetFloor() < this->getFloorNum())) {
                return true;
            }
        }
        */
        return false;
    }
    int getFloorNum() {
        return this->floorNum;
    }
};


class Elevator {
private:
    pos_t position;
    int speed;
    direction_t direction[2];
    int stopTimer;
    int actualFloorNum;
    std::vector<People> people;


public:
    Elevator(pos_t position):position(position) {
        this->speed = config::elevatorStandardSpeed;
        this->direction[1] = wait;
        this->direction[0] = up;
        this->stopTimer = -1;
        this->actualFloorNum = 0;
    };
    void newClient(People people) {
        this->people.push_back(people);
        if (this->actualFloorNum > people.getTargetFloor()) {
            this->direction[0] = this->direction[1];
            this->direction[1] = down;
        }else if (this->actualFloorNum < people.getTargetFloor()) {
            this->direction[0] = this->direction[1];
            this->direction[1] = up;
        }
    }
    void draw(Graphics &graphics) {
        Image image(config::elevatorImagePath);
        graphics.DrawImage(&image, this->position.x,this->position.y, config::elevatorImageHeight, config::elevatorImageWidth);
    }
    bool needStop(int floorNum) {
        for (auto p : this->people) {
            if (p.getTargetFloor() == floorNum) {
                return true;
            }
        }
        return false;
    }
    void goOut() {
        for (auto p = this->people.begin(); p != this->people.end();p++) {
            if (p->getTargetFloor() == this->actualFloorNum) {
                //TODO add some exit animation
                this->people.erase(p);
            }
        }
    }
    void updatePos(RECT &rect,std::vector<Floor> floors) {
        if (this->stopTimer != -1) {
            this->stopTimer++;
        }
        for (auto f : floors) {
            direction_t d;
            if (this->direction[1] != wait) {
                d = this->direction[1];
            }
            else {
                d = this->direction[0];
            }
            if ((f.getY() == this->position.y+config::elevatorImageHeight+config::elevatorMargines)&&(this->direction[1]!=wait)){
                if (f.needStop(d) || this->needStop(this->actualFloorNum)) {
                    this->direction[0] = this->direction[1];
                    this->direction[1] = wait;
                    this->stopTimer = 0;
                    this->goOut();
                }
                if (this->direction[1] == wait) {
                    if (f.getFloorNum() == (floors.size() - 1)) {
                        direction[0] = down;
                    }
                    else if (f.getFloorNum() == 0) {
                        direction[0] = up;
                    }
                }
                else {
                    if (f.getFloorNum() == (floors.size() - 1)) {
                        direction[0] = wait;
                        direction[1] = down;
                    }
                    else if (f.getFloorNum() == 0) {
                        direction[0] = wait;
                        direction[1] = up;
                    }
                }
            }
        }
        if (this->stopTimer >= 20&&this->direction[1]==wait) {
            this->direction[1]=this->direction[0];
            this->direction[0] = wait;
            this->stopTimer = -1;
       }


        //poruszanie winda w zaleznosci od direction
        if (this->direction[1] == up) {
            this->position.y -= this->speed;
        }else if (this->direction[1] == down) {
            this->position.y += this->speed;
        }
        //zmiana kierunku w skrajnych przypadkach

    }
};

pos_t p[2] = { { 0,150 },{100,150} };
pos_t p2[2] = { {0,350},{100,350} };
pos_t p3[2] = { {0,550},{100,550} };
std::vector<Floor> floors = { {p,2},{ p2,1 },{p3,0} };
Elevator elevator({100,400});

bool one = false;

VOID OnPaint(HDC hdc)
{
    if (one == false) {
        one = true;
        elevator.newClient(People(0,2));
    }
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