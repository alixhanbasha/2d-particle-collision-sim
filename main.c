#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080

#define NUM_PARTICLES 350

#ifndef __PARTICLES
#include "particles.h"
#endif /* ifndef __PARTICLES */

#ifndef __REPORTS
#include "report.h"
#endif /* ifndef __REPORTS */

Particle _particles[NUM_PARTICLES];

void InitializeParticles(Particle particles[], int size) {
  SetRandomSeed(time(NULL));
  for (int i = 0; i < size; i++) {
    particles[i].radius = GetRandomValue(10, 15);
    particles[i].xCoord =
        GetRandomValue(particles[i].radius, WIDTH - particles[i].radius);
    particles[i].yCoord =
        GetRandomValue(particles[i].radius, HEIGHT - particles[i].radius);
    particles[i].xVelocity = GetRandomValue(-5, 5);
    particles[i].yVelocity = GetRandomValue(-5, 5);
    particles[i].color = LIGHTGRAY;
  }
}

int main(void) {
  SetTargetFPS(60);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WIDTH, HEIGHT, "Raylib in C?");

  InitializeParticles(_particles, NUM_PARTICLES);

  bool shouldPause = false;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_P))
      shouldPause = !shouldPause;

    BeginDrawing();
    ClearBackground(BLACK);

    if (!shouldPause) {
      UpdateAllParticles(_particles, NUM_PARTICLES);
      ParticleCollisions(_particles, NUM_PARTICLES);
    }
    DrawAllParticles(_particles, NUM_PARTICLES);

    char buffer[100];
    Report *report = GetReport();
    sprintf(buffer, "Whites: %d, Reds: %d, Blues: %d, Greens: %d",
            report->whites, report->reds, report->blues, report->greens);

    DrawFPS(5, 5);
    DrawText(buffer, 5, GetScreenHeight() - 25, 20, LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
