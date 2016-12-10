#include "Enemy.h"
#include "../Source/EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CEnemy::CEnemy() : GenericEntity(NULL), defaultPosition(Vector3(0.0f, 0.0f, 0.0f)), defaultTarget(Vector3(0.0f, 0.0f, 0.0f)), defaultUp(Vector3(0.0f, 0.0f, 0.0f)), target(Vector3(0.0f, 0.0f, 0.0f)), up(Vector3(0.0f, 0.0f, 0.0f)), maxBoundary(Vector3(0.0f, 0.0f, 0.0f)), minBoundary(Vector3(0.0f, 0.0f, 0.0f)), m_pTerrain(NULL)
{

}

CEnemy::~CEnemy()
{

}

void CEnemy::Init(/*GroundEntity* ground*/)
{
    // Set the default values
    defaultPosition.Set(0, 0, 10);
    defaultTarget.Set(0, 0, 0);
    defaultUp.Set(0, 1, 0);
    // Set the current values
    position.Set(10.0f, 0.0f, 0.0f);
    target.Set(10.0f, 0.0f, 450.0f);
    //scale.Set(50, 50, 50);
    up.Set(0.0f, 1.0f, 0.0f);
    // Set Boundary
    maxBoundary.Set(1, 1, 1);
    minBoundary.Set(-1, -1, -1);
    // Set speed
    m_dSpeed = 1.0;
    // Set terrain
    //if (ground != NULL)
    //{
    //    this->m_pTerrain = ground;
    //    SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
    //}
    // Initialise the LOD meshes
    InitLOD("cube", "sphere", "cubeSG");
    // Initialise the Collider
    this->SetCollider(true);
    this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
    // Add to EntityManager
    EntityManager::GetInstance()->AddEntity(this, true);
}

void CEnemy::Reset(void)
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

void CEnemy::SetPos(const Vector3& pos)
{
    position = pos;
}

void CEnemy::SetTarget(const Vector3& target)
{
    this->target = target;
}

void CEnemy::SetUp(const Vector3& up)
{
    this->up = up;
}

void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
    maxBoundary = max;
    minBoundary = min;
}

void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
    if (m_pTerrain != NULL)
    {
        this->m_pTerrain = m_pTerrain;
        SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
    }
}

Vector3 CEnemy::GetPos(void) const
{
    return position;
}

Vector3 CEnemy::GetTarget(void) const
{
    return target;
}

Vector3 CEnemy::GetUp(void) const
{
    return up;
}

GroundEntity* CEnemy::GetTerrain(void)
{
    return m_pTerrain;
}

void CEnemy::Update(double dt)
{
    Vector3 viewVector = (target - position).Normalized();
    position += viewVector * (float)m_dSpeed * (float)dt;

    Constrain();

    if (position.z > 400.0f)
        target.z = position.z * -1;
    else if (position.z < -400.0f)
        target.z = position.z * -1;
}

void CEnemy::Constrain(void)
{
    if (position.x > maxBoundary.x - 1.0f)
        position.x = maxBoundary.x - 1.0f;
    if (position.z > maxBoundary.z - 1.0f)
        position.z = maxBoundary.z - 1.0f;
    if (position.x < minBoundary.x + 1.0f)
        position.x = minBoundary.x + 1.0f;
    if (position.z < minBoundary.z + 1.0f)
        position.z = minBoundary.z + 1.0f;

    if (position.y != m_pTerrain->GetTerrainHeight(position))
    {
        position.y = m_pTerrain->GetTerrainHeight(position);
    }
}

void CEnemy::Render(void)
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Scale(scale.x, scale.y, scale.z);
    if (GetLODStatus() == true)
    {
        if (theDetailLevel != NO_DETAILS)
        {
            RenderHelper::RenderMesh(GetLODMesh());
        }
    }
    modelStack.PopMatrix();
}