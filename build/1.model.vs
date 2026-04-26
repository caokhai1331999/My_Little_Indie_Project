#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in ivec4 boneids;
layout (location = 6) in vec4 weights;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{

    gl_Position = projection * view * model * vec4(-aPos, 1.0f);

	//aPos is the vertex position so the Fragment position is
	//the dot product of model and vertex Position (plus two vector)
	// This is to create a world space coor of fragment

     FragPos = vec3(model *vec4(aPos, 1.0f));
     Normal = vec3(transpose(inverse(view * model))) * aNormal;

     TexCoord = aTexCoord;
}
