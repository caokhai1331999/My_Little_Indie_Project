/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Tile.h"


void DrawTile(std::string path, GLuint ShaderID){
    std::ifstream file(".map");
    std::string content;
    content.resize(fs::file_size("file.txt"));
    file.read(&content[0], content.size());
    Tile temptile = {};

    for(int i = 0; i<(int)file.size(), i++){
        temptile = content[i];
    };

    // Draw here
    debug_read_file_result* MapFile = DEBUGReadFileWhole("");
    useProgram(ShaderID);
    int i = 0;
    for(int y = 0; y < MapHeight; y++){
        for(int x = 0; x < MapWidth; x++)
        {
            // Set each Tile Matrix then Draw
            //glBindVertexArray()
            setMat4(MapFile->content[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            i++;
        }
    }
}
