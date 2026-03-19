#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in ivec4 boneids;
layout (location = 6) in vec4 weights;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
//vertex shader run every vertex
void main()
{
//aPos is the vertex position so the Fragment position is
//the dot product of model and vertex Position (plus two vector)
// This is to create a world space coor of fragment

   vec4 totalPosition = vec4(0.0f);
   vec3 localNormal;

     for (int i = 0; i < MAX_BONE_INFLUENCE; i++){
	 if(boneids[i] == -1)
	   continue;
	 if(boneids[i] >= MAX_BONES){
	   totalPosition = vec4(pos, 1.0f);
	   break;
	 }
         vec4 localPosition = finalBoneMatrices[boneids[i]]*vec4(pos,1.0f);
	 totalPosition += localPosition * weights[i];
	 localNormal = mat3(finalBoneMatrices[boneids[i]]) * norm;
	 Normal += localNormal;
       }

	 // vec3 localNormal = vec3(finalBoneMatrices[boneids[0]] * model)* norm; 
	 // localNormal += vec3(finalBoneMatrices[boneids[1]] * model)* norm; 
	 // localNormal += vec3(finalBoneMatrices[boneids[2]] * model)* norm; 
	 // localNormal += vec3(finalBoneMatrices[boneids[3]] * model)* norm; 

	 // mat4 boneTransform;

	 // for (int i = 0; i < MAX_BONE_INFLUENCE; i++){
     	 // if(boneids[i] == -1)
	 //   continue;
	 // if(boneids[i] <= MAX_BONES){
	 //   boneTransform += finalBoneMatrices[boneids[i]] * weights[i];
	 //   break;
	 // }

	 // vec3 localNormal = vec3(finalBoneMatrices[boneids[i]] * model)* norm;
	 //For What???
       // }
//Second method

	 // mat4 boneTransform = finalBoneMatrices[boneids[0]] * weights[0];
 	 // boneTransform += finalBoneMatrices[boneids[1]] * weights[1];
 	 // boneTransform += finalBoneMatrices[boneids[2]] * weights[2];
 	 // boneTransform += finalBoneMatrices[boneids[3]] * weights[3];

	 // mat3 normalMatrix = transpose(inverse(mat3(boneTransform)));
	 // vec3 T = normalize(normalMatrix * tangent);
	 // vec3 B = normalize(normalMatrix * bitangent);
	 // vec3 N = normalize(normalMatrix * norm);

         // vec4 localPosition = finalBoneMatrices[boneids[0]]*vec4(pos,1.0f);
	 // totalPosition += localPosition * weights[0];
	 // Normal = mat3(finalBoneMatrices[boneids[0]]) * norm;

         // localPosition = finalBoneMatrices[boneids[1]]*vec4(pos,1.0f);
	 // totalPosition += localPosition * weights[1];
	 // Normal += mat3(finalBoneMatrices[boneids[1]]) * norm;

	 // localPosition = finalBoneMatrices[boneids[2]]*vec4(pos,1.0f);
	 // totalPosition += localPosition * weights[2];
	 // Normal += mat3(finalBoneMatrices[boneids[2]]) * norm;

	 // localPosition = finalBoneMatrices[boneids[3]]*vec4(pos,1.0f);
	 // totalPosition += localPosition * weights[3];
	 // Normal += mat3(finalBoneMatrices[boneids[3]]) * norm;

     // local postion, total position, weights,
     vec4 modelPos_after= boneTransform * vec4(-pos, 1.0f);

     //mat4 view_after = model * modelPos_after;
     //gl_Position = World_after_transform * viewModel;

     //2nd method
     gl_Position = projection * view * model * modelPos_after;
	// gl_Position = projection * view * model * vec4(pos, 1.0f);
	//   /window2d space//clip space//world space//local space/
	TexCoords = tex;
     	// FragPos = vec3(model *vec4(pos, 1.0f));
        FragPos = vec3(model * totalPosition);
}
