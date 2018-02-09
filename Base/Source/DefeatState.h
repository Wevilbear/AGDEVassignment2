#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "FPSCamera.h"
#include "SpriteEntity.h"
#include "Spatial Partition\SpatialPartition.h"

class SceneManager;
//class TextEntity;
class CDefeatState : public Scene
{
public:
	CDefeatState();
	~CDefeatState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* IntroStateBackground;
	//	TextEntity* textObj[3];
};
