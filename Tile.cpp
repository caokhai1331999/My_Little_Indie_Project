/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Tile.h"


void DrawTile(std::string path){
    std::ifstream file(".map");
    std::string content;
    content.resize(fs::file_size("file.txt"));
    file.read(&content[0], content.size());
    Tile temptile = {};

    for(int i = 0; i<(int)file.size(), i++){
        temptile = content[i];
    };

    // Draw here
}
