//
//  MainMenuResultData.h
//  MAX
//
//  Created by  Developer on 10.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MainMenuResultData__
#define __MAX__MainMenuResultData__

#include "miniPrefix.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct __ScenarioInfo
{
	int			_id;
	string		_scenarioName;
	string		_scenarioDescription;
	string		_scenarioFileName;
	
	int			_currentPlayerId;
	string		_currentPlayerName;
	Color		_currentPlayerColor;

	int			_turnNumber;
	
} typedef ScenarioInfo;

#endif /* defined(__MAX__MainMenuResultData__) */
