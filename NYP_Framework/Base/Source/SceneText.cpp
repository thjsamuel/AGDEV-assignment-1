#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph.h"


#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
    MeshBuilder::GetInstance()->GenerateText("world text", 16, 16);
    MeshBuilder::GetInstance()->GetMesh("world text")->textureID = LoadTGA("Image//calibri.tga");
    MeshBuilder::GetInstance()->GetMesh("world text")->material.kAmbient.Set(1, 0, 0);
	//MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	//MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f,1.f,1.f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.f);
	// Robot head
	MeshBuilder::GetInstance()->GenerateCube("Head", Color(1.0f, 0.0f, 0.0f), 5.0f,5.f,5.f);
	MeshBuilder::GetInstance()->GenerateSphere("Headsphere", Color(1, 0, 0), 18, 36, 1.f);
	// Robot body
	MeshBuilder::GetInstance()->GenerateCube("Body", Color(0.0f, 0.0f, 0.5f), 10.0f,10.f,10.f);
	MeshBuilder::GetInstance()->GenerateSphere("Bodysphere", Color(1, 0, 0), 18, 36, 1.f);
	// Robot right arm
	MeshBuilder::GetInstance()->GenerateCube("RightArm", Color(0.0f, 0.5f, 0.0f), 3.0f,10.f,3.f);
	MeshBuilder::GetInstance()->GenerateSphere("RightArmsphere", Color(1, 0, 0), 18, 36, 1.f);
	// Robot left arm
	MeshBuilder::GetInstance()->GenerateCube("LeftArm", Color(0.0f, 0.5f, 0.0f), 3.0f, 10.f, 3.f);
	MeshBuilder::GetInstance()->GenerateSphere("LeftArmsphere", Color(1, 0, 0), 18, 36, 1.f);
	// Robot right leg
	MeshBuilder::GetInstance()->GenerateCube("RightLeg", Color(0.0f, 0.5f, 0.5f), 3.0f, 15.f, 3.f);
	MeshBuilder::GetInstance()->GenerateSphere("RightLegsphere", Color(1, 0, 0), 18, 36, 1.f);
	// Robot left leg
	MeshBuilder::GetInstance()->GenerateCube("LeftLeg", Color(0.0f, 0.5f, 0.5f), 3.0f,15.f,3.f);
	MeshBuilder::GetInstance()->GenerateSphere("LeftLegsphere", Color(1, 0, 0), 18, 36, 1.f);

	MeshBuilder::GetInstance()->GenerateCube("test", Color(1.0f, 1.0f, 0.f), 3.0f, 15.f, 3.f);

	CSpatialPartition::GetInstance()->Init(150, 150, 10, 10);
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f, 1.f, 1.f);
    MeshBuilder::GetInstance()->GenerateSphere("Bullet", Color(1.0f, 1.0f, 0.0f), 10, 15, 0.1f);
    MeshBuilder::GetInstance()->GenerateQuad("target", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GetMesh("target")->textureID = LoadTGA("Image//Misc//target.tga");
    MeshBuilder::GetInstance()->GenerateQuad("machine gun", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GetMesh("machine gun")->textureID = LoadTGA("Image//Weapons//M4A1.tga");
    MeshBuilder::GetInstance()->GenerateQuad("laser gun", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GetMesh("laser gun")->textureID = LoadTGA("Image//Weapons//lasergun.tga");
    MeshBuilder::GetInstance()->GenerateQuad("sniper rifle", Color(1, 1, 1), 1.f);
    MeshBuilder::GetInstance()->GetMesh("sniper rifle")->textureID = LoadTGA("Image//Weapons//sniper.tga");
    MeshBuilder::GetInstance()->GenerateCube("wallblack", Color(0.f, 0.f, 0.f), 1.0f);
    MeshBuilder::GetInstance()->GetMesh("wallblack")->textureID = LoadTGA("Image//Objects//Cinderblock.tga");
    MeshBuilder::GetInstance()->GenerateCube("wall", Color(0.75f, 0.75f, 0.75f), 1.0f);
    MeshBuilder::GetInstance()->GetMesh("wall")->textureID = LoadTGA("Image//Objects//Cinderblock.tga");
    MeshBuilder::GetInstance()->GenerateOBJ("wallobj", "OBJ//wall.obj");
    MeshBuilder::GetInstance()->GetMesh("wallobj")->textureID = LoadTGA("Image//Objects//Cinderblock.tga");
    MeshBuilder::GetInstance()->GenerateCube("tiny", Color(0.75f, 0.75f, 0.75f), 1.0f, 1.0f,1.0f);
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);

	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 160000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	// Robot head
	Head = Create::Entity("Head", Vector3(0.0f, 20.0f, -20.0f));
	Head->SetCollider(true);
	Head->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	Head->InitLOD("Head", "sphere", "sphere");
	
	CSceneNode* theHead = CSceneGraph::GetInstance()->GetInstance()->AddNode(Head);
	// Robot head

	// Robot body
	 Body = Create::Entity("Body", Vector3(0.0f, 13.0f, -20.0f));
	Body->SetCollider(true);
	Body->SetAABB(Vector3(3.f, 3.f, 3.f), Vector3(-3.f, -3.f, -3.f));
	Body->InitLOD("Body", "sphere", "sphere");
	

	CSceneNode* theBody = theHead->AddChild(Body);
	
	// Robot body

	// Robot Rightarm
	RightArm = Create::Entity("RightArm", Vector3(6.0f, 10.0f, -20.f));
	RightArm->SetCollider(true);
	RightArm->SetAABB(Vector3(0.5f, 5.f, 0.5f), Vector3(-0.5f, -5.f, -0.5f));
	RightArm->InitLOD("RightArm", "sphere", "sphere");
	RightArm->ApplyRotate(45, 1.f, 0.f, 0.f);


	CSceneNode* theRightArm = theBody->AddChild(RightArm);
	//Robot Rightarm

	// Robot Leftarm
	 LeftArm = Create::Entity("LeftArm", Vector3(-6.0f, 10.0f, -20.f));
	LeftArm->SetCollider(true);
	LeftArm->SetAABB(Vector3(0.5f, 5.f, 0.5f), Vector3(-0.5f, -5.f, -0.5f));
	LeftArm->InitLOD("LeftArm", "sphere", "sphere");
	//LeftArm->ApplyTranslate(0, 0, 0);

	CSceneNode* theLeftArm = theBody->AddChild(LeftArm);
	// Robot Leftarm

	// Robot Rightleg
	Rightleg = Create::Entity("RightLeg", Vector3(2.0f, 0.0f, -20.f));
	Rightleg->SetCollider(true);
	Rightleg->SetAABB(Vector3(0.5f, 5.5f, 0.5f), Vector3(-0.5f, -5.5f, -0.5f));
	Rightleg->InitLOD("RightLeg", "sphere", "sphere");
	

	CSceneNode* theRightleg = theBody->AddChild(Rightleg);
	// Robot Rightleg

	// Robot Leftleg
	 Leftleg = Create::Entity("LeftLeg", Vector3(-2.0f, 0.0f, -20.f));
	Leftleg->SetCollider(true);
	Leftleg->SetAABB(Vector3(0.5f, 5.5f, 0.5f), Vector3(-0.5f, -5.5f, -0.5f));
	Leftleg->InitLOD("LeftLeg", "sphere", "sphere");


	CSceneNode* theLeftleg = theBody->AddChild(Leftleg);
	// Robot Leftleg

	//// test
	//GenericEntity* test = Create::Entity("Leg" , Vector3(0.0f, -3.0f, 0.f));
	//test->SetCollider(true);
	//test -> SetAABB(Vector3(0.5f, 7.5f, 0.5f), Vector3(-0.5f, -7.5f, -0.5f));
	//test->InitLOD("Leg", "Leg", "cubeSG");


	//CSceneNode* thetest = theBody->AddChild(test);
	//// test


	/*GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	aCube->InitLOD("cube", "sphere", "cubeSG");

	CSceneNode* theNode = CSceneGraph::GetInstance()->GetInstance()->AddNode(aCube);
	if (theNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}

	GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 5.1f, -20.f));
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode* anotherNode = CSceneGraph::GetInstance()->GetInstance()->AddNode(anotherCube);


	if (anotherNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}*/

    num_targets = 4;
    targetY = -15.f;
    for (int i = 0; i < num_targets; ++i)
    {
        Target* tempTarget = new Target();
        tempTarget->board = Create::Entity("target", Vector3(20.f, targetY, 200.0f));
        tempTarget->board->SetCollider(true);
        tempTarget->board->SetAABB(Vector3(5.0f, 5.0f, 5.0f), Vector3(-5.0f, -5.0f, -5.0f));
        tempTarget->board->isTarget = true;
        tempTarget->board->SetScale(Vector3(10, 10, 10));
        targets.push_back(*tempTarget);
    }
    targets[0].board->SetPositionX(-52.5f);
    targets[0].board->SetPositionZ(100.f);
    targets[1].board->SetPositionX(-26);
    targets[1].board->SetPositionZ(150.f);
    targets[2].board->SetPositionX(-1);
    //targets[2].board->SetPositionZ(54.f);

    targets[3].board->SetPositionX(-80);
    targets[3].board->SetPositionZ(66.f);

    for (int i = 0; i < num_targets; ++i)
    {
        Target* tempTarget = new Target();
        tempTarget->board = Create::Entity("target", Vector3(-200.f, targetY, 200.0f));
        tempTarget->board->SetCollider(true);
        tempTarget->board->SetAABB(Vector3(5.0f, 5.0f, 5.0f), Vector3(-5.0f, -5.0f, -5.0f));
        tempTarget->board->isTarget = true;
        tempTarget->board->SetScale(Vector3(10, 10, 10));
        targetsSide.push_back(*tempTarget);
    }

    targetsSide[0].board->SetPositionX(-200.5f);
    targetsSide[0].board->SetPositionZ(150.f);
    targetsSide[0].position = targetsSide[0].board->GetPosition();
    targetsSide[1].board->SetPositionX(-150);
    targetsSide[1].board->SetPositionZ(100.f);
    targetsSide[1].position = targetsSide[1].board->GetPosition();
    targetsSide[2].board->SetPositionX(-175);
    targetsSide[2].board->SetPositionZ(0);
    targetsSide[2].position = targetsSide[2].board->GetPosition();
    targetsSide[3].board->SetPositionX(-200);
    targetsSide[3].board->SetPositionZ(-50.f);
    targetsSide[3].position = targetsSide[3].board->GetPosition();

    for (int i = 0; i < num_targets; ++i)
    {
        target = Create::Entity("tiny", Vector3(-200.f, 0, 200.0f));
        target->SetCollider(false);
        //target->SetAABB(Vector3(5.0f, 5.0f, 5.0f), Vector3(-5.0f, -5.0f, -5.0f));
        //target->isTarget = true;
        target->SetScale(Vector3(0.001f, 0.001f, 0.001f));
        Target tempTarget(target);
        targetTrigger.push_back(tempTarget);
    }

    //targetTrigger[0].position = targets[5].board->GetPosition();
    //targetTrigger[1].position = Vector3(20, 0, 50);

 //   CSceneNode* targetInGrid = CSceneGraph::GetInstance()->AddNode(target);
	//target->ApplyTranslate(0.0f, 0.0f, 0.f);

	//aRotateMtx = new CUpdateTransformation();
	//aRotateMtx->ApplyUpdate(1.0f, 1.0f, 0.0f, 0.0f);
	//aRotateMtx->SetSteps(-90, 10);
	//target->SetUpdateTransformation(aRotateMtx);
	//aRotateMtx->rotateDown = true;
	
	

	//GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);


	//GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* childNode = baseNode->AddChild(childCube);
	//childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);
	//
	//GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	//grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);

	//CUpdateTransformation* bRotateMtx = new CUpdateTransformation();
	//bRotateMtx->ApplyUpdate(1.0f, 0.0f, 1.0f, 0.0f);
	//bRotateMtx->SetSteps(-120, 60);
	//grandchildNode->SetUpdateTransformation(bRotateMtx);

    //CSceneNode* targetInGrid = CSceneGraph::GetInstance()->GetInstance()->AddNode(target);

    timeBoard = Create::Entity("cubeSG", Vector3(50.f, 0.0f, -250.0f), Vector3(15, 5, 5));
    timeBoard->isText = true;
    timeBoard->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
    timeBoard->InitLOD("cubeSG", "cubeSG", "cubeSG");

    timer = new Timer(300.f);

	//GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);
	//GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* childNode = baseNode->AddChild(childCube);
	//childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);
	//GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	//grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);
    
    theEnemy = new CEnemy[10];
    //for (int i = 0; i < 10; ++i)
    {
        //theEnemy[i].Init();
    }
	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(2.5f, -2.5f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);

    num_walls = 10;
    for (int i = 0; i < num_walls; ++i)
    {
        GenericEntity* blockade = Create::Entity("wall", Vector3(0, 0.0f, i * -200.0f), Vector3(400.f, 15.0f, 5.0f));
        blockade->SetCollider(true);
        blockade->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
        //4blockade->InitLOD("wall", "wall", "wall");
        blockade->isWall = true;
        collidableWalls.push_back(blockade);
    }

    float joinX = collidableWalls[0]->GetPosition().x - collidableWalls[0]->GetScaleE().x * 0.5f;
    float joinZ = collidableWalls[0]->GetPosition().z - collidableWalls[0]->GetScaleE().z * 0.5f;
    collidableWalls[0]->SetPosition(Vector3(0, -8.5, 200));
    collidableWalls[1]->SetScale(Vector3(5.f, 15.0f, 400.0f));
    collidableWalls[1]->SetPosition(Vector3(joinX, -8.5f, 0));
    collidableWalls[2]->SetScale(Vector3(5.f, 15.0f, 400.0f));
    collidableWalls[2]->SetPosition(Vector3(-joinX, -8.5f, 0));
    collidableWalls[3]->SetPosition(Vector3(0, -8.5f, -200));
    collidableWalls[4]->SetPosition(Vector3(0, -8.5f, 250));
    collidableWalls[5] = Create::Entity("wallblack", Vector3(0, 3.5f, 55), Vector3(200.f, 30.0f, 20.0f)); // try to make 50 no align with the grid cuz need space for targets to be pasted on the wall, might crash otherwise

    for (int i = 0; i < num_walls; ++i)
    {
        Vector3 objPos = collidableWalls[i]->GetPosition();
        Vector3 objSca = collidableWalls[i]->GetScaleE();
        playerInfo->CollideFront(objPos, objSca);
    }

    //for (int i = 0; i < 10; ++i)
    {
        //theEnemy[i].SetTerrain(groundEntity);
    }

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	//textObj[0]->SetText("HELLO WORLD");
    textObj[2]->SetPosition(Vector3(textObj[2]->GetPosition().x, textObj[2]->GetPosition().y - 45, textObj[2]->GetPosition().z));
    Vector3 timePos = timeBoard->GetPosition();
    timePos.x -= 7.f;
    timePos.z += 4.f;
    textObj[3] = Create::Text3DObject("world text", timePos, "", Vector3(fontSize / 20, fontSize /20, fontSize / 20), Color(1, 0, 0));

	scoop = 45;
	isScoop = false;

    for (int i = 0; i < 4; ++i)
    {
        textUI[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.5f, 0.5f, 0.5f));
    }
    mcsprite = nullptr; pisprite = nullptr; sisprite = nullptr;
    canAppear = false;
}

//void SceneText::TargetUpdate(double dt)
//{
//    int index = 0;
//    GenericEntity* temp;
//	// to add to child, just replaace playerinfo with target.pos
//	vector<EntityBase*> list = CSpatialPartition::GetInstance()->GetObjects(playerInfo->GetPos(), 1.0f);
//	for (int i = 0; i < list.size(); ++i)
//	{
//        temp = dynamic_cast<GenericEntity*>(list[i]);
//        for (int j = 0; j < targetTrigger.size(); ++j)
//        {
//            if (temp == targetTrigger[j].board)
//            {
//                vector<EntityBase*> grid = CSpatialPartition::GetInstance()->GetObjects(targetTrigger[j].position, 1.0f);
//                if (grid.size() != 0)
//                    for (int k = 0; k < grid.size(); ++k)
//                    {
//                        if (grid[k]->isTarget)
//                        {
//                            //if (EntityManager::GetInstance()->CheckerTarget == true)
//                            {
//                                if (grid[k]->GetPosition().y < 5)
//                                    grid[k]->SetPositionY(grid[k]->GetPosition().y + (2 * dt));
//                                else
//                                    grid[k]->SetPositionY(5.f);
//                            }
//                        }
//                    }
//            }
//        }
//	}
//}

void SceneText::TargetUpdate(double dt)
{
    const int height = 5;
    // to add to child, just replaace playerinfo with target.pos
    if (canAppear == false)
        userSpace = CSpatialPartition::GetInstance()->GetObjects(playerInfo->GetPos(), 1.0f);
    if (userSpace.size() != 0)
    for (int i = 0; i < userSpace.size(); ++i)
    {
        if (userSpace[i] != nullptr)
        {
            if (userSpace[i]->isTarget)
            {
                canAppear = true;
                //if (EntityManager::GetInstance()->CheckerTarget == true)
                {
                    if (userSpace[i]->GetPosition().y < height)
                        userSpace[i]->SetPositionY(userSpace[i]->GetPosition().y + (10 * dt));
                    else
                    {
                        userSpace[i]->SetPositionY(height);
                        canAppear = false;
                        timer->run = true;
                        break;
                    }
                }
            }
        }
    }
    if (!canAppear && targetsSide[0].position.y >= height)
    {
        for (int i = 0; i < targetsSide.size(); ++i)
        {
            float rx = Math::RandFloatMinMax(-200, -130);
            float ry = Math::RandFloatMinMax(height, 12 * height);
            float rz = Math::RandFloatMinMax(-50, 150);
            if (targetsSide[i].destination == Vector3(0, 0, 0))
                targetsSide[i].destination.Set(rx, ry, rz);
            targetsSide[i].Update(dt);
        }
    }
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if (KeyboardController::GetInstance()->IsKeyDown('8'))
	{
		scoop = 5;
		isScoop = true;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('9'))
	{
		scoop = 45;
		isScoop = false;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);


	// if the left mouse button was released
	//if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	//{
	//	cout << "Left Mouse Button was released!" << endl;
	//}
	//if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	//{
	//	cout << "Right Mouse Button was released!" << endl;
	//}
	//if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	//{
	//	cout << "Middle Mouse Button was released!" << endl;
	//}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	//{
	//	cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	//}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	//{
	//	cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	//}
	// <THERE>

    timer->Update(dt, 5);
     
    // to add to child, just replace playerinfo with target.pos
    vector<EntityBase*> list = CSpatialPartition::GetInstance()->GetObjects(playerInfo->GetPos(), 1.0f);
    for (int i = 0; i < list.size(); ++i)
    {
        //if (list[i]->isTarget)
          //  CSceneGraph::GetInstance()->DeleteNode(list[i]);
        if (list[i]->isText)
        {
            //Timer* tempTimer = dynamic_cast<Timer*>(list[i]);
            timer->run = !timer->run;
        }
    }

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);
	TargetUpdate(dt);
    if (prevWeapon != playerInfo->GetWeaponType())
    {
        if (playerInfo->GetWeaponType() == playerInfo->MACHINEGUN)
        {
            mcsprite = Create::Sprite2DObject("machine gun", Vector3(100.0f, -130.0f, 1.0f), Vector3(1024.0f, 450.0f, 300.0f));
            prevWeapon = playerInfo->GetWeaponType();
        }
        else if (mcsprite != nullptr && !mcsprite->IsDone())
        {
            mcsprite->SetIsDone(true);
            mcsprite = nullptr;
            delete mcsprite;
        }
        if (playerInfo->GetWeaponType() == playerInfo->SNIPERRIFLE)
        {
            sisprite = Create::Sprite2DObject("sniper rifle", Vector3(100.0f, -130.0f, 1.0f), Vector3(1024.0f, 450.0f, 300.0f));
            prevWeapon = playerInfo->GetWeaponType();
        }
        else if (sisprite != nullptr && !sisprite->IsDone())
        {
            sisprite->SetIsDone(true);
            sisprite = nullptr;
            delete sisprite;
        }
        if (playerInfo->GetWeaponType() == playerInfo->PISTOL)
        {
            pisprite = Create::Sprite2DObject("laser gun", Vector3(100.0f, -130.0f, -2.0f), Vector3(1200.0f, 600.0f, 300.0f));
            prevWeapon = playerInfo->GetWeaponType();
        }
        else if (pisprite != nullptr && !pisprite->IsDone())
        {
            pisprite->SetIsDone(true);
            pisprite = nullptr;
            delete pisprite;
        }
    }

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	//ss.precision(5);
	//float fps = (float)(1.f / dt);
	//ss << "FPS: " << fps;
	//textObj[1]->SetText(ss.str());

	//std::ostringstream ss1;
	//ss1.precision(4);
	//ss1 << "Player:" << playerInfo->GetPos();
	//textObj[2]->SetText(ss1.str());

	/*if (target->TargetActivated == false)
		target->SetUpdateTransformation(aRotateMtx);

	if (aRotateMtx->curSteps >= 0)
		target->TargetActivated = true;*/
	//aRotateMtx->Update();
	

	//cout << aRotateMtx->curSteps << endl;

    ss.str("");
    ss.precision(4);
    ss << "Time: " << timer->countdown;
    textObj[3]->SetText(ss.str());

	if (EntityManager::GetInstance()->CheckerHead == true)
	{
		if (isScoop)
			Head->InitLOD("Head", "Head", "Head");
		else
			Head->InitLOD("Headsphere", "Headsphere", "Headsphere");
	}
	if (EntityManager::GetInstance()->CheckerBody == true)
	{
		if (isScoop)
			Body->InitLOD("Body", "Body", "Body");
		else
			Body->InitLOD("Bodysphere", "Bodysphere", "Bodysphere");
	}
	if (EntityManager::GetInstance()->CheckerRightArm == true)
	{
		if (isScoop)
			RightArm->InitLOD("RightArm", "RightArm", "RightArm");
		else
			RightArm->InitLOD("RightArmsphere", "RightArmsphere", "RightArmsphere");
	}
	if (EntityManager::GetInstance()->CheckerLeftArm == true)
	{
		if (isScoop)
			LeftArm->InitLOD("LeftArm", "LeftArm", "LeftArm");
		else
			LeftArm->InitLOD("LeftArmsphere", "LeftArmsphere", "LeftArmsphere");
	}
	if (EntityManager::GetInstance()->CheckerRightleg == true)
	{
		if (isScoop)
			Rightleg->InitLOD("RightLeg", "RightLeg", "RightLeg");
		else
			Rightleg->InitLOD("RightLegsphere", "RightLegsphere", "RightLegsphere");
	}
	if (EntityManager::GetInstance()->CheckerLeftleg == true)
	{
		if (isScoop)
			Leftleg->InitLOD("LeftLeg", "LeftLeg", "LeftLeg");
		else
			Leftleg->InitLOD("LeftLegsphere", "LeftLegsphere", "LeftLegsphere");
	}
    int windowWidth = Application::GetInstance().GetWindowWidth();
    int windowHeight = Application::GetInstance().GetWindowHeight();

    if (playerInfo->GetWeaponType() == playerInfo->MACHINEGUN || playerInfo->GetWeaponType() == playerInfo->SNIPERRIFLE)
    {
        ss.str("");
        ss.precision(4);
        ss << playerInfo->GetPrimaryWeapon().GetMagRound() << " / " << playerInfo->GetPrimaryWeapon().GetTotalRound();
        textUI[0]->SetPosition(Vector3(windowWidth * 0.25f, -285, 1.1f));
        textUI[0]->SetText(ss.str());
    }
    else if (playerInfo->GetWeaponType() == playerInfo->PISTOL)
    {
        ss.str("");
        ss.precision(4);
        ss << playerInfo->GetSecondaryWeapon().GetMagRound() << " / " << playerInfo->GetSecondaryWeapon().GetTotalRound();
        textUI[0]->SetPosition(Vector3(windowWidth * 0.25f, -285, 1.1f));
        textUI[0]->SetText(ss.str());
    }
        ss.str("");
        ss.precision(4);
        ss << playerInfo->GetTertiaryWeapon().GetMagRound() << " / " << playerInfo->GetTertiaryWeapon().GetTotalRound();
        textUI[1]->SetPosition(Vector3(windowWidth * 0.325f, 250, 0));
        textUI[1]->SetColor(Color(0, 0, 1.f));
        textUI[1]->SetText(ss.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(scoop, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();
	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
