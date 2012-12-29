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

using namespace std;

const int pal_size = 0x300;
const int max_width = 640;
const int max_height = 480;

Color default_palette[256] =
{
    {0x00, 0x00, 0x00, 0xff}, //0
    {0xff, 0x00, 0x00, 0x00}, //1
    {0x00, 0xff, 0x00, 0x00}, //2
    {0x00, 0x00, 0xff, 0x00}, //3
    {0xff, 0xff, 0x00, 0x00}, //4
    {0xff, 0xab, 0x00, 0x00}, //5
    {0x83, 0x83, 0xa3, 0x00}, //6
    {0xff, 0x47, 0x00, 0x00}, //7
    {0xff, 0xff, 0x93, 0x00}, //8
    
    {0xff, 0xff, 0xff, 0x00}, //9
    {0xff, 0xff, 0xff, 0x00}, //10
    {0xff, 0xff, 0xff, 0x00}, //11
    {0xff, 0xff, 0xff, 0x00}, //12
    
    {0xff, 0xff, 0xff, 0x00}, //13
    {0xff, 0xff, 0xff, 0x00}, //14
    {0xff, 0xff, 0xff, 0x00}, //15
    {0xff, 0xff, 0xff, 0x00}, //16
    
    {0xff, 0xff, 0xff, 0x00}, //17
    {0xff, 0xff, 0xff, 0x00}, //18
    {0xff, 0xff, 0xff, 0x00}, //19
    {0xff, 0xff, 0xff, 0x00}, //20
    
    {0xff, 0xff, 0xff, 0x00}, //21
    {0xff, 0xff, 0xff, 0x00}, //22
    {0xff, 0xff, 0xff, 0x00}, //23
    {0xff, 0xff, 0xff, 0x00}, //24
    
    {0xff, 0xff, 0xff, 0x00}, //25
    {0xff, 0xff, 0xff, 0x00}, //26
    {0xff, 0xff, 0xff, 0x00}, //27
    {0xff, 0xff, 0xff, 0x00}, //28
    {0xff, 0xff, 0xff, 0x00}, //29
    {0xff, 0xff, 0xff, 0x00}, //30
    
    {0x00, 0x00, 0x00, 0x00}, //31
    {0x83, 0xbb, 0x1b, 0x00}, //32
    {0x6f, 0xab, 0x0f, 0x00}, //33
    {0x5f, 0x9f, 0x0b, 0x00}, //34
    {0x4f, 0x93, 0x07, 0x00}, //35
    {0x43, 0x77, 0x07, 0x00}, //36
    {0x33, 0x5f, 0x07, 0x00}, //37
    {0x27, 0x43, 0x07, 0x00}, //38
    {0x1b, 0x2b, 0x07, 0x00}, //39
    {0xbb, 0xbb, 0x07, 0x00}, //40
    {0xb3, 0x87, 0x07, 0x00}, //41
    {0xab, 0x57, 0x07, 0x00}, //42
    {0xa3, 0x2f, 0x07, 0x00}, //43
    {0xff, 0xff, 0xff, 0x00}, //44
    {0x67, 0x07, 0x7b, 0x00}, //45
    {0x7b, 0x37, 0x07, 0x00}, //46
    {0x93, 0xbb, 0x0f, 0x00}, //47
    {0x6b, 0x9f, 0xbb, 0x00}, //48
    {0x47, 0x87, 0xab, 0x00}, //49
    {0x2f, 0x73, 0x97, 0x00}, //50
    {0x17, 0x63, 0x87, 0x00}, //51
    {0x0f, 0x4f, 0x6f, 0x00}, //52
    {0x0b, 0x3b, 0x57, 0x00}, //53
    {0x07, 0x2b, 0x43, 0x00}, //54
    {0x07, 0x1b, 0x2b, 0x00}, //55
    {0xbb, 0x7b, 0x57, 0x00}, //56
    {0xaf, 0x63, 0x37, 0x00}, //57
    {0xa3, 0x4f, 0x1b, 0x00}, //58
    {0x97, 0x3b, 0x07, 0x00}, //59
    {0x7b, 0x2f, 0x07, 0x00}, //60
    {0x63, 0x27, 0x07, 0x00}, //61
    {0x47, 0x1b, 0x07, 0x00}, //62
    {0x27, 0x0f, 0x07, 0x00}, //63
    {0x00, 0x00, 0x00, 0x00}, //64
    {0x00, 0x00, 0x00, 0x00}, //65
    {0x00, 0x00, 0x00, 0x00}, //66
    {0x00, 0x00, 0x00, 0x00}, //67
    {0x00, 0x00, 0x00, 0x00}, //68
    {0x00, 0x00, 0x00, 0x00}, //69
    {0x00, 0x00, 0x00, 0x00}, //70
    {0x00, 0x00, 0x00, 0x00}, //71
    {0x00, 0x00, 0x00, 0x00}, //72
    {0x00, 0x00, 0x00, 0x00}, //73
    {0x00, 0x00, 0x00, 0x00}, //74
    {0x00, 0x00, 0x00, 0x00}, //75
    {0x00, 0x00, 0x00, 0x00}, //76
    {0x00, 0x00, 0x00, 0x00}, //77
    {0x00, 0x00, 0x00, 0x00}, //78
    {0x00, 0x00, 0x00, 0x00}, //79
    {0x00, 0x00, 0x00, 0x00}, //80
    {0x00, 0x00, 0x00, 0x00}, //81
    {0x00, 0x00, 0x00, 0x00}, //82
    {0x00, 0x00, 0x00, 0x00}, //83
    {0x00, 0x00, 0x00, 0x00}, //84
    {0x00, 0x00, 0x00, 0x00}, //85
    {0x00, 0x00, 0x00, 0x00}, //86
    {0x00, 0x00, 0x00, 0x00}, //87
    {0x00, 0x00, 0x00, 0x00}, //88
    {0x00, 0x00, 0x00, 0x00}, //89
    {0x00, 0x00, 0x00, 0x00}, //90
    {0x00, 0x00, 0x00, 0x00}, //91
    {0x00, 0x00, 0x00, 0x00}, //92
    {0x00, 0x00, 0x00, 0x00}, //93
    {0x00, 0x00, 0x00, 0x00}, //94
    {0x00, 0x00, 0x00, 0x00}, //95
    {0x00, 0x00, 0x00, 0x00}, //96
    {0x00, 0x00, 0x00, 0x00}, //97
    {0x00, 0x00, 0x00, 0x00}, //98
    {0x00, 0x00, 0x00, 0x00}, //99
    {0x00, 0x00, 0x00, 0x00}, //100
    {0x00, 0x00, 0x00, 0x00}, //101
    {0x00, 0x00, 0x00, 0x00}, //102
    {0x00, 0x00, 0x00, 0x00}, //103
    {0x00, 0x00, 0x00, 0x00}, //104
    {0x00, 0x00, 0x00, 0x00}, //105
    {0x00, 0x00, 0x00, 0x00}, //106
    {0x00, 0x00, 0x00, 0x00}, //107
    {0x00, 0x00, 0x00, 0x00}, //108
    {0x00, 0x00, 0x00, 0x00}, //109
    {0x00, 0x00, 0x00, 0x00}, //110
    {0x00, 0x00, 0x00, 0x00}, //111
    {0x00, 0x00, 0x00, 0x00}, //112
    {0x00, 0x00, 0x00, 0x00}, //113
    {0x00, 0x00, 0x00, 0x00}, //114
    {0x00, 0x00, 0x00, 0x00}, //115
    {0x00, 0x00, 0x00, 0x00}, //116
    {0x00, 0x00, 0x00, 0x00}, //117
    {0x00, 0x00, 0x00, 0x00}, //118
    {0x00, 0x00, 0x00, 0x00}, //119
    {0x00, 0x00, 0x00, 0x00}, //120
    {0x00, 0x00, 0x00, 0x00}, //121
    {0x00, 0x00, 0x00, 0x00}, //122
    {0x00, 0x00, 0x00, 0x00}, //123
    {0x00, 0x00, 0x00, 0x00}, //124
    {0x00, 0x00, 0x00, 0x00}, //125
    {0x00, 0x00, 0x00, 0x00}, //126
    {0x00, 0x00, 0x00, 0x00}, //127
    {0x00, 0x00, 0x00, 0x00}, //128
    {0x00, 0x00, 0x00, 0x00}, //129
    {0x00, 0x00, 0x00, 0x00}, //130
    {0x00, 0x00, 0x00, 0x00}, //131
    {0x00, 0x00, 0x00, 0x00}, //132
    {0x00, 0x00, 0x00, 0x00}, //133
    {0x00, 0x00, 0x00, 0x00}, //134
    {0x00, 0x00, 0x00, 0x00}, //135
    {0x00, 0x00, 0x00, 0x00}, //136
    {0x00, 0x00, 0x00, 0x00}, //137
    {0x00, 0x00, 0x00, 0x00}, //138
    {0x00, 0x00, 0x00, 0x00}, //139
    {0x00, 0x00, 0x00, 0x00}, //140
    {0x00, 0x00, 0x00, 0x00}, //141
    {0x00, 0x00, 0x00, 0x00}, //142
    {0x00, 0x00, 0x00, 0x00}, //143
    {0x00, 0x00, 0x00, 0x00}, //144
    {0x00, 0x00, 0x00, 0x00}, //145
    {0x00, 0x00, 0x00, 0x00}, //146
    {0x00, 0x00, 0x00, 0x00}, //147
    {0x00, 0x00, 0x00, 0x00}, //148
    {0x00, 0x00, 0x00, 0x00}, //149
    {0x00, 0x00, 0x00, 0x00}, //150
    {0x00, 0x00, 0x00, 0x00}, //151
    {0x00, 0x00, 0x00, 0x00}, //152
    {0x00, 0x00, 0x00, 0x00}, //153
    {0x00, 0x00, 0x00, 0x00}, //154
    {0x00, 0x00, 0x00, 0x00}, //155
    {0x00, 0x00, 0x00, 0x00}, //156
    {0x00, 0x00, 0x00, 0x00}, //157
    {0x00, 0x00, 0x00, 0x00}, //158
    {0x00, 0x00, 0x00, 0x00}, //159
    {0xff, 0xfb, 0xf7, 0x00}, //160
    {0xf3, 0xdf, 0xd3, 0x00}, //161
    {0xf3, 0xdb, 0xbb, 0x00}, //162
    {0xdf, 0xc7, 0xaf, 0x00}, //163
    {0xdf, 0xc3, 0x9b, 0x00}, //164
    {0xdb, 0xb7, 0x8f, 0x00}, //165
    {0xc7, 0xa7, 0x7f, 0x00}, //166
    {0xb7, 0xa3, 0x83, 0x00}, //167
    {0xab, 0x9b, 0x7b, 0x00}, //168
    {0x9f, 0x97, 0x8b, 0x00}, //169
    {0xaf, 0xa7, 0x93, 0x00}, //170
    {0xbf, 0xab, 0x97, 0x00}, //171
    {0xc7, 0xbb, 0xaf, 0x00}, //172
    {0xcf, 0xa3, 0x6b, 0x00}, //173
    {0xbf, 0x9b, 0x67, 0x00}, //174
    {0xab, 0x8b, 0x5f, 0x00}, //175
    {0xa3, 0x8b, 0x6b, 0x00}, //176
    {0x9b, 0x87, 0x63, 0x00}, //177
    {0x93, 0x87, 0x73, 0x00}, //178
    {0x83, 0x7f, 0x77, 0x00}, //179
    {0x7b, 0x73, 0x67, 0x00}, //180
    {0x83, 0x73, 0x5b, 0x00}, //181
    {0x8b, 0x7b, 0x63, 0x00}, //182
    {0x93, 0x77, 0x53, 0x00}, //183
    {0x9f, 0x7f, 0x4b, 0x00}, //184
    {0xab, 0x83, 0x4b, 0x00}, //185
    {0xb3, 0x8b, 0x53, 0x00}, //186
    {0xc3, 0x93, 0x53, 0x00}, //187
    {0xc7, 0x8b, 0x43, 0x00}, //188
    {0xb3, 0x7f, 0x3b, 0x00}, //189
    {0xa7, 0x73, 0x37, 0x00}, //190
    {0x93, 0x6f, 0x3b, 0x00}, //191
    {0x83, 0x6b, 0x3b, 0x00}, //192
    {0x7b, 0x63, 0x47, 0x00}, //193
    {0x73, 0x63, 0x3b, 0x00}, //194
    {0x73, 0x57, 0x2b, 0x00}, //195
    {0x67, 0x53, 0x2f, 0x00}, //196
    {0x5b, 0x4f, 0x3b, 0x00}, //197
    {0x53, 0x47, 0x33, 0x00}, //198
    {0x53, 0x3f, 0x2b, 0x00}, //199
    {0x4b, 0x3b, 0x27, 0x00}, //200
    {0x43, 0x3b, 0x2b, 0x00}, //201
    {0x3b, 0x33, 0x27, 0x00}, //202
    {0x33, 0x2b, 0x1f, 0x00}, //203
    {0x2b, 0x27, 0x23, 0x00}, //204
    {0x27, 0x23, 0x1f, 0x00}, //205
    {0x1f, 0x1b, 0x17, 0x00}, //206
    {0x0f, 0x0f, 0x0f, 0x00}, //207
    {0x37, 0x1f, 0x1f, 0x00}, //208
    {0x2f, 0x2b, 0x2b, 0x00}, //209
    {0x37, 0x33, 0x33, 0x00}, //210
    {0x3f, 0x3b, 0x3b, 0x00}, //211
    {0x4b, 0x47, 0x47, 0x00}, //212
    {0x57, 0x53, 0x53, 0x00}, //213
    {0x5f, 0x5b, 0x5b, 0x00}, //214
    {0x67, 0x63, 0x63, 0x00}, //215
    {0x6f, 0x6b, 0x6b, 0x00}, //216
    {0x73, 0x67, 0x53, 0x00}, //217
    {0x6b, 0x5f, 0x4b, 0x00}, //218
    {0x63, 0x57, 0x43, 0x00}, //219
    {0x57, 0x43, 0x23, 0x00}, //220
    {0x4b, 0x2b, 0x2b, 0x00}, //221
    {0x2f, 0x2b, 0x3b, 0x00}, //222
    {0x83, 0x63, 0x2b, 0x00}, //223
    {0x83, 0x6b, 0x4b, 0x00}, //224
    {0xcf, 0x83, 0x6b, 0x00}, //225
    {0xab, 0x6f, 0x5b, 0x00}, //226
    {0xbb, 0x53, 0x37, 0x00}, //227
    {0x7b, 0x4f, 0x43, 0x00}, //228
    {0x9b, 0x3f, 0x2f, 0x00}, //229
    {0x73, 0x27, 0x23, 0x00}, //230
    {0x4b, 0x1f, 0x17, 0x00}, //231
    {0x1f, 0x0f, 0x0f, 0x00}, //232
    {0x8b, 0xab, 0x63, 0x00}, //233
    {0x73, 0x93, 0x4f, 0x00}, //234
    {0x57, 0x93, 0x3b, 0x00}, //235
    {0x5f, 0x73, 0x43, 0x00}, //236
    {0x43, 0x6b, 0x2f, 0x00}, //237
    {0x3b, 0x53, 0x23, 0x00}, //238
    {0x2b, 0x43, 0x1b, 0x00}, //239
    {0x17, 0x1b, 0x0f, 0x00}, //240
    {0x77, 0x6f, 0x9f, 0x00}, //241
    {0x63, 0x57, 0x83, 0x00}, //242
    {0x3b, 0x43, 0x8b, 0x00}, //243
    {0x43, 0x43, 0x6b, 0x00}, //244
    {0x2f, 0x33, 0x6b, 0x00}, //245
    {0x43, 0x3b, 0x4f, 0x00}, //246
    {0x1f, 0x23, 0x4b, 0x00}, //247
    {0x0f, 0x13, 0x2b, 0x00}, //248
    {0xb7, 0x67, 0x00, 0x00}, //249
    {0x87, 0x4b, 0x00, 0x00}, //250
    {0x5b, 0x33, 0x00, 0x00}, //251
    {0x9b, 0x9b, 0x00, 0x00}, //252
    {0x6f, 0x6f, 0x00, 0x00}, //253
    {0x43, 0x43, 0x00, 0x00}, //254
    {0xff, 0xff, 0xff, 0x00}, //255
};

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
    
    shared_ptr<MAXContentMap> result = shared_ptr<MAXContentMap>(new MAXContentMap());
    result->LoadFromStream(br);
    
    delete br;
    
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

int MAXContentLoader::FindImage(string name)
{
    int index = -1;
    for (int i = 0; i < hdr.dirlength / 16; i++)
    {
        bool suit = true;
        char* cname = dir[i].name;
        for(int j = 0; j < name.length(); j++)
        {
            if (name[j] != cname[j]) {
                suit = false;
                break;
            }
        }
        if (suit) {
            index = i;
            break;
        }
    }
    
    return index;
}

void MAXContentLoader::LoadFrame(BinaryReader* source, int index, MAXUnitMaterial* target, long baseOffset)
{    
    ushort width = source->ReadUInt16();
    ushort height = source->ReadUInt16();
    short center_x = source->ReadInt16();
    short center_y = source->ReadInt16();
    
    target->frames[index].centerDelta.x = center_x;
    target->frames[index].centerDelta.y = center_y;
    target->frames[index].rect.size.width = width;
    target->frames[index].rect.size.height = height;
    
    
    int size = width * height;
    if (size == 0)
        return;
    
    unsigned char* pixels = (unsigned char*)malloc(size);
    
    // Rows offsets.
    unsigned int* rows = new unsigned int[height];
    source->ReadBuffer(height * 4, (char *)rows);
    
    //BinaryReader* dest = new BinaryReader(pixels);
    int destOffset = 0;
    
    
    unsigned char buf;
    char tmpbuffer[256];
    for (int i = 0; i < height; i++)
    {
        source->SetPosition(rows[i] + baseOffset);
        buf = source->ReadChar();
        while (buf != 0xff)
        {
            destOffset += buf;
            buf = source->ReadUChar();
            source->ReadBuffer(buf, tmpbuffer);
            memcpy(pixels + destOffset, tmpbuffer, buf);
            
            buf = source->ReadUChar();
        }
        
        int new_pos = (i + 1) * width;
        destOffset = new_pos;
    }
    
    Texture* result = TextureFromIndexAndDefaultPalette((int)width, (int)height, pixels);
    target->textures[index] = result;
    delete [] rows;
}

MAXUnitMaterial* MAXContentLoader::LoadUnitMaterial(string name)
{
    int index = FindImage(name);
    void* cashed = loadedData[index];
    if(cashed)
        return (MAXUnitMaterial*)cashed;
    inf->SetPosition(dir[index].offset);

    
    long baseOffset = inf->GetPosition();
    short picCount = inf->ReadInt16();
    
    int* picbounds = new int[picCount];
    inf->ReadBuffer(picCount*4, (char*) picbounds);
    
    MAXUnitMaterial* result = new MAXUnitMaterial(picCount);
    for (int picIndex = 0; picIndex < picCount; picIndex++)
    {
        inf->SetPosition(picbounds[picIndex] + baseOffset);
        LoadFrame(inf, picIndex, result, baseOffset);
    }
    loadedData[index] = (void*)result;
    
    delete []picbounds;
    return result;
}

#pragma mark - fabric

shared_ptr<MAXUnitObject> MAXContentLoader::CreateUnit(string bodyName)
{
   
    MAXUnitMaterial *material = MAXSCL->LoadUnitMaterial(bodyName);
    MAXUnitRenderObject *renderObject = new MAXUnitRenderObject(unitMesh);
    
    return shared_ptr<MAXUnitObject>(new MAXUnitObject(renderObject, material));
}



