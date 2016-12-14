#include "EntityBase.h"

EntityBase::EntityBase() 
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, isDone(false)
	, m_bCollider(false)
	, bLaser(false)
    , bCube(false)
    , isTarget(false)
<<<<<<< HEAD
	, TargetActivated(false)
=======
    , isText(false)
>>>>>>> 660765b2db1261dc75c28a1be01fd6ffc960ecb7
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::Update(double _dt)
{
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}

// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool _value)
{
	m_bCollider = _value;
}

void EntityBase::SetIsLaser(const bool bLaser)
{
	this->bLaser = bLaser;
}

bool EntityBase::GetIsLaser(void) const
{
	return bLaser;
}
