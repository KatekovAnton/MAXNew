//
//  MAXUnitConfig.h
//  MAX
//
//  Created by Anton Katekov on 07.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXUnitConfig__
#define __MAX__MAXUnitConfig__

#include <iostream>
#include "miniPrefix.h"

class MAXUnitConfig {
    
    void SetResurceConfigValue(string key, string value);
    
public:
    //Тип=Tank
    string _type;
    
    //КадрыБазы=0,7,2
    //КадрыПушки=8,15,2
    //КадрыПушкиСтрел=16,23,2
    bool _isAnimBase;
    bool _isAnimHead;
    bool _isAbleToFire;
    
    //ФайлB=TANK
    string _bodyName;
    
    //ФайлF=P_TANK
    string _imageName;
    
    //ФайлV=Tank03.flc
    //ФайлI=I_TANK,1
    
    //ФайлA=A_TANK
    string _instoreName;
    
    //ФайлS=S_TANK
    string _shadowName;
    
    ////Sound//
    //Взрывзвук=blast-15.wav
    //Выстзвук=shoot-0.wav
    //Запзвук=on.wav
    //Рабзвук=hld.wav
    //Стрзвук=-1
    //Стопзвук=off.wav
    //Двигзвук=run.wav
    //Степзвук=off.wav
    
    MAXUnitConfig(string balanceConfigName, string resourceConfigName);
    bool IsValid() const;
    
    
    
};

class MAXUnitConfigManager {
    
    map<string, MAXUnitConfig*> _configs;
    
    void LoadUnitSegment(string source);
    
public:
    
    static MAXUnitConfigManager* SharedMAXUnitConfigManager();
    
    void LoadConfigsFromFile(string file);
    MAXUnitConfig* GetConfig(string type);
    
};

#endif /* defined(__MAX__MAXUnitConfig__) */