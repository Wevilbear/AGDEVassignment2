#include <iostream>
using namespace std;

#include "introstate.h"
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

CMenuState::CMenuState()
{

}
CMenuState::~CMenuState()
{

}

void CMenuState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//MenuState.tga");

	MeshBuilder::GetInstance()->GenerateQuad("startState", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("startState")->textureID = LoadTGA("Image//startState.tga");

	MeshBuilder::GetInstance()->GenerateQuad("shopState", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("shopState")->textureID = LoadTGA("Image//shopState.tga");

	MeshBuilder::GetInstance()->GenerateQuad("highScoreState", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("highScoreState")->textureID = LoadTGA("Image//highScoreState.tga");

	MeshBuilder::GetInstance()->GenerateQuad("gameMenuState", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("gameMenuState")->textureID = LoadTGA("Image//gameMenuState.tga");


	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("gameMenuState",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f), true);

	cout << "CMenuState loaded\n" << endl;

	cout << "SCENE MAIN MENU" << endl;
	cout << endl;

	bounceTime = 0.f;
	elapsedTime = 0.f;
	menuOption = 0;
}
void CMenuState::Update(double dt)
{
	elapsedTime += (float)dt;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
	{
		menuOption--;

		if (menuOption < 0)
		{
			menuOption = 3;
		}
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
	{
		menuOption++;

		if (menuOption > 3)
		{
			menuOption = 0;
		}
	}
	if  (KeyboardController::GetInstance()->IsKeyPressed('1'))
	{

		cout << "Loading CMenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
	
	}

	else if  (KeyboardController::GetInstance()->IsKeyPressed('2'))
	{
		cout << "Loading CMenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("ShopState");


	}
	else if  (KeyboardController::GetInstance()->IsKeyPressed('3'))

	{
		cout << "Loading CMenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("HighScoreState");

	}
	else if (KeyboardController::GetInstance()->IsKeyPressed('4'))
	{
		Application::exitApp = true;
	}


if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
{
	cout << "Loading CMenuState" << endl;
	SceneManager::GetInstance()->SetActiveScene("GameState");
}
}
void CMenuState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Render the required entities
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0,
		Application::GetInstance().GetWindowWidth(),
		0,
		Application::GetInstance().GetWindowHeight(),
		-10, 10);

	if (menuOption == 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.Scale((float)Application::GetInstance().GetWindowWidth(), (float)Application::GetInstance().GetWindowHeight(), 0.f);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("startState"));
		modelStack.PopMatrix();
	}
	else if (menuOption == 1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.Scale((float)Application::GetInstance().GetWindowWidth(), (float)Application::GetInstance().GetWindowHeight(), 0.f);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shopState"));
		modelStack.PopMatrix();
	}
	else if (menuOption == 2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.Scale((float)Application::GetInstance().GetWindowWidth(), (float)Application::GetInstance().GetWindowHeight(), 0.f);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("highScoreState"));
		modelStack.PopMatrix();
	}
	else if (menuOption == 3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.Scale((float)Application::GetInstance().GetWindowWidth(), (float)Application::GetInstance().GetWindowHeight(), 0.f);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("quitState"));
		modelStack.PopMatrix();
	}



	GraphicsManager::GetInstance()->DetachCamera();
	modelStack.LoadIdentity();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();
}
void CMenuState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	// Remove the meshes which are specific to CMenuState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}