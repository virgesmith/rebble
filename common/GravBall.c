
#include "GravBall.h"
#include "Accelerometer.h"
#include <pebble.h>

static float* p_pX; //[2] = {22.f, 122.f}; 
static float* p_pY; //[2] = {84.f, 84.f}; 
//static float pH[2] = {72.f, 84.f}; 
static float* p_vX; //[2] = {0.f, 0.f};
static float* p_vY; //[2] = {0.f, 0.f};
static float* p_aX; //[2] = {0.f, 0.f};
static float* p_aY; //[2] = {0.f, 0.f};

static int p_nBalls;
static BallInfo* p_balls;
static float p_minX;
static float p_maxX;
static float p_minY;
static float p_maxY;
static float p_scaling;
static float p_dt;
static CollisionHandler p_onCollision;

void GravBall_construct(GRect bounds, int nBalls, BallInfo* balls, float* pX, float* pY, CollisionHandler onCollision)
{
	p_minX = bounds.origin.x;
	p_minY = bounds.origin.y;
	p_maxX = bounds.origin.x + bounds.size.w;
	p_maxY = bounds.origin.y + bounds.size.h;

	p_nBalls = nBalls;
	p_balls = balls;
	p_pX = pX; //(float*)malloc(p_nBalls * sizeof(float));
	p_pY = pY; //(float*)malloc(p_nBalls * sizeof(float));
	p_vX = (float*)malloc(p_nBalls * sizeof(float));
	p_vY = (float*)malloc(p_nBalls * sizeof(float));
	p_aX = (float*)malloc(p_nBalls * sizeof(float));
	p_aY = (float*)malloc(p_nBalls * sizeof(float));

	p_onCollision = onCollision;

	for (int i = 0; i < p_nBalls; ++i)
	{
		//p_colours[i] = colours[i];
		//p_pX[i] = pX[i];
		//p_pY[i] = pY[i];
		p_vX[i] = 0.0f;
		p_vY[i] = 0.0f;
		p_aX[i] = 0.0f;
		p_aY[i] = 0.0f;
	}

  // milligs to m/s/s, scaled up since 1 pixel << 1m
  p_scaling = 0.00980665 * 50.0;
  p_dt = Accelerometer_deltaT() * p_scaling;
}

void GravBall_destroy()
{
	//free(p_colours);
	//free(p_pX);
	//free(p_pY);
	free(p_vX);
	free(p_vY);
	free(p_aX);
	free(p_aY);
}

void GravBall_update(Layer* frame, GContext* ctx)
{
  const float bounciness = 0.85f;
  // need all intermediate calcs in FP
  

  // detect collision between balls
	for (int i = 0; i < p_nBalls; ++i)
	{
		for (int j = i + 1; j < p_nBalls; ++j)
		{ 
			int rX = p_pX[i] - p_pX[j]; 
			int rY = p_pY[i] - p_pY[j];  
   
			int r2 = rX * rX + rY * rY + 1; 
			if (r2 <= 4 * p_balls[i].radius * p_balls[j].radius + 1)
			{
				float c = ((p_vX[i] - p_vX[j]) * rX + (p_vY[i] - p_vY[j]) * rY) / r2;
				p_vX[i] -= c * rX;
				p_vY[i] -= c * rY;
				p_vX[j] += c * rX;
				p_vY[j] += c * rY;
		
				if (p_onCollision)
				{
					p_onCollision(i, j);
				}
			} 
		}
	}
	
	// adjust for gravity
  const Vector3i* acc = Accelerometer_accel();
  for (int i = 0; i < p_nBalls; ++i)
  {  
		p_aX[i] = acc->x * p_balls[i].grav;
		p_aY[i] = acc->y * p_balls[i].grav;
	}
	
	// update position and velocity
  for (int i = 0; i < p_nBalls; ++i)
  {  
    p_vX[i] += p_aX[i] * p_dt;
    p_vY[i] += p_aY[i] * p_dt; 
    p_pX[i] += p_vX[i] * p_dt + 0.5 * p_aX[i] * p_dt * p_dt;
    p_pY[i] += p_vY[i] * p_dt + 0.5 * p_aY[i] * p_dt * p_dt;

    // check collision with boundary
    if (p_pX[i] < p_minX + p_balls[i].radius)
    {
      p_vX[i] *= -bounciness;
      p_pX[i] = p_minX + p_balls[i].radius;
    } 
    if (p_pX[i] > p_maxX - p_balls[i].radius)
    {
      p_vX[i] *= -bounciness;
      p_pX[i] = p_maxX - p_balls[i].radius; 
    }
    if (p_pY[i] < p_minY + p_balls[i].radius)
    {
      p_vY[i] *= -bounciness;
      p_pY[i] = p_minY + p_balls[i].radius;  
    }
    if (p_pY[i] > p_maxY - p_balls[i].radius)
    {
      p_vY[i] *= -bounciness;
      p_pY[i] = p_maxY - p_balls[i].radius;
    }
	}
	
  for (int i = 0; i < p_nBalls; ++i)
  {  
		GPoint position = {(int)p_pX[i], (int)p_pY[i]};

		if (p_balls[i].style == FILLED)
		{
			graphics_context_set_fill_color(ctx, p_balls[i].colour);
			graphics_fill_circle(ctx, position, p_balls[i].radius);
		} 
		else if (p_balls[i].style == OUTLINE)
		{		
			graphics_context_set_stroke_color(ctx, p_balls[i].colour);
			graphics_context_set_stroke_width(ctx, 4);
			graphics_draw_circle(ctx, position, p_balls[i].radius - 2);
		}
	}
}

