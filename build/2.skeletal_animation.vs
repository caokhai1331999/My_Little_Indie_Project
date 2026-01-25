#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

//out vec3 Normal;
//out vec3 FragPos;
out vec2 TexCoords;

void main()
{
//aPos is the vertex position so the Fragment position is
//the dot product of model and vertex Position (plus two vector)
// This is to create a world space coor of fragment

   vec4 totalPosition = vec4(0.0f);

/*
     for (int i = 0; i < MAX_BONE_INFLUENCE; i++){
     	 if(boneIds[i] == -1)
	   continue;
	 if(boneIds[i] >= MAX_BONES){
	   totalPosition = vec4(Pos, 1.0f);
	   break;
	 }
         vec4 localPosition = finalBoneMatrices[boneIds[i]]*vec4(pos,1.0f);
	 totalPostion += localPosition * weights[i];
	 vec3 localNormal = mat3(finalBoneMatrices[boneIds[i]]) * norm;
       }
*/


//Second method
	vec4 boneTransform = (finalBoneMatrices[boneIds[0]] * vec4(pos, 1.0f)) * weights[0];

     for (int i = 0; i < MAX_BONE_INFLUENCE; i++){
     	 if(boneIds[i] == -1)
	   continue;
	 if(boneIds[i] >= MAX_BONES){
	   boneTransform += (finalBoneMatrices[boneIds[i]] * vec4(pos, 1.0f)) * weights[i];
	   break;
	 }
	 //vec3 localNormal += mat3(finalBoneMatrices[boneIds[i]]) * norm; //For What???
       }

     // local postion, total position, weights, 
     mat4 viewModel = view * model;

     //gl_Position = projection * viewModel * totalPosition;

     //2nd method
     gl_Position = projection * viewModel * boneTransform;
     TexCoords = tex;
}
