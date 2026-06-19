#version 330 core

in vec4 FragColorr;
out vec4 FragColor;

uniform float colorOffset;

//in vec3 ModelPos;
in vec2 TextCoord;

struct Material{
    sampler2D diffused_texture;
    sampler2D specular_texture;
    float shininess;
};

uniform Material material;

void main(){
    vec4 sampled = texture(material.diffused_texture, vec2(TextCoord.x, TextCoord.y));

        if(sampled != vec4(0.0f)){

            if(sampled.rg == vec2(0.0f)){
                discard;
            } 

            FragColor = vec4(sampled.r!=0.0f?1.0f:0.0f, sampled.r!=0.0f?1.0f:0.0f, sampled.r!=0.0f?1.0f:0.0f, 1.0f);
        }else{
            FragColor = FragColorr;
        }
        // FragColor = FragColorr;
        //* FragColorr;
}
