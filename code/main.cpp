#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h> 
#include <stdio.h>
#include <stdint.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define b8 u8
#define b16 u16

#define r32 float
#define r64 double

#include "math.h"
#include "gl_graphics.cpp"

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height)
{
  glViewport(0, 0, width, height);
}

void HandleInputs(GLFWwindow *Window)
{
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(Window, true);
    }
}

int main()
{
  // initialise glfw, set opengl versions
  // create glfw window
  // load opengl functions with glad
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *Window = glfwCreateWindow(1920, 1080, "LearnOpenGL", NULL, NULL);
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

  glViewport(0, 0, 1920, 1080);
  glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

  r32 vertices[] = {
    // position             colors                  texture coords
   -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,       0.0f, 0.0f, //bottom left
    0.0f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,       1.0f, 0.0f, //bottom right
   -0.5f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f,       0.0f, 1.0f, //top left
    0.0f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f,       1.0f, 1.0f //top right
  };

  u32 indices[] = {
    0, 1, 2,
    2, 1, 3,
  };

  u32 i_sz = sizeof(indices);
  u32 v_sz = sizeof(vertices);

  BufferO BO_Container = CreateRectangleTextured(vertices, v_sz, indices, i_sz);
  BO_Container.TextureO = (u32*)malloc(sizeof(u32)*15); // at max we can have 15 textures on one object
  // load texture using stb image loader
  i32 width, height, nrChannels;
  unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    Texture2D container = (Texture2D){.width=width,.height=height,.nrChannels=nrChannels,.data=data};

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
    Texture2D container = (Texture2D){.width=width1,.height=height1,.nrChannels=nrChannels1,.data=data1};
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
    "uniform mat4 Transform;\n"
    "void main() {\n"
    "gl_Position = vec4(aPos, 1.0f);\n"
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
    "void main() {\n"
    "FragColor = mix(texture(Texture0, TextureCoord), texture(Texture1, TextureCoord), TexWeight);\n"
    "}\0";

  u32 FragmentShader = CreateFragmentShader(FragmentShaderSource);
  u32 ShaderProgram = CreateShaderProgram(VertexShader, FragmentShader);

  // shader program using color attributes
  glUseProgram(ShaderProgram);
  glUniform1i(glGetUniformLocation(ShaderProgram, "Texture0"), 0);
  glUniform1i(glGetUniformLocation(ShaderProgram, "Texture1"), 1);
  glUniform1i(glGetUniformLocation(ShaderProgram, "Transform"), 2);

  r32 TexWeight = 0.2f;
  while (!glfwWindowShouldClose(Window)) {
    glfwPollEvents();
    HandleInputs(Window);
    if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
    {
      TexWeight += TexWeight < 1.0f ? 0.01f : 0.0f;
    }
    if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
      TexWeight -= TexWeight > 0.0f ? 0.01f : 0.0f;
    }

    glUniform1f(glGetUniformLocation(ShaderProgram, "TexWeight"), TexWeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawRectangle(BO_Container.VAO);


    glfwSwapBuffers(Window);
  };

  glfwTerminate();
  return 0;
}
