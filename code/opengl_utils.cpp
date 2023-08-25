// They go from basic to more complex
#if 0
/*
 * Draw Simple Rectangle
 */
    int w = 512;
    int h = 512;
    int n = 3;
    Texture2D container = (Texture2D){.width=w, .height=h, .nrChannels=n, .data=bitmap_data};
    r32 vertices[12] = {
      -0.70f, -0.70f, 0.0f,
       0.70f, -0.70f, 0.0f,
      -0.70f,  0.70f, 0.0f,
       0.70f,  0.70f, 0.0f
    };

    u32 indices[6] = {
      2, 1, 0,
      3, 1, 2
    };

    u32 VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(r32), (void*)0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    const char *VertexShaderSource =
    "#version 330 core \n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
    "}\n";
    
    const char *FragmentShaderSource =
    "#version 330 core \n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
    "}\n";

    u32 vS = CreateVertexShader(VertexShaderSource);
    u32 fS = CreateFragmentShader(FragmentShaderSource);
    u32 shaderProg = CreateShaderProgram(vS, fS);

    glUseProgram(shaderProg);
    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        HandleInputs(Window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawRectangle(VAO);
        glfwSwapBuffers(Window);
    };
#endif

#if 0
// using uniform variables in shaders
const char *VertexShaderSource =
"#version 330 core \n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"}\n";

const char *FragmentShaderSource =
"#version 330 core \n"
"uniform vec4 VecColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = VecColor; \n"
"}\n";

u32 vS = CreateVertexShader(VertexShaderSource);
u32 fS = CreateFragmentShader(FragmentShaderSource);
u32 shaderProg = CreateShaderProgram(vS, fS);

i32 VecColorLoc = glGetUniformLocation(shaderProg, "VecColor");
if (VecColorLoc == -1)
{
  printf("ERROR: Failed to get uniform location");
  return -1;
}

glUseProgram(shaderProg);
glUniform4f(VecColorLoc, 1.0f, 0.7f, 0.5f, 1.0f);
    
#endif

#if 0
// passing color as a vertex attribute to shader
    r32 vertices[] = {
      // POSITION             COLOR
      -0.70f, -0.70f, 0.0f,   0.5f, 0.0f, 0.0f,
       0.70f, -0.70f, 0.0f,   0.0f, 0.5f, 0.0f,
      -0.70f,  0.70f, 0.0f,   0.0f, 0.0f, 0.5f,
       0.70f,  0.70f, 0.0f,   0.5f, 0.0f, 0.5f
    };

    u32 indices[] = {
      2, 1, 0,
      3, 1, 2
    };

    u32 VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // @note: we changed how glVertexAttributes are defined
    // positoin and color are separate attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)(3*sizeof(r32)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    const char *VertexShaderSource =
    "#version 330 core \n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec4 VertexColor;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
    " VertexColor = vec4(aColor, 1.0f); \n"
    "}\n";
    
    const char *FragmentShaderSource =
    "#version 330 core \n"
    "in vec4 VertexColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = VertexColor; \n"
    "}\n";

    u32 vS = CreateVertexShader(VertexShaderSource);
    u32 fS = CreateFragmentShader(FragmentShaderSource);
    u32 shaderProg = CreateShaderProgram(vS, fS);

    glUseProgram(shaderProg);
    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        HandleInputs(Window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawRectangle(VAO);
        glfwSwapBuffers(Window);
    }
#endif

#if 0
// Draw textured rectangle
r32 vertices[] = {
  // POSITION             COLOR               TEXTURE COORDS
  -0.70f, -0.70f, 0.0f,   0.5f, 0.0f, 0.0f,   0.0f, 0.0f,
   0.70f, -0.70f, 0.0f,   0.0f, 0.5f, 0.0f,   1.0f, 0.0f,
  -0.70f,  0.70f, 0.0f,   0.0f, 0.0f, 0.5f,   0.0f, 1.0f,
   0.70f,  0.70f, 0.0f,   0.5f, 0.0f, 0.5f,   1.0f, 1.0f
};

u32 indices[] = {
  2, 1, 0,
  3, 1, 2
};

u32 VBO, VAO, EBO, Texture;
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);

glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glGenTextures(1, &Texture);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);
// define texture scaling parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
glGenerateMipmap(GL_TEXTURE_2D);

// @note: we changed how glVertexAttributes are defined
// positoin and color are separate attributes
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)(3*sizeof(r32)));
glEnableVertexAttribArray(1);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)(6*sizeof(r32)));
glEnableVertexAttribArray(2);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

const char *VertexShaderSource =
"#version 330 core \n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec4 VertexColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
" VertexColor = vec4(aColor, 1.0f); \n"
" TexCoord = aTexCoord; \n"
"}\n";

const char *FragmentShaderSource =
"#version 330 core \n"
"in vec4 VertexColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = texture(ourTexture, TexCoord); \n"
"}\n";

u32 vS = CreateVertexShader(VertexShaderSource);
u32 fS = CreateFragmentShader(FragmentShaderSource);
u32 shaderProg = CreateShaderProgram(vS, fS);

glUseProgram(shaderProg);
i32 samplerLoc = glGetUniformLocation(shaderProg, "ourTexture");
glUniform1i(samplerLoc, 0);

while (!glfwWindowShouldClose(Window)) {
    glfwPollEvents();
    HandleInputs(Window);
    
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, Texture);
    DrawRectangle(VAO);
    glfwSwapBuffers(Window);
}
#endif

#if 0
/*
 *  Reading a texture using stb_image
 *  Then assigning it to a rectangle vertex with an element buffer
 */
  BufferO BO_Container = CreateRectangleTextured(vertices, v_sz, indices, i_sz);
  BO_Container.TextureO = (unsigned int*)malloc(sizeof(unsigned int)*15); // at max we can have 15 textures on one object
  // load texture using stb image loader
  int width, height, nrChannels;
  unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    Texture2D container = (Texture2D){.width=width,.height=height,.nrChannels=nrChannels,.data=data};

    glBindVertexArray(BO_Container.VAO);

    unsigned int TextureO;
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

// ... Some code ...


 glUseProgram(ShaderProgram);
 // here Texture0 is the name of a uniform variable used in a fragment shader
 glUniform1i(glGetUniformLocation(ShaderProgram, "Texture0"), 0);
 // ************************** END SAMPLE ***************************
#endif
