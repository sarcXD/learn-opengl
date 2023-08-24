#include "gl_graphics.h"

void LoadUniformInt(u32 SP, const char *Uniform, i32 Val)
{
    glUniform1i(glGetUniformLocation(SP, Uniform), Val);
}

void LoadUniformFloat(u32 SP, const char *Uniform, r32 Val)
{
    glUniform1f(glGetUniformLocation(SP, Uniform), Val);
}

void LoadUniformVec3(u32 SP, const char *Uniform, Vec3 Val)
{
    glUniform3f(glGetUniformLocation(SP, Uniform), Val.x, Val.y, Val.z);
}

void LoadUniformMat4(u32 SP, const char *Uniform, const r32 *Val)
{
    glUniformMatrix4fv(glGetUniformLocation(SP, Uniform), 1, GL_TRUE, Val);
}

u32 CreateTriangle(r32 vertices[], i32 sz)
{
    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // bind to setup triangle attributes
    
    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sz, vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)(3*sizeof(r32)));
    glEnableVertexAttribArray(1);
    
    // unbind post setup
    glBindVertexArray(0);
    
    return VAO;
}

void DrawTriangle(u32 VAO)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

u32 CreateRectangle(r32 vertices[], i32 v_sz, u32 indices[], i32 i_sz)
{
    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // bind to setup triangle attributes
    
    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_sz, vertices, GL_STATIC_DRAW);
    
    u32 EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_sz, indices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // unbind post setup
    glBindVertexArray(0);
    
    return VAO;
    
}

void DrawRectangle(u32 VAO)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DrawCube(u32 VAO)
{
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

BufferO CreateCube(r32 vertices[], i32 v_sz)
{
    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // bind to setup triangle attributes
    
    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_sz, vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)0);
    glEnableVertexAttribArray(0);
    
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(r32), (void*)(3*sizeof(r32)));
    glEnableVertexAttribArray(1);

    // unbind post setup
    glBindVertexArray(0);
    
    BufferO BO = {0};
    BO.VAO=VAO;
    BO.VBO=VBO;
    
    return BO;
}

BufferO CreateCubeTextured(r32 vertices[], i32 v_sz)
{
    /*
     * This will setup options and buffer objects to create a rectangle based on a texture
     * The actual assigning of a texture will happen outside of this function
     */
    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // bind to setup triangle attributes
    
    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_sz, vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)0);
    glEnableVertexAttribArray(0);
    
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)(3*sizeof(r32)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)(6*sizeof(r32)));
    glEnableVertexAttribArray(2);
    
    // unbind post setup
    glBindVertexArray(0);
    
    BufferO BO = {0};
    BO.VAO=VAO;
    BO.VBO=VBO;
    
    return BO;
}

BufferO CreateRectangleTextured(r32 vertices[], i32 v_sz, u32 indices[], i32 i_sz)
{
    /*
     * This will setup options and buffer objects to create a rectangle based on a texture
     * The actual assigning of a texture will happen outside of this function
     */
    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // bind to setup triangle attributes
    
    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_sz, vertices, GL_STATIC_DRAW);
    
    u32 EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_sz, indices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)(3*sizeof(r32)));
    glEnableVertexAttribArray(1);
    
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(r32), (void*)(6*sizeof(r32)));
    glEnableVertexAttribArray(2);
    
    // unbind post setup
    glBindVertexArray(0);
    
    BufferO BO = {0};
    BO.VAO=VAO;
    BO.VBO=VBO;
    BO.EBO=EBO;
    
    return BO;
}

/*
 * Defines texture loaded from a Texture2D object into opengl. 
 * Texture wrapping and filtering options are defined. Should they need be altered
 * define a new function or change them inside. It depends on the art style, but 
 * I think it should remain the same for everything
 *
 * Parameters:
 * 1. Texture2D Tex: Contains information about the loaded texture
 * 2. u32 VAO: Contains the VertexArrayObject index for the VAO to attach the texture to
 * 3. u32 *TexO: Pointer to a TextureObject. This needs to be adtivated by glActiveTexture
 * in order to define texture properties
 * 4. u32 Ind: The index of this texture. This is the unique identifier that allows us to let
 * opengl know which texture we are talking about when assigning data to it. So far I have used it
 * to let sampler2D know which texture I need to attach
 */
void DefineGlTextures(Texture2D Tex, u32 VAO, u32 *TexO, u32 Ind)
{
  glBindVertexArray(VAO);

  glGenTextures(1, TexO);
  glActiveTexture(GL_TEXTURE0 + Ind);
  glBindTexture(GL_TEXTURE_2D, *TexO);

  // defining texture wrapping options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Tex.width, Tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Tex.data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindVertexArray(0);
}

u32 CreateVertexShader(const char *VertexShaderSource)
{
    u32 VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(VertexShader);
    
    i32 Success;
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

u32 CreateFragmentShader(const char *FragmentShaderSource)
{
    u32 FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);
    
    i32 Success;
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

u32 CreateShaderProgram(unsigned int VertexShader, unsigned int FragmentShader)
{
    u32 ShaderProgram = glCreateProgram();
    
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    
    i32 Success;
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
