#include "CameraEffects.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "MouseController.h"

CCameraEffects::CCameraEffects(void)
	: m_iAngle(-90)
	, m_bStatusBloodScreen(false)
	, m_CameraEffects_BloodyScreen(NULL)
	, m_fCountdownTimer_BloodScreen(2.0f)
	, mode(MODE_2D)
{
	Init();
}
CCameraEffects::~CCameraEffects(void)
{
	if (m_CameraEffects_BloodyScreen)
	{
		delete m_CameraEffects_BloodyScreen;
		m_CameraEffects_BloodyScreen = NULL;
	}
}

// Initialise this class instance
bool CCameraEffects::Init(void)
{
	m_iAngle = -90;
	position.Set(0.0f, 0.0f, 0.0f);
	scale.Set(800.0f, 600.0f, 100.0f);

	return true;
}

bool CCameraEffects::SetBloodScreen(Mesh* aBloodScreen)
{
	if (aBloodScreen != NULL)
	{
		m_CameraEffects_BloodyScreen = aBloodScreen;
		return true;
	}
	return false;
}
Mesh* CCameraEffects::GetBloodScreen(void) const
{
	return m_CameraEffects_BloodyScreen;
}

// Set m_iAngle of avatar
bool CCameraEffects::SetAngle(const int m_iAngle)
{
	this->m_iAngle = m_iAngle;
	return true;
}
// Get m_iAngle
int CCameraEffects::GetAngle(void) const
{
	return m_iAngle;
}

// Set boolean flag for BloodScreen
bool CCameraEffects::SetStatus_BloodScreen(const bool m_bStatusBloodScreen)
{
	this->m_bStatusBloodScreen = m_bStatusBloodScreen;
	return true;
}

// Set Countdown Timer for BloodScreen
bool CCameraEffects::SetTimer_BloodScreen(const float m_fCountdownTimer_BloodScreen)
{
	this->m_fCountdownTimer_BloodScreen = m_fCountdownTimer_BloodScreen;
	return true;
}

// Get Countdown Timer for BloodScreen
float CCameraEffects::GetTimer_BloodScreen(void) const
{
	return m_fCountdownTimer_BloodScreen;
}

// Update the camera effects
void CCameraEffects::Update(const float dt)
{
	if (m_bStatusBloodScreen)
	{
		m_fCountdownTimer_BloodScreen -= dt;
		if (m_fCountdownTimer_BloodScreen <= 0.0f)
		{
			// Set the bool flag for Render BloodScreen to false
			m_bStatusBloodScreen = false;
			// Reset the countdown timer to the default value
			m_fCountdownTimer_BloodScreen = 2.0f;
		}
	}
}

// Render the UI
void CCameraEffects::RenderUI()
{
	if (mode == MODE_3D)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	if (MouseController::GetInstance()->IsButtonDown(MouseController::RMB)&& CPlayerInfo::GetInstance()->m_iCurrentWeapon == 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x , position.y , position.z);
		modelStack.Scale(scale.x + 100, scale.y + 100, scale.z + 100);
		RenderHelper::RenderMesh(Scope);
		modelStack.PopMatrix();
	}
	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	// Translate the current transformation
	modelStack.Translate(position.x, position.y, position.z);
	// Scale the current transformation
	modelStack.Scale(scale.x, scale.y, scale.z);

	// Push the current transformation into the modelStack


	modelStack.PopMatrix();

	//------------------------------------//

	modelStack.PushMatrix();

	

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);

	modelStack.Scale(scale.x - 700, scale.y - 500, scale.z - 500);
	RenderHelper::RenderMesh(Crosshair);
	modelStack.PopMatrix();

	// Display the Avatar
	if ((m_bStatusBloodScreen) && (m_CameraEffects_BloodyScreen))
	{
		RenderHelper::RenderMesh(m_CameraEffects_BloodyScreen);

	}

	if (CPlayerInfo::GetInstance()->m_iCurrentWeapon == 0 && CPlayerInfo::GetInstance()->SwitchPrimary == true)
	{
		modelStack.Translate(position.x + 200, position.y - 200, position.z);

		modelStack.Scale(scale.x - 270, scale.y - 270, scale.z - 270);
		RenderHelper::RenderMesh(RifleMeshFired);
	}
	if (CPlayerInfo::GetInstance()->m_iCurrentWeapon == 0)
	{
		modelStack.Translate(position.x + 300, position.y - 150, position.z);

		modelStack.Scale(scale.x - 270, scale.y - 270, scale.z - 270);
		RenderHelper::RenderMesh(RifleMesh);

		
	}

	if (CPlayerInfo::GetInstance()->m_iCurrentWeapon == 1 && CPlayerInfo::GetInstance()->SwitchSecondary == true)
	{
		modelStack.Translate(position.x + 200, position.y - 200, position.z);

		modelStack.Scale(scale.x - 270, scale.y - 270, scale.z - 270);
		RenderHelper::RenderMesh(PistolMeshFired);
	}
	

	if (CPlayerInfo::GetInstance()->m_iCurrentWeapon == 1)
	{
		modelStack.Translate(position.x + 200, position.y - 200, position.z);

		modelStack.Scale(scale.x - 270, scale.y - 270, scale.z - 270);
		RenderHelper::RenderMesh(PistolMesh);
	}

	
	


	modelStack.PopMatrix();
	modelStack.PopMatrix();


	if (CPlayerInfo::GetInstance()->hp == 3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x - 200, position.y + 200, position.z);
		modelStack.Scale(scale.x - 300, scale.y - 300, scale.z - 270);
		RenderHelper::RenderMesh(FullHP);
		modelStack.PopMatrix();
	}

	if (CPlayerInfo::GetInstance()->hp == 2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x - 200, position.y + 200, position.z);
		modelStack.Scale(scale.x - 300, scale.y - 300, scale.z - 270);
		RenderHelper::RenderMesh(One_Third_HP);
		modelStack.PopMatrix();
	}
	
	if (CPlayerInfo::GetInstance()->hp == 1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x - 200, position.y + 200, position.z);
		modelStack.Scale(scale.x - 300, scale.y - 300, scale.z - 270);
		RenderHelper::RenderMesh(Half_HP);
		modelStack.PopMatrix();
	}

	if (CPlayerInfo::GetInstance()->hp == 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x - 200, position.y + 200, position.z);
		modelStack.Scale(scale.x - 300, scale.y - 300, scale.z - 270);
		RenderHelper::RenderMesh(Two_third_HP);
		modelStack.PopMatrix();
	}

	
	if (CPlayerInfo::GetInstance()->AmmoPrimary == false || CPlayerInfo::GetInstance()->AmmoSecondary == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x -300, position.y - 200, position.z);
		modelStack.Scale(scale.x - 500, scale.y - 400, scale.z - 100);
		RenderHelper::RenderMesh(ammo_empty);
		modelStack.PopMatrix();
	}
	if (CPlayerInfo::GetInstance()->AmmoPrimary == true || CPlayerInfo::GetInstance()->AmmoSecondary == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x - 300, position.y -200, position.z);
		modelStack.Scale(scale.x - 500, scale.y - 400, scale.z - 500);
		RenderHelper::RenderMesh(ammo);
		modelStack.PopMatrix();

	}

	
	
}

CCameraEffects* Create::CameraEffects(const bool m_bAddToLibrary)
{
	CCameraEffects* result = CCameraEffects::GetInstance();
	if (m_bAddToLibrary)
		EntityManager::GetInstance()->AddEntity(result,false);
	return result;
}
