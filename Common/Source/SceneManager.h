#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SingletonTemplate.h"
#include <map>
#include <string>

using std::map;

class Scene;

class SceneManager : public Singleton<SceneManager>
{
	friend Singleton<SceneManager>;
public:
	// System Interface
	void Update(double _dt);
	void Render();
	void Exit();

	// User Interface
	void AddScene(const std::string& _name, Scene* _scene);
	void RemoveScene(const std::string& _name);
	void SetActiveScene(const std::string& _name);
	bool CheckSceneExist(const std::string& _name);

private:
	SceneManager();
	~SceneManager();

	std::map<std::string, Scene*> sceneMap;
	Scene* activeScene, *nextScene;

	static SceneManager* only_instance;
	map <unsigned, Scene*> sceneStorage;	//database for scene
	int currSceneID, nextSceneID;			//to iterate through the different scene
	bool b_isTime;					//check if time is up for timed scene;
	bool b_isNext;
	float f_sceneTimer;
};

#endif // SCENE_MANAGER_H