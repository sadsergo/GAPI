#include "VBHB.h"


int
orient(Vec<4, float> triangle[3])
{
  float ori = (triangle[1][0] * triangle[2][1] - triangle[1][1] * triangle[2][0]) - 
                (triangle[0][0] * triangle[2][1] - triangle[0][1] * triangle[2][0]) + 
                (triangle[0][0] * triangle[1][1] - triangle[0][1] * triangle[1][0]);
  
  return (ori < 0) ? -1 : (ori > 0) ? 1 : 0;
}
