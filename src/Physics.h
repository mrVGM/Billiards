#pragma once

#include <vector>
#include "Utils.h"
#include "Primitives.h"
#include "EventInstances.h"

class Physics
{
public:
	long time;
	std::vector<Ball> balls;
	static Utils::Segment s1, s2, s3, s4;

	void updateState();
	static Physics & getEngine();
	static float acceleration;

	void handle(EventInstances::BallStopped * e);
	void handle(EventInstances::BoardBounce * e);
	void handle(EventInstances::EndFrame * e);
};

