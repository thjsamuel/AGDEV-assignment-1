#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile\Laser.h"
#include "SceneGraph.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	CSceneGraph::GetInstance()->Update();

	if (theSpatialPartition)
		theSpatialPartition->Update();

	CheckForCollision();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
//if ((*it)->isTarget == false)
        if ((*it)->IsDone() == false)
		    (*it)->Render();
	}

	CSceneGraph::GetInstance()->Render();

	if (theSpatialPartition)
		theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition)
{
	entityList.push_back(_newEntity);
	// Add to the Spatial Partition
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}


// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity) == false)
			cout << "EntityManager::RemoveEntity: Unable to remove.";
		else
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		return true;	
	}
	// Return false if not found
	return false;
}

bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	return false;
}

void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	//if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	//	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	//	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	//	||
	//	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	//	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	//	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	//{
	//	return true;
	//}
	//return false;
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	// Check if that object is overlapping this object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMinAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMinAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMinAABB.z <= thatMaxAABB.z))
	||
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	// Check if this object is within that object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z))
	&&
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	// Check if that object is within this object
	/*
	if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	&&
	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	*/
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) > DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}
	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	CCollider * thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thatMinAABB.z);

	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;
	return false;
}

bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
    if ((fDst1 * fDst2) >= 0.0f)
        return false;
    if (fDst1 == fDst2)
        return false;
    Hit = P1 + (P2 - P1) * (fDst1 / (fDst2 - fDst1));
    return true;
}

bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
    if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
    if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
    if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
    return false;
}

bool EntityManager::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit)
		&& InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit)
		&& InBox(Hit, minAABB, maxAABB, 2))
		||
		(GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit)
		&& InBox(Hit, minAABB, maxAABB, 3))
		||
		(GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit)
		&& InBox(Hit, minAABB, maxAABB, 1))
		||
		(GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit)
		&& InBox(Hit, minAABB, maxAABB, 2))
		||
		(GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit)
		&& InBox(Hit, minAABB, maxAABB, 3)))
		return true;
	return false;
}


// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();

	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
        if ((*colliderThis)->GetIsLaser()/* || (*colliderThis)->bCube*/)
		{
            CProjectile* thisEntity;
            if ((*colliderThis)->GetIsLaser())
			    thisEntity = dynamic_cast<CLaser*>(*colliderThis);
            else
                thisEntity = dynamic_cast<CProjectile*>(*colliderThis);

			colliderThatEnd = entityList.end();
            int counter = 0;
            //std::list<EntityBase*>::iterator next = colliderThis++;
            for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
            {
                if (colliderThat == colliderThis)
                    continue;

                if ((*colliderThat)->HasCollider())
                {
                    Vector3 hitPosition = Vector3(0, 0, 0);
                    CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
                    Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
                    Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();
                    if (CheckLineSegmentPlane(thisEntity->GetPosition(), thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(), thatMinAABB, thatMaxAABB, hitPosition) == true)
                    {
                        (*colliderThis)->SetIsDone(true);
                        (*colliderThat)->SetIsDone(true);

                        CSceneGraph::GetInstance()->DeleteNode((*colliderThis));
                        CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
                    }
                }
            }
		}
        else if ((*colliderThis)->HasCollider() && (*colliderThis)->isWall == false)
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					// This object was derived from a CCollider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity) == true)
					{
						if (CheckAABBCollision(thisEntity, thatEntity) == true)
						{
                                thisEntity->SetIsDone(true);
                                thatEntity->SetIsDone(true);

                                if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
                                    cout << "*** This Entity removed ***" << endl;
                                if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
                                    cout << "*** That Entity removed ***" << endl;
						}
                        else
                        {
                            CProjectile* thatEntity = dynamic_cast<CProjectile*>(*colliderThat);
                            if (thatEntity == NULL)
                                continue;
                            Vector3 hitPosition = Vector3(0, 0, 0);
                            CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
                            Vector3 thisMinAABB = (*colliderThis)->GetPosition() + thisCollider->GetMinAABB();
                            Vector3 thisMaxAABB = (*colliderThis)->GetPosition() + thisCollider->GetMaxAABB();
                            if (CheckLineSegmentPlane(thatEntity->GetPosition(), thatEntity->GetPosition() - thatEntity->GetDirection() * 10, thisMinAABB, thisMaxAABB, hitPosition) == true)
                            {
                                (*colliderThis)->SetIsDone(true);
                                (*colliderThat)->SetIsDone(true);

                                CSceneGraph::GetInstance()->DeleteNode((*colliderThis));
                                CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
                            }
                        }
					}
				}
			}
		}
	}
	return false;
}
