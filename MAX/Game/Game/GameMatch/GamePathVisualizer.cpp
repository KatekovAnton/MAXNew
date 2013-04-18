//
//  GamePathVisualizer.cpp
//  MAX
//
//  Created by Anton Katekov on 18.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GamePathVisualizer.h"
#include "GameEffect.h"
#include "MAXContentLoader.h"

GameEffect* GamePathVisualizer::CreateEffectWithImage(int imageType)
{
    return NULL;
}

GamePathVisualizer::GamePathVisualizer()
{
}

GamePathVisualizer::~GamePathVisualizer()
{
    Clear();
}

void GamePathVisualizer::VisualizePath(vector<PathElement> elements)
{
    Clear();
    _elements = elements;
    for (int i = 0; i < _elements.size(); i++)
    {
        PathElement element = elements[i];
        GameEffect* effect = GameEffect::CreatePathArrow(element.image % 8, true, element.unitLevel);
        effect->SetLocation(ccp(element.x, element.y));
        _views.push_back(effect);
        effect->Show();
    }
}

void GamePathVisualizer::Clear()
{
    for (int i = 0; i < _views.size(); i++)
    {
        GameEffect* effect = _views[i];
        effect->Hide();
        delete effect;
    }
    _elements.clear();
    _views.clear();
}

void GamePathVisualizer::RemoveFirstItem()
{
    if (!_views.size() > 0) 
        return;
    GameEffect* e = _views[0];
    e->Hide();
    delete e;
    _elements.erase(_elements.begin());
    _views.erase(_views.begin());
}