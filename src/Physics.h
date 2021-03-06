#pragma once

#include <vector>
#include "Utils.h"
#include "Primitives.h"
#include "EventInstances.h"

class Physics
{
public:
	double time;
	std::vector<Ball> balls;
	static const Utils::Segment s1, s2, s3, s4;

	double lastCollisionTime;
	int ball1Id;
	int ball2Id;

	bool finishedUpdatingState;

	Physics();

	void updateState();
	static Physics & getEngine();
	static float acceleration;

	void move(float t);

	void handle(EventInstances::BallStopped * e);
	void handle(EventInstances::BoardBounce * e);
	void handle(EventInstances::EndFrame * e);
	void handle(EventInstances::BallCollision * e);
};

