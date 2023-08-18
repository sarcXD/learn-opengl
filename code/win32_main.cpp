#include <windows.h>
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

typedef  float    r32;
typedef  double   r64;

#include "math.h"
#include "gl_graphics.cpp"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 1024
#define internal static

/*
 * @todo:
 * 1. fix the frametime checking. Using glfwWallClock and QueryPerformanceFrequency and QueryPerformanceCounter
 *    - need to learn how to use QueryPerformanceFrequency and QueryPerformanceCounter
 */
typedef struct {
  r32 LastMouseX;
  r32 LastMouseY;
  r64 MouseX;
  r64 MouseY;
  r32 Sensitivity;
} GameInput;

typedef struct {
  r32 MoveSpeed;
  r32 PitchAngle;
  r32 YawAngle;
  Vec3 CameraPos;
  Vec3 CameraFront;
  Vec3 CameraUp;
} GameCamera;

typedef struct GameState {
  GameCamera Camera;
  GameInput Input;
} GameState;

internal i64 GlobalPerfCountFrequency;

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

void DrawModel(Mat4 Model, u32 ShaderProgram, BufferO BO_Container)
{
  const r32 model[16] = {Model.x0, Model.x1, Model.x2, Model.x3,
      Model.y0, Model.y1, Model.y2, Model.y3,
      Model.z0, Model.z1, Model.z2, Model.z3,
      Model.w0, Model.w1, Model.w2, Model.w3};
  
  glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Model"), 1, GL_TRUE, model);
  DrawCube(BO_Container.VAO);
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
    
    r32 _vertices[] = {
        // position             colors                  texture coords
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,       0.0f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,       1.0f, 0.0f, //bottom right
        -0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,       0.0f, 1.0f, //top left
        0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,       1.0f, 1.0f //top right
    };

    r32 vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    u32 indices[] = {
        0, 1, 2,
        2, 1, 3,
    };
    
    u32 i_sz = sizeof(indices);
    u32 v_sz = sizeof(vertices);
    
    BufferO BO_Container = CreateCubeTextured(vertices, v_sz);
    BO_Container.TextureO = (u32*)malloc(sizeof(u32)*15); // at max we can have 15 textures on one object
    // load texture using stb image loader
    i32 width, height, nrChannels;
    unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        Texture2D container = {0};
        container.width=width;
        container.height=height;
        container.nrChannels=nrChannels;
        container.data=data;
        
        glBindVertexArray(BO_Container.VAO);
        
        u32 TextureO;
        glGenTextures(1, &TextureO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureO);
        // defining texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // defining texture filtering options with mipmaps
        /* List of texture filtering options with mipmaps"
         * GL_LINEAR_MIPMAP_LINEAR
         * GL_LINEAR_MIPMAP_NEAREST
         * GL_NEAREST_MIPMAP_LINEAR
         * GL_NEAREST_MIPMAP_NEAREST
         *
         * TODO: understand how all the different options work
         */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, container.width, container.height, 0, GL_RGB, GL_UNSIGNED_BYTE, container.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        BO_Container.TextureO[0] = TextureO;
        
        glBindVertexArray(0);
        stbi_image_free(container.data);
    }
    else
    {
        // TODO: logging
    }
    
    stbi_set_flip_vertically_on_load(1);
    i32 width1, height1, nrChannels1;
    unsigned char *data1 = stbi_load("assets/awesomeface.png", &width1, &height1, &nrChannels1, 0);
    if (data1)
    {
        Texture2D container = {0};
        container.width=width1;
        container.height=height1;
        container.nrChannels=nrChannels1;
        container.data=data1;
        glBindVertexArray(BO_Container.VAO);
        
        // texture 0
        u32 TextureO;
        glGenTextures(1, &TextureO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TextureO);
        // defining texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        // defining texture filtering options with mipmaps
        /* List of texture filtering options with mipmaps"
         * GL_LINEAR_MIPMAP_LINEAR
         * GL_LINEAR_MIPMAP_NEAREST
         * GL_NEAREST_MIPMAP_LINEAR
         * GL_NEAREST_MIPMAP_NEAREST
         *
         * TODO: understand how all the different options work
         */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, container.width, container.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, container.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        BO_Container.TextureO[1] = TextureO;
    }
    else
    {
        // TODO: logging
    }
    
    const char *VertexShaderSource = 
        "#version 330 core\n"
        "layout (location=0) in vec3 aPos;\n"
        "layout (location=1) in vec3 aColor;\n"
        "layout (location=2) in vec2 aTexCoord;\n"
        "out vec3 VertexColor;\n"
        "out vec2 TextureCoord;\n"
        "uniform mat4 Model;\n"
        "uniform mat4 View;\n"
        "uniform mat4 Projection;\n"
        "void main() {\n"
        "gl_Position = Projection*View*Model*vec4(aPos, 1.0f);\n"
        "VertexColor = aColor;\n"
        "TextureCoord = aTexCoord;\n"
        "}\0";
    
    /*
     * understanding the positions:
     * -.5, .5 - top left
     *   0,-.5 - bottom center
     *  .5, .5 - top right
     */
    u32 VertexShader = CreateVertexShader(VertexShaderSource);
    
    const char *FragmentShaderSource = 
        "#version 330 core\n"
        "in vec3 VertexColor;\n"
        "in vec2 TextureCoord;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D Texture0;\n"
        "uniform sampler2D Texture1;\n"
        "uniform float TexWeight;\n"
        "void main() {\n"
        "FragColor = mix(texture(Texture0, TextureCoord), texture(Texture1, TextureCoord), TexWeight);\n"
        "}\0";
    
    u32 FragmentShader = CreateFragmentShader(FragmentShaderSource);
    u32 ShaderProgram = CreateShaderProgram(VertexShader, FragmentShader);
    
    
    // shader program using color attributes
    glUseProgram(ShaderProgram);
    glUniform1i(glGetUniformLocation(ShaderProgram, "Texture0"), 0);
    glUniform1i(glGetUniformLocation(ShaderProgram, "Texture1"), 1);
    glEnable(GL_DEPTH_TEST);

    r32 TexWeight = 0.2f;
    
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
    State.Input.Sensitivity = 0.01f;
    State.Camera.PitchAngle = PI;
    State.Camera.YawAngle = -PI/2.0f;

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
          State.Camera.CameraPos = AddVec3(State.Camera.CameraPos, ScalerMul3(State.Camera.CameraFront, State.Camera.MoveSpeed));
        }
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        {
          State.Camera.CameraPos = AddVec3(State.Camera.CameraPos, ScalerMul3(ScalerMul3(State.Camera.CameraFront, State.Camera.MoveSpeed), -1.0f));
        }
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        {
          Vec3 HorizontalVec = ScalerMul3(UnitVec3(CrossProductVec3(State.Camera.CameraFront, State.Camera.CameraUp)), State.Camera.MoveSpeed);
          State.Camera.CameraPos = AddVec3(State.Camera.CameraPos, ScalerMul3(HorizontalVec, -1.0f));
        }
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        {
          Vec3 HorizontalVec = ScalerMul3(UnitVec3(CrossProductVec3(State.Camera.CameraFront, State.Camera.CameraUp)), State.Camera.MoveSpeed);
          State.Camera.CameraPos = AddVec3(State.Camera.CameraPos, HorizontalVec);
        }
        glfwGetCursorPos(Window, &State.Input.MouseX, &State.Input.MouseY);
        
        // @todo: how to create multiple cubes??
        //Mat4 Tx1 = CreateTranslationMat(InitVec4(5.0f, 0, -5.0f, 1));
        //Tx1 = Mul_Mat4Mat4(Tx1, Ry);
        //Mat4 Model1 = IdentityMat();
        //Model1 = Mul_Mat4Mat4(Tx1, Model);
        //DrawModel(Model1, ShaderProgram, BO_Container);
        
        // view matrix
        // @note: in normalised device coordinates, OPENGL switches the handedness
        // this is why:
        // x -> -ve
        // x <- +ve
        r32 OffsetX = State.Input.MouseX - State.Input.LastMouseX;
        r32 OffsetY = State.Input.LastMouseY - State.Input.MouseY;
        State.Input.LastMouseX = State.Input.MouseX;
        State.Input.LastMouseY = State.Input.MouseY;

        OffsetX *= State.Input.Sensitivity;
        OffsetY *= State.Input.Sensitivity;
        State.Camera.YawAngle += OffsetX;
        State.Camera.PitchAngle += OffsetY;

        if (State.Camera.PitchAngle > 89.0f) State.Camera.PitchAngle = 89.0f;
        if (State.Camera.PitchAngle < -89.0f) State.Camera.PitchAngle = -89.0f;

        Vec3 Dir = {0};
        Dir.y = sin(State.Camera.PitchAngle);
        Dir.x = cos(State.Camera.YawAngle) * cos(State.Camera.PitchAngle);
        Dir.z = sin(State.Camera.YawAngle) * cos(State.Camera.PitchAngle);
        State.Camera.CameraFront = UnitVec3(Dir);

        /*
         * @note: camera direction is calculated by: CameraPos - CameraTarget
         * this makes it point in the direction of the cameras +ve axis
         * which also allows us to have the z-component by positive whilst by
         * convention in OpenGL the +ve z-component points to the cameras -ve 
         * direction
         * */
        Mat4 LookAt = CreateLookAtMat4(State.Camera.CameraPos, AddVec3(State.Camera.CameraPos, State.Camera.CameraFront), State.Camera.CameraUp);
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
              glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "View"), 1, GL_TRUE, view);
              glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Projection"), 1, GL_TRUE, projection);

        //Mat4 Rx = CreateRotationMat((r32)PI/4.0f, PIVOT_X);
        //Mat4 Ry = CreateRotationMat((r32)PI/8.0f, PIVOT_Y);
        //Ry = Mul_Mat4Mat4(Ry, Rx);
        Mat4 Tx = CreateTranslationMat(InitVec4(0.0f, 0, 0.0f, 1));
        //Tx = Mul_Mat4Mat4(Tx, Ry);
        Mat4 Model = IdentityMat();
        Model = Mul_Mat4Mat4(Tx, Model);
        DrawModel(Model, ShaderProgram, BO_Container);

        // model 2
        Mat4 Tx1 = CreateTranslationMat(InitVec4(3.0f, 1.0f, 0.0f, 1));
        //Tx = Mul_Mat4Mat4(Tx, Ry);
        Mat4 Model1 = IdentityMat();
        Model1 = Mul_Mat4Mat4(Tx1, Model1);
        DrawModel(Model1, ShaderProgram, BO_Container);
        
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
        
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Transform"), 1, GL_TRUE, ScaleArr);
        DrawRectangle(BO_Container.VAO);
#endif        
        
        glfwSwapBuffers(Window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1f(glGetUniformLocation(ShaderProgram, "TexWeight"), TexWeight);
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
    
    glfwTerminate();
    return 0;
}
