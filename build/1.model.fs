#version 330 core

#define NR_POINT_LIGHTS 2
//Set the output color vector first

out vec4 FragColor;

in vec3 Normal;

in VS_OUT{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 tangent_light_pos;
    vec3 tangentViewPos;
    vec3 tangentpointLight_Pos[NR_POINT_LIGHTS];
}fs_in;

// in vec3 FragPos;
// in vec3 Normal;
// in vec2 TexCoord;

struct light_in_general{
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
};

uniform light_in_general light;

struct Material{

// Ambient will be the same even with change in input
   sampler2D texture_ambient1;
   sampler2D texture_diffused1;
   sampler2D texture_specular1;
   sampler2D texture_normal1;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float shininess;
};

uniform Material material;

struct DirLight{
 // Inherent component
 vec3 direction;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

uniform DirLight dirLight;

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

uniform PointLight pointLights [NR_POINT_LIGHTS];
//uniform PointLight pointlight;

struct SpotLight{
 // Inherent component
 vec3 direction;
 vec3 position;

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

uniform SpotLight spotlight;

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 tangent_light_pos, vec3 norm, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir);

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
  vec3 ambient = light.ambient * texture(material.texture_ambient1, fs_in.TexCoord).rgb;

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
vec3 lightDir = normalize(light.position - fs_in.FragPos);
//theta is always equal to 180 but why
float theta = dot(lightDir, normalize(-light.direction));


//if(theta > light.CutOff)
//{
// Light direction, fragpos, norm

   reflecDir  = reflect(-lightDir, norm);
   float distance = length(light.position - fs_in.FragPos);

   ambient = light.ambient * texture(material.texture_diffused1, fs_in.TexCoord).rgb;
   spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);
   specular = light.specular * spec * texture(material.texture_diffused1, fs_in.TexCoord).rgb;
   diff = max(dot(norm, lightDir), 0.0f);
   diffuse = light.diffuse * diff * texture(material.texture_diffused1, fs_in.TexCoord).rgb;

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

void main()
{	 
	//=========================================================
	// We plus results of each light type in the order : directional light, pointlight and spotlight
	vec3 result;

	// norm is a vector represent the angle between light ray and fragment
	//vec3 norm = normalize(Normal);	// Turn this into unit vector
	vec3 normal_ = texture(material.texture_normal1, fs_in.TexCoord).rgb;	// Turn this into unit vector
    // This one is to tranform to tangent space
    vec3 norm = normalize(normal_ * 2.0 - 1.0);//

    // if(norm == vec3(0.0f, 0.0f, 0.0f)){
    //     norm = Normal;
    // }
    
	// FOR POINT LIGHT
	// The direction from fragment to light source

	// when the light source is blocked how to simulate them
  	vec3 viewDir = normalize( fs_in.tangentViewPos - fs_in.FragPos);// work

	//vec3 lightDir;
	//vec3 reflectDir;

	// DIRECTIONAL
	// Light(No light source position)
	result = CalcDirLight(dirLight, norm, viewDir);

	// POINT
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{	
        result += CalcPointLight(pointLights[i], fs_in.tangentpointLight_Pos[i], norm, viewDir);
	};

	// Emission vec3 emission = texture(material.emissionMap, fs_in.TexCoord).rgb;
	    //result += CalcPointLight(pointlight, norm, viewDir);
	// SPOT
	//lightDir = normalize(spotlight.position - fs_in.FragPos);// lightDir is the direction from flashlight to the fragment
// NOTHING wrong with the calcspotlight fx. The model messed something else up
	//result += CalcSpotLight(spotlight, norm, viewDir);

	 FragColor = vec4(result, 1.0f);
	//FragColor = vec4(texture(material.texture_diffused1, fs_in.TexCoord).rgb, 1.0f); 
}

