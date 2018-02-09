#include <iostream>
using namespace std;

#include "HighScoreState.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "TextEntity.h"
#include "RenderHelper.h"
#include "SpriteEntity.h"
#include "EntityManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "CameraEffects\CameraEffects.h"
#include "LuaInterface.h"


CHighScoreState::CHighScoreState()
{

}
CHighScoreState::~CHighScoreState()
{

}

void CHighScoreState::Init()
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

	MeshBuilder::GetInstance()->GenerateQuad("gameHighScoreState", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("gameHighScoreState")->textureID = LoadTGA("Image//gameHighScoreState.tga");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	// Setup the 2D entities
	float halfWindowWidth1 = Application::GetInstance().GetWindowWidth() / 3.3f;
	float halfWindowHeight1 = Application::GetInstance().GetWindowHeight() / 2.3f;
	float fontSize = 30.0f;
	float halfFontSize1 = fontSize / 2.0f;
	for (int i = 0; i < 4; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth1, -halfWindowHeight1 + fontSize*i + halfFontSize1 + 200, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("gameHighScoreState",
		Vector3(0, 0, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f), true);

	cout << "CHighScoreState loaded\n" << endl;

	cout << "SCENE MAIN MENU" << endl;
	cout << endl;

	bounceTime = 0.f;
	elapsedTime = 0.f;
	menuOption = 0;

	
	
}
void CHighScoreState::Update(double dt)
{
	elapsedTime += (float)dt;
	GraphicsManager::GetInstance()->UpdateLights(dt);
	EntityManager::GetInstance()->Update(dt);
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
	if (KeyboardController::GetInstance()->IsKeyPressed('Q'))
	{

		cout << "Loading CHighScoreState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");

	}


	else if (KeyboardController::GetInstance()->IsKeyPressed('4'))
	{
		Application::exitApp = true;
	}


	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading CHighScoreState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
	}

	std::ostringstream ss4;
	ss4.precision(15);
	
	luaL_dofile(CLuaInterface::GetInstance()->theLuaState, "Image//DM2240_HighScore.lua");
	float value=	CLuaInterface::GetInstance()->getFloatValue("Player1");
	ss4 << "Player 1 Score: "<< value;
	textObj[2]->SetText(ss4.str());
}
void CHighScoreState::Render()
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



	//GraphicsManager::GetInstance()->DetachCamera();
	//modelStack.LoadIdentity();

	// Render the required entities
	//EntityManager::GetInstance()->RenderUI();

	// Enable blend mode
	glEnable(GL_BLEND);

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();


	


	// Disable blend mode
	glDisable(GL_BLEND);
}

void CHighScoreState::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizeX, float sizeY)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode

	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate((float)x, (float)y, 0);
	modelStack.Scale((float)sizeX, (float)sizeY, 1);
	//to do: scale and translate accordingly
	//RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);

}
void CHighScoreState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	// Remove the meshes which are specific to CHighScoreState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");
	EntityManager::GetInstance()->entityList.clear();
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}