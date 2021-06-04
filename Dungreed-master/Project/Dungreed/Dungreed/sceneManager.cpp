#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

//스태틱으로 선언 되어있으므로 초기화는 여기서
gameNode* sceneManager::_currentScene = NULL;
gameNode* sceneManager::_loadingScene = NULL;
gameNode* sceneManager::_readyScene = NULL;
sceneManager::sceneManager()
{
}

sceneManager::~sceneManager()
{
}

HRESULT sceneManager::init(void)
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;

	return S_OK;
}

void sceneManager::release(void)
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			//만약 현재 돌아가는 씬일 경우
			if (miSceneList->second == _currentScene)
			{
				miSceneList->second->release();
			}
			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}
	_mSceneList.clear();
}

void sceneManager::update(void)
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render(void)
{
	if (_currentScene) _currentScene->render();
}

//씬 추가 함수
gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}
//씬 변경
HRESULT sceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//이터레이터가 맵의 끝까지 갔다 == 찾고자 하는게 없다 
	if (find == _mSceneList.end()) return E_FAIL;

	if (find->second == _currentScene) return S_OK;

	//만약에 너희들이 init을 부르고 싶지 않으면 바꾸면 된다.
	//성공적으로 씬이 바뀐다면 init함수 실행
	if (SUCCEEDED(find->second->init()))
	{
		//만약 현재씬에 다른 씬이 있다면 해제 해주고
		if (_currentScene) _currentScene->release();

		//바꾸려는 씬으로 체인지 한다.
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}
