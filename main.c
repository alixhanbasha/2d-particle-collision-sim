#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080

#define NUM_PARTICLES 3000

#include "forcefield.c"
#include "gravity.c"
#include "particles.h"
#include "report.h"

Particle _particles[NUM_PARTICLES];

void InitializeParticles(Particle particles[], int size) {
  SetRandomSeed(time(NULL));
  for (int i = 0; i < size - 1; i++) {
    particles[i].radius = GetRandomValue(3, 5);
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

  ForceField forcefield = {100, 100, 50};

  bool shouldPause = false;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_P))
      shouldPause = !shouldPause;
    if (IsKeyPressed(KEY_G)) {
      ToggleGravity();
    }

    BeginDrawing();
    ClearBackground(BLACK);

    Vector2 mouse = GetMousePosition();
    forcefield.xCoord = mouse.x;
    forcefield.yCoord = mouse.y;

    if (!shouldPause) {
      UpdateAllParticles(_particles, NUM_PARTICLES);
      ParticleCollisions(_particles, NUM_PARTICLES);
      DrawForceField(&forcefield);
      ForceFieldCollision(&forcefield, _particles, NUM_PARTICLES);
    }
    DrawAllParticles(_particles, NUM_PARTICLES);

    char buffer[100];
    Report *report = GetReport();
    sprintf(buffer, "Whites: %d, Reds: %d, Blues: %d, Greens: %d",
            report->whites, report->reds, report->blues, report->greens);

    DrawFPS(5, 5);
    DrawText(buffer, 5, GetScreenHeight() - 25, 20, LIGHTGRAY);

    char *gravity = "Gravity: OFF";
    if (GetGravity()->isEnabled)
      gravity = "Gravity: ON";
    DrawText(gravity, GetScreenWidth() - 250, 5, 25, RED);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
