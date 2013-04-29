//
//  Unit.cpp
//  MAX
//
//  Created by Anton Katekov on 28.12.12.
//  Copyright (c) 2012 AntonKatekov. All rights reserved.
//

#include "GameUnit.h"
#include "Geometry.h"
#include "MyRandom.h"
#include "MAXGame.h"
#include "MAXUnitObject.h"
#include "MAXAnimationPrefix.h"
#include "MAXEngine.h"
#include "MAXObjectConfig.h"

#include "GameUnitCurrentState.h"
#include "GameUnitParameters.h"
#include "GameEffect.h"
#include "GameMatch.h"
#include "GameMap.h"
#include "PFWaveCell.h"
#include "MatchMapAgregator.h"

using namespace cocos2d;

GameUnit::GameUnit(MAXUnitObject* unitObject, GameUnitParameters* config)
:GameObject(unitObject, config->GetConfig()), _currentTopAnimation(NULL), _unitCurrentParameters(new GameUnitCurrentState(config)), _effectUnder(NULL), _isInProcess(false), _isPlacedOnMap(false), _delegate_w(NULL), _disabledByInfiltrator(false), pathIndex(0), pathIsTemp(true)
{
    unitObject->_delegate_w = this;
    unitObject->_needShadow = !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater;
    _onDraw = false;
    _detected = false;
    if (config->GetConfig()->_isBuilding && config->GetConfig()->_isAllwaysOn)
    {
        _isInProcess = true;
        CheckBodyAndShadow();
    }
    
    if(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isNeedUndercover)
    {
        _effectUnder = GameEffect::CreateBuildingBase(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 2?BUILDING_BASE_TYPE_LARGE:BUILDING_BASE_TYPE_SMALL, OBJECT_LEVEL_ONGROUND);
    }
    ChackForAnimanteBody();
}

GameUnit::~GameUnit()
{
    delete _unitCurrentParameters;

    if (_effectUnder) {
        _effectUnder->Hide();
        delete _effectUnder;
    }
}

void GameUnit::ChackForAnimanteBody()
{
    _shouldAnimateBody = (_isInProcess && _unitCurrentParameters->_unitBaseParameters->GetIsBuilding() && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn) && !_disabledByInfiltrator && !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuldozer && !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_hasHead;
}

void GameUnit::SetDirection(int dir)
{
    if(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
        return;
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->SetBodyDirection(dir);
    _unitObject->SetHeadDirection(dir);
}

void GameUnit::SetRandomDirection()
{
    if(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
        return;
    SetDirection(nextIntMax(8));
}

void GameUnit::SetColor(GLKVector4 color)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    _unitObject->playerColor = color;
}

void GameUnit::Show()
{
    GameObject::Show();
    if (_effectUnder)
        _effectUnder->Show();
    CheckBodyAndShadow();
}

void GameUnit::Hide()
{
    GameObject::Hide();
    if (_effectUnder)
        _effectUnder->Hide();
}

void GameUnit::LiftBridge()
{
    GetUnitObject()->LiftBridge();
}

void GameUnit::DropBridge()
{
    GetUnitObject()->DropBridge();
}

void GameUnit::TakeOff()
{
    if (_unitCurrentParameters->_landed)
    {
        _unitCurrentParameters->_landed = false;
        GetUnitObject()->TakeOff();
    }
}

void GameUnit::Landing()
{
    if (!_unitCurrentParameters->_landed)
    {
        _unitCurrentParameters->_landed = true;
        GetUnitObject()->Landing();
    }
}

void GameUnit::PlaceUnitOnMap()
{
    if (_isPlacedOnMap) 
        return;
    _isPlacedOnMap = true;
    Show();
    // update the fog of war for the current gamer
    _delegate_w->GameUnitDidPlaceOnMap(this);
}

void GameUnit::RemoveUnitFromMap()
{
    if (!_isPlacedOnMap)
        return;
    _isPlacedOnMap = false;
    Hide();
    // update the fog of war for the current gamer
    _delegate_w->GameUnitDidRemoveFromMap(this);
}

void GameUnit::SetLocation(const cocos2d::CCPoint &cell)
{
    GameObject::SetLocation(cell);
    if (_effectUnder) 
        _effectUnder->SetLocation(_unitCell);
}

void GameUnit::CheckBodyAndShadow()
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if (!(_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater || CanStartBuildProcess()))
    {
        //all passive-worked buildings, which cannot be topped by infiltrator
        if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding && _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isActiveBody)
        {
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 1)
                _unitObject->SetBodyOffset(0);//radar
            else
                _unitObject->SetBodyOffset(_unitCurrentParameters->_unitBaseParameters->GetConfig()->bodyIdleFrame0);//hangar
        }
        return;
    };
    
    
    EXTENDED_GROUND_TYPE groundType = game->_match->_agregator->GroundTypeAtXY(_unitCell.x, _unitCell.y);
    if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding)
    {
        _unitObject->SetBodyOffset((_isInProcess && (!_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn || !_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isActiveBody))?1:0);
    }
    else
    {
        if (groundType == EXTENDED_GROUND_TYPE_WATER)
        {
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater && !_detected)
            {
                _unitObject->SetBodyOffset(0);
                _unitObject->_needShadow = false;
                return;
            }
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious)
            {
                _unitObject->SetBodyOffset(_isInProcess?24:8);
                return;
            }
        }
        else
        {
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isUnderwater)
            {
                _unitObject->SetBodyOffset(8);
                _unitObject->_needShadow = _unitCurrentParameters->_unitBaseParameters->GetConfig()->_haveShadow;
                return;
            }
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAmphibious)
            {
                _unitObject->SetBodyOffset(_isInProcess?16:0);
                return;
            }
            if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuldozer)
            {
                int a =0;
                a++;
                _unitObject->SetBodyOffset(_isInProcess?8:0);
                return;
            }
        }
    }
    
}

void GameUnit::SetPath(std::vector<PFWaveCell*> path)
{
    movePath = path;
    pathIsTemp = true;
	if (movePath.size() > 1)
	{
        pathIndex = movePath.size() - 2; // last value is current position
	}
	else
	{
		path.clear();
        pathIndex = 0;
	}
}

void GameUnit::ClearTempPath()
{
    if (pathIsTemp)
    {
        movePath.clear();
    }
}

std::vector<PFWaveCell*> GameUnit::GetPath()
{
	return movePath;
}

int GameUnit::GetPathIndex()
{
    return pathIndex;
}

bool GameUnit::IsPathTargetedTo(const int x, const int y)
{
	bool result = FALSE;
	if (movePath.size() > 0)
	{
		PFWaveCell* cell = movePath[0];
		if (cell && (cell->x == x) && (cell->y == y))
		{
			result = TRUE;
		}
	}
	return result;
}

void GameUnit::ConfirmCurrentPath()
{
	if (movePath.size() > 1)
	{
		if (selectedGameObjectDelegate)
			selectedGameObjectDelegate->onUnitStartMove(this);

        pathIsTemp = false;
        if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isPlane)
        {
            TakeOff();
        }
		FollowPath();
	}
}

MAXANIMATION_CURVE GetCurveForStep(const int step, const int pathSize)
{
    MAXANIMATION_CURVE curve;
    if (pathSize == 1)
        curve = MAXANIMATION_CURVE_EASE_IN_OUT;
    else if (step == pathSize - 1)
        curve = MAXANIMATION_CURVE_EASE_IN;
    else if (step == 0)
        curve = MAXANIMATION_CURVE_EASE_OUT;
    else
        curve = MAXANIMATION_CURVE_EASE_LINEAR;
    return curve;
}

void GameUnit::FollowPath(void)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    MAXAnimationSequence* sequence = new MAXAnimationSequence();
    sequence->_delegate = this;
    CCPoint pos = _unitCell;
    int bodyIndex = _unitObject->GetBodyIndex();
    int pi = pathIndex;
    bool first = true;
    int speed = _unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed * 10; // rework to current data
    MAXAnimationObjectUnit* move = NULL;
    while (pi >= 0)
    {
        PFWaveCell* cell = movePath[pi];
        
        speed -= cell->cost;
        if (speed < 0)
        {
            if (move)
            {
                MAXANIMATION_CURVE curve = GetCurveForStep(0, pathIndex - pi);
                move->_moveCurve = curve;
				PFWaveCell* cell2 = movePath[pi + 1];
				float moveFactor = cell2->cost * 10.0 / _unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed; // change to current max speed
				move->SetMoveFactor(moveFactor);
            }
            break;
        }
        
        CCPoint destination = CCPointMake(cell->x, cell->y);
        int neededBodyIndex = MAXObject::CalculateImageIndex(pos, destination);
        if (neededBodyIndex != bodyIndex)
        {
            MAXAnimationObjectUnit* turn = new MAXAnimationObjectUnit(bodyIndex, neededBodyIndex, _unitObject->GetPureHeadIndex(), _unitObject);
            if (!first) // speed up unit rotation in middle of path
                turn->_aniTime = 0.01;
            sequence->AddAnimation(turn);
            bodyIndex = neededBodyIndex;
        }
        int pathSize = movePath.size() - 1;
        MAXANIMATION_CURVE curve = GetCurveForStep(pi, pathSize);
        move = new MAXAnimationObjectUnit(pos ,destination, _unitObject, curve);
        move->_delegate = this;
        float moveFactor = cell->cost * 10.0 / _unitCurrentParameters->_unitBaseParameters->GetConfig()->_pSpeed; // change to current max speed
        move->SetMoveFactor(moveFactor);
        sequence->AddAnimation(move);
        
        pos = destination;
        pi--;
        first = false;
    }
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
    _currentTopAnimation = sequence;
}

bool GameUnit::MoveToNextCell(void)
{
    bool result = false;
    if (pathIndex >= 0)
    {
        PFWaveCell* cell = movePath[pathIndex];
        SetUnitLocationAnimated(CCPointMake(cell->x, cell->y));
        pathIndex--;
    }
    return result;
}

void GameUnit::SetUnitLocationAnimated(const cocos2d::CCPoint &destination)
{
    if (_currentTopAnimation) 
        return;
    
    MAXUnitObject* _unitObject = GetUnitObject();
    int neededBodyIndex = MAXObject::CalculateImageIndex(_unitCell, destination);
    MAXAnimationSequence* sequence = new MAXAnimationSequence();
    sequence->_delegate = this;
    if (neededBodyIndex != _unitObject->GetBodyIndex())
    {
        MAXAnimationObjectUnit* step1 = new MAXAnimationObjectUnit(_unitObject->GetBodyIndex(), neededBodyIndex, _unitObject->GetPureHeadIndex(), _unitObject);
        sequence->AddAnimation(step1);
    }
    MAXANIMATION_CURVE curve;
    int pathSize = movePath.size();
    if (pathSize == 2)
        curve = MAXANIMATION_CURVE_EASE_IN_OUT;
    else if (pathIndex == pathSize - 2)
        curve = MAXANIMATION_CURVE_EASE_IN;
    else if (pathIndex == 0)
        curve = MAXANIMATION_CURVE_EASE_OUT;
    else
        curve = MAXANIMATION_CURVE_EASE_LINEAR;
    
    MAXAnimationObjectUnit* step2 = new MAXAnimationObjectUnit(_unitCell ,destination, _unitObject, curve);
    step2->_delegate = this;
    sequence->AddAnimation(step2);
    
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(sequence);
    _currentTopAnimation = sequence;
}

bool GameUnit::GetIsConnectored()const
{
    return _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isRetranslator || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isConnector;
}

void GameUnit::UpdateConnectors()
{
    MAXUnitObject *object = GetUnitObject();
    object->RemoveConnectors();
    if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 1)
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR4);
        
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR2);
       
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR1);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR3);
    }
    else
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR4);
        cell = ccp(GetUnitCell().x-1, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR8);
        cell = ccp(GetUnitCell().x, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR3);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR7);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR1);
        
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR5);
        
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR2);
        
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell) && game->_match->_agregator->ContainConnectoredBuildingInPosition(cell.x, cell.y, _owner_w))
            object->AddConnector(MAXUNITOBJECT_CONNECTOR6);
    }
}

vector<CCPoint> GameUnit::GetNerbyCells() const
{
    vector<CCPoint> resuplt;
    if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSize == 1)
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
    }
    else
    {
        CCPoint cell = ccp(GetUnitCell().x-1, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x-1, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y+2);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        
        cell = ccp(GetUnitCell().x, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+1, GetUnitCell().y-1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
        cell = ccp(GetUnitCell().x+2, GetUnitCell().y+1);
        if (game->_match->GetIsCellValid(cell))
            resuplt.push_back(cell);
    }
    return resuplt;
}

#pragma mark - Fire methods

bool GameUnit::CanFire(const cocos2d::CCPoint &target)
{
    MAXUnitObject* _unitObject = GetUnitObject();
    if(!_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAbleToFire)
        return false;
    if(_unitObject->GetFireing())
        return false;
    
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    return IsInFireRadius(targetCenter) && _unitCurrentParameters->_unitBaseParameters->_pMaxAmmo != 0 && _unitCurrentParameters->_unitBaseParameters->_pMaxShots != 0;
}

GameEffect* GameUnit::Fire(const cocos2d::CCPoint &target)
{
    if (!CanFire(target))
        return NULL;
    MAXUnitObject* _unitObject = GetUnitObject();
    CCPoint targetCenter = CCPoint((int)(target.x), (int)(target.y));
    if (_unitObject->params_w->_hasHead)
        _unitObject->SetHeadDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    else
        _unitObject->SetBodyDirection(MAXObject::CalculateImageIndex(_unitCell, target));
    
    int level = _unitCurrentParameters->_unitBaseParameters->GetConfig()->_bLevel + 1;
    if (_unitCurrentParameters->_unitBaseParameters->GetConfig()->_pFireType >= FIRE_TYPE_Air) {
        level = OBJECT_LEVEL_OVERAIR;
    }
    
    MAXAnimationObjectUnit* fireAnim = new MAXAnimationObjectUnit(_unitObject->IsSingleFire()?0.15:0.3, _unitObject);
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(fireAnim);
    
    BULLET_TYPE type = BULLET_TYPE_ROCKET;
    GameEffect* effect = GameEffect::CreateBullet(type, level, BLAST_TYPE_AIR, SECONDARY_TYPE_SMOKE);
    effect->SetLocation(GetUnitCell());
    effect->Show();
    if (type != BULLET_TYPE_PLASMA) {
        effect->SetDirection(MAXObject::CalculateRocketImageIndex(_unitCell, targetCenter));
    }
    
    MAXAnimationObject* anim = new MAXAnimationObject(GetUnitCell(), targetCenter, effect->GetObject());
    anim->_delegate = effect;
    MAXAnimationManager::SharedAnimationManager()->AddAnimatedObject(anim);
    return effect;
}

bool GameUnit::CanStartBuildProcess()
{
    return ((_unitCurrentParameters->_unitBaseParameters->GetConfig()->_isAllwaysOn != _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuilding ) || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_bSelfCreatorType != 0 || _unitCurrentParameters->_unitBaseParameters->GetConfig()->_isBuldozer);
}

void GameUnit::StartBuildProcess()
{
    if (!CanStartBuildProcess())
        return;
    _isInProcess = !_isInProcess;
    CheckBodyAndShadow();
    ChackForAnimanteBody();
}

void GameUnit::CheckMovementUpdate()
{
    CCPoint unitCell = GetUnitCell();
    CCPoint realCell = GetUnitObject()->GetObjectCell();
    realCell.x = (int)(realCell.x + 0.5);
    realCell.y = (int)(realCell.y + 0.5);
    //        printf("unitcell = %f, %f\n", unitCell.x, unitCell.y);
    //        printf("realcell = %f, %f\n\n", realCell.x, realCell.y);
    
    if ((int)unitCell.x != (int)realCell.x || (int)unitCell.y != (int)realCell.y)
    {
        //            printf("Update radar!\n");
        _delegate_w->GameUnitWillLeaveCell(this);
        _unitCell = realCell;
        _delegate_w->GameUnitDidEnterCell(this);
        CheckBodyAndShadow();
    }
}

MAXObjectConfig* GameUnit::GetBaseConfig()
{
    return _unitCurrentParameters->_unitBaseParameters->GetConfig();
}

int GameUnit::GetParameterMaxValue(UNIT_PARAMETER_TYPE parameterType) const
{
    return _unitCurrentParameters->GetMaxParameterValue(parameterType);
}
 
int GameUnit::GetParameterValue(UNIT_PARAMETER_TYPE parameterType) const
{
    return _unitCurrentParameters->GetParameterValue(parameterType);
}

#pragma mark - MAXAnimationDelegate

void GameUnit::OnAnimationStart(MAXAnimationBase* animation)
{}

void GameUnit::OnAnimationUpdate(MAXAnimationBase* animation)
{
    CheckMovementUpdate();
}

void GameUnit::OnAnimationFinish(MAXAnimationBase* animation)
{
//    if (animation == _moveAnimation)
//    {
//        _unitCell = ((MAXAnimationObjectUnit*)animation)->GetEndLocation();
//        _moveAnimation->_delegate = NULL;
//        _moveAnimation = NULL;
////        CheckBodyAndShadow();
//
//        
//        BoundingBox bb;
//        bb.min.x = _unitCell.x;
//        bb.min.y = _unitCell.y;
//        bb.max.x = _unitCell.x + 1;
//        bb.max.y = _unitCell.y + 1;
//        USimpleContainer<MAXObject*> *buffer = new USimpleContainer<MAXObject*>(10);
//        engine->GetAllObjectsInArea(bb, buffer);
//        delete buffer;
//    }
//    else
    CheckMovementUpdate();
    
    if (animation == _currentTopAnimation)
    {
        _currentTopAnimation->_delegate = NULL;
        _currentTopAnimation = NULL;
        if (pathIndex < 0)
        {
            // move completed succesfully
            movePath.clear();
            pathIndex = 0;
            pathIsTemp = true;
        }
        if (selectedGameObjectDelegate)
            selectedGameObjectDelegate->onUnitStopMove(this);
        //MoveToNextCell();
        
    }
    else // move
    {
        pathIndex--;
//        if (pathIndex == 1)
//        {
//            // move stop
//            _currentTopAnimation->Stop();
//        }
    }
}

#pragma mark - MAXUnitObjectDelegate

int GameUnit::GetScan() const
{
    return _unitCurrentParameters->_unitBaseParameters->_pMaxScan;
}

int GameUnit::GetRange() const
{
    return _unitCurrentParameters->_unitBaseParameters->_pMaxRange;
}

float GameUnit::GetHealStatus() const
{
    return 1.0;
}

float GameUnit::GetShots() const
{
    return _unitCurrentParameters->_pShots;
}

bool GameUnit::ShouldAnimateBody() const
{
    return _shouldAnimateBody;
}

bool GameUnit::ShoudDrawFakeCircle() const
{
	return (movePath.size() > 0) && (!GetIsFreezed() && (game->_match->_currentPlayer_w == _owner_w));
}

CCPoint GameUnit::GetFakeCenter() const
{
	PFWaveCell* cell = movePath[0];
	return CCPointMake(cell->x * 64 + 32, cell->y * 64 + 32);
}

