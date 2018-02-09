#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "FPSCamera.h"
#include "SpriteEntity.h"
#include "CameraEffects\CameraEffects.h"
class TextEntity;
class SceneManager;
class CHighScoreState : public Scene
{
public:
	CHighScoreState();
	~CHighScoreState();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizeX, float sizeY);
	MS modelStack;
	MS viewStack;
	MS projectionStack;
	CCameraEffects* theCameraEffects;
	TextEntity* textObj[4];

private:
	FPSCamera camera;
	SpriteEntity* MenuStateBackground;
	//	TextEntity* textObj[3];

protected:
	enum MenuOption
	{
		START = 0,
		SHOP,
		HIGH_SCORE,
		QUIT
	};

	int menuOption;

	float elapsedTime;
	float bounceTime;

	float m_worldWidth;
	float m_worldHeight;

	float colorX, colorY, colorZ;
	bool setColor;
};
