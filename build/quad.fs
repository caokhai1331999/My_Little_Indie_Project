#version 330 core

in vec4 FragColorr;
out vec4 FragColor;

uniform sampler2D ttexture;
uniform float colorOffset;
uniform bool textPass;

in vec3 ModelPos;
in vec3 ViewPos_;
in vec2 TextCoord;

struct Material{
    sampler2D diffused_texture;
    sampler2D specular_texture;
    float shininess;
};

uniform Material material;

struct PointLight{
 // Inherent component
 // vec3 position;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;
 float quadraticTerm;
};

uniform PointLight pointLights[1];

vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir);

vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir){

// Based on light direction(Light Postion), attenuation base on constant, linear and quadratic term, distance 1/(kc + kl * d + kq * d * d)
  // This is represent the angle between lightDir and norm

	//if fragpos is betweeen of viewPos and and lightPos
	// create line formed by viewPos and fs_in.tangentLightPos first
	 vec3 AB = ViewPos_ - tangent_light_pos;//Light Dir
	 vec3 AP = ModelPos - ViewPos_;//View Dir

	// Check if cross product is close to zero vector (i.e., colinear)
	 bool colinear = length(cross(AB, AP)) < 1e-5f;

	// Optional: Check if P is between A and B
	 float dot1 = dot(AP, AB);
         float dot2 = dot(AB, AB);
	 bool onSegment = colinear && dot1 >= 0.0f && dot1 <= dot2;


	// Wrong at point light 
	 // vec3 lightDir = onSegment?vec3(0.0):normalize(light.position - fs_in.FragPos);
	 vec3 lightDir = onSegment?vec3(0.0):normalize(tangent_light_pos - ModelPos);


  //vec3 lightDir = normalize(light.position - fs_in.FragPos);
  vec3 reflecDir  = reflect(-lightDir, norm);

  // Light direction, fragpos, norm
  vec3 ambient = light.ambient;

  float diff = max(dot(lightDir, norm), 0.0f);
  vec3 diffuse = light.diffuse * diff;

  // Wrong
  float spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);
  vec3 specular = light.specular * spec;


// Now the attenuation calculation
// This length is for calculating the distance
   float distance;
   float attenuation;
   distance = length(tangent_light_pos - ModelPos);
   attenuation = 1.0f / (light.constant + light.linearTerm * distance + light.quadraticTerm * (distance * distance));

   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;

   return (ambient + diffuse + specular);
}


vec4 interpolateColor(vec4 color, float colorOffset){
    vec4 outColor;
       if(colorOffset < 1.0f){
	  outColor.x = color.x + sin(colorOffset);
          outColor.y = color.y + cos(colorOffset);
          outColor.z = color.z + tan(colorOffset);
	  return outColor;
	  }
	  outColor = color;
	  return outColor ;
}

bool isNULL(vec4 color){
    for(int i = 0; i < 4; i++){
        if(color[i] != 0){
            return false;
        }
    };
    return true;
}

void resetColor(vec4 color){
   if(color.x >= 1.0f){
      color.x -= 1.0f;      
   }

   if(color.y >= 1.0f){
      color.y -= 1.0f;
   }

   if(color.x >= 1.0f){
      color.z -= 1.0f;
   }
}

void main(){
    //     vec4 texture_ = texture(ttexture, TextCoord);
    // if(textPass){
    //     if(texture_.a < 0.1)
    //         discard;
        // if(ModelPos.x >= 0.0f){
        // FragColor = texture_;
        // } else {
        //  FragColor = vec4(0.0f);
        // }        
    // }else{
    //     FragColor = FragColorr;
    // };
    if(material.shininess == 0.0f){
        float shininesss  = 32.0f;
    };
    vec3 viewDir = normalize(ModelPos - ViewPos_);
    vec3 defaultNormal =  vec3(0.0f, 1.0f, 0.0f);
    vec3 pointlight = CalcPointLight(pointLights[0], ModelPos, defaultNormal, viewDir);
    vec3 FragColor_ = vec3(0.49609375f, 1.0f, 0.0f) * pointlight;
    // FragColor = vec4(FragColor_, 1.0f);
    FragColor = FragColorr;
}
