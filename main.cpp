#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h> 

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

  //unsigned int EBO;
  //glGenBuffers(1, &EBO);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
  glGetShaderiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
    // todo(talha): error logging
    // program linking failed
  }

  glDeleteShader(VertexShader);
  glDeleteShader(FragmentShader);

  return ShaderProgram;
}


int main()
{
  // setup
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *Window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (Window == NULL)
  {
    // todo(talha): add error logging for failed to create glfw window
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(Window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    // todo(talha): error logging - failed to initialize glad
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

  float vertices1[] = {
    -1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  unsigned int VAO1 = CreateTriangle(vertices1, sizeof(vertices1));

  // float vertices2[9] = {
  //   0.0f, -0.5f, 0.0f,
  //   0.5f, 0.5f, 0.0f,
  //   1.0f, -0.5f, 0.0f
  // };

  // unsigned int VAO2 = CreateTriangle(vertices2, sizeof(vertices2));

  const char *VertexShaderSource = 
    "#version 330 core\n"
    "layout (location=0) in vec3 aPos;\n"
    "layout (location=1) in vec3 aColor;\n"
    "out vec3 vertexPos;\n"
    "uniform vec3 horizOffset;\n"
    "void main() {\n"
    "gl_Position = vec4(horizOffset.x + aPos.x, -aPos.y, aPos.z, 1.0f);\n"
    "vertexPos = vec3(gl_Position);\n"
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
    "in vec3 vertexPos;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(vertexPos, 1.0f);\n"
    "}\0";

  unsigned int FragmentShader = CreateFragmentShader(FragmentShaderSource);
  unsigned int ShaderProgram1 = CreateShaderProgram(VertexShader, FragmentShader);

  while (!glfwWindowShouldClose(Window)) {
    glfwPollEvents();
    HandleInputs(Window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // shader program using color attributes
    glUseProgram(ShaderProgram1);
    int OffsetUniformPos = glGetUniformLocation(ShaderProgram1, "horizOffset");
    glUniform3f(OffsetUniformPos, 0.5f, 0.0f, 0.0f);
    DrawTriangle(VAO1);

    // updating color using a uniform
    // float TimeValue = glfwGetTime();
    // float ColorValue = (sin(TimeValue)/2.0f) + 0.5f;
    // glUseProgram(ShaderProgram2);
    // DrawTriangle(VAO2);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(Window);
  };

  glfwTerminate();
  return 0;
}
