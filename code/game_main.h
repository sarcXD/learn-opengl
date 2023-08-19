#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#define KB(b) (((u64)1024)*(b))
#define MB(b) (((u64)1024)*(KB(b)))
#define GB(b) (((u64)1024)*(MB(b)))

typedef struct GameInput {
  r32 LastMouseX;
  r32 LastMouseY;
  r64 MouseX;
  r64 MouseY;
  r32 Sensitivity;
} GameInput;

typedef struct GameCamera {
  r32 MoveSpeed;
  r32 PitchAngle;
  r32 YawAngle;
  Vec3 CameraPos;
  Vec3 CameraFront;
  Vec3 CameraUp;
} GameCamera;

typedef struct GameMemory {
  void* BaseAddress;
  u64 Size;
} GameMemory;

typedef struct GameState {
  GameCamera Camera;
  GameInput Input;
  GameMemory Memory;
} GameState;

#endif
