#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec3 FragPos;

void main() {
  gl_Position = Projection*View*Model*vec4(aPos, 1.0f);
  FragPos = vec3(Model*vec4(aPos, 1.0f));

  // @note: high performance penalty for this. do on the cpu
  Normal = mat3(transpose(inverse(Model))) * aNormal;
}

