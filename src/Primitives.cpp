#include "stdafx.h"

#include "Primitives.h"

const float Ball::diameter = 5.25f;
int Ball::count = 0;

Ball::Ball() : id(count++) {}
