#version 330 core

out vec4 FragColorr;

in vec2 TexCoord;
in vec3 Normal;

// in vec3 FragPos;

#define NR_POINT_LIGHTS 2

uniform vec3 ViewPos;
uniform vec3 lightPos;
uniform vec3 pointLight_Pos[NR_POINT_LIGHTS];


in VS_OUT{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 tangent_light_pos;
    vec3 tangentViewPos;
    vec3 tangentpointLight_Pos[NR_POINT_LIGHTS];
}fs_in;


struct Materiall{
// Ambient will be the same even with change in input
   sampler2D texture_diffused1;
   sampler2D texture_specular1;
   sampler2D texture_normal1;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float shininess;
};

uniform Materiall material;

void main(){
     // if(TexCoord_ != vec2(0.0f) || texture(material.texture_diffused1, TexCoord_).rgb != vec3(0.0f)){

vec4 outFrag = vec4(texture(material.texture_diffused1, fs_in.TexCoord).rgb, 1.0f);

    // vec4 outFrag = vec4(TexCoord_, 0.0, 1.0f);
     // } else {
     // FragColorr = vec4(TexCoord_, 0.0, 1.0f);
     //  }
outFrag += vec4(texture(material.texture_specular1, fs_in.TexCoord).rgb, 1.0f); 
     FragColorr = outFrag;
      // FragColorr = vec4(0.3, 0.4, 0.5, 1.0f); 
};
