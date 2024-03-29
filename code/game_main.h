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

typedef struct BufferO {
  u32 VAO; // Vertex Array Object
  u32 VBO; // Vertex Buffer Object
  u32 EBO; // Element Buffer Object
  u32* TexO; // Texture Buffer Object
} BufferO;

typedef struct Texture2D {
  i32 width;
  i32 height;
  i32 nrChannels;
  unsigned char* data;
} Texture2D;

typedef struct GameMemory {
  void *PermanentStorage;
  u64 PermanentStorageSize;
  DebugArena Arena;
} GameMemory;

typedef struct GameState {
  GameCamera Camera;
  GameInput Input;
  GameMemory Memory;
} GameState;


#endif
