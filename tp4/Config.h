#pragma once


namespace config {
	const int elevatorImageHeight = 100;
	const int elevatorImageWidth = 120;

	const int windowWidth = 800;
	const int windowHeight = 600;

	const int simulationSpeed = 2;

	namespace people{
		const bool randomWeight = false;
		const int staticPeopleWeight = 70;
		const int minWeight = 50;
		const int maxWeight = 120;
	}
	namespace floor {
		const int numberFloors = 5;
	}
	namespace elevator {
		const int maxPeopleWeight = 600;
		const int maxPeopleNum = 10;
	}

}