#version 330 core

layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aNormal;
layout(location = 1) in vec2 aTextCoord;
//color per vetex (one point)
layout(location = 3) in vec3 VertexColor;

out vec2 TextCoord;
out vec4 FragColorr;
out vec4 GlyphPos_;

// uniform vec4 GlyphPoses[20];
uniform vec4 GlyphPos;
// uniform mat4 GlyphPos;
uniform mat4 projection;
// uniform mat4 model;

bool notNUll(mat4 matrix){
 for(int i = 0; i < 4; i++){
 for(int j = 0 ; j < 4; j++){
      if(matrix[i][j] != 0.0){
        return true;
     };     
   }
 }
 return false;
}

void main(){
    // so the model is the culprit;
    // projection and view made this wrong.
    // how to just use the orthorgraphy with unchange view
    // map to its size actually
    // vec4 GlyphPos = GlyphPoses[gl_InstanceID];
    gl_Position = projection * vec4(aPos.x > 0.0f?((GlyphPos.x) + GlyphPos.w):(GlyphPos.x), (aPos.y > 0.0f)?((GlyphPos.y) + GlyphPos.z):(GlyphPos.y), 0.0f, 1.0f);
    
    // gl_Position = projection * vec4(aPos.xy, 0.0f, 1.0f);
    // GlypSize = GlyphPos.zw;
    // gl_Position = projection * vec4(Pos.x * aPos.x, Pos.y * aPos.y, 0.0f, 1.0f);

    GlyphPos_ = GlyphPos;
    TextCoord = aTextCoord;
    FragColorr = vec4(VertexColor,1.0f);
}
