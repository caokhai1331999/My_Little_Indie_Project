#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in ivec4 boneids;
layout (location = 6) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{
   vec4 totalPosition = vec4(0.0f);

for (int i = 0; i < MAX_BONE_INFLUENCE; i++){
	 if(boneids[i] == -1)
	   continue;
	 if(boneids[i] >= MAX_BONES){
	   totalPosition = vec4(-aPos, 1.0f);
	   break;
	 }
         vec4 localPosition = finalBoneMatrices[boneids[i]]*vec4(-aPos, 1.0f);
	 totalPosition += localPosition * weights[i];
	 vec3 localNormal = mat3(finalBoneMatrices[boneids[i]]) * aNormal;
	 Normal += localNormal;
       }
         vec4 localPosition = finalBoneMatrices[boneids[0]]*vec4(-aPos, 1.0f);
	 totalPosition += localPosition * weights[0];
	 vec3 localNormal = mat3(finalBoneMatrices[boneids[0]]) * aNormal;
	 Normal += localNormal;

	 localPosition = finalBoneMatrices[boneids[1]]*vec4(-aPos, 1.0f);
	 totalPosition += localPosition * weights[1];
	 localNormal = mat3(finalBoneMatrices[boneids[1]]) * aNormal;
	 Normal += localNormal;

	 localPosition = finalBoneMatrices[boneids[2]]*vec4(-aPos, 1.0f);
	 totalPosition += localPosition * weights[2];
	 localNormal = mat3(finalBoneMatrices[boneids[2]]) * aNormal;
	 Normal += localNormal;

	 localPosition = finalBoneMatrices[boneids[3]]*vec4(-aPos, 1.0f);
	 totalPosition += localPosition * weights[3];
	 localNormal = mat3(finalBoneMatrices[boneids[3]]) * aNormal;
	 Normal += localNormal;


// vec4 LocalPosition = finalBoneMatrices[boneids[0]]*vec4(aPos, 1.0f);
// vec4 TotalPosition = LocalPosition * weights[0];

 // gl_Position = projection * view * model * totalPosition;
 gl_Position = projection * view * model * vec4(-aPos, 1.0f);

	//aPos is the vertex position so the Fragment position is
	//the dot product of model and vertex Position (plus two vector)
	// This is to create a world space coor of fragment

     FragPos = vec3(model *vec4(aPos, 1.0f));
     Normal = vec3(transpose(inverse(view * model))) * aNormal;

     TexCoord = aTexCoord;
};
