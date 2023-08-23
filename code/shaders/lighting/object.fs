#version 330 core
struct Material {
  sampler2D Diffuse;
  sampler2D Specular;
  sampler2D Emission;
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
in vec2 TexCoords;

uniform vec3 ViewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main() {
  // ambient
  vec3 AmbientLight = light.Ambient * vec3(texture(material.Diffuse, TexCoords));

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 LightDir = normalize(light.Position - FragPos);
  float DiffuseVal = max(dot(norm, LightDir), 0.0);
  vec3 DiffuseLight = light.Diffuse * DiffuseVal * vec3(texture(material.Diffuse, TexCoords));
  
  // specular
  vec3 ViewDir = normalize(ViewPos - FragPos);
  vec3 ReflectDir = reflect(-LightDir, norm);
  float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), material.Shininess);
  vec3 SpecularLight =  light.Specular * Spec * vec3(texture(material.Specular, TexCoords));

  // emission map
  vec3 Emission = vec3(texture(material.Emission, TexCoords));
  vec3 Mapping = step(vec3(1.0), vec3(1.0)-texture(material.Specular, TexCoords).rgb);

  // Color
  vec3 Result = AmbientLight + DiffuseLight + SpecularLight + (Mapping*Emission);
  FragColor = vec4(Result, 1.0);
}

