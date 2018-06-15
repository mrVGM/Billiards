#pragma once

#include "EventManager.h"
#include "Primitives.h"
#include "Utils.h"

#define BEGIN_DECLARE_EVENT(name) class name : public Event { \
public: \
	void handle() override;
#define END_DECLARE_EVENT };


namespace EventInstances
{
	BEGIN_DECLARE_EVENT(BallStopped)
		Ball * ball;
	END_DECLARE_EVENT

	BEGIN_DECLARE_EVENT(EndFrame)
	END_DECLARE_EVENT

	BEGIN_DECLARE_EVENT(BoardBounce)
		Ball * ball;
		const Utils::Segment * segment;
	END_DECLARE_EVENT

	BEGIN_DECLARE_EVENT(BallCollision)
		Ball * ball1;
		Ball * ball2;
	END_DECLARE_EVENT
}

#undef BEGIN_DECLARE_EVENT
#undef END_DECLARE_EVENT

