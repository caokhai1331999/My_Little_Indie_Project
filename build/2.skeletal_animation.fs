#version 330 core

out vec4 FragColorr;

in vec2 TexCoord;
// in vec3 Normal;
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


struct DirLight{
 // Inherent component
 vec3 direction;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};


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


uniform DirLight dirLight;
uniform Material material;
uniform PointLight pointLights [NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir);

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir){
  // Light direction, fragpos, norm
    vec3 lightDirection = fs_in.tangentViewPos - fs_in.tangent_light_pos;

    vec3 ambient = light.ambient * texture(material.texture_diffused1, fs_in.TexCoord).rgb;

  // This is represent the angle between lightDir and norm
  float diff = max(dot(-lightDirection, norm), 0.0f);
  vec3 diffuse = light.diffuse * diff * texture(material.texture_diffused1, fs_in.TexCoord).rgb;

  vec3 reflecDir  = reflect(-lightDirection, norm);
  float spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);

  vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.TexCoord).rgb;

  return (ambient + diffuse + specular);
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
    if(norm != vec3(0.0f) && viewDir !=vec3(0.0f)){
        outFrag = CalcDirLight(dirLight, norm, viewDir);
    }else{
        outFrag = texture(material.texture_diffused1, fs_in.TexCoord).rgb;
    }

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{	
        outFrag += CalcPointLight(pointLights[i], fs_in.tangentpointLight_Pos[i], norm, viewDir);
	};
    // if(fs_in.TexCoord != vec2(0.0f) && texture(material.texture_diffused1, fs_in.TexCoord).rgb != vec3(0.0f)){
    // vec3 norm = texture(material.texture_normal1, fs_in.TexCoord).rgb;
// vec outFrag = vec4(texture(material.texture_diffused1, fs_in.TexCoord).rgb * dirLight.diffuse, 1.0f);
        // vec3 ViewDir = normalize(fs_in.tangentViewPos - fs_in.FragPos);
        // vec3 outFrag = CalcDirLight(dirLight, norm, ViewDir);
    // vec4 outFrag = vec4(TexCoord_, 0.0, 1.0f);
     // } else {
     // FragColorr = vec4(TexCoord_, 0.0, 1.0f);
//     }else{
//         if(fs_in.TexCoord == vec2(0.0f))
//         outFrag = vec4(0.5, 0.4, 0.3, 1.0f);

//         outFrag = vec4(0.7, 0.2, 0.1, 1.0f);
// }
// outFrag += vec4(texture(material.texture_specular1, fs_in.TexCoord).rgb, 1.0f); 
        // FragColorr = vec4(outFrag, 1.0f);
    FragColorr = vec4(outFrag, 1.0f);
      // FragColorr = vec4(0.3, 0.4, 0.5, 1.0f); 
};
