#include "PlayerInfo.h"
#include <iostream>
#include "../WeaponInfo/WeaponInfo.h"
#include "../WeaponInfo/Rifle.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "../Minimap/Minimap.h"
#include <Windows.h>
#include "LuaInterface.h"


// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(30.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, m_dElapsedTime(0.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
	, primaryWeapon(NULL)
	, secondaryWeapon(NULL)
	, theCurrentPosture(STAND)
	, weaponManager(NULL)
	, m_iCurrentWeapon(0)
	, m_fCameraSwayAngle(0.0f)
	, m_fCameraSwayDeltaAngle(0.1f)
	, m_fCameraSwayAngle_LeftLimit(-0.3f)
	, m_fCameraSwayAngle_RightLimit(0.3f)
	, m_bCameraSwayDirection(false)
	, keyMoveForward('W')
	, keyMoveBackward('S')
	, keyMoveLeft('A')
	, keyMoveRight('D')
{
}

CPlayerInfo::~CPlayerInfo(void)
{
	if (weaponManager)
	{
		for (int i = 0; i < m_iNumOfWeapon; i++)
		{
			delete weaponManager[i];
		}
		delete [] weaponManager;
		weaponManager = NULL;
	}
	m_pTerrain = NULL;
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	position.Set(0, 0, 10);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	// Set the pistol as the primary weapon
	primaryWeapon = new CRifle();
	primaryWeapon->Init();
	//secondaryWeapon = new CPistol();
	//secondaryWeapon->Init();
	secondaryWeapon = new CGrenadeThrow();
	secondaryWeapon->Init();

	weaponManager = new CWeaponInfo*[m_iNumOfWeapon];
	
	weaponManager[0] = primaryWeapon;
	weaponManager[1] = secondaryWeapon;

	m_fCameraSwayAngle = 0.0f;
	m_fCameraSwayDeltaAngle = 1.0f;
	m_fCameraSwayAngle_LeftLimit = -5.0f;
	m_fCameraSwayAngle_RightLimit = 5.0f;
	m_bCameraSwayDirection = false;

	CLuaInterface::GetInstance()->Init();
	//Initialise the custom keyboard inputs
	keyMoveForward = CLuaInterface::GetInstance()->getCharValue("moveForward");
	keyMoveBackward = CLuaInterface::GetInstance()->getCharValue("moveBackward");
	keyMoveLeft = CLuaInterface::GetInstance()->getCharValue("moveLeft");
	keyMoveRight = CLuaInterface::GetInstance()->getCharValue("moveRight");

	float distanceSquare = CLuaInterface::GetInstance()->getDistanceSquareValue("CalculateDistanceSquare", Vector3(0, 0, 0), Vector3(10, 10, 10));
	int a = 1, b = 2, c = 3, d = 4;
	CLuaInterface::GetInstance()->getVariableValues("GetMinMax", a, b, c, d);
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		m_dJumpSpeed = 10.0;
	}
}

// Set position
void CPlayerInfo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
	
	
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set m_dJumpAcceleration of the player
void CPlayerInfo::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayerInfo::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void CPlayerInfo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Stop the player's movement
void CPlayerInfo::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
}

// Get position x of the player
Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

// Get m_dJumpAcceleration of the player
double CPlayerInfo::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

GroundEntity * CPlayerInfo::GetTerrain(void)
{
	return m_pTerrain;
}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t; v is m_dJumpSpeed AFTER updating using SUVAT where u is 
	// the initial speed and is equal to m_dJumpSpeed
	m_dJumpSpeed = m_dJumpSpeed + m_dJumpAcceleration * m_dElapsedTime;
	// Check if the jump speed is less than zero, then it should be falling
	if (m_dJumpSpeed < 0.0)
	{
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
		m_dElapsedTime = 0.0;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values.
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dFallSpeed = m_dFallSpeed + m_dFallAcceleration * m_dElapsedTime;
	// Check if the jump speed is below terrain, then it should be reset to terrain height
	if (position.y < m_pTerrain->GetTerrainHeight(position))
	{
		Vector3 viewDirection = target - position;
		position.y = m_pTerrain->GetTerrainHeight(position);
		target = position + viewDirection;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = false;
		m_dElapsedTime = 0.0;
	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	//Update the position if the WQASD buttons were activated 
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	{
		if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward))
			Move_FrontBack(dt, true);
		if (KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward))
			Move_FrontBack(dt, false);
		if (KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft))
			Move_LeftRight(dt, true);
		if (KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
			Move_LeftRight(dt, false);
	}

	// Check if there is a need to change posture
	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		theCurrentPosture = (CURRENT_POSTURE)(theCurrentPosture + 1);
		if (theCurrentPosture == NUM_POSTURE)
			theCurrentPosture = STAND;

		Vector3 viewDirection = target - position;
		switch (theCurrentPosture)
		{
		case STAND:
			position = m_pTerrain->GetTerrainHeight(Vector3(position.x, 10.0f, position.z));
			target = position + viewDirection;
			break;
		case CROUCH:
			position = m_pTerrain->GetTerrainHeight(Vector3(position.x, 10.0f, position.z));
			position.y -= 5.0f;
			target = position + viewDirection;
			break;
		case PRONE:
			position = m_pTerrain->GetTerrainHeight(Vector3(position.x, 10.0f, position.z));
			position.y -= 8.0f;
			target = position + viewDirection;
			break;
		default:
			break;
		}
	}
	
	
	if (KeyboardController::GetInstance()->IsKeyReleased('I'))
	{
		hp--;
		if (hp < 0)
			hp = 0;
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('O'))
	{
		hp++;
		if (hp > 3)
			hp = 3;
	}
	if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB) && secondaryWeapon->magRounds != 0)
		SwitchSecondary = true;
	else
		SwitchSecondary = false;
	if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB) && primaryWeapon->magRounds != 0)
		SwitchPrimary = true;
	else
		SwitchPrimary = false;

	if (primaryWeapon->magRounds != 0)
		bool AmmoPrimary = true;
	if(primaryWeapon->magRounds == 0)
		bool AmmoPrimary = false;
	if (secondaryWeapon->magRounds != 0)
		bool AmmoSecondary = true;
	if (secondaryWeapon->magRounds == 0)
		bool AmmoSecondary = false;

	if (primaryWeapon)
		primaryWeapon->Update(dt);
	if (secondaryWeapon)
		secondaryWeapon->Update(dt);
	if (weaponManager[m_iCurrentWeapon])
		weaponManager[m_iCurrentWeapon]->Update(dt);
	{
		// Constrain the position
		//Constrain();
		UpdateJumpUpwards(dt);
		UpdateFreeFall(dt);
	}

	// Do camera sway
	if (m_fCameraSwayAngle != 0.0f)
	{
		Mtx44 rotation;
		if (m_bCameraSwayDirection == false)
			rotation.SetToRotation(-m_fCameraSwayDeltaAngle, 0.0f, 0.0f, 1.0f);
		else if (m_bCameraSwayDirection == true)
			rotation.SetToRotation(m_fCameraSwayDeltaAngle, 0.0f, 0.0f, 1.0f);
		up = rotation * up;
	}

	// Update minimap rotation angle
	Vector3 viewUV = (target - position).Normalized();
	CMinimap::GetInstance()->SetAngle(atan2(viewUV.z, viewUV.x) * 57.2883513685549146);

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->SetCameraPos(position);
		attachedCamera->SetCameraTarget(target);
		attachedCamera->SetCameraUp(up);
	}

}

// Detect and process front / back movement on the controller
bool CPlayerInfo::Move_FrontBack(const float deltaTime, const bool direction,  float speedMultiplier)
{
	//// Add camera sway
	//if (m_bCameraSwayDirection == false)
	//{
	//	m_fCameraSwayAngle -= m_fCameraSwayDeltaAngle;
	//	if (m_fCameraSwayAngle < m_fCameraSwayAngle_LeftLimit * speedMultiplier)
	//		m_bCameraSwayDirection = !m_bCameraSwayDirection;
	//}
	//else
	//{
	//	m_fCameraSwayAngle += m_fCameraSwayDeltaAngle;
	//	if (m_fCameraSwayAngle > m_fCameraSwayAngle_RightLimit * speedMultiplier)
	//		m_bCameraSwayDirection = !m_bCameraSwayDirection;
	//}

	Vector3 viewVector = (target - position).Normalized();
	if (direction)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT))
		{
			speedMultiplier *= 2;
		}

		if (theCurrentPosture == CROUCH)
		{
			position += viewVector * (float)m_dSpeed * 0.75f * (float)deltaTime;
		}
		else if (theCurrentPosture == PRONE)
		{
			position += viewVector * (float)m_dSpeed * 0.25f * (float)deltaTime;
		}
		else
		position += viewVector * (float)m_dSpeed * speedMultiplier * (float)deltaTime;

		
		//	 Constrain the position
		Constrain();
		// Update the target
		target = position + viewVector;

		return true;
	}
	else
	{
		position -= viewVector * (float)m_dSpeed * (float)deltaTime;
		//	 Constrain the position
		Constrain();
		// Update the target
		target = position + viewVector;
		return true;
	}

	return false;
}
// Detect and process left / right movement on the controller
bool CPlayerInfo::Move_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	Vector3 viewVector = target - position;
	Vector3 rightUV;
	if (direction)
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		position -= rightUV * (float)m_dSpeed * deltaTime;
		// Update the target
		target = position + viewVector;
		return true;
	}
	else
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		position += rightUV * (float)m_dSpeed * deltaTime;
		// Update the target
		target = position + viewVector;
		return true;
	}
	return false;
}

// Detect and process look up / down on the controller
bool CPlayerInfo::Look_UpDown(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	float pitch = (float)(-m_dSpeed * speedMultiplier * (float)deltaTime);
	rightUV = viewUV.Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	up = rightUV.Cross(viewUV).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	viewUV = rotation * viewUV;
	target = position + viewUV;

	return true;
}
// Detect and process look left / right on the controller
bool CPlayerInfo::Look_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	float yaw = (float)-m_dSpeed * speedMultiplier * (float)deltaTime;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	viewUV = rotation * viewUV;
	target = position + viewUV;
	rightUV = viewUV.Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	up = rightUV.Cross(viewUV).Normalized();

	return true;
}

// Stop sway
bool CPlayerInfo::StopSway(const float deltaTime)
{
	m_bCameraSwayDirection = false;
	m_fCameraSwayAngle = 0.0f;
	up = Vector3(0.0f, 1.0f, 0.0f);
	return true;
}

// Reload current weapon
bool CPlayerInfo::ReloadWeapon(void)
{
	if (weaponManager[m_iCurrentWeapon])
	{
		weaponManager[m_iCurrentWeapon]->Reload();
		return true;
	}
	return false;
}

// Change current weapon
bool CPlayerInfo::ChangeWeapon(void)
{
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != m_iCurrentWeapon)
	{
		if ((MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) >= 0) &&
			(MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) < m_iNumOfWeapon))
		{
			m_iCurrentWeapon = MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET);
		}
	}


	return true;
}

// Get Current Weapon
int CPlayerInfo::GetWeapon(void) const
{
	return m_iCurrentWeapon;
}

// Discharge Primary Weapon
bool CPlayerInfo::DischargePrimaryWeapon(const float deltaTime)
{
	//if (primaryWeapon)
	//	primaryWeapon->Discharge(position, target, this);

	if (weaponManager[m_iCurrentWeapon])
	{
		weaponManager[m_iCurrentWeapon]->Discharge(position, target, this);
		return true;
	}
	return false;
	
}

// Discharge Secondary Weapon
bool CPlayerInfo::DischargeSecondaryWeapon(const float deltaTime)
{
	if (secondaryWeapon)
	{
		secondaryWeapon->Discharge(position, target, this);
		return true;
	}
	return false;
}
#include "../CameraEffects/CameraEffects.h"

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
	{
		position.x = maxBoundary.x - 1.0f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}
	if (position.y > maxBoundary.y - 1.0f)
	{
		position.y = maxBoundary.y - 1.0f;
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
		m_dElapsedTime = 0.0;
	}
	if (position.z > maxBoundary.z - 1.0f)
	{
		position.z = maxBoundary.z - 1.0f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}
	if (position.x < minBoundary.x + 1.0f)
	{
		position.x = minBoundary.x + 1.0f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}
	if (position.y < minBoundary.y + 1.0f)
		position.y = minBoundary.y + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
	{
		position.z = minBoundary.z + 1.0f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}
	// if the player is not jumping nor falling, then adjust his y position
	if ((m_bJumpUpwards == false) && (m_bFallDownwards == false))
	{
		// if the y position is not equal to terrain height at that position, 
		// then update y position to the terrain height
		Vector3 viewDirection = target - position;
		switch (theCurrentPosture)
		{
		case STAND:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z));
			target = position + viewDirection;
			break;
		case CROUCH:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 5.0f;
			target = position + viewDirection;
			break;
		case PRONE:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 8.0f;
			target = position + viewDirection;
			break;
		default:
			break;
		}

	}
}

void CPlayerInfo::AttachCamera(FPSCamera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}
