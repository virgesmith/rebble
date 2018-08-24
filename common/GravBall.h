
#pragma once

#include <pebble.h>

typedef enum { FILLED, OUTLINE } BallStyle;

typedef void (*CollisionHandler)(int ball1, int ball2);

typedef struct BallInfo
{
	int radius;
	bool grav;
	GColor colour;
	BallStyle style;
} BallInfo;

void GravBall_construct(GRect bounds, int nBalls, BallInfo* balls, float* pX, float* pY, CollisionHandler onCollision);

void GravBall_destroy();

void GravBall_update(Layer* frame, GContext* ctx);
