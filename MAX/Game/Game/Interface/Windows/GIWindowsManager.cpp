//
//  GIWindowsManager.cpp
//  MAX
//
//  Created by  Developer on 27.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "GIWindowsManager.h"
#include "miniPrefix.h"
#include "GIWindow.h"
#include "CocosHelper.h"

GIWindowsManager::GIWindowsManager(CCNode *parentNode)
:_baseNode(parentNode), _currentWindow(NULL)
{
    _baseNode->retain();
    _nodeBase = CCLayerColor::create(CocosHelper::normalColor(), _baseNode->getContentSize().width, 0);
    _nodeBase->setPosition(ccp(0, (_baseNode->getContentSize().height)/2.0));
    _baseNode->addChild(_nodeBase);
}

GIWindowsManager::~GIWindowsManager()
{
    _baseNode->release();
}

void GIWindowsManager::ProcessQueue()
{}

void GIWindowsManager::PresentWindow(GIWindow *window, float h, bool queue)
{
    if (_currentWindow || queue) {
        _windowQueue.push_back(window);
        return;
    }
    
    CCWaitExtended *wait = new CCWaitExtended();
	wait->initWithDuration(interfaceAnimationTime, NULL, NULL);
	wait->autorelease();
    
    _nodeBase->setContentSize(ccz(_baseNode->getContentSize().width, 0));
    _nodeBase->setPosition(ccp(0, (_baseNode->getContentSize().height + h)/2.0));
    CCSetFrameExtended *action = new CCSetFrameExtended();
	action->initWithDuration(interfaceAnimationTime, ccz(_baseNode->getContentSize().width, h), ccp(_nodeBase->getPosition().x, _nodeBase->getPosition().y - h), _nodeBase->getOpacity(), NULL, NULL);
	action->autorelease();
	action->setTag(0);
    CCEaseInOut* action1 = CCEaseInOut::create(action, 3.0);
	action1->setTag(0);

    
    CCAction *seq = CCSequence::createWithTwoActions(wait, action1);
    _nodeBase->runAction(seq);
}

void GIWindowsManager::CloseCurrentWindow()
{
//    if (!_currentWindow) {
//        return;
//    }
    
    CCSetFrameExtended *action = new CCSetFrameExtended();
	action->initWithDuration(interfaceAnimationTime, ccz(_baseNode->getContentSize().width, 0), ccp(0, (_baseNode->getContentSize().height + _nodeBase->getContentSize().height)/2.0), _nodeBase->getOpacity(), NULL, NULL);
	action->autorelease();
	action->setTag(0);
    CCEaseInOut* action1 = CCEaseInOut::create(action, 3.0);
	action1->setTag(0);
    _nodeBase->runAction(action1);
}
