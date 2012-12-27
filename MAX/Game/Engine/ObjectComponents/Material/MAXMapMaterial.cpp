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
#include "MAXContetnLoader.h"
#include "Sys.h"

const double frameTime = 1.0/10.0;

MAXMapMaterial::MAXMapMaterial(shared_ptr<MAXContentMap> map)
{
    int w = map->w;
    index = 0;
    GLubyte* colors = (GLubyte*)malloc(map->w * map->h * 4);
    for (int i = 0; i < map->h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            short value = map->map[i*w + j];
            GLubyte starshi = (GLubyte)((float)value/255.0);
            GLubyte mladshi = (GLubyte)(value - (short)starshi * 255.0);
            colors[(i * w + j) * 4] = starshi;
            colors[(i * w + j) * 4 + 1] = mladshi;
        }
    }
    
    fullMapTexture = new Texture(GL_NEAREST, colors, map->w, map->h);
    
    {
        int w = 20;
        texW = 20;
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
    
    palettes = MAXSCL->CreatePalletes(map->palette);
    currentPalette = palettes[0];
}

MAXMapMaterial::~MAXMapMaterial()
{
    for (int i = 0; i < palettes.size(); i++) {
        Texture* tex = palettes[i];
        delete tex;
    }
    delete fullMapTexture;
    delete mapElementsSingle;
}

void MAXMapMaterial::DoFrame(double elapsedTime)
{
    time += elapsedTime;
    if (time > frameTime)
    {
        time -= frameTime;
        index++;
        if(index == palettes.size())
            index = 0;
    }
    currentPalette = palettes[index];
    
}

void MAXMapMaterial::ApplyLod(int lod, Shader *shader)
{
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fullMapTexture->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE], 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, currentPalette->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE3], 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mapElementsSingle->GetTextureName());
    glUniform1i(shader->GetShaderUniforms()[UNIFORM_COLOR_TEXTURE1], 2);
    
}

void MAXMapMaterial::SetFrame(int frame)
{}

void MAXMapMaterial::StopBackgroundLoading()
{}