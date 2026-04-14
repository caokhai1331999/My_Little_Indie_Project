#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoordd;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (location = 5) in ivec4 boneids;
layout (location = 6) in vec4 weights;

const int MAX_BONES = 52;
const int MAX_BONE_INFLUENCE = 4;

layout (std140) uniform finalBone{
 mat4 finalBoneMatrices [MAX_BONES];
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 WorldToCamera;

// uniform block
// uniform mat4[MAX_BONES] finalBoneMatrices;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord_;

void main()
{
     TexCoord_ = TexCoordd;
     Normal = vec3(0.0f);
    vec4 totalPosition = vec4(0.0f);
    vec4 totalPosition_ = vec4(0.0f);

mat4 totalBoneMatrices = mat4(1.0f);
mat4 localBone = mat4(1.0f);

   for (int i = 0; i < MAX_BONE_INFLUENCE; i++){

	 if(boneids[i] == -1 || boneids[i] >= MAX_BONES){
	   continue;
	 }

	// if(boneids[i] >= MAX_BONES){
	//    totalPosition = vec4(aPos, 1.0f);
	//    break;
	//  }

	 vec4 localPosition = finalBone.finalBoneMatrices[boneids[i]] * vec4(aPos, 1.0f);
	 // totalBoneMatrices *= finalBone.finalBoneMatrices[boneids[i]] * weights[i];
	 totalPosition += localPosition * weights[i];
	 // vec2 localTexCoord = mat2(finalBone.finalBoneMatrices[boneids[i]]) * weights[i] * TexCoordd;
	 vec3 localNormal = mat3(finalBone.finalBoneMatrices[boneids[i]]) * aNormal;
	 // totalPosition += finalBone.finalBoneMatrices[boneids[i]] * vec4(aPos, 1.0f) * weights[i];
	 // TexCoord_ += localTexCoord;
	 Normal += localNormal;
       }

   //     if(totalPosition.w == 0.0f){
   //     totalPosition = vec4(-aPos, 1.0f);
   //     }

// This one froze the shader here
// vec3 localNormal;
// vec4 localPosition = vec4(0.0f);

// if(boneids[0]!=-1){
// 	 localBone = finalBone.finalBoneMatrices[boneids[0]];
// 	 if(weights[0]>0.0f){
// 	 totalPosition += (localBone * vec4(aPos, 1.0f)) * weights[0];
// 	 }else{
// 	 totalPosition += mat4(1.0f) * vec4(aPos, 1.0f) * 1.0f;	 
// 	 }
// 	 localNormal = mat3(finalBone.finalBoneMatrices[boneids[0]]) * aNormal;
// 	 Normal += localNormal;
//    }

// if(boneids[1]!=-1){
// 	 localBone = finalBone.finalBoneMatrices[boneids[1]];
// 	 if(weights[1]>0.0f){
// 	 totalPosition += (localBone * vec4(aPos, 1.0f)) * weights[1];
// 	 }else{
// 	 totalPosition += mat4(1.0f) * vec4(aPos, 1.0f);
//  	 }
// 	 localNormal = mat3(finalBone.finalBoneMatrices[boneids[1]]) * aNormal;
// 	 Normal += localNormal;
// }

// if(boneids[2]!=-1){
// 	 localBone = finalBone.finalBoneMatrices[boneids[2]];
// 	 if(weights[2]!=0.0f){
// 	 totalPosition += (localBone * vec4(aPos, 1.0f)) * weights[2];
// 	 }else{
// 	 totalPosition += mat4(1.0f) * vec4(aPos, 1.0f);
//  	 }
// 	 localNormal = mat3(finalBone.finalBoneMatrices[boneids[2]]) * aNormal;
// 	 Normal += localNormal;
// }

// if(boneids[3]!=-1){
// 	localBone = finalBone.finalBoneMatrices[boneids[3]];
// 	 if(weights[3]!=0.0f){
// 	 totalPosition += (localBone * vec4(aPos, 1.0f)) * weights[3];
// 	 }else{
// 	 totalPosition += mat4(1.0f) * vec4(aPos, 1.0f);
//  	 }
// 	 localNormal = mat3(finalBone.finalBoneMatrices[boneids[3]]) * aNormal;
// 	 Normal += localNormal;
// }

totalPosition.y *= -1;
totalPosition.z *= -1;

//aPos is the vertex position so the Fragment position is
	//the dot product of model and vertex Position (plus two vector)
	// This is to create a world space coor of fragment

     // FragPos = vec3(model *vec4(aPos, 1.0f));
     // Normal = vec3(transpose(inverse(view * model))) * aNormal;
     // Actually the model matrix is what carried world's feature
      gl_Position = projection * WorldToCamera * totalPosition;     
      // gl_Position = projection * WorldToCamera * totalBoneMatrices * vec4(aPos, 1.0f);     
    // gl_Position = projection * view * model * vec4(-aPos, 1.0f);
};
