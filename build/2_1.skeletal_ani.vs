#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Norm;
layout (location = 2) in vec2 Tex;
layout (location = 5) in ivec4 BoneIDs;
layout (location = 6) in vec4 Weights;

