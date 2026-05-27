#version 330 core

#define NR_POINT_LIGHTS_ 2

out vec4 FragColorr;

// in vec2 TexCoord;
// in vec3 Normal;
// in vec3 FragPos;

in VS_OUT{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 tangent_light_pos;
    vec3 tangentViewPos;
    vec3 tangentpointLight_Pos[NR_POINT_LIGHTS_];
}fs_in;


struct light_in_general{
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
};

uniform light_in_general light;

struct Material{
// Ambient will be the same even with change in input
   sampler2D texture_diffused1;
   sampler2D texture_specular1;
   sampler2D texture_normal1;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float shininess;
};

uniform Material material;

struct DirLight_{
 // Inherent component
 vec3 direction;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

uniform DirLight_ dirLight;

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

uniform PointLight pointLights [NR_POINT_LIGHTS_];
// These two contain bugs

vec3 CalcDirLight(DirLight_ light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir);

vec3 CalcDirLight(DirLight_ light, vec3 norm, vec3 viewDir){
  // Light direction, fragpos, norm
    vec3 lightDirection = fs_in.tangentViewPos - fs_in.tangent_light_pos;
    vec3 ambient = texture(material.texture_specular1, fs_in.TexCoord).rgb;
// 
  // This is represent the angle between lightDir and norm
  // float diff = max(dot(-lightDirection, norm), 0.0f);
  // vec3 diffuse = diff * texture(material.texture_diffused1, fs_in.TexCoord).rgb;
//light.diffuse * 
  // vec3 reflecDir  = reflect(-lightDirection, norm);
  // float spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);

  // vec3 specular = spec * texture(material.texture_specular1, fs_in.TexCoord).rgb;
//light.specular * 
  return ambient ;
  //+ diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir){

// Based on light direction(Light Postion), attenuation base on constant, linear and quadratic term, distance 1/(kc + kl * d + kq * d * d)
  // This is represent the angle between lightDir and norm

	//if fragpos is betweeen of viewPos and and lightPos
	// create line formed by viewPos and fs_in.tangentLightPos first
	 vec3 AB = fs_in.tangentViewPos - tangent_light_pos;//Light Dir
	 vec3 AP = fs_in.FragPos - fs_in.tangentViewPos;//View Dir

	// Check if cross product is close to zero vector (i.e., colinear)
	 bool colinear = length(cross(AB, AP)) < 1e-5f;

	// Optional: Check if P is between A and B
	 float dot1 = dot(AP, AB);
         float dot2 = dot(AB, AB);
	 bool onSegment = colinear && dot1 >= 0.0f && dot1 <= dot2;


	// Wrong at point light 
	 // vec3 lightDir = onSegment?vec3(0.0):normalize(light.position - fs_in.FragPos);
	 vec3 lightDir = onSegment?vec3(0.0):normalize(tangent_light_pos - fs_in.FragPos);


  //vec3 lightDir = normalize(light.position - fs_in.FragPos);
  vec3 reflecDir  = reflect(-lightDir, norm);

  // Light direction, fragpos, norm
  vec3 ambient = light.ambient * texture(material.texture_specular1, fs_in.TexCoord).rgb;

  float diff = max(dot(lightDir, norm), 0.0f);
  vec3 diffuse = light.diffuse * diff * texture(material.texture_diffused1, fs_in.TexCoord).rgb;

  // Wrong
  float spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.TexCoord).rgb;


// Now the attenuation calculation
// This length is for calculating the distance
   float distance;
   float attenuation;

   distance = length(tangent_light_pos - fs_in.FragPos);
   attenuation = 1.0f / (light.constant + light.linearTerm * distance + light.quadraticTerm * (distance * distance));

   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;

   return (ambient + diffuse + specular);
}


void main(){

    vec3 outFrag ;

    vec3 normal_ = texture(material.texture_normal1, fs_in.TexCoord).rgb;
    vec3 norm = normalize(normal_ * 2.0 - 1.0);

  	vec3 viewDir = normalize( fs_in.tangentViewPos - fs_in.FragPos);
    
//     && viewDir !=vec3(0.0f)
    if(dirLight.diffuse != vec3(0.0f)){
        // outFrag = CalcDirLight(dirLight, norm, viewDir);
    vec3 lightDirection = fs_in.tangentViewPos - fs_in.tangent_light_pos;
    vec3 ambient = texture(material.texture_specular1, fs_in.TexCoord).rgb;
    outFrag += dirLight.ambient * ambient;
    }else{
        outFrag = texture(material.texture_diffused1, fs_in.TexCoord).rgb;
    }

    // outFrag += texture(material.texture_specular1, fs_in.TexCoord).rgb * dirLight.specular;
    // outFrag += texture(material.texture_diffused1, fs_in.TexCoord).rgb;
    // if(dirLight.specular != vec3(0.0f)){
    //     FragColorr = vec4(texture(material.texture_diffused1, fs_in.TexCoord).rgb *  dirLight.diffuse, 0.0f);
    // }else{
    //     FragColorr = texture(material.texture_diffused1, fs_in.TexCoord);
    // }

	// for (int i = 0; i < NR_POINT_LIGHTS; i++)
	// {	
    //     outFrag += CalcPointLight(pointLights[i], fs_in.tangentpointLight_Pos[i], norm, viewDir);
	// };

    // FragColorr = texture(material.texture_diffused1, fs_in.TexCoord);
    FragColorr = vec4(outFrag, 1.0f);
};
