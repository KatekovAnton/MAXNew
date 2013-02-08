//
//  MAXContetnLoader.cpp
//  MAX
//
//  Created by Anton Katekov on 26.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "MAXContetnLoader.h"
#include "MAXContentMap.h"
#include "BinaryReader.h"
#include "FileManger.h"
#include <string>
#include "Texture.h"
#include "MAXUnitMaterial.h"
#include "MAXUnitRenderObject.h"
#include "MAXUnitObject.h"
#include "Sys.h"
#include "EngineMesh.h"
#include "MAXUnitConfig.h"

using namespace std;

const int pal_size = 0x300;
const int max_width = 640;
const int max_height = 480;

Color default_palette[256] =
{
    {0x00, 0x00, 0x00, 0x00}, //0
    {0xff, 0x00, 0x00, 0xff}, //1
    {0x00, 0xff, 0x00, 0xff}, //2
    {0x00, 0x00, 0xff, 0xff}, //3
    {0xff, 0xff, 0x00, 0xff}, //4
    {0xff, 0xab, 0x00, 0xff}, //5
    {0x83, 0x83, 0xa3, 0xff}, //6
    {0xff, 0x47, 0x00, 0xff}, //7
    {0xff, 0xff, 0x93, 0xff}, //8
    
    {0xff, 0xff, 0xff, 0xff}, //9
    {0xff, 0xff, 0xff, 0xff}, //10
    {0xff, 0xff, 0xff, 0xff}, //11
    {0xff, 0xff, 0xff, 0xff}, //12
    
    {0xff, 0xff, 0xff, 0xff}, //13
    {0xff, 0xff, 0xff, 0xff}, //14
    {0xff, 0xff, 0xff, 0xff}, //15
    {0xff, 0xff, 0xff, 0xff}, //16
    
    {0xff, 0xff, 0xff, 0xff}, //17
    {0xff, 0xff, 0xff, 0xff}, //18
    {0xff, 0xff, 0xff, 0xff}, //19
    {0xff, 0xff, 0xff, 0xff}, //20
    
    {0xff, 0xff, 0xff, 0xff}, //21
    {0xff, 0xff, 0xff, 0xff}, //22
    {0xff, 0xff, 0xff, 0xff}, //23
    {0xff, 0xff, 0xff, 0xff}, //24
    
    {0xff, 0xff, 0xff, 0xff}, //25
    {0xff, 0xff, 0xff, 0xff}, //26
    {0xff, 0xff, 0xff, 0xff}, //27
    {0xff, 0xff, 0xff, 0xff}, //28
    {0xff, 0xff, 0xff, 0xff}, //29
    {0xff, 0xff, 0xff, 0xff}, //30
    
    {0x00, 0x00, 0x00, 0xff}, //31
    {0x83, 0xbb, 0x1b, 0xff}, //32
    {0x6f, 0xab, 0x0f, 0xff}, //33
    {0x5f, 0x9f, 0x0b, 0xff}, //34
    {0x4f, 0x93, 0x07, 0xff}, //35
    {0x43, 0x77, 0x07, 0xff}, //36
    {0x33, 0x5f, 0x07, 0xff}, //37
    {0x27, 0x43, 0x07, 0xff}, //38
    {0x1b, 0x2b, 0x07, 0xff}, //39
    {0xbb, 0xbb, 0x07, 0xff}, //40
    {0xb3, 0x87, 0x07, 0xff}, //41
    {0xab, 0x57, 0x07, 0xff}, //42
    {0xa3, 0x2f, 0x07, 0xff}, //43
    {0xff, 0xff, 0xff, 0xff}, //44
    {0x67, 0x07, 0x7b, 0xff}, //45
    {0x7b, 0x37, 0x07, 0xff}, //46
    {0x93, 0xbb, 0x0f, 0xff}, //47
    {0x6b, 0x9f, 0xbb, 0xff}, //48
    {0x47, 0x87, 0xab, 0xff}, //49
    {0x2f, 0x73, 0x97, 0xff}, //50
    {0x17, 0x63, 0x87, 0xff}, //51
    {0x0f, 0x4f, 0x6f, 0xff}, //52
    {0x0b, 0x3b, 0x57, 0xff}, //53
    {0x07, 0x2b, 0x43, 0xff}, //54
    {0x07, 0x1b, 0x2b, 0xff}, //55
    {0xbb, 0x7b, 0x57, 0xff}, //56
    {0xaf, 0x63, 0x37, 0xff}, //57
    {0xa3, 0x4f, 0x1b, 0xff}, //58
    {0x97, 0x3b, 0x07, 0xff}, //59
    {0x7b, 0x2f, 0x07, 0xff}, //60
    {0x63, 0x27, 0x07, 0xff}, //61
    {0x47, 0x1b, 0x07, 0xff}, //62
    {0x27, 0x0f, 0x07, 0xff}, //63
    {0x00, 0x00, 0x00, 0xff}, //64
    {0x00, 0x00, 0x00, 0xff}, //65
    {0x00, 0x00, 0x00, 0xff}, //66
    {0x00, 0x00, 0x00, 0xff}, //67
    {0x00, 0x00, 0x00, 0xff}, //68
    {0x00, 0x00, 0x00, 0xff}, //69
    {0x00, 0x00, 0x00, 0xff}, //70
    {0x00, 0x00, 0x00, 0xff}, //71
    {0x00, 0x00, 0x00, 0xff}, //72
    {0x00, 0x00, 0x00, 0xff}, //73
    {0x00, 0x00, 0x00, 0xff}, //74
    {0x00, 0x00, 0x00, 0xff}, //75
    {0x00, 0x00, 0x00, 0xff}, //76
    {0x00, 0x00, 0x00, 0xff}, //77
    {0x00, 0x00, 0x00, 0xff}, //78
    {0x00, 0x00, 0x00, 0xff}, //79
    {0x00, 0x00, 0x00, 0xff}, //80
    {0x00, 0x00, 0x00, 0xff}, //81
    {0x00, 0x00, 0x00, 0xff}, //82
    {0x00, 0x00, 0x00, 0xff}, //83
    {0x00, 0x00, 0x00, 0xff}, //84
    {0x00, 0x00, 0x00, 0xff}, //85
    {0x00, 0x00, 0x00, 0xff}, //86
    {0x00, 0x00, 0x00, 0xff}, //87
    {0x00, 0x00, 0x00, 0xff}, //88
    {0x00, 0x00, 0x00, 0xff}, //89
    {0x00, 0x00, 0x00, 0xff}, //90
    {0x00, 0x00, 0x00, 0xff}, //91
    {0x00, 0x00, 0x00, 0xff}, //92
    {0x00, 0x00, 0x00, 0xff}, //93
    {0x00, 0x00, 0x00, 0xff}, //94
    {0x00, 0x00, 0x00, 0xff}, //95
    {0x00, 0x00, 0x00, 0xff}, //96
    {0x00, 0x00, 0x00, 0xff}, //97
    {0x00, 0x00, 0x00, 0xff}, //98
    {0x00, 0x00, 0x00, 0xff}, //99
    {0x00, 0x00, 0x00, 0xff}, //100
    {0x00, 0x00, 0x00, 0xff}, //101
    {0x00, 0x00, 0x00, 0xff}, //102
    {0x00, 0x00, 0x00, 0xff}, //103
    {0x00, 0x00, 0x00, 0xff}, //104
    {0x00, 0x00, 0x00, 0xff}, //105
    {0x00, 0x00, 0x00, 0xff}, //106
    {0x00, 0x00, 0x00, 0xff}, //107
    {0x00, 0x00, 0x00, 0xff}, //108
    {0x00, 0x00, 0x00, 0xff}, //109
    {0x00, 0x00, 0x00, 0xff}, //110
    {0x00, 0x00, 0x00, 0xff}, //111
    {0x00, 0x00, 0x00, 0xff}, //112
    {0x00, 0x00, 0x00, 0xff}, //113
    {0x00, 0x00, 0x00, 0xff}, //114
    {0x00, 0x00, 0x00, 0xff}, //115
    {0x00, 0x00, 0x00, 0xff}, //116
    {0x00, 0x00, 0x00, 0xff}, //117
    {0x00, 0x00, 0x00, 0xff}, //118
    {0x00, 0x00, 0x00, 0xff}, //119
    {0x00, 0x00, 0x00, 0xff}, //120
    {0x00, 0x00, 0x00, 0xff}, //121
    {0x00, 0x00, 0x00, 0xff}, //122
    {0x00, 0x00, 0x00, 0xff}, //123
    {0x00, 0x00, 0x00, 0xff}, //124
    {0x00, 0x00, 0x00, 0xff}, //125
    {0x00, 0x00, 0x00, 0xff}, //126
    {0x00, 0x00, 0x00, 0xff}, //127
    {0x00, 0x00, 0x00, 0xff}, //128
    {0x00, 0x00, 0x00, 0xff}, //129
    {0x00, 0x00, 0x00, 0xff}, //130
    {0x00, 0x00, 0x00, 0xff}, //131
    {0x00, 0x00, 0x00, 0xff}, //132
    {0x00, 0x00, 0x00, 0xff}, //133
    {0x00, 0x00, 0x00, 0xff}, //134
    {0x00, 0x00, 0x00, 0xff}, //135
    {0x00, 0x00, 0x00, 0xff}, //136
    {0x00, 0x00, 0x00, 0xff}, //137
    {0x00, 0x00, 0x00, 0xff}, //138
    {0x00, 0x00, 0x00, 0xff}, //139
    {0x00, 0x00, 0x00, 0xff}, //140
    {0x00, 0x00, 0x00, 0xff}, //141
    {0x00, 0x00, 0x00, 0xff}, //142
    {0x00, 0x00, 0x00, 0xff}, //143
    {0x00, 0x00, 0x00, 0xff}, //144
    {0x00, 0x00, 0x00, 0xff}, //145
    {0x00, 0x00, 0x00, 0xff}, //146
    {0x00, 0x00, 0x00, 0xff}, //147
    {0x00, 0x00, 0x00, 0xff}, //148
    {0x00, 0x00, 0x00, 0xff}, //149
    {0x00, 0x00, 0x00, 0xff}, //150
    {0x00, 0x00, 0x00, 0xff}, //151
    {0x00, 0x00, 0x00, 0xff}, //152
    {0x00, 0x00, 0x00, 0xff}, //153
    {0x00, 0x00, 0x00, 0xff}, //154
    {0x00, 0x00, 0x00, 0xff}, //155
    {0x00, 0x00, 0x00, 0xff}, //156
    {0x00, 0x00, 0x00, 0xff}, //157
    {0x00, 0x00, 0x00, 0xff}, //158
    {0x00, 0x00, 0x00, 0xff}, //159
    {0xff, 0xfb, 0xf7, 0xff}, //160
    {0xf3, 0xdf, 0xd3, 0xff}, //161
    {0xf3, 0xdb, 0xbb, 0xff}, //162
    {0xdf, 0xc7, 0xaf, 0xff}, //163
    {0xdf, 0xc3, 0x9b, 0xff}, //164
    {0xdb, 0xb7, 0x8f, 0xff}, //165
    {0xc7, 0xa7, 0x7f, 0xff}, //166
    {0xb7, 0xa3, 0x83, 0xff}, //167
    {0xab, 0x9b, 0x7b, 0xff}, //168
    {0x9f, 0x97, 0x8b, 0xff}, //169
    {0xaf, 0xa7, 0x93, 0xff}, //170
    {0xbf, 0xab, 0x97, 0xff}, //171
    {0xc7, 0xbb, 0xaf, 0xff}, //172
    {0xcf, 0xa3, 0x6b, 0xff}, //173
    {0xbf, 0x9b, 0x67, 0xff}, //174
    {0xab, 0x8b, 0x5f, 0xff}, //175
    {0xa3, 0x8b, 0x6b, 0xff}, //176
    {0x9b, 0x87, 0x63, 0xff}, //177
    {0x93, 0x87, 0x73, 0xff}, //178
    {0x83, 0x7f, 0x77, 0xff}, //179
    {0x7b, 0x73, 0x67, 0xff}, //180
    {0x83, 0x73, 0x5b, 0xff}, //181
    {0x8b, 0x7b, 0x63, 0xff}, //182
    {0x93, 0x77, 0x53, 0xff}, //183
    {0x9f, 0x7f, 0x4b, 0xff}, //184
    {0xab, 0x83, 0x4b, 0xff}, //185
    {0xb3, 0x8b, 0x53, 0xff}, //186
    {0xc3, 0x93, 0x53, 0xff}, //187
    {0xc7, 0x8b, 0x43, 0xff}, //188
    {0xb3, 0x7f, 0x3b, 0xff}, //189
    {0xa7, 0x73, 0x37, 0xff}, //190
    {0x93, 0x6f, 0x3b, 0xff}, //191
    {0x83, 0x6b, 0x3b, 0xff}, //192
    {0x7b, 0x63, 0x47, 0xff}, //193
    {0x73, 0x63, 0x3b, 0xff}, //194
    {0x73, 0x57, 0x2b, 0xff}, //195
    {0x67, 0x53, 0x2f, 0xff}, //196
    {0x5b, 0x4f, 0x3b, 0xff}, //197
    {0x53, 0x47, 0x33, 0xff}, //198
    {0x53, 0x3f, 0x2b, 0xff}, //199
    {0x4b, 0x3b, 0x27, 0xff}, //200
    {0x43, 0x3b, 0x2b, 0xff}, //201
    {0x3b, 0x33, 0x27, 0xff}, //202
    {0x33, 0x2b, 0x1f, 0xff}, //203
    {0x2b, 0x27, 0x23, 0xff}, //204
    {0x27, 0x23, 0x1f, 0xff}, //205
    {0x1f, 0x1b, 0x17, 0xff}, //206
    {0x0f, 0x0f, 0x0f, 0xff}, //207
    {0x37, 0x1f, 0x1f, 0xff}, //208
    {0x2f, 0x2b, 0x2b, 0xff}, //209
    {0x37, 0x33, 0x33, 0xff}, //210
    {0x3f, 0x3b, 0x3b, 0xff}, //211
    {0x4b, 0x47, 0x47, 0xff}, //212
    {0x57, 0x53, 0x53, 0xff}, //213
    {0x5f, 0x5b, 0x5b, 0xff}, //214
    {0x67, 0x63, 0x63, 0xff}, //215
    {0x6f, 0x6b, 0x6b, 0xff}, //216
    {0x73, 0x67, 0x53, 0xff}, //217
    {0x6b, 0x5f, 0x4b, 0xff}, //218
    {0x63, 0x57, 0x43, 0xff}, //219
    {0x57, 0x43, 0x23, 0xff}, //220
    {0x4b, 0x2b, 0x2b, 0xff}, //221
    {0x2f, 0x2b, 0x3b, 0xff}, //222
    {0x83, 0x63, 0x2b, 0xff}, //223
    {0x83, 0x6b, 0x4b, 0xff}, //224
    {0xcf, 0x83, 0x6b, 0xff}, //225
    {0xab, 0x6f, 0x5b, 0xff}, //226
    {0xbb, 0x53, 0x37, 0xff}, //227
    {0x7b, 0x4f, 0x43, 0xff}, //228
    {0x9b, 0x3f, 0x2f, 0xff}, //229
    {0x73, 0x27, 0x23, 0xff}, //230
    {0x4b, 0x1f, 0x17, 0xff}, //231
    {0x1f, 0x0f, 0x0f, 0xff}, //232
    {0x8b, 0xab, 0x63, 0xff}, //233
    {0x73, 0x93, 0x4f, 0xff}, //234
    {0x57, 0x93, 0x3b, 0xff}, //235
    {0x5f, 0x73, 0x43, 0xff}, //236
    {0x43, 0x6b, 0x2f, 0xff}, //237
    {0x3b, 0x53, 0x23, 0xff}, //238
    {0x2b, 0x43, 0x1b, 0xff}, //239
    {0x17, 0x1b, 0x0f, 0xff}, //240
    {0x77, 0x6f, 0x9f, 0xff}, //241
    {0x63, 0x57, 0x83, 0xff}, //242
    {0x3b, 0x43, 0x8b, 0xff}, //243
    {0x43, 0x43, 0x6b, 0xff}, //244
    {0x2f, 0x33, 0x6b, 0xff}, //245
    {0x43, 0x3b, 0x4f, 0xff}, //246
    {0x1f, 0x23, 0x4b, 0xff}, //247
    {0x0f, 0x13, 0x2b, 0xff}, //248
    {0xb7, 0x67, 0x00, 0xff}, //249
    {0x87, 0x4b, 0x00, 0xff}, //250
    {0x5b, 0x33, 0x00, 0xff}, //251
    {0x9b, 0x9b, 0x00, 0xff}, //252
    {0x6f, 0x6f, 0x00, 0xff}, //253
    {0x43, 0x43, 0x00, 0xff}, //254
    {0xff, 0xff, 0xff, 0xff}, //255
};

//for i:=32 to 39 do
//
//это коды цветов в палитре
//
//вот код переопределения
//
//for n:=0 to mg_game.plr_cnt-1 do for i:=32 to 39 do
//palpx[n][i]:=gnearestpc(tcrgb(round(mg_game.plr[n].color[2]*((40-i)/8*200)/255),
//                              round(mg_game.plr[n].color[1]*((40-i)/8*200)/255),
//                              round(mg_game.plr[n].color[0]*((40-i)/8*200)/255)));
//
//for i:=0 to 255 do al_palpx[i]:=i;
//for i:=32 to 39 do al_palpx[i]:=gnearestpc(tcrgb(round(128*((40-i)/8*200)/255),
//                                                 round(128*((40-i)/8*200)/255),
//                                                 round(128*((40-i)/8*200)/255)));

MAXContentLoader* _sharedContentLoader = nullptr;

MAXContentLoader::MAXContentLoader()
{
    inf = new BinaryReader("Max.res");
    inf->ReadBuffer(12, (char*)&hdr);
    inf->SetPosition(hdr.diroffset);
    
    dir = new typdiritem[hdr.dirlength / 16];
    for (int f = 0; f < hdr.dirlength / 16; f++)
    {
        inf->ReadBuffer(8, dir[f].name);
        dir[f].offset = inf->ReadInt();
        dir[f].size = inf->ReadInt();
        //SysLogInfo("%s", dir[f].name);
    }
    loadedData = new void*[hdr.dirlength / 16];
    memset(loadedData, 0, hdr.dirlength / 4);
    
    
    GLubyte* currentPalette = (GLubyte*)malloc(4 * pal_size/3);
    memcpy(currentPalette, &defaultPalette, 4 * pal_size/3);
    defaultPalette = new Texture(GL_LINEAR, (GLubyte*)currentPalette, pal_size/3, 1);
    unitMesh = EngineMesh::CreateUnitQuad();
    
}

MAXContentLoader::~MAXContentLoader()
{
    delete inf;
    delete []dir;
    delete []loadedData;
}

MAXContentLoader* MAXContentLoader::SharedLoader()
{
    if (!_sharedContentLoader)
        _sharedContentLoader = new MAXContentLoader();
    return _sharedContentLoader;
}

void MAXContentLoader::palshiftu(Color* pal, int s, int e)
{
    Color cl = pal[e];
    for (int i = e; i >= s + 1; i--)
        pal[i] = pal[i - 1];
    pal[s] = cl;
}

void MAXContentLoader::palshiftd(Color* pal, int s, int e)
{
    Color cl = pal[s];
    for (int i = s; i <= e - 1; i++)
        pal[i] = pal[i + 1];
    pal[e] = cl;
}

void MAXContentLoader::animatePalette(Color* thepal)
{
    palshiftd(thepal, 9, 12);//3
    palshiftu(thepal, 13, 16);//3
    palshiftu(thepal, 17, 20);//3
    palshiftu(thepal, 21, 24);//3
    
    palshiftu(thepal, 25, 30);//5
    //palblnkd(thepal, 31, 1 - frac(gct), gclgreen);
    
    palshiftu(thepal, 96, 102);//6
    palshiftu(thepal, 103, 109);//6
    palshiftu(thepal, 110, 116);//6
    palshiftu(thepal, 117, 122);//5
    palshiftu(thepal, 123, 127);//4
}

shared_ptr<MAXContentMap> MAXContentLoader::LoadMapWithName(string name)
{
    BinaryReader* br = new BinaryReader(name);
    if(!br->isValid())
		return nullptr;
    shared_ptr<MAXContentMap> result = shared_ptr<MAXContentMap>(new MAXContentMap());
    result->LoadFromStream(br);
    
    delete br;
    
    return result;
}

vector<Texture*> MAXContentLoader::CreatePalletes(Color* palette)
{
    Color* colors = new Color[pal_size/3];
    memcpy(colors, palette, 4 * pal_size/3);
    
    vector<Texture*> result;
    for(int i = 0;i<30;i++)
    {
        GLubyte* currentPalette = (GLubyte*)malloc(4 * pal_size/3);
        memcpy(currentPalette, colors, 4 * pal_size/3);
        result.push_back(new Texture(GL_LINEAR, (GLubyte*)currentPalette, pal_size/3, 1));
        if (i != 29)
            animatePalette(colors);
    }
    
    delete colors;
    return result;
}

Texture* MAXContentLoader::TextureIdexedFromIndex(int w, int h, unsigned char* indexes)
{
    Color* colors = (Color*)malloc(w * h * 4);
    memset(colors, 0x00000000, w * h * 4);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            unsigned char colornumber = indexes[i * w + j];
            colors[i * w + j].r = colornumber;
        }
    
    Texture *result = new Texture(GL_NEAREST, (GLubyte*)colors, w, h);
    return result;
}

Texture* MAXContentLoader::TextureFromIndexAndPalette(int w, int h, unsigned char* indexes, unsigned char* palette)
{
    Color* colors = (Color*)malloc(w * h * 4);
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            int colornumber = indexes[j * w + i];
            colors[j * w + i].r = palette[colornumber * 3];
            colors[j * w + i].g = palette[colornumber * 3 + 1];
            colors[j * w + i].b = palette[colornumber * 3 + 2];
            colors[j * w + i].a = 1.0;
        }
    
    return new Texture(GL_NEAREST, (GLubyte*)colors, w, h);
}

Texture* MAXContentLoader::TextureFromIndexAndDefaultPalette(int w, int h, unsigned char* indexes)
{
    Color* colors = (Color*)malloc(w * h * 4);
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            int colornumber = indexes[j * w + i];
            colors[j * w + i] = default_palette[colornumber];
        }
    
    return new Texture(GL_NEAREST, (GLubyte*)colors, w, h);
}

Texture* MAXContentLoader::TexturePalleteFormDefaultPalleteAndPlayerColor(const Color& color)
{
    GLubyte* currentPalette = (GLubyte*)malloc(4 * pal_size/3);
    memcpy(currentPalette, &default_palette, 4 * pal_size/3);
    Texture* result = new Texture(GL_LINEAR, (GLubyte*)currentPalette, pal_size/3, 1);
    return result;
}

int MAXContentLoader::FindImage(string name)
{
    int index = -1;
    for (int i = 0; i < hdr.dirlength / 16; i++)
    {
        bool suit = true;
        char* cname = dir[i].name;
        for(int j = 0; j < name.length(); j++)
        {
            if (name[j] != cname[j])
            {
                suit = false;
                break;
            }
        }
        if (suit)
        {
            index = i;
            break;
        }
    }
    
    return index;
}

void MAXContentLoader::LoadUnitFrame(BinaryReader* source, int index, MAXUnitMaterial* target, long baseOffset)
{    
    ushort width = source->ReadUInt16();
    ushort height = source->ReadUInt16();
    short center_x = source->ReadInt16();
    short center_y = source->ReadInt16();
    
    if (index == 15 + 8) {
//        SysLogInfo("width = %d, height=%d, center_x=%d, center_y=%d", (int)width, (int)height, (int)center_x, (int)center_y);
    }
  //  
    
    target->frames[index].center.x = center_x;
    target->frames[index].center.y = center_y;
    target->frames[index].size.x = width;
    target->frames[index].size.y = height;
    
    //MAXUnitMaterialFrame frame = target->frames[index];
    int size = width * height;
    if (size == 0)
        return;
    
    unsigned char* pixels = (unsigned char*)malloc(size);
    memset(pixels, 0, size);
    // Rows offsets.
    unsigned int* rows = new unsigned int[height];
    source->ReadBuffer(height * 4, (char *)rows);
    
    int destOffset = 0;
    
    
    unsigned char buf;
    char tmpbuffer[256];
    memset(tmpbuffer, 0, 256);
    for (int i = 0; i < height; i++)
    {
        unsigned int rowi = rows[i];
        source->SetPosition(rowi + baseOffset);
        buf = source->ReadUChar();
        while (buf != 0xff)
        {
            destOffset += buf;
            buf = source->ReadUChar();
            source->ReadBuffer((int)buf, tmpbuffer);
            memcpy(pixels + destOffset, tmpbuffer, buf);
            destOffset+=buf;
            memset(tmpbuffer, 0, 256);
            buf = source->ReadUChar();
        }
        
        int new_pos = (i + 1) * width;
        destOffset = new_pos;
    }
    
    Texture* result = TextureIdexedFromIndex((int)width, (int)height, pixels);
    target->textures[index] = result;
    delete [] rows;
}

void MAXContentLoader::LoadUnitFrameWithShadow(BinaryReader* source, BinaryReader* shadowSource, int index, MAXUnitMaterial* target, long baseOffset, long shadowBaseOffset)
{
    ushort width = source->ReadUInt16();
    ushort height = source->ReadUInt16();
    short center_x = source->ReadInt16();
    short center_y = source->ReadInt16();
    
    if (index == 15 + 8) {
        //        SysLogInfo("width = %d, height=%d, center_x=%d, center_y=%d", (int)width, (int)height, (int)center_x, (int)center_y);
    }
    //
    
    target->frames[index].center.x = center_x;
    target->frames[index].center.y = center_y;
    target->frames[index].size.x = width;
    target->frames[index].size.y = height;
    
    //MAXUnitMaterialFrame frame = target->frames[index];
    int size = width * height;
    if (size == 0)
        return;
    
    unsigned char* pixels = (unsigned char*)malloc(size);
    memset(pixels, 0, size);
    // Rows offsets.
    unsigned int* rows = new unsigned int[height];
    source->ReadBuffer(height * 4, (char *)rows);
    
    int destOffset = 0;
    
    
    unsigned char buf;
    char tmpbuffer[256];
    memset(tmpbuffer, 0, 256);
    for (int i = 0; i < height; i++)
    {
        unsigned int rowi = rows[i];
        source->SetPosition(rowi + baseOffset);
        buf = source->ReadUChar();
        while (buf != 0xff)
        {
            destOffset += (int)buf;
            buf = source->ReadUChar();
            source->ReadBuffer((int)buf, tmpbuffer);
            memcpy(pixels + destOffset, tmpbuffer, buf);
            destOffset+=buf;
            memset(tmpbuffer, 0, 256);
            buf = source->ReadUChar();
        }
        
        int new_pos = (i + 1) * width;
        destOffset = new_pos;
    }
    
    Texture* result = TextureIdexedFromIndex((int)width, (int)height, pixels);
    target->textures[index] = result;
    delete [] rows;
}

MAXUnitMaterial* MAXContentLoader::LoadUnitMaterial(string name, string shadowName)
{
    int index = FindImage(name);
    void* cashed = loadedData[index];
    if(cashed)
        return (MAXUnitMaterial*)cashed;
    
    MAXUnitMaterial* result  = new MAXUnitMaterial();
    
    int shadowIndex = FindImage(shadowName);
    char *data;
    char *shadowData;
    
    inf->SetPosition(dir[index].offset);
    data = (char*)malloc(dir[index].size);
    inf->ReadBuffer(dir[index].size, data);
    
    inf->SetPosition(dir[shadowIndex].offset);
    shadowData = (char*)malloc(dir[shadowIndex].size);
    inf->ReadBuffer(dir[shadowIndex].size, shadowData);
    
    BinaryReader* dataReader = new BinaryReader(data, dir[index].size);
    BinaryReader* shadowDataReader = new BinaryReader(shadowData, dir[shadowIndex].size);
    long baseOffset = 0;//inf->GetPosition();
    long shadowBaseOffset = 0;//inf->GetPosition();
    
    short picCount = dataReader->ReadInt16();
    short shadowPicCount = shadowDataReader->ReadInt16();
    int* picbounds = new int[picCount];
    int* shadowPicbounds = new int[shadowPicCount];
    dataReader->ReadBuffer(picCount*4, (char*) picbounds);
    shadowDataReader->ReadBuffer(shadowPicCount*4, (char*) shadowPicbounds);
    
    result ->SetImagesCount(picCount);
    for (int picIndex = 0; picIndex < picCount; picIndex++)
    {
        if(picIndex<8)
        {
            dataReader->SetPosition(picbounds[picIndex] + baseOffset);
            shadowDataReader->SetPosition(shadowPicbounds[picIndex] + shadowBaseOffset);
            LoadUnitFrameWithShadow(dataReader, shadowDataReader, picIndex, result, baseOffset, shadowBaseOffset);
        }
        else
        {
            dataReader->SetPosition(picbounds[picIndex] + baseOffset);
            LoadUnitFrame(dataReader, picIndex, result, baseOffset);
        }
    }
    loadedData[index] = (void*)result;
    
    //TODO:replace it to use one texure per player not per unit
    Color unitColor = {1,0,0,1};
    result->pallete = TexturePalleteFormDefaultPalleteAndPlayerColor(unitColor);
    delete []picbounds;
    delete []shadowPicbounds;
    delete dataReader;
    delete shadowDataReader;
    free(data);
    free(shadowData);
    
    return result;
}

/*
 
 s.cnt:=picCount;
 setlength(s.sprc,picCount);
 result:=true;
 for picIndex:=0 to picCount-1 do begin
 picbegin:=buf[2+picIndex*4] + 256 * buf[3+picIndex*4] +65536 * (buf[4+picIndex*4] + 256 * buf[5+picIndex*4]);
 picEnd:=length(buf);
 
 for X:=0 to High(picbounds) do if (picEnd > picbounds[X]) and (picbegin < picbounds[X]) then picEnd:=picbounds[X];
 picWidth:=buf[picbegin] + 256 * buf[picbegin+1];
 picHeight:=buf[picbegin+2] + 256 * buf[picbegin+3];
 picHX:=smallint(buf[picbegin+4] + 256 * buf[picbegin+5]);
 picHY:=smallint(buf[picbegin+6] + 256 * buf[picbegin+7]);
 s.sprc[picIndex].tp:=3;
 s.sprc[picIndex].xs:=picWidth;
 s.sprc[picIndex].ys:=picHeight;
 s.sprc[picIndex].cx:=picHX;
 s.sprc[picIndex].cy:=picHY;
 s.sprc[picIndex].ldd:=true;
 s.sprc[picIndex].ltyp:=0;
 getmem(s.sprc[picIndex].srf,picWidth*picHeight);
 fillchar(s.sprc[picIndex].srf^,picWidth*picHeight,0);
 //getmem(mMaxImages[picIndex].Alpha, picWidth*picHeight);
 //FillChar(mMaxImages[picIndex].Alpha[0], picWidth*picHeight, 255);
 Setlength(picRows,picHeight);
 if picbegin+8+length(picRows)*4 > picEnd then begin Result:=False;break;end;
 Move(buf[picbegin+8], picRows[0], length(picRows)*4);
 for Y:=0 to High(picRows) do begin
 Color:=0;
 X:=0;
 Opacity:=255;
 blockIndex:=0;
 while (picRows[Y]+blockIndex < picEnd) and(buf[picRows[Y]+blockIndex]<>$FF) do begin
 if Y*picWidth + X + buf[picRows[Y]+blockIndex] >picWidth*picHeight then begin Result:=False;break;end;
 
 FillChar(pbytea(s.sprc[picIndex].srf)[Y*picWidth + X],buf[picRows[Y]+blockIndex], Color);
 //FillChar(mMaxImages[picIndex].Alpha[Y*picWidth + X],mbuffer[picRows[Y]+blockIndex], Opacity);
 Color:=1-Color;
 Opacity:=255-Opacity;
 Inc(X,buf[picRows[Y]+blockIndex]);
 Inc(blockIndex);
 end;
 end;
 if not Result then break;
 end;
 if Result then begin
 //FillChar(mPalette[0], 3, 255);
 //FillChar(mPalette[1], 3, 0);
 end;
 picRows:=nil;
 picbounds:=nil;
 end;
 */

#pragma mark - fabric

shared_ptr<MAXUnitObject> MAXContentLoader::CreateUnit(MAXUnitConfig* unitConfig)
{
    MAXUnitMaterial *material = MAXSCL->LoadUnitMaterial(unitConfig->_bodyName, unitConfig->_shadowName);
    MAXUnitRenderObject *renderObject = new MAXUnitRenderObject(unitMesh);
    shared_ptr<MAXUnitObject>result = shared_ptr<MAXUnitObject>(new MAXUnitObject(renderObject, material, unitConfig));
    return result;
}



