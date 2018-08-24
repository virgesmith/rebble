
#pragma once

typedef struct Vector3f
{
  float x;
  float y;
  float z;
} Vector3f;

typedef struct Vector3i
{
  int x;
  int y;
  int z;
} Vector3i;

int utils_isgn(int i);

void utils_swap(float* i, float *j);

int utils_isqrt(int i);

// ||v||
int utils_mod(const Vector3f* v);

int utils_max(int x, int y);
int utils_min(int x, int y);

void utils_decompose(float change, int places, int* sign, int* integer, int* decimal);  
