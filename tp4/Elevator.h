#pragma once

#include "frameworks.h"

class Elevator {
private:
    pos_t position;
    int speed;
    direction_t direction;


public:
    Elevator(pos_t position) :position(position) {
        this->speed = config::elevatorStandardSpeed;
        this->direction = up;
    };
    void draw(Graphics& graphics) {
        Image image(config::elevatorImagePath);
        graphics.DrawImage(&image, this->position.x, this->position.y, config::elevatorImageHeight, config::elevatorImageWidth);
    }
    void updatePos(RECT& rect) {
        //poruszanie winda w zaleznosci od direction
        if (this->direction == up) {
            this->position.y -= this->speed;
        }
        else if (this->direction == down) {
            this->position.y += this->speed;
        }
        //zmiana kierunku w skrajnych przypadkach
        if ((this->position.y + config::elevatorImageHeight + config::elevatorMargines >= rect.bottom)) {
            this->direction = up;
        }
        else if (this->position.y <= 0) {
            this->direction = down;
        }
    }
};