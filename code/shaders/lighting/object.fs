#version 330 core
in vec3 Normal;
in vec3 FragPos;

uniform vec3 VertexColor;
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 ViewPos;

out vec4 FragColor;

void main() {
  // ambient
  float AmbientStrength = 0.1;
  vec3 AmbientLight = AmbientStrength * LightColor;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 LightDir = normalize(LightPos - FragPos);
  float DiffuseVal = max(dot(norm, LightDir), 0.0);
  vec3 DiffuseLight = DiffuseVal*LightColor;
  
  // specular
  float SpecularStrength = 0.5;
  vec3 ViewDir = normalize(ViewPos - FragPos);
  vec3 ReflectDir = reflect(-LightDir, norm);
  float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), 32);
  vec3 SpecularLight = SpecularStrength * Spec * LightColor;

  // Color
  vec3 Result = (AmbientLight + DiffuseLight + SpecularLight)*VertexColor;
  FragColor = vec4(Result, 1.0);
}

