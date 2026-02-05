#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material{

// Ambient will be the same even with change in input
   sampler2D texture_diffused1;
   sampler2D texture_specular1;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float shininess;
};
uniform Material material;

void main(){
  FragColor = texture(material.texture_diffused1, TexCoords);
};