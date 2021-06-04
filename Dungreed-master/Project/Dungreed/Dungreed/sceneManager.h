#pragma once
#include "singletonBase.h"
#include <string>
#include <map>

class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<string, gameNode*>           mapSceneList;
	typedef map<string, gameNode*>::iterator mapSceneIter;

private:
	static gameNode* _currentScene;  // ÇöÀç ¾À
	static gameNode* _loadingScene;	 // ·Îµù ¾À
	static gameNode* _readyScene;	 // ´ë±â ¾À

	mapSceneList _mSceneList;
	mapSceneList _mnLoadingSceneList;
public:
	sceneManager();
	~sceneManager();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//¾À Ãß°¡ ÇÔ¼ö
	gameNode* addScene(string sceneName, gameNode* scene);
	//¾À º¯°æ
	HRESULT changeScene(string sceneName);
};

