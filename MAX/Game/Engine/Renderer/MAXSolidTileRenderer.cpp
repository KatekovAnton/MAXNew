//
//  MAXSolidTileRenderer.cpp
//  MAX
//
//  Created by  Developer on 30.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXSolidTileRenderer.h"
#include "EngineTiledDynamicMesh.h"
#include "Shader.h"
#include "MAXContentLoader.h"

static int emptyValue = -1;

int MAXSolidTileRenderer::GetIndexForCoordinates(const int x, const int y) const
{
    return y * _mapW + x;
}

MAXSolidTileRenderer::MAXSolidTileRenderer(const int mapW, const int mapH)
:_mesh(NULL), _cells(new int[mapH * mapW]), _mapW(mapW), _mapH(mapH)
{
    memset(_cells, emptyValue, mapH * mapW * sizeof(int));
    EngineTiledDynamicMeshTextureInfo info;
    info.tileCountH = 1;
    info.tileCountW = 1;
    _mesh = new EngineTiledDynamicMesh(info, mapW, mapH);
    _mesh->_delegate_w = this;
}

MAXSolidTileRenderer::~MAXSolidTileRenderer()
{
    delete _mesh;
    delete[] _cells;
}

void MAXSolidTileRenderer::CompletlyFillMap()
{
    for (int i = 0; i < _mapH; i++) {
        for (int j = 0; j < _mapW; j++) {
            AddCell(j, i);
        }
    }
}

void MAXSolidTileRenderer::AddCell(const int x, const int y)
{
//    if (x==76 && y == 93) {
//        int a = 0;
//        a++;
//    }
    int mapIndex = GetIndexForCoordinates(x, y);
    if (_cells[mapIndex] != emptyValue)
        return;
    int inMeshIndex = _mesh->AddPolygon(x, y, 0, mapIndex);
    _cells[mapIndex] = inMeshIndex;
}

void MAXSolidTileRenderer::RemoveCell(const int x, const int y)
{
//    if (x==76 && y == 93) {
//        int a = 0;
//        a++;
//    }
    int index = GetIndexForCoordinates(x, y);
    int meshIndex = _cells[index];
    _mesh->RemovePolygon(meshIndex);
    _cells[index] = emptyValue;
}

void MAXSolidTileRenderer::Clear()
{
    memset(_cells, emptyValue, _mapH * _mapW * sizeof(int));
    _mesh->Clear();
}

void MAXSolidTileRenderer::Draw(Shader *shader)
{
    shader->SetVector4Value(UNIFORM_VECTOR1, reinterpret_cast<float*>(&color));
    _mesh->Draw();
}

#pragma mark - EngineTiledDynamicMeshDelegate

void MAXSolidTileRenderer::ElementDidChangePosition(int oldIndex, int newIndex, int singleArrayIndex)
{
//    if (singleArrayIndex == 10492) {
//        int a = 0;
//        a++;
//    }
    _cells[singleArrayIndex] = newIndex;
 //   _cells[oldIndex] = emptyValue;
}




