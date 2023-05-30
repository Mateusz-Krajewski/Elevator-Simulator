#pragma once


class Floor {
private:
    int floorNumber;

public:
    Floor(int number) : floorNumber(number) {}

    int getFloorNumber() const {
        return floorNumber;
    }
};