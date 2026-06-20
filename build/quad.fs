#version 330 core

in vec4 FragColorr;
out vec4 FragColor;

uniform float colorOffset;
uniform bool textPass;
uniform vec3 lightPosition;

//in vec3 ModelPos;
in vec3 ViewPos_;
in vec2 TextCoord;
in vec3 FragPos;

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


struct SpotLight{
 // Inherent component
 vec3 direction;
 //vec3 position;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;
 float quadraticTerm;

 // For spotlight effect
 // spotlight area defining angle(Phi) maybe with the different name such as cutoff
 float CutOff;
 // Now the smooth/soft edge effect
 float OuterCutOff;
};

uniform SpotLight spotlight[1];

vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir);

vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir){

// Based on light direction(Light Postion), attenuation base on constant, linear and quadratic term, distance 1/(kc + kl * d + kq * d * d)
  // This is represent the angle between lightDir and norm

	//if fragpos is betweeen of viewPos and and lightPos
	// create line formed by viewPos and tangentLightPos first
	 vec3 AB = ViewPos_ - tangent_light_pos;//Light Dir
	 vec3 AP = lightPosition - ViewPos_;//View Dir

	// Check if cross product is close to zero vector (i.e., colinear)
	 bool colinear = length(cross(AB, AP)) < 1e-5f;

	// Optional: Check if P is between A and B
	 float dot1 = dot(AP, AB);
         float dot2 = dot(AB, AB);
	 bool onSegment = colinear && dot1 >= 0.0f && dot1 <= dot2;


	// Wrong at point light 
	 // vec3 lightDir = onSegment?vec3(0.0):normalize(light.position - FragPos);
	 vec3 lightDir = onSegment?vec3(0.0):normalize(lightPosition - FragPos);


  //vec3 lightDir = normalize(light.position - FragPos);
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
   distance = length(lightPosition - FragPos);
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

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir){
// Spot light effect
// The so called lightDir is the spotDir
  vec3 ambient;
  float diff;

  vec3 reflecDir;
  float spec;

  vec3 diffuse;
  vec3 specular;

// Cause cos value of the angle is inversed( opposite) with angle value so this is when the theta is smaller than the cutoff

// lightDir is the direction from flashlight to the fragment
//light.direction is light direction while lightDir is alway spot direction while is perpencular with the fragment
vec3 lightDir = normalize(lightPosition - FragPos);
//theta is always equal to 180 but why
float theta = dot(lightDir, normalize(-light.direction));


//if(theta > light.CutOff)
//{
// Light direction, fragpos, norm

   reflecDir  = reflect(-lightDir, norm);
   float distance = length(lightPosition - FragPos);

   ambient = light.ambient * texture(material.diffused_texture, TextCoord).rgb;
   spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);
   specular = light.specular * spec * texture(material.diffused_texture, TextCoord).rgb;
   diff = max(dot(norm, lightDir), 0.0f);
   diffuse = light.diffuse * diff * texture(material.diffused_texture, TextCoord).rgb;

// Smooth edge calculation
// This is the cosine value of theta angle
// Dot is cos value between 2 vector
   float epsilon = (light.CutOff - light.OuterCutOff);
   float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

// Attenuation may worked
   float attenuation = 1.0f / ( light.constant + (light.linearTerm * distance) + light.quadraticTerm * (distance * distance) );

   // distance is reversed
   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;


   diffuse *= (intensity);
   specular *= (intensity);

  // This is represent the angle between lightDir and norm

// For some reason the diffuse and specular is to weak may be it wasn't scaled
   return (ambient + diffuse + specular);
// } else {
  // return ambient; 
 //}
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
    vec3 norm = vec3(0.0f, 1.0f, 0.0f);
    vec3 viewDir = normalize(FragPos - ViewPos_);
    vec3 defaultNormal =  vec3(0.0f, 1.0f, 0.0f);
    //vec3 pointlight = CalcPointLight(pointLights[0], ModelPos, defaultNormal, viewDir);
    vec3 FragColor_;
    if(spotlight[0].constant != 0.0f){
        FragColor_ = CalcSpotLight(spotlight[0], norm, viewDir);
        FragColor = vec4(FragColor_, 1.0f);
    } else {
        // vec4 sampled = vec4(1.0f, 1.0f, 1.0f,texture(material.diffused_texture, TextCoord).a);
        vec4 sampled = vec4(texture(material.diffused_texture, TextCoord));
        FragColor = sampled;
    }
    // vec3 FragColor_ = mix(vec3(TextCoord, 1.0f), vec3(1.0f), 0.5f);
    // vec3 FragColor_ = texture(material.diffused_texture, TextCoord).rgb * spotlight[0].ambient.y;
                                //// FragColor = vec4(FragColor_, 1.0f);
}
