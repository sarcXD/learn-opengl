#ifndef GL_GRAPHICS_H
#define GL_GRAPHICS_H
typedef struct BufferO {
  u32 VAO; // Vertex Array Object
  u32 VBO; // Vertex Buffer Object
  u32 EBO; // Element Buffer Object
  u32* TextureO; // Texture Buffer Object
} BufferO;

typedef struct Texture2D {
  i32 width;
  i32 height;
  i32 nrChannels;
  unsigned char* data;
} Texture2D;

#endif
