#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

struct Material{

// Ambient will be the same even with change in input
   sampler2D texture_diffused0;
   sampler2D texture_specular0;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float shininess;
};

uniform Material material;

void main(){
  vec4 Texturee = texture(material.texture_diffused, TexCoords);
  FragColor = Texturee * vec4(FragPos, 1.0f);
};