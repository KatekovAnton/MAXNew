//
//  ScreenMenu.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__ScreenMenu__
#define __MAX__ScreenMenu__

#include <iostream>
#include "cocos2d.h"
#include "ScreenMenuElement.h"

using namespace cocos2d;

class MAXMainMenuController;
class ScreenSelectGameType;
class CCParallaxNodeCustom;


class ScreenMenu : public CCScene
{   

	bool _freezed;

public:
    CCNode *_nodeBackgroundBase;
    CCParallaxNodeCustom* _parallax;
    

	MAXMainMenuController	*_menuController;
	
	ccColor4B				_colorBG;
	CCLayerColor			*_layerBg;
	CCLayerColor			*_layerButtonsBG;

	CCArray					*_screenStack;

    ScreenMenu();
    ~ScreenMenu();

	void PushScreen(ScreenMenuElement *screen);
	void PopScreen();
	void PopToFirstScreen();

	void OnMoveDidFinish(CCObject* param);
	void OnPushNewScreen(CCObject* sender);
	void OnPopOldScreen(CCObject* sender);
	void OnFrameChangeDidFinish(CCObject* sender);

	void AlignBGForScreen(ScreenMenuElement *screen, float delayForChangeSize);

	CCPoint CalculatePointsForNode(CCNode *node, int index, int fullNumber, bool outOfScreen, bool down, ScreenMenuElement *screen);
	void MoveNodeAtIndex(CCNode *node, int index, int fullNumber, bool outOfScreen, bool down, ScreenMenuElement *screen);
    
    void InitBaseInterface();

};

#endif /* defined(__MAX__ScreenMenu__) */
