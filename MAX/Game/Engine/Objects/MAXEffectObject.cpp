//
//  MAXEffectObject.cpp
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXEffectObject.h"
#include "MAXUnitMaterial.h"
#include "MAXUnitRenderObject.h"
#include "Shader.h"
#include "MAXEngine.h"

MAXEffectObject::MAXEffectObject(MAXUnitRenderObject *renderObject, MAXUnitMaterial *material, MAXObjectConfig* config)
:MAXObject(config)
{
    
}

void MAXEffectObject::SetEffectFrame(int frame)
{}