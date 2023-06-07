#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h> 
#include <stdio.h>

typedef struct BufferO {
  unsigned int VAO; // Vertex Array Object
  unsigned int VBO; // Vertex Buffer Object
  unsigned int EBO; // Element Buffer Object
  unsigned int TextureO; // Texture Buffer Object
} BufferO;

typedef struct Texture2D {
  int width;
  int height;
  int nrChannels;
  unsigned char* data;
} Texture2D;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
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

unsigned int CreateTriangle(float vertices[], int sz)
{
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // bind to setup triangle attributes

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sz, vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // unbind post setup
  glBindVertexArray(0);

  return VAO;
}

void DrawTriangle(unsigned int VAO)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

unsigned int CreateRectangle(float vertices[], int v_sz, unsigned int indices[], int i_sz)
{
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // bind to setup triangle attributes

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, v_sz, vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_sz, indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // unbind post setup
  glBindVertexArray(0);

  return VAO;

}

void DrawRectangle(unsigned int VAO)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

BufferO CreateRectangleTextured(float vertices[], int v_sz, unsigned int indices[], int i_sz, Texture2D *texture)
{
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // bind to setup triangle attributes

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, v_sz, vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_sz, indices, GL_STATIC_DRAW);

  /*
   * im a bit concerned about the impact defining textures here has on flexibility of my rendering
   * on one hand I dont want to redefine everything so im pretty sure if I take a single approach and 
   * go with it, it will work
   * */
  unsigned int TextureO;
  glGenTextures(1, &TextureO);
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

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  // unbind post setup
  glBindVertexArray(0);

  BufferO BO = (BufferO){.VAO=VAO,.VBO=VBO,.EBO=EBO,.TextureO=TextureO};

  return BO;
}

void DrawRectangleTextured(BufferO BO)
{
    glBindTexture(GL_TEXTURE_2D, BO.TextureO);
    glBindVertexArray(BO.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int CreateVertexShader(const char *VertexShaderSource)
{
  unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
  glCompileShader(VertexShader);

  int Success;
  char InfoLog[512];
  glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
  if (!Success)
  {
    glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
    // todo(talha): error logging
    // vertex shader compilation failed
    printf("================================\n");
    printf("vertex shader compilation failed:\n%s\n", InfoLog);
  }

  return VertexShader;
}

unsigned int CreateFragmentShader(const char *FragmentShaderSource)
{
  unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
  glCompileShader(FragmentShader);

  int Success;
  char InfoLog[512];
  glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
  if (!Success)
  {
    glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
    // todo(talha): error logging
    // fragment shader compilation failed
    printf("================================\n");
    printf("fragment shader compilation failed:\n%s\n", InfoLog);
  }

  return FragmentShader;
}

unsigned int CreateShaderProgram(unsigned int VertexShader, unsigned int FragmentShader)
{
  unsigned int ShaderProgram = glCreateProgram();

  glAttachShader(ShaderProgram, VertexShader);
  glAttachShader(ShaderProgram, FragmentShader);
  glLinkProgram(ShaderProgram);

  int Success;
  char InfoLog[512];
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
    // todo(talha): error logging
    // program linking failed
    printf("================================\n");
    printf("program linking failed:\n%s\n", InfoLog);
  }

  glDeleteShader(VertexShader);
  glDeleteShader(FragmentShader);

  return ShaderProgram;
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

  GLFWwindow *Window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

  float vertices[] = {
    // position             colors                  texture coords
   -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,       0.0f, 0.0f, //bottom left
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,       1.0f, 0.0f, //bottom right
   -0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,       0.0f, 1.0f, //top left
    0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,       1.0f, 1.0f //top right
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 1, 3,
  };

  int i_sz = sizeof(indices);
  int v_sz = sizeof(vertices);

  // load texture using stb image loader
  int width, height, nrChannels;
  unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);

  Texture2D container = (Texture2D){.width=width,.height=height,.nrChannels=nrChannels,.data=data};

  BufferO BO_Container = CreateRectangleTextured(vertices, v_sz, indices, i_sz, &container);
  stbi_image_free(container.data);

  const char *VertexShaderSource = 
    "#version 330 core\n"
    "layout (location=0) in vec3 aPos;\n"
    "layout (location=1) in vec3 aColor;\n"
    "layout (location=2) in vec2 aTexCoord;\n"
    "out vec3 VertexColor;\n"
    "out vec2 TextureCoord;\n"
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
  unsigned int VertexShader = CreateVertexShader(VertexShaderSource);


  const char *FragmentShaderSource = 
    "#version 330 core\n"
    "in vec3 VertexColor;\n"
    "in vec2 TextureCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D Texture;\n"
    "void main() {\n"
    "FragColor = texture(Texture, TextureCoord);\n"
    "}\0";

  unsigned int FragmentShader = CreateFragmentShader(FragmentShaderSource);
  unsigned int ShaderProgram = CreateShaderProgram(VertexShader, FragmentShader);

  while (!glfwWindowShouldClose(Window)) {
    glfwPollEvents();
    HandleInputs(Window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // shader program using color attributes
    glUseProgram(ShaderProgram);
    // DrawRectangle(VAO);
    DrawRectangleTextured(BO_Container);


    glfwSwapBuffers(Window);
  };

  glfwTerminate();
  return 0;
}
