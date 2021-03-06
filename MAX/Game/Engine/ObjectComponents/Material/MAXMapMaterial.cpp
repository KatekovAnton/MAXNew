//
//  MAXMapMaterial.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXMapMaterial.h"
#include "MAXContentMap.h"
#include "Geometry.h"
#include "Texture.h"
#include "Shader.h"
#include "MAXContentLoader.h"
#include "Sys.h"

const double frameTime = 1.0/10.0;

MAXMapMaterial::MAXMapMaterial(shared_ptr<MAXContentMap> map)
{
//    int w = map->w;
    index = 0;
    time = 0;
    
    {
        const int w = 32;
        texW = w;
        int h = map->elementCount / w;
        if (w * h < map->elementCount)
            h++;
        texH = h;
        Color* singleArray = (Color*)malloc(w * h * 64 * 64 * 4);
        //по блокам
        for (int i = 0; i < map->elementCount; i++)
        {
            int x = i % w;
            int y = i / w;
            
            //каждый блок по строчке
            for (int j = 0; j < 64; j++)
            {
                //каждая строчка по пикселю
                for (int k = 0; k < 64; k++)
                {
                    singleArray[w * y * 64 * 64 + j * w * 64 + x * 64 + k].r = map->mapElements[i * 64 * 64 + j * 64 + k];
                }
            }
        }
        
        mapElementsSingle = new Texture(GL_NEAREST, (GLubyte*)singleArray, w*64, h*64);
    }
    MAXSCL->SetMapColorsToDefaultPalette(map->palette);
    palettes = MAXSCL->CreatePalletes(map->palette);
    currentPalette = palettes[0];
}

MAXMapMaterial::~MAXMapMaterial()
{
    for (int i = 0; i < palettes.size(); i++)
    {
        Texture* tex = palettes[i];
        delete tex;
    }
    delete mapElementsSingle;
}

void MAXMapMaterial::DoFrame(double elapsedTime)
{
    time = time + elapsedTime;
    int f = (time/frameTime);
    index = f%palettes.size();

    currentPalette = palettes[index];
}

void MAXMapMaterial::ApplyLod(int lod, Shader *shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mapElementsSingle->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
 
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, currentPalette->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE3], 1);
}

void MAXMapMaterial::SetFrame(int frame)
{}

void MAXMapMaterial::StopBackgroundLoading()
{}



