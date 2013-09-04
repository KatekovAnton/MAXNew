//
//  GIMessageWindow.cpp
//  MAX
//
//  Created by Anton Katekov on 04.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIMessageWindow.h"
#include "GameInterfacePrefix.h"
#include "CocosHelper.h"
#include "SoundEngine.h"

GIMessageWindow::GIMessageWindow(string title, string message, vector<string> buttons)
{
    float maxW = 300;
    CCLabelTTF *label = CCLabelTTF::create(message.c_str(), MAX_DEFAULT_FONT, 12, ccz(maxW, 0), CCTextAlignment::kCCTextAlignmentCenter, CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
   
    float menuSize = 35 * buttons.size();
    
    CCSize labelSZ = label->getContentSize();
    setContentSize(ccz(maxW + 20, labelSZ.height + menuSize));
    CCSize maxSize = getContentSize();
    CCLayerColor *underLay = CCLayerColor::create(CocosHelper::normalColor(), maxSize.width, labelSZ.height + 20);
    
    {
        CCPoint origin;
        origin.x = (int)((maxSize.width - underLay->getContentSize().width) / 2);
        origin.y = (int)(maxSize.height - underLay->getContentSize().height);
        underLay->setPosition(origin);
        addChild(underLay);
    }
    {
        CCPoint origin;
        origin.x = (int)(maxSize.width / 2);
        origin.y = (int)(underLay->getPosition().y + underLay->getContentSize().height / 2);
        label->setPosition(origin);
        addChild(label);
    }
    
    CCArray *array = CCArray::create();
    CCPoint origin = ccp(0, (buttons.size()-2) * 30);
    for (int i = 0; i < buttons.size(); i++)
    {
        string title = buttons[i];
        CCMenuItemNodes *button = createMenuItemWithLayers(ccz(getContentSize().width, 30), CocosHelper::normalColor(), CocosHelper::selectedColor(), title, MAX_DEFAULT_FONT, 10, MAX_COLOR_WHITE, this, callfuncO_selector(GIMessageWindow::OnButton));
        button->setTag(i);
		CocosHelper::MoveNode(button->getChildByTag(BUTTON_LABEL_TAG), ccp(15, 5));
        button->setPosition(origin);
        origin.y -= 35;
        array->addObject(button);
    }
    CCMenu *menu = CCMenu::createWithArray(array);
    menu->setContentSize(ccz(getContentSize().width, menuSize));
    menu->setPosition(0, underLay->getPosition().y - underLay->getContentSize().height);
    addChild(menu);
}

GIMessageWindow::~GIMessageWindow()
{}

void GIMessageWindow::OnButton(CCObject* sender)
{
    SOUND->PlaySystemSound(SOUND_TYPE_BUTTON_AVERAGE);
    if (_delegate_w)
    {
        CCMenuItem* menuItem = reinterpret_cast<CCMenuItem*>(sender);
        _delegate_w->OnMessageWindowButtonClicked(menuItem->getTag(), this);
    }
}
