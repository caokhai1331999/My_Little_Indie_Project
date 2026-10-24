#version 330 core

in vec4 FragColorr;
in vec2 GlyphSpecs;
in vec4 GlyphPos_;
out vec4 FragColor;

uniform float colorOffset;

//in vec3 ModelPos;
in vec2 TextCoord;

struct Material{
    usampler2D diffused_texture;
    usampler2D specular_texture;
    float shininess;
};
// This one automatically bound by the glBindTexture
uniform Material material;
uniform sampler2D diffused_texture_;
 void main(){
     // vec4 sampled = texture(material.diffused_texture, vec2(TextCoord.x, TextCoord.y));
     vec4 sampled = texture(diffused_texture_, vec2(TextCoord.x, TextCoord.y));
    // vec4 sampled = texture(material.diffused_texture, vec2(TextCoord.x > 0.0f?GlyphPos_.x + GlyphPos_.w: 0.0f, TextCoord.y > 0.0f?GlyphPos_.y + GlyphPos_.z : 0.0f));
    
    if(sampled != vec4(0.0f, 0.0f, 0.0f, 0.0f)){
        // if(sampled.a == 0.0f){
        /*
          if(sampled.r == 0.0f){
          discard;
          }else{                 
          // FragColor = vec4(sampled.a, sampled.a, sampled.a/2, sampled.a/3);
          }
         */
        FragColor = vec4(sampled.rgb, 1.0f);
        }else{
            FragColor = FragColorr;
        }
        // FragColor = FragColorr;
}







