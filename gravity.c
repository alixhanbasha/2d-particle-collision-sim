
#ifndef __GRAVITY
#define __GRAVITY
#include <stdbool.h>

typedef struct {
  float gforce;
  float damp;
  bool isEnabled;
} Gravity;

Gravity _GRAVITY = {0.1, 0.90, true};

void ToggleGravity() {
  _GRAVITY.isEnabled = !_GRAVITY.isEnabled;
  if (_GRAVITY.isEnabled) {
    _GRAVITY.gforce = 0.01;
    return;
  }
  _GRAVITY.gforce = 0;
}
Gravity *GetGravity() { return &_GRAVITY; }

#endif
