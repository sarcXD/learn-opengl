  const char *TexturedRectangleVS = 
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

  const char *MultiTexturedRectangleFS = 
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
