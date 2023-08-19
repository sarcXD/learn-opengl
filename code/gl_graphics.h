#ifndef GL_GRAPHICS_H
#define GL_GRAPHICS_H

u32 CreateTriangle(r32 vertices[], i32 sz);
void DrawTriangle(u32 VAO);

u32 CreateRectangle(r32 vertices[], i32 v_sz, u32 indices[], i32 i_sz);
/**
 * @note: the texture functions for rectangle are more obtuse and less clear
 * they don't make as much sense. And they dont just create something. 
 * They play on to the state machine-ness of opengl, and just enable a set of 
 * operations I find repetitive.
 *
 * @todo: I need to probably add the examples to my utils file as notes
 * */
BufferO CreateRectangleTextured(r32 vertices[], i32 v_sz, u32 indices[], i32 i_sz);
void AddTextureToRect(BufferO *BO, Texture2D *Texture, i32 FilteringOpt, i32 TextureUnit, i32 InternalFormat);
void DrawRectangle(u32 VAO);

BufferO CreateCube(r32 vertices[], i32 v_sz);
BufferO CreateCubeTextured(r32 vertices[], i32 v_sz);
void DrawCube(u32 VAO);

u32 CreateVertexShader(const char *VertexShaderSource);
u32 CreateFragmentShader(const char *FragmentShaderSource);
u32 CreateShaderProgram(unsigned int VertexShader, unsigned int FragmentShader);
#endif
