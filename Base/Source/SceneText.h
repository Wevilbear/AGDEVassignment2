#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "Enemy/Enemy3D.h"
#include "HardwareAbstraction\Keyboard.h"
#include "Minimap\Minimap.h"
#include "CameraEffects\CameraEffects.h"
#include "HardwareAbstraction\Mouse.h"
#include "Projectile\Projectile.h"
#include "SceneGraph\UpdateTransformation.h"
#include "Enemy\Enemy3D.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizeX, float sizeY);
	void SceneText::RenderMesh(Mesh *mesh, bool enableLight);
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_WALL,
		GEO_PILLAR,
		GEO_PADDLE,
		GEO_FLOOR,
		GEO_BACKGROUND,
		GEO_BORDER,
		NUM_GEOMETRY,
	};

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[4];
	Light* lights[2];

	CKeyboard* theKeyboard;
	CMouse* theMouse;
	CMinimap* theMinimap;
	CCameraEffects* theCameraEffects;

	GenericEntity* theCube;
	CEnemy3D* anEnemy3D;	// This is the CEnemy class for 3D use.

	static SceneText* sInstance; // The pointer to the object that gets registered

	//Collision
	std::list<GenericEntity*> Objects;
	std::list<CProjectile*> Projectiles;
	std::list<CEnemy3D*> Enemy;



protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	

	MS modelStack;
	MS viewStack;
	MS projectionStack;


	bool bLightEnabled;

	float fps;
};

#endif