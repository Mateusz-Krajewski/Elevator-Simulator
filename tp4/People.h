#pragma once
#include "Config.h"
#include <random>

enum direct {
	up,
	down,
	none
};


class People {
private:
	direct wantDrive;
	int fromFloor;
	int toFloor;
	int weight;
public:
	~People() {};
	People(int fromFloor, int toFloor) {
		this->fromFloor = fromFloor;
		this->toFloor = toFloor;
		if (config::people::randomWeight == false) {
			this->weight = config::people::staticPeopleWeight;
		}
		else {
			srand(time(0));
			this->weight= rand() % config::people::maxWeight + config::people::minWeight;
		}

		if (fromFloor > toFloor) {
			this->wantDrive = down;
		}
		else {
			this->wantDrive = up;
		}
	}
	direct getWantDrive() { return this->wantDrive; }
	int getFromFloor() { return this->fromFloor; }
	int getToFloor() { return this->toFloor; }
	int getWeight() { return this->weight; }




};