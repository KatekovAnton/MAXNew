//
//  MAXUnitObjectDelegate.h
//  MAX
//
//  Created by Anton Katekov on 22.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_MAXUnitObjectDelegate_h
#define MAX_MAXUnitObjectDelegate_h

class MAXUnitObjectDelegate
{
public:
    
    
    virtual bool ShouldSkipThisUnit() const = 0;
    
    virtual int GetScan() const = 0;
    virtual int GetRange() const = 0;
    virtual float GetHealStatus() const = 0;
    virtual float GetShots() const = 0;
    virtual bool ShouldAnimateBody() const = 0;
	virtual bool ShoudDrawFakeCircle() const = 0;
	virtual CCPoint GetFakeCenter() const = 0;
};

#endif
