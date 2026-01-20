#ifndef __FORCEFIELD
#define __FORCEFIELD
#include "particles.h"
#include <math.h>
#include <raylib.h>

typedef struct {
  float xCoord;
  float yCoord;
  float radius;
} ForceField;

void DrawForceField(ForceField *forcefield) {
  DrawCircle(forcefield->xCoord, forcefield->yCoord, forcefield->radius,
             YELLOW);
}

void ForceFieldCollision(ForceField *forcefield, Particle particles[],
                         int size) {
  for (int i = 0; i < size; i++) {
    Vector2 partcleCoords = {particles[i].xCoord, particles[i].yCoord};
    Vector2 forcefieldCoords = {forcefield->xCoord, forcefield->yCoord};
    bool areColliding =
        CheckCollisionCircles(partcleCoords, particles[i].radius,
                              forcefieldCoords, forcefield->radius);
    if (areColliding) {
      float distanceX = (partcleCoords.x - forcefield->xCoord);
      float distanceY = (partcleCoords.y - forcefield->yCoord);

      float distanceAbs = sqrtf(pow(distanceX, 2) + pow(distanceY, 2));
      float normalizedXVector = distanceX / distanceAbs;
      float normalizedYVector = distanceY / distanceAbs;

      float overlap = forcefield->radius + particles[i].radius - distanceAbs;

      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        forcefield->radius *= 2;
        particles[i].xVelocity += normalizedXVector * 20;
        particles[i].yVelocity += normalizedYVector * 30;
        forcefield->radius /= 2;
      } else {
        particles[i].xVelocity += normalizedXVector * overlap / 2;
        particles[i].yVelocity += normalizedYVector * overlap / 2;
      }
    }
  }
}
#endif /* ifndef __FORCEFIELD */
