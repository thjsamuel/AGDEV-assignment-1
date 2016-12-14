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
#include "Enemy.h"
<<<<<<< HEAD
#include "UpdateTransform.h"


=======
#include "Timer\Timer.h"
>>>>>>> 660765b2db1261dc75c28a1be01fd6ffc960ecb7

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

	void TargetUpdate(double dt);

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[4];
	Light* lights[2];

<<<<<<< HEAD
	//vector<GenericEntity*> target;
	GenericEntity* target;
=======
    GenericEntity* target;
    GenericEntity* timeBoard;
    Timer* timer;
>>>>>>> 660765b2db1261dc75c28a1be01fd6ffc960ecb7
    CEnemy* theEnemy;
	CUpdateTransformation* aRotateMtx;
	float targetY;

	static SceneText* sInstance; // The pointer to the object that gets registered
};

#endif