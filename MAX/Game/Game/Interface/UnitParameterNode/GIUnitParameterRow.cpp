//
//  GIUnitParameterRow.cpp
//  MAX
//
//  Created by  Developer on 17.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIUnitParameterRow.h"
#include "GameInterfacePrefix.h"
#include "StringUtils.h"
#include "CocosHelper.h"
#include "MAXContentLoader.h"
#include "Display.h"

GIUnitParameterRow::GIUnitParameterRow()
:_labelDiscr(NULL), _labelValue(NULL), _imageFull(NULL), _value(6), _maxValue(6)
{
    string text = intToString(_value) + "/" + intToString(_maxValue);
    _labelValue = CCLabelTTF::create(text.c_str(), MAX_DEFAULT_FONT, 9);
    _labelValue->setAnchorPoint(ccp(0, 0));
    
    CCSize sz = _labelValue->getContentSize();
    float scale = Display::currentDisplay()->GetDisplayScale();
    _labelValue->setPosition(ccp(19, 3+ (scale-2)));
    
    _labelValue->setColor(this->colorForLabel());
    addChild(_labelValue);
    
    _lineSprite = CCSprite::create("parameterSeparator.png");
    _lineSprite->setScaleX(12);
    _lineSprite->setScaleY(0.25);
    _lineSprite->setPosition(ccp(30, 14));
    addChild(_lineSprite);
}

GIUnitParameterRow::~GIUnitParameterRow()
{}

ccColor3B GIUnitParameterRow::colorForLabel() const
{
    if(_value <= _maxValue/4)
        return MAX_COLOR_RED;
    if(_value <= _maxValue/2)
        return MAX_COLOR_YELLOW;
    return MAX_COLOR_WHITE;
}

void GIUnitParameterRow::SetImageForParameterType(UNIT_PARAMETER_TYPE type, int subtype)//subtype is 0 1 2
{
    string name = "";
    string text = "";
    
    switch (type) {
        case UNIT_PARAMETER_TYPE_SPEED:
        {name = MAX_IMAGE_SPEED; text = "SPD";}break;
        case UNIT_PARAMETER_TYPE_HEALTH:
        {
             text = "HLTH";
            switch (subtype) {
                case 0:
                    name = MAX_IMAGE_HITSB;
                    break;
                case 1:
                    name = MAX_IMAGE_HITSY;
                    break;
                case 2:
                    name = MAX_IMAGE_HITSR;
                    break;
                default:
                    break;
            }
        }break;
        case UNIT_PARAMETER_TYPE_SHOTS:
        {name = MAX_IMAGE_SHOTS; text = "SHOT";}break;
        case UNIT_PARAMETER_TYPE_AMMO:
        {name = MAX_IMAGE_AMMO; text = "AMMO";}break;
            
            //storage
        case UNIT_PARAMETER_TYPE_GAS:
        case UNIT_PARAMETER_TYPE_FUEL:
        case UNIT_PARAMETER_TYPE_FUEL_BASE:
        {name = MAX_IMAGE_FUEL; text = "FUEL";}break;
        case UNIT_PARAMETER_TYPE_GOLD:
        case UNIT_PARAMETER_TYPE_GOLD_BASE:
        {name = MAX_IMAGE_GOLD; text = "GOLD";}break;
        case UNIT_PARAMETER_TYPE_MATERIAL:
        case UNIT_PARAMETER_TYPE_MATERIAL_BASE:
        {name = MAX_IMAGE_RAWMAT; text = "MATR";}break;
        case UNIT_PARAMETER_TYPE_PEOPLE:
        case UNIT_PARAMETER_TYPE_PEOPLE_BASE:
        {name = MAX_IMAGE_WORKERS; text = "WORK";}break;
        case UNIT_PARAMETER_TYPE_ENERGY:
        case UNIT_PARAMETER_TYPE_ENERGY_BASE:
        {name = MAX_IMAGE_POWER; text = "POWR";}break;
            
            //caro
        case UNIT_PARAMETER_TYPE_CARGO_UNITS:
        {name = MAX_IMAGE_LANDUNITS; text = "UNIT";}break;
        case UNIT_PARAMETER_TYPE_CARGO_PLANES:
        {name = MAX_IMAGE_AIRUNITS; text = "UNIT";}break;
        case UNIT_PARAMETER_TYPE_CARGO_SHIPS:
        {name = MAX_IMAGE_SEAUNITS; text = "UNIT";}break;
            
        default:
            break;
    }
    
    if (_imageFull)
    {
        _imageFull->removeFromParentAndCleanup(true);
        _imageFull = NULL;
        
    }
    
    if (_labelDiscr)
    {
        _labelDiscr->removeFromParentAndCleanup(true);
        _labelDiscr = NULL;
    }
    
    if (name.length() > 0)
    {
		Color c;
		c.r = 0;
		c.g = 0;
		c.b = 0;
		c.a = 0;
        _imageFull = MAXSCL->CreateSpriteFromSimpleImage(name, c);
        _imageFull->setScale(Display::currentDisplay()->GetDisplayScale());
        _imageFull->setAnchorPoint(ccp(0.5, 0.5));
        _imageFull->setPosition(ccp(12, 8));
        addChild(_imageFull);
    }
    
    float scale = Display::currentDisplay()->GetDisplayScale();
    CCPoint labelPos = ccp((10 + 10 + (scale-1)*2), (3+ (scale-2)));
    if (_imageFull)
    {
        labelPos.x = 10 + _imageFull->getContentSize().width + (scale-1)*2;
    }
    _labelValue->setPosition(labelPos);
    
    
//    _labelDiscr = CCLabelTTF::create(text.c_str(), MAX_DEFAULT_FONT, 8);
//    _labelDiscr->setAnchorPoint(ccp(0, 0));
//    _labelDiscr->setPosition(ccp(16, 1));
//    _labelDiscr->setContentSize(ccz(30, 12));
//    _labelDiscr->setColor(MAX_COLOR_WHITE);
//    addChild(_labelDiscr);
    
}

void GIUnitParameterRow::SetCurrentValue(int value)
{
    _value = value;
    string text = intToString(_value) + "/" + intToString(_maxValue);
    _labelValue->setString(text.c_str());
    _labelValue->setColor(this->colorForLabel());
}

void GIUnitParameterRow::SetMaxValue(int value)
{
    _maxValue = value;
    string text = intToString(_value) + "/" + intToString(_maxValue);
    _labelValue->setString(text.c_str());
    _labelValue->setColor(this->colorForLabel());
}

void GIUnitParameterRow::SetTextColor(GIPARAMETER_COLOR color)
{}

void GIUnitParameterRow::SetSeparatorVisible(bool visible)
{
    _lineSprite->setVisible(visible);
}

GIUnitParameterRow* GIUnitParameterRow::create()
{
    GIUnitParameterRow* result = new GIUnitParameterRow();
    result->autorelease();
    return result;
}

