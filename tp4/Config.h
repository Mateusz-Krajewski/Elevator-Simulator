#pragma once

namespace config {
	/*
	/ window configs
	*/
	const int windowWidth = 800;
	const int windowHeight = 600;
	// end window configs

	/*
	/ elevator configs
	*/
	const WCHAR* elevatorImagePath = L"C:/Users/matik/Documents/Elevator-Simulator/img/elevator.png"; //path to image(TODO change to no absolute path)
	const int elevatorStandardSpeed = 1; //elevator moving speed
	const int elevatorMargines = 12;     //from some reason elevator go to bottom, this fix this
	const int elevatorImageHeight = 100; //wymiary obrazka
	const int elevatorImageWidth = 120;  //wymiary obrazka
	// end elevator configs

}