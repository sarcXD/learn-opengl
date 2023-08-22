#version 330 core
struct Material {
  vec3 Ambient;
  vec3 Diffuse;
  vec3 Specular;
  float Shininess;
};

struct Light {
  vec3 Ambient;
  vec3 Diffuse;
  vec3 Specular;

  vec3 Position;
};

in vec3 Normal;
in vec3 FragPos;

uniform vec3 ViewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main() {
  // ambient
  vec3 AmbientLight = light.Ambient * material.Ambient ;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 LightDir = normalize(light.Position - FragPos);
  float DiffuseVal = max(dot(norm, LightDir), 0.0);
  vec3 DiffuseLight = light.Diffuse * (DiffuseVal * material.Diffuse);
  
  // specular
  vec3 ViewDir = normalize(ViewPos - FragPos);
  vec3 ReflectDir = reflect(-LightDir, norm);
  float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), material.Shininess);
  vec3 SpecularLight =  light.Specular * (Spec * material.Specular );

  // Color
  vec3 Result = AmbientLight + DiffuseLight + SpecularLight;
  FragColor = vec4(Result, 1.0);
}

