#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void getCircle(int originX, int originY, int radius, int x[], int y[])
{
  const double pi = acos(-1.0);

  for (int i = 0; i < 90; i++)
  {
    // 加0.5做四舍五入
    int cosDelta = (int)(radius * cos(i * pi / 180) + 0.5);
    int sinDelta = (int)(radius * sin(i * pi / 180) + 0.5);

    x[i] = originX - cosDelta;
    y[i] = originY + sinDelta;
    x[i + 90] = originX + sinDelta;
    y[i + 90] = originY + cosDelta;
    x[i + 180] = originX + cosDelta;
    y[i + 180] = originY - sinDelta;
    x[i + 270] = originX - sinDelta;
    y[i + 270] = originY - cosDelta;
  }
}

int main(int argc, char* argv[])
{
  if(argc != 4)
  {
    printf("usage: circle x y radius");
    return -1;
  }

  int originX = atoi(argv[1]);
  int originY = atoi(argv[2]);
  int radius = atoi(argv[3]);

  int x[360], y[360];
  getCircle(originX, originY, radius, x, y);

  for (int i = 0; i < 360; i++)
  {
    if (i >= 10 && i % 10 == 0)
    {
      printf("\n");
    }
    printf("(%d %d) ", x[i], y[i]);
  }

  return 0;
}
