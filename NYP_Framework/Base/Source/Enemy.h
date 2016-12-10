#pragma once
#include "../Source/GenericEntity.h"
#include "../Source/GroundEntity.h"

class Mesh;

class CEnemy : public GenericEntity
{
protected:
    Vector3 defaultPosition, defaultTarget, defaultUp;
    Vector3 target, up;
    Vector3 maxBoundary, minBoundary;
    GroundEntity* m_pTerrain;
    double m_dSpeed;
    double m_dAcceleration;
public:
    CEnemy(void);
    virtual ~CEnemy();

    void Init(/*GroundEntity* ground*/);
    void Reset(void);
    void SetPos(const Vector3& pos);
    void SetTarget(const Vector3& target);
    void SetUp(const Vector3& up);
    void SetBoundary(Vector3 max, Vector3 min);
    void SetTerrain(GroundEntity* m_pTerrain);

    Vector3 GetPos(void) const;
    Vector3 GetTarget(void) const;
    Vector3 GetUp(void) const;

    GroundEntity* GetTerrain(void);
    void Update(double dt = 0.0333f);
    void Constrain(void);
    void Render(void);
};