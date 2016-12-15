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
#include "UpdateTransform.h"


#include "Timer\Timer.h"

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

	//vector<GenericEntity*> target;
	GenericEntity* target;
    GenericEntity* timeBoard;
    Timer* timer;
    CEnemy* theEnemy;
	CUpdateTransformation* aRotateMtx;
	float targetY;
	float scoop;
	bool isScoop;

	GenericEntity* Head;
	GenericEntity* Body;
	GenericEntity* RightArm;
	GenericEntity* LeftArm;
	GenericEntity* Rightleg;
	GenericEntity* Leftleg;

	static SceneText* sInstance; // The pointer to the object that gets registered
};

#endif