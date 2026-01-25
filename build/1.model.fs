#version 330 core

//Set the output color vector first

out vec4 FragColor;

uniform vec3 ViewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

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

uniform DirLight dirlight;

struct PointLight{
 // Inherent component
 vec3 position;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;
 float quadraticTerm;
};

#define NR_POINT_LIGHTS 2
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
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir);

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir){
  // Light direction, fragpos, norm
  vec3 ambient = light.ambient * texture(material.texture_diffused1, TexCoord).rgb;

  // This is represent the angle between lightDir and norm
  float diff = max(dot(-light.direction, norm), 0.0f);
  vec3 diffuse = light.diffuse * diff * texture(material.texture_diffused1, TexCoord).rgb;

  vec3 reflecDir  = reflect(-light.direction, norm);
  float spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);

  vec3 specular = light.specular * spec * texture(material.texture_diffused1, TexCoord).rgb;

  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir){

// Based on light direction(Light Postion), attenuation base on constant, linear and quadratic term, distance 1/(kc + kl * d + kq * d * d)
  // This is represent the angle between lightDir and norm

	//if fragpos is betweeen of viewPos and and lightPos
	// create line formed by viewPos and LightPos first
	 vec3 AB = ViewPos - light.position;//Light Dir
	 vec3 AP = FragPos - ViewPos;//View Dir

	// Check if cross product is close to zero vector (i.e., colinear)
	 bool colinear = length(cross(AB, AP)) < 1e-5f;

	// Optional: Check if P is between A and B
	 float dot1 = dot(AP, AB);
         float dot2 = dot(AB, AB);
	 bool onSegment = colinear && dot1 >= 0.0f && dot1 <= dot2;


	// Wrong at point light 
	 vec3 lightDir = onSegment?vec3(0.0):normalize(light.position - FragPos);


  //vec3 lightDir = normalize(light.position - FragPos);
  vec3 reflecDir  = reflect(-lightDir, norm);

  // Light direction, fragpos, norm
  vec3 ambient = light.ambient * texture(material.texture_diffused1, TexCoord).rgb;

  float diff = max(dot(lightDir, norm), 0.0f);
  vec3 diffuse = light.diffuse * diff * texture(material.texture_diffused1, TexCoord).rgb;

  // Wrong
  float spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * texture(material.texture_diffused1, TexCoord).rgb;


// Now the attenuation calculation
// This length is for calculating the distance
   float distance;
   float attenuation;
   distance = length(light.position - FragPos);
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
vec3 lightDir = normalize(light.position - FragPos);
//theta is always equal to 180 but why
float theta = dot(lightDir, normalize(-light.direction));


//if(theta > light.CutOff)
//{
// Light direction, fragpos, norm

   reflecDir  = reflect(-lightDir, norm);
   float distance = length(light.position - FragPos);

   ambient = light.ambient * texture(material.texture_diffused1, TexCoord).rgb;
   spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);
   specular = light.specular * spec * texture(material.texture_diffused1, TexCoord).rgb;
   diff = max(dot(norm, lightDir), 0.0f);
   diffuse = light.diffuse * diff * texture(material.texture_diffused1, TexCoord).rgb;

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
	vec3 norm = normalize(Normal);	// Turn this into unit vector

	// FOR POINT LIGHT
	// The direction from fragment to light source

	// when the light source is blocked how to simulate them
  	vec3 viewDir = normalize( ViewPos - FragPos);// work
	vec3 lightDir;
	vec3 reflectDir;

	// DIRECTIONAL
	// Light(No light source position)
	result = CalcDirLight(dirlight, norm, viewDir);

	// POINT
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{	
	    result += CalcPointLight(pointLights[i], norm, viewDir);
	}

	// Emission vec3 emission = texture(material.emissionMap, TexCoord).rgb;
	    //result += CalcPointLight(pointlight, norm, viewDir);
	// SPOT
	lightDir = normalize(spotlight.position - FragPos);// lightDir is the direction from flashlight to the fragment
// NOTHING wrong with the calcspotlight fx. The model messed something else up
	//result += CalcSpotLight(spotlight, norm, viewDir);

	FragColor = vec4(result, 1.0f);
}

