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
#include "GIWindowsManagerDelegate.h"

CCSize maximumSize;

GIWindowsManager::GIWindowsManager(CCNode *parentNode)
:_baseNode(parentNode)
{
    _baseNode->retain();
    
    {
        CCMenuItem *bigButton = CCMenuItem::create();
        bigButton->setContentSize(_baseNode->getContentSize());
        _menu = CCMenu::createWithItem(bigButton);
        _menu->retain();
        _menu->setContentSize(_baseNode->getContentSize());
    }
    {
        CCMenuItem *bigButton = CCMenuItem::create();
        bigButton->setContentSize(_baseNode->getContentSize());
        _windowBlockMenu = CCMenu::createWithItem(bigButton);
        _windowBlockMenu->retain();
        _windowBlockMenu->setContentSize(_baseNode->getContentSize());
    }
    
    
    maximumSize = _baseNode->getContentSize();
    
    ccColor4B backgroundColor; backgroundColor.a = 190; backgroundColor.r = 30; backgroundColor.g = 30; backgroundColor.b = 30;
    _nodeBlackBase = CCLayerColor::create(backgroundColor, _baseNode->getContentSize().width, 0);
    _nodeBlackBase->setPosition(ccp(0, (_baseNode->getContentSize().height)/2.0));
    _nodeBlackBase->retain();
}

GIWindowsManager::~GIWindowsManager()
{
    _baseNode->release();
}

void GIWindowsManager::ProcessQueue()
{}

void GIWindowsManager::PresentWindow(GIWindow *window, float w, bool queue, float animDelay)
{
    window->retain();
    if (queue) {
        _windowQueue.push_back(window);
        return;
    }
    
    GIWindow *currentWindow = NULL;
    if (_windowStack.size()>0)
        currentWindow = _windowStack[_windowStack.size() - 1];
    
        
    _windowStack.push_back(window);
    
    if (!currentWindow) {
        
        //present black background
        _baseNode->addChild(_menu);
        _baseNode->addChild(_nodeBlackBase);
        _nodeBlackBase->setContentSize(ccz(0, _baseNode->getContentSize().height));
        _nodeBlackBase->setPosition(ccp((_baseNode->getContentSize().width + w) / 2.0 , 0));
        
        //wait while game menu is fading out
        CCWaitExtended *wait = new CCWaitExtended();
        wait->initWithDuration(animDelay, NULL, NULL);
        wait->autorelease();
        //enlarge black background
        CCSetFrameExtended *action = new CCSetFrameExtended();
        action->initWithDuration(interfaceAnimationTime, ccz(w, _baseNode->getContentSize().height), ccp((_baseNode->getContentSize().width - w) / 2.0, 0), _nodeBlackBase->getOpacity(), this, callfuncO_selector(GIWindowsManager::OnBlackThingEnlaged));
        action->autorelease();
        action->setTag(0);
        CCEaseInOut* action1 = CCEaseInOut::create(action, 3.0);
        action1->setTag(0);
        
        //run animations, when frame will be done it calls GIWindowsManager::OnBlackThingEnlaged
        CCAction *seq = CCSequence::createWithTwoActions(wait, action1);
        _nodeBlackBase->runAction(seq);
        
    }
}

void GIWindowsManager::OnCloseFinished(CCObject* sender)
{
    _menu->removeFromParentAndCleanup(true);
    _nodeBlackBase->removeFromParentAndCleanup(true);
    if (_delegate_w) 
        _delegate_w->WindowManagerDidCloseLastWindow();
}

CCSize GIWindowsManager::MaximumSize()
{
    return maximumSize;
}

bool GIWindowsManager::IsWindowOpened()
{
    return _windowStack.size()>0;
}

#pragma mark - animations and delays callbacks
void GIWindowsManager::OnBlackThingEnlaged(CCObject *sender)
{
    GIWindow* presentingWidow = _windowStack[_windowStack.size() - 1];
    _nodeBlackBase->addChild(presentingWidow);
    presentingWidow->WindowWillAppear();
    
    _windowBlockMenu->setContentSize(presentingWidow->getContentSize());
    presentingWidow->addChild(_windowBlockMenu);
    
    CCPoint position = ccp (0, _nodeBlackBase->getContentSize().height);
    presentingWidow->setPosition(position);
    
    CCPoint destination;
    destination.x = position.x;
    destination.y = (_nodeBlackBase->getContentSize().height - presentingWidow->getContentSize().height) / 2.0;
    
    CCSetFrameExtended *action = new CCSetFrameExtended();
    action->initWithDuration(interfaceAnimationTime, presentingWidow->getContentSize(), destination, _nodeBlackBase->getOpacity(), this, callfuncO_selector(GIWindowsManager::OnWindowApperarAnimationFinished));
    action->autorelease();
    action->setTag(0);
    
    CCEaseInOut* action1 = CCEaseInOut::create(action, 3.0);
    action1->setTag(0);
    presentingWidow->runAction(action1);
    
}

void GIWindowsManager::OnWindowApperarAnimationFinished(CCObject *sender)
{
    GIWindow* presentingWidow = _windowStack[_windowStack.size() - 1];
    _windowBlockMenu->removeFromParentAndCleanup(true);
    presentingWidow->WindowDidAppear();
}

void GIWindowsManager::OnWindowDisapperarAnimationFinished(CCObject *sender)
{
    GIWindow *window = _windowStack[_windowStack.size()-1];
    _windowBlockMenu->removeFromParentAndCleanup(true);
    window->removeFromParentAndCleanup(true);
    window->release();
    _windowStack.pop_back();
    CloseBlackBase();
}

void GIWindowsManager::DisappearWindow(GIWindow* window)
{
    if (_windowStack.size() == 0)
        return;
    
    if (window == _windowStack[_windowStack.size()-1])
    {
        window->WindowWillDisapper();
        _windowBlockMenu->setContentSize(window->getContentSize());
        window->addChild(_windowBlockMenu);
        
        CCPoint position = window->getPosition();
        
        CCPoint destination;
        destination.x = position.x;
        destination.y = _nodeBlackBase->getContentSize().height;
        
        CCSetFrameExtended *action = new CCSetFrameExtended();
        action->initWithDuration(interfaceAnimationTime, window->getContentSize(), destination, _nodeBlackBase->getOpacity(), this, callfuncO_selector(GIWindowsManager::OnWindowDisapperarAnimationFinished));
        action->autorelease();
        action->setTag(0);
        
        CCEaseInOut* action1 = CCEaseInOut::create(action, 3.0);
        action1->setTag(0);
        window->runAction(action1);
        
    }
    else
    {
        vector<GIWindow*>::iterator element = _windowStack.begin();
        while (element != _windowStack.end())
        {
            if (*element == window)
            {
                _windowStack.erase(element);
                break;
            }
            element ++;
        }
    }
}

void GIWindowsManager::CloseBlackBase()
{
    CCSetFrameExtended *action = new CCSetFrameExtended();
	action->initWithDuration(interfaceAnimationTime, ccz(0, _baseNode->getContentSize().height), ccp(_nodeBlackBase->getPosition().x + _nodeBlackBase->getContentSize().width, 0), _nodeBlackBase->getOpacity(), this, callfuncO_selector(GIWindowsManager::OnCloseFinished));
	action->autorelease();
	action->setTag(0);
    CCEaseInOut* action1 = CCEaseInOut::create(action, 3.0);
	action1->setTag(0);
    _nodeBlackBase->runAction(action1);
}


