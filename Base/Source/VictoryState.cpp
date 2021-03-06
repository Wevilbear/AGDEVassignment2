#include <iostream>
using namespace std;

#include "VictoryState.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "TextEntity.h"
#include "RenderHelper.h"
#include "SpriteEntity.h"
#include "EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"

CVictoryState::CVictoryState()
{

}
CVictoryState::~CVictoryState()
{

}

void CVictoryState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("gameMenuState", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("gameMenuState")->textureID = LoadTGA("Image//gameMenuState.tga");
	MeshBuilder::GetInstance()->GenerateQuad("VictoryState", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VictoryState")->textureID = LoadTGA("Image//gameVictoryState.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	IntroStateBackground = Create::Sprite2DObject("VictoryState", 
													Vector3(halfWindowWidth, halfWindowHeight, 0.0f), 
													Vector3(800.0f, 600.0f, 0.0f), true);

	cout << "CVictoryState loaded\n" << endl;
}
void CVictoryState::Update(double dt)
{
	
	if (KeyboardController::GetInstance()->IsKeyPressed('Q'))
	{

		cout << "Loading VictoryState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");

	}
}
void CVictoryState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, 
															  Application::GetInstance().GetWindowWidth(), 
															  0, 
															  Application::GetInstance().GetWindowHeight(), 
															  -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();
}
void CVictoryState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(IntroStateBackground);

	// Remove the meshes which are specific to CVictoryState
	MeshBuilder::GetInstance()->RemoveMesh("VictoryState");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}