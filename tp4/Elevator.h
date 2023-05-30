#pragma once
#include "Config.h"
#include "People.h"
#include <list>
#include <queue>

class Elevator {
private:
	const int MAX_PEOPLE_NUM = config::elevator::maxPeopleNum;
	const int MAX_PEOPLE_WEIGHT = config::elevator::maxPeopleWeight;
	int currentFloor;
	int currentWeight;
	int currentPeopleNum;
	direct direction;
	std::list<int> targetFloors;
	std::list<People> peopleList;
public:
	Elevator(){
		currentFloor = 0;
		currentWeight = 0;
		currentPeopleNum = 0;
		direction = direct::none;
	}
void setDirection(direct d) {
		direction = d;
	}
	direct getDirection() {
		return direction;
	}
void setCurrentFloor(int floor) {
		currentFloor = floor;
	}
	int getCurrentFloor() {
		return currentFloor;
	}
void setCurrentWeight(int weight) {
		currentWeight = weight;
	}
	int getCurrentWeight() {
		return currentWeight;
	}
void setCurrentPeopleNum(int num) {
		currentPeopleNum = num;
	}
	int getCurrentPeopleNum() {
		return currentPeopleNum;
	}
	void addTargetFloor(int floor) {
		targetFloors.push_back(floor);
	}


};