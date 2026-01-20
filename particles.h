#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#ifndef __PARTICLES

#include "raylib.h"
#define __PARTICLES

#ifndef __REPORTS
#include "report.h"
#endif

#define GRAVITY 0.01
#define GRAVITY_DAMP_EFFECT 0.95

Report report = {.whites = 0, .blues = 0, .reds = 0, .greens = 0};

Report *GetReport() { return &report; }
int IsColorEqual(Color one, Color two) {
  return one.r == two.r && one.g == two.g && one.b == two.b && one.a == two.a;
}
/* representation of a single particle */
typedef struct {
  float xCoord;
  float yCoord;
  float radius;
  float xVelocity;
  float yVelocity;
  Color color;
} Particle;

void UpdateParticle(Particle *particle) {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  particle->xCoord += particle->xVelocity;
  particle->yCoord += particle->yVelocity;
  particle->yVelocity += GRAVITY;

  bool hasCollided = false;

  // right side
  if (particle->xCoord >= screen_width - particle->radius) {
    particle->xCoord = screen_width - particle->radius;
    particle->xVelocity = -particle->xVelocity;

    if (!IsColorEqual(particle->color, WHITE)) {
      if (IsColorEqual(particle->color, RED) && !(report.reds <= 0))
        report.reds--;
      else if (IsColorEqual(particle->color, GREEN))
        report.greens--;
      else if (IsColorEqual(particle->color, BLUE))
        report.blues--;
      report.whites++;

      particle->color = WHITE;
    }

    hasCollided = true;
  }
  // bottom
  if (particle->yCoord >= screen_height - particle->radius) {
    particle->yCoord = screen_height - particle->radius;
    particle->yVelocity = -particle->yVelocity;

    if (!IsColorEqual(particle->color, BLUE)) {
      if (IsColorEqual(particle->color, RED) && !(report.reds <= 0))
        report.reds--;
      else if (IsColorEqual(particle->color, GREEN))
        report.greens--;
      else if (IsColorEqual(particle->color, WHITE))
        report.whites--;
      report.blues++;

      particle->color = BLUE;
    }
    hasCollided = true;
  }
  // left
  if (particle->xCoord <= particle->radius) {
    particle->xCoord = particle->radius;
    particle->xVelocity = -particle->xVelocity;

    if (!IsColorEqual(particle->color, GREEN)) {
      if (IsColorEqual(particle->color, RED) && !(report.reds <= 0))
        report.reds--;
      else if (IsColorEqual(particle->color, WHITE))
        report.whites--;
      else if (IsColorEqual(particle->color, BLUE))
        report.blues--;

      report.greens++;
      particle->color = GREEN;
    }
    hasCollided = true;
  }
  // top
  if (particle->yCoord <= particle->radius) {
    particle->yCoord = particle->radius;
    particle->yVelocity = -particle->yVelocity;

    if (!IsColorEqual(particle->color, RED)) {
      if (IsColorEqual(particle->color, WHITE))
        report.whites--;
      else if (IsColorEqual(particle->color, GREEN))
        report.greens--;
      else if (IsColorEqual(particle->color, BLUE))
        report.blues--;
      report.reds++;

      particle->color = RED;
    }
    hasCollided = true;
  }
  // reduce velocity
  if (hasCollided) {
    particle->xVelocity *= GRAVITY_DAMP_EFFECT + 0.1;
    particle->yVelocity *= GRAVITY_DAMP_EFFECT + 0.1;
  }
}

void UpdateAllParticles(Particle particles[], int array_size) {
  for (int i = 0; i < array_size; i++) {
    UpdateParticle(&particles[i]);
  }
}

void ParticleCollisions(Particle particles[], int size) {
  Particle p1;
  Particle p2;
  for (int i = 0; i < size; i++) {
    for (int j = i + 1; j < size; j++) {
      // if the particle is "colliding" with itself, skip
      if (i == j)
        continue;

      p1 = particles[i];
      p2 = particles[j];

      Vector2 v1 = {p1.xCoord, p1.yCoord};
      Vector2 v2 = {p2.xCoord, p2.yCoord};
      bool areColliding = CheckCollisionCircles(v1, p1.radius, v2, p2.radius);

      if (areColliding) {
        // do not allow particles to go through each other
        float distanceX = (p1.xCoord - p2.xCoord);
        float distanceY = (p1.yCoord - p2.yCoord);

        float distanceAbs = sqrtf(pow(distanceX, 2) + pow(distanceY, 2));
        float normalizedXVector = distanceX / distanceAbs;
        float normalizedYVector = distanceY / distanceAbs;

        float overlap = p2.radius + p1.radius - distanceAbs;
        particles[i].xCoord += normalizedXVector * overlap / 2;
        particles[i].yCoord += normalizedYVector * overlap / 2;
        particles[j].xCoord += -normalizedXVector * overlap / 2;
        particles[j].yCoord += -normalizedYVector * overlap / 2;
        // -----------------------------------------------------
        // change direcctions
        float tanX = -normalizedYVector;
        float tanY = normalizedXVector;

        float v1tan = p1.xVelocity * tanX + p1.yVelocity * tanY;
        float v2tan = p2.xVelocity * tanX + p2.yVelocity * tanY;
        float v1norm =
            p1.xVelocity * normalizedXVector + p1.yVelocity * normalizedYVector;
        float v2norm =
            p2.xVelocity * normalizedXVector + p2.yVelocity * normalizedYVector;

        float temp = v2norm;
        v2norm = v1norm;
        v1norm = temp;

        particles[i].xVelocity = v1tan;
        particles[i].yVelocity = v1norm;

        particles[j].xVelocity = v2tan;
        particles[j].yVelocity = v2norm;
        // -----------------------------------------------------
        if (areColliding) {
          // apply gravity, and slow things down
          particles[i].xVelocity *= GRAVITY_DAMP_EFFECT;
          particles[i].yVelocity *= GRAVITY_DAMP_EFFECT;
          particles[j].xVelocity *= GRAVITY_DAMP_EFFECT;
          particles[j].yVelocity *= GRAVITY_DAMP_EFFECT;
        }
      }
    }
  }
}

void DrawParticle(Particle *particle) {
  DrawCircle(particle->xCoord, particle->yCoord, particle->radius,
             particle->color);
}

void DrawAllParticles(Particle particles[], int size) {
  for (int i = 0; i < size; i++) {
    DrawParticle(&particles[i]);
  }
}

#endif // !__PARTICLES
