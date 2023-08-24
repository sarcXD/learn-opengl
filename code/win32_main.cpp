#include <windows.h>
#include <direct.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h> 
#include <stdio.h>
#include <stdint.h>

typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;

typedef  int8_t   i8;
typedef  int16_t  i16;
typedef  int32_t  i32;
typedef  int64_t  i64;

typedef  u8       b8;
typedef  u16      b16;
typedef  u32      b32;

typedef  float    r32;
typedef  double   r64;

#include "math.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 1024
#define internal static

/*
 * @todo:
 * 1. fix the frametime checking. Using glfwWallClock and QueryPerformanceFrequency and QueryPerformanceCounter
 *    - need to learn how to use QueryPerformanceFrequency and QueryPerformanceCounter
 * 2. Add cwd paths for the exe, so no matter where the executable is called from, it can read and open files correctly
 */

internal i64 GlobalPerfCountFrequency;

i8 PlatformDebugReadFile(char *FileName, void *FileBuffer, u32 MaxSize, LPDWORD BytesRead)
{
  HANDLE FileHandle = CreateFileA(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (FileHandle != INVALID_HANDLE_VALUE)
  {
    if (ReadFile(FileHandle, FileBuffer, MaxSize, BytesRead, NULL) != 0)
    {
      return 1;
    }
    else
    {
      // @todo: logging
      return -2;
    }
  }
  else
  {
    // @todo: logging
    return -1;
  }
}

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

void HandleInputs(GLFWwindow *Window)
{
}

inline LARGE_INTEGER
Win32GetWallClock(void)
{
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    
    return Result;
}

inline r32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    r32 Result = (r32)(End.QuadPart - Start.QuadPart)/(r32)GlobalPerfCountFrequency;
    return(Result);
}

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

typedef struct DebugArena {
  u8 *Buffer;
  size_t Size;
  size_t CurrOffset;
  size_t PrevOffset;
} DebugArena;

bool IsPowerOfTwo(uintptr_t x)
{
  return (x & (x-1)) == 0;
}

uintptr_t AlignForward(uintptr_t ptr, size_t align)
{
  uintptr_t p, a, modulo;

  assert(IsPowerOfTwo(align));

  p = ptr;
  a = (uintptr_t)align;
  modulo = p & (a-1);

  if (modulo != 0)
  {
    p += a - modulo;
  }
  return p;
}

void *ArenaAllocAlign(DebugArena *Alloc, size_t Size, size_t Align)
{
  uintptr_t CurrOffset = (uintptr_t)Alloc->Buffer + (uintptr_t)Alloc->CurrOffset;
  uintptr_t AlignedOffset = AlignForward(CurrOffset, Align);
  AlignedOffset -= (uintptr_t)(Alloc->Buffer);

  if (AlignedOffset + Size < Alloc->Size)
  {
    void *Ptr = &Alloc->Buffer[AlignedOffset];
    Alloc->PrevOffset = AlignedOffset;
    Alloc->CurrOffset = AlignedOffset + Size;

    ZeroMemory(Ptr, Size);
    return Ptr;
  }
  return NULL;
}

void ArenaInit(DebugArena *Alloc, void* BackingBuffer, size_t Size)
{
  Alloc->Buffer = (u8 *)BackingBuffer;
  Alloc->Size = Size;
  Alloc->CurrOffset = 0;
  Alloc->PrevOffset = 0;
}

void *ArenaAlloc(DebugArena *Alloc, size_t Size)
{
  return ArenaAllocAlign(Alloc, Size, DEFAULT_ALIGNMENT);
}

void ArenaFree(DebugArena *Alloc, void *Ptr)
{
  // do nothing
}

void *ArenaResizeAlign(DebugArena *Alloc, void *OldMem, size_t OldSize,
                        size_t NewSize, size_t Align)
{
  assert(IsPowerOfTwo(Align));
  if (OldMem == NULL || OldSize == 0)
  {
    return ArenaAllocAlign(Alloc, NewSize, Align);
  }
  else if (Alloc->Buffer < OldMem && OldMem < Alloc->Buffer + Alloc->Size)
  {
    // check if old_memory falls on prev_offset
    if (Alloc->Buffer + Alloc->PrevOffset == OldMem)
    {
      // re-use prev_offset and resize from there
      size_t _CurrOffset = Alloc->CurrOffset;
      Alloc->CurrOffset = Alloc->PrevOffset + NewSize;
      if (NewSize > OldSize)
      {
        ZeroMemory(&Alloc->Buffer[_CurrOffset], NewSize - OldSize); 
      }
      return OldMem;
    }
    else
    {
      // generate new memory
      // will have some fragmentation
      void *NewMem = ArenaAllocAlign(Alloc, NewSize, Align);
      size_t CopySize = OldSize < NewSize ? OldSize : NewSize;

      // copy old memory to new memory location
      CopyMemory(NewMem, OldMem, CopySize);
      return NewMem;
    }
  }
  else
  {
    assert(0 && "Memory is out of bounds of the buffer in this arena");
    return NULL;
  }
}

void *ArenaResize(DebugArena *Alloc, void *OldMem, size_t OldSize, size_t NewSize)
{
  return ArenaResizeAlign(Alloc, OldMem, OldSize, NewSize, DEFAULT_ALIGNMENT);
}

void ArenaFreeAll(DebugArena *Alloc)
{
  Alloc->CurrOffset = 0;
  Alloc->PrevOffset = 0;
}

#include "game_main.h"
#include "gl_graphics.cpp"

void DrawModel(Mat4 Model, u32 ShaderProgram, u32 VAO)
{
  const r32 model[16] = {Model.x0, Model.x1, Model.x2, Model.x3,
      Model.y0, Model.y1, Model.y2, Model.y3,
      Model.z0, Model.z1, Model.z2, Model.z3,
      Model.w0, Model.w1, Model.w2, Model.w3};
  
  glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Model"), 1, GL_TRUE, model);
  DrawCube(VAO);
}


int main()
{
    
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;
    
    // Note(talha): Set Windows scheduler granularity to 1ms
    // so that our sleep can be more granular
    UINT DesiredSchedulerMS = 1;
    b8 SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);
    
    // TODO(talha): how to use a library to query this
    i32 MonitorRefreshHz = 60;
    i32 GameUpdateHz = MonitorRefreshHz;
    r32 TargetSecondsPerFrame = 1.0f / (r32)GameUpdateHz;

    GameState State = {0};
    State.Input.LastMouseX = WIN_WIDTH/2;
    State.Input.LastMouseY = WIN_HEIGHT/2;
    
    // initialise glfw, set opengl versions
    // create glfw window
    // load opengl functions with glad
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *Window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (Window == NULL)
    {
        // todo(talha): add error logging for failed to create glfw window
        glfwTerminate();
        printf("ERROR: Failed to create a glfw window\n");
        return -1;
    }
    
    glfwMakeContextCurrent(Window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // todo(talha): error logging - failed to initialize glad
        printf("ERROR: Failed to initialise glad\n");
        return -1;
    }

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(Window, State.Input.LastMouseX, State.Input.LastMouseY);
    
    size_t PermanentStorageSize = GB((u64)2);
    void *PermanentStorage = VirtualAlloc(NULL, PermanentStorageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (PermanentStorage == NULL)
    {
      printf("ERROR: Failed to allocate game memory\n");
      return -1;
    }
    State.Memory.PermanentStorage = PermanentStorage;
    State.Memory.PermanentStorageSize = PermanentStorageSize;

    ArenaInit(&State.Memory.Arena, State.Memory.PermanentStorage, State.Memory.PermanentStorageSize);

   float vertices[] = {
          // coords           // normals            // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f
    }; 

    u32 v_sz = sizeof(vertices);
    BufferO BO_Container = CreateCubeTextured(vertices, v_sz);
    u32 LightVAO;
    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, BO_Container.VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)0);
    glEnableVertexAttribArray(0);

    //stbi_set_flip_vertically_on_load(1);

    // @todo: set exact file sizes later on
    u64 MaxFileSize = MB(1);
    const char *ObjectVSProg = (const char *)ArenaAlloc(&State.Memory.Arena, MaxFileSize);
    DWORD BytesRead = 0;
    i8 OpRes = PlatformDebugReadFile("./code/shaders/lighting/object.vs", (void *)ObjectVSProg, MaxFileSize, &BytesRead);
    
    /*
     * understanding the positions:
     * -.5, .5 - top left
     *   0,-.5 - bottom center
     *  .5, .5 - top right
     */
    u32 ObjectVS = CreateVertexShader(ObjectVSProg);

    const char *ObjectFSProg = (const char *)ArenaAlloc(&State.Memory.Arena, MaxFileSize);
    OpRes = PlatformDebugReadFile("./code/shaders/lighting/object.fs", (void *)ObjectFSProg, MaxFileSize, &BytesRead);

    u32 ObjectFS = CreateFragmentShader(ObjectFSProg);
    u32 ObjectSP = CreateShaderProgram(ObjectVS, ObjectFS);

    const char *LightVSProg = (const char *)ArenaAlloc(&State.Memory.Arena, MaxFileSize);
    OpRes = PlatformDebugReadFile("./code/shaders/lighting/light_source.vs", (void *)LightVSProg, MaxFileSize, &BytesRead);
    
    u32 LightVS = CreateVertexShader(LightVSProg);

    const char *LightFSProg = (const char *)ArenaAlloc(&State.Memory.Arena, MaxFileSize);
    OpRes = PlatformDebugReadFile("./code/shaders/lighting/light_source.fs", (void *)LightFSProg, MaxFileSize, &BytesRead); 

    u32 LightFS = CreateFragmentShader(LightFSProg);

    u32 LightSP = CreateShaderProgram(LightVS, LightFS);

    ObjectVSProg = NULL;
    ObjectFSProg = NULL;
    LightVSProg = NULL;
    LightFSProg = NULL;
    ArenaFreeAll(&State.Memory.Arena); // file memory can be freed
    
    u32 tex_obj;
    BO_Container.TexO = &tex_obj;
    Texture2D Tex = {0};
    Tex.data = stbi_load("./assets/wood-container.png", &Tex.width, &Tex.height, &Tex.nrChannels, 0);
    if (Tex.data != NULL)
    {
      DefineGlTextures(Tex, BO_Container.VAO, &tex_obj, 0);
      stbi_image_free(Tex.data);
    }
    else
    {
      printf("ERROR: Failed to open image");
      return -1;
    }

    Tex.data = stbi_load("./assets/wood-container-specular-map.png", &Tex.width, &Tex.height, &Tex.nrChannels, 0);
    if (Tex.data != NULL)
    {
      DefineGlTextures(Tex, BO_Container.VAO, &tex_obj, 1);
      stbi_image_free(Tex.data);
    }
    else
    {
      printf("Error: Failed to load specular map");
      return -1;
    }

    // shader program using color attributes
    Vec3 LightColor = {1.0f, 1.0f, 1.0f};
    Vec3 LightPos = {1.8f, 1.0f, 0.0f};

    LARGE_INTEGER LastCounter = Win32GetWallClock();
    u64 LastCycleCount = __rdtsc();
    
    // matrix stuff
    State.Camera.CameraPos = {0};
    State.Camera.CameraPos.x = 0.0f;
    State.Camera.CameraPos.z = 5.0f;
    State.Camera.CameraFront = {0};
    // pointing inside to the screen
    State.Camera.CameraFront.z = -1.0f;

    State.Camera.CameraUp = {0};
    State.Camera.CameraUp.y = 1.0f;

    State.Camera.MoveSpeed = 0.1f;

    State.Input.Sensitivity = 0.1f;
    State.Camera.PitchAngle = 0;
    State.Camera.YawAngle = -90.0f;

    glUseProgram(ObjectSP);
    LoadUniformInt(ObjectSP, "material.Diffuse", 0);
    LoadUniformInt(ObjectSP, "material.Specular", 1);
    LoadUniformFloat(ObjectSP, "material.Shininess", 32.0f);
    LoadUniformVec3(ObjectSP, "light.Position", LightPos);
    LoadUniformVec3(ObjectSP, "light.Ambient", InitVec3(1.0f));
    LoadUniformVec3(ObjectSP, "light.Diffuse", InitVec3(1.0f));
    LoadUniformVec3(ObjectSP, "light.Specular", InitVec3(1.0f, 1.0f, 1.0f));

    glUseProgram(LightSP);
    LoadUniformVec3(LightSP, "LightPos", LightPos);
    LoadUniformVec3(LightSP, "LightColor", InitVec3(1.0f));
    
    while (!glfwWindowShouldClose(Window)) {
        LARGE_INTEGER WorkCounter = Win32GetWallClock();
        r32 WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);
        
        r32 SecondsElapsedForFrame = WorkSecondsElapsed;
        // @note: this caps the framerate
        // also prevents, for now, incorrect speeds based on timings
        // @todo: fix framerate capping and speed timings being affected by framerate
        if(SecondsElapsedForFrame < TargetSecondsPerFrame){
            while(SecondsElapsedForFrame < TargetSecondsPerFrame)
            {
                if (SleepIsGranular)
                {
                    DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
                    if (SleepMS > 0) 
                    {
                        Sleep(SleepMS);
                    }
                }
                SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, 
                                                                Win32GetWallClock());
            }
        }
        else
        {
            // TODO(talha): Missed frame rate
            // TODO(talha): Logging
        }

        glfwPollEvents();
        // HandleInputs
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(Window, 1);
        }
        if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        {
          State.Camera.CameraPos = State.Camera.CameraPos + (State.Camera.CameraFront * State.Camera.MoveSpeed);
        }
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        {
          State.Camera.CameraPos = State.Camera.CameraPos + ((State.Camera.CameraFront * State.Camera.MoveSpeed) * -1.0f);
        }
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        {
          Vec3 HorizontalVec = (UnitVec3(CrossProductVec3(State.Camera.CameraFront, State.Camera.CameraUp)) * State.Camera.MoveSpeed);
          State.Camera.CameraPos = (State.Camera.CameraPos + (HorizontalVec * -1.0f));
        }
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        {
          Vec3 HorizontalVec = (UnitVec3(CrossProductVec3(State.Camera.CameraFront, State.Camera.CameraUp)) * State.Camera.MoveSpeed);
          State.Camera.CameraPos = State.Camera.CameraPos + HorizontalVec;
        }
        glfwGetCursorPos(Window, &State.Input.MouseX, &State.Input.MouseY);
        
        // @todo: how to create multiple cubes??
        //Mat4 Tx1 = CreateTranslationMat(InitVec4(5.0f, 0, -5.0f, 1));
        //Tx1 = Mul_Mat4Mat4(Tx1, Ry);
        //Mat4 Model1 = IdentityMat();
        //Model1 = Mul_Mat4Mat4(Tx1, Model);
        //DrawModel(Model1, ObjectSP, BO_Container);
        
        // view matrix
        // @note: in normalised device coordinates, OPENGL switches the handedness
        // this is why:
        // x -> -ve
        // x <- +ve
        r32 OffsetX = State.Input.MouseX - State.Input.LastMouseX;
        r32 OffsetY = State.Input.MouseY - State.Input.LastMouseY;
        State.Input.LastMouseX = State.Input.MouseX;
        State.Input.LastMouseY = State.Input.MouseY;

        OffsetX *= State.Input.Sensitivity;
        OffsetY *= State.Input.Sensitivity;
        State.Camera.YawAngle += OffsetX;
        State.Camera.PitchAngle += OffsetY;

        if (State.Camera.PitchAngle > 89.0f) State.Camera.PitchAngle = 89.0f;
        if (State.Camera.PitchAngle < -89.0f) State.Camera.PitchAngle = -89.0f;

        Vec3 Dir = {0};
        Dir.y = sin(DegToRad(State.Camera.PitchAngle));
        Dir.x = cos(DegToRad(State.Camera.YawAngle)) * cos(DegToRad(State.Camera.PitchAngle));
        Dir.z = sin(DegToRad(State.Camera.YawAngle)) * cos(DegToRad(State.Camera.PitchAngle));
        State.Camera.CameraFront = UnitVec3(Dir);
        
        /*
         * @note: camera direction is calculated by: CameraPos - CameraTarget
         * this makes it point in the direction of the cameras +ve axis
         * which also allows us to have the z-component by positive whilst by
         * convention in OpenGL the +ve z-component points to the cameras -ve 
         * direction
         * */
        Mat4 LookAt = CreateLookAtMat4(State.Camera.CameraPos, State.Camera.CameraPos + State.Camera.CameraFront, State.Camera.CameraUp);
        Mat4 View = LookAt;
        // projection matrix
        Mat4 Projection = CreatePerspectiveUsingFrustum((r32)PI/4, (r32)WIN_WIDTH/(r32)WIN_HEIGHT, 0.1f, 100.0f); 
        
        const r32 view[16] = {View.x0, View.x1, View.x2, View.x3,
            View.y0, View.y1, View.y2, View.y3,
            View.z0, View.z1, View.z2, View.z3,
            View.w0, View.w1, View.w2, View.w3};
        const r32 projection[16] = {Projection.x0, Projection.x1, Projection.x2, Projection.x3,
            Projection.y0, Projection.y1, Projection.y2, Projection.y3,
            Projection.z0, Projection.z1, Projection.z2, Projection.z3,
            Projection.w0, Projection.w1, Projection.w2, Projection.w3};

        glEnable(GL_DEPTH_TEST);

        glUseProgram(ObjectSP);
        LoadUniformVec3(ObjectSP, "ViewPos", State.Camera.CameraPos);
        LoadUniformMat4(ObjectSP, "View", view);
        LoadUniformMat4(ObjectSP, "Projection", projection);

        // model 2
        Mat4 Tx = CreateTranslationMat(InitVec4(3.0f, 1.0f, -3.0f, 1));
        Mat4 Model = IdentityMat();
        Model = Mul_Mat4Mat4(Tx, Model);
        DrawModel(Model, ObjectSP, BO_Container.VAO);

        glUseProgram(LightSP);
        LoadUniformMat4(LightSP, "View", view);
        LoadUniformMat4(LightSP, "Projection", projection);
        // light model
        Mat4 Model1 = IdentityMat();
        Mat4 ScaledModel = Mul_Mat4Mat4(CreateScaleMat(InitVec4(0.2f, 0.2f, 0.2f, 1.0f)), Model1);
        Mat4 Tx1 = CreateTranslationMat(InitVec4(LightPos.x, LightPos.y, LightPos.z, 1.0f));
        ScaledModel = Mul_Mat4Mat4(Tx1, ScaledModel);
        DrawModel(ScaledModel, LightSP, LightVAO);


        
#if RECT_THAT_GETS_LARGER_AND_SMALLER_OVER_TIME
        // @note: creating the same rect in a different position using transformation
        ScaleFactor = InitVec4(scale_t, scale_t, 1.0f, 1.0f);
        S = CreateScaleMat(ScaleFactor);
        if (scale_dir) {
            if (scale_t < 3) {
                scale_t += scale_amount;
            } else {
                scale_dir = 0;
                scale_t -= scale_amount;
            }
        }
        else {
            if (scale_t > 0.025f) {
                scale_t -= scale_amount;
            } else {
                scale_dir = 1;
                scale_t += scale_amount;
            }
        }
        TransMat = InitVec4(-.5, 0.5, 0, 0);
        T = CreateTranslationMat(TransMat);
        T = Mul_Mat4Mat4(T,S);
        const r32 ScaleArr[16] = {T.x0, T.x1, T.x2, T.x3,
            T.y0, T.y1, T.y2, T.y3,
            T.z0, T.z1, T.z2, T.z3,
            T.w0, T.w1, T.w2, T.w3};
        
        glUniformMatrix4fv(glGetUniformLocation(ObjectSP, "Transform"), 1, GL_TRUE, ScaleArr);
        DrawRectangle(BO_Container.VAO);
#endif        
        
        glfwSwapBuffers(Window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if QUANTM_DEBUG
        r32 MSPerFrame = 1000.0f*SecondsElapsedForFrame;
        char OutBuffer[256];
        _snprintf_s(OutBuffer, sizeof(OutBuffer), "%0.2fms/f\n", MSPerFrame);
        OutputDebugStringA(OutBuffer);
#endif
        
        // TODO(talha): should these be cleared
        LastCounter = Win32GetWallClock();
        LastCycleCount = __rdtsc();
    };

    VirtualFree(PermanentStorage, 0, MEM_RELEASE);
    glfwTerminate();
    return 0;
}
