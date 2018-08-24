
#include "utils.h"

int utils_isgn(int i)
{
  return i > 0 ? 1 : i < 0 ? -1 : 0;
}


void utils_swap(float* i, float *j)
{
  float tmp = *i;
  *i = *j;
  *j = tmp;
}

// Newtons method
int utils_isqrt(int i)
{
  const int MAXITERS = 16;
  int iters = 0;
  int n = i/2;
  int n2 = n * n;
  int oldn2;
  
  if (i < 0)
  {
    return -1; //error
  }
  else if (i < 2)
  { 
    return i; // 0 or 1
  }
  else
  {
    do
    {
      ++iters;
      oldn2 = n2;
      n = (n + i / n) / 2;
      n2 = n * n;
    } while (n2 != oldn2 && iters < MAXITERS); // can oscillate between 2 values, e.g. 13688 -> (116,117)
  }
  if ((n2 - i) < i - (n + 1) * (n + 1))
  {
    return n + 1;
  }
  else
  {
    return n;
  }
}

int utils_mod(const Vector3f* v)
{
  return utils_isqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}


int utils_max(int x, int y)
{
	return x > y ? x : y;
}

int utils_min(int x, int y)
{
	return x < y ? x : y;
}


void utils_decompose(float value, int places, int* sign, int* integer, int* decimal)
{
	int divisor = 1;
	for (int i = 0; i < places; ++i)
	{
		divisor *= 10;
	}

	*sign = 1;
	if (value < 0)
	{
		*sign = -1;
		value = -value;	
	}
	*decimal = (int)(value * divisor) % divisor;
	*integer = ((value * divisor) - *decimal) / divisor;
}