#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Enemy\Enemy3D.h"
#include "SceneGraph\SceneGraph.h"
#include "SceneGraph\SceneNode.h"
#include <iostream>
#include "PlayerInfo\PlayerInfo.h"
using namespace std;

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

	//Add to the Spatial Partition
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

		//Remove from SceneNode too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingEntity))
		{
			cout << "EntityManager::RemoveEntity: Unable to remove Node";
		}
		else
		{
			//Remove from Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}
		return true;
	}
	// Return false if not found
	return false;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

Vector3 EntityManager::getMinBox(EntityBase * _entity)
{

	return Vector3((-_entity->getBoxSizeAABB().x / 2) + _entity->getPos().x,
		(-_entity->getBoxSizeAABB().y / 2) + _entity->getPos().y,
		(-_entity->getBoxSizeAABB().z / 2) + _entity->getPos().z);
}

Vector3 EntityManager::getMaxBox(EntityBase * _entity)
{
	return Vector3((_entity->getBoxSizeAABB().x / 2) + _entity->getPos().x,
		(_entity->getBoxSizeAABB().y / 2) + _entity->getPos().y,
		(_entity->getBoxSizeAABB().z / 2) + _entity->getPos().z);
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	return
		((thisMinAABB.x <= thatMaxAABB.x && thisMaxAABB.x >= thatMinAABB.x) && (thisMinAABB.y <= thatMaxAABB.y && thisMaxAABB.y >= thatMinAABB.y) && (thisMinAABB.z <= thatMaxAABB.z && thisMaxAABB.z >= thatMinAABB.z));
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{

	return false;
}

// Check if this entity collided with another entity, but both must have collider
//bool EntityManager::CheckAABBCollision(CCollider *ThisEntity, CCollider *ThatEntity)
//{
//	
//	return ((ThisEntity->GetMinAABB().x <= ThatEntity->GetMaxAABB().x&& ThisEntity->GetMaxAABB().x >= ThatEntity->GetMinAABB().x)&& (ThisEntity->GetMinAABB().y <= ThatEntity->GetMaxAABB().y&& ThisEntity->GetMaxAABB().y >= ThatEntity->GetMinAABB().y)&& (ThisEntity->GetMinAABB().z <= ThatEntity->GetMaxAABB().z&& ThisEntity->GetMaxAABB().z >= ThatEntity->GetMinAABB().z));
//}

bool EntityManager::CheckAABBCollision(EntityBase * ThisEntity, EntityBase * ThatEntity)
{
	Vector3 thisMin = getMinBox(ThisEntity);
	Vector3 thisMax = getMaxBox(ThisEntity);

	Vector3 thatMin = getMinBox(ThatEntity);
	Vector3 thatMax = getMaxBox(ThatEntity);

	if ((thisMin.x <= thatMax.x && thisMax.x >= thatMin.x) &&
		(thisMin.y <= thatMax.y && thisMax.y >= thatMin.y) &&
		(thisMin.z <= thatMax.z && thisMax.z >= thatMin.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	for (auto it = EntityManager::GetInstance()->GetEntityList().begin(); it != EntityManager::GetInstance()->GetEntityList().end(); it++)
	{
		if ((*it)->getType() == EntityBase::ENEMY)
		{
			if (std::next(it) != EntityManager::GetInstance()->GetEntityList().end())
			{
				for (auto it2 = std::next(it); it2 != EntityManager::GetInstance()->GetEntityList().end(); it2++)
				{

					if ((*it2)->getType() == EntityBase::BULLET)
					{
						if ((*it) == (*it2))
							continue;

						if (EntityManager::GetInstance()->CheckAABBCollision(*it, *it2) == true)// will be replaced with AABB in future  
						{
							// change direction of first enemy
							//static_cast<CEnemy3D*> (*it)->unitDistance.x = -(static_cast<CEnemy3D*>(*it)->unitDistance.x);
							//static_cast<CEnemy3D*>  (*it)->unitDistance.z = -(static_cast<CEnemy3D*>(*it)->unitDistance.z);

							//// update previous position
							//static_cast<CEnemy3D*>(*it2)->position = static_cast<CEnemy3D*>(*it2)->enemyPrevPos;
							cout << "Explode" << endl;
							static_cast<CEnemy3D*> (*it)->SetIsDone(true);
							static_cast<CEnemy3D*> (*it2)->SetIsDone(true);
							CPlayerInfo::GetInstance()->score++;


							// Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*it)) == true)
							{
								cout << "*** This Entity removed ***" << endl;
							}
							// Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*it2)) == true)
							{
								cout << "*** That Entity removed ***" << endl;
							}

						}
						else
						{
							// update previous position
							//static_cast<CEnemy3D*>(*it2)->enemyPrevPos = static_cast<CEnemy3D*>(*it2)->position;
						}
					}
					//else if ((*it2)->getType() == EntityBase::E_BULLET)
					//{
					//if (EntityManager::GetInstance()->CheckAABBCollision((*it), (*it2)) == true)
					//{
					//	static_cast<CProjectile *>((*it2))->SetStatus(false);
					//	(*it2)->SetIsDone(true);

					//	static_cast<CEnemy3D*>(*it)->_hp -= 5.f;
					//	break;
					//}
					//}
				}
			}
		}
	}
	return false;
}

//Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase * _existingEntity)
{
	//Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	//Delete the entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	//Return false if not found
	return false;
}
//Set a handle to the spatial Partition to this class
void EntityManager::SetSpatialPartition(CSpatialPartition * theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (auto it = EntityManager::GetInstance()->GetEntityList().begin(); it != EntityManager::GetInstance()->GetEntityList().end(); it++)
	{
		(*it)->Update(_dt);
	}

	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			it = entityList.erase(it);
		}
		else
		{
			++it;
		}

	}


	//Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	//Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			//delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	CheckForCollision();
}

void EntityManager::Render()
{
	// Update all entities
	std::list<EntityBase*>::iterator it,
		end = entityList.end();
	for (auto it = EntityManager::GetInstance()->GetEntityList().begin(); it != EntityManager::GetInstance()->GetEntityList().end(); it++)
	{
		(*it)->Render();
	}
	//Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	//Render the Spatial Partition
	if (theSpatialPartition != nullptr)
	{
		theSpatialPartition->Render();
	}
}

bool EntityManager::pointtoAABB(Vector3 pos, Vector3 forward, GenericEntity *ThatEntity)
{
	//returna true when in bounds
	return (pos.x  < ThatEntity->GetMaxAABB().x && pos.x  > ThatEntity->GetMinAABB().x)
		&& (pos.y < ThatEntity->GetMaxAABB().y && pos.y  > ThatEntity->GetMinAABB().y)
		&& (pos.z  < ThatEntity->GetMaxAABB().z && pos.z > ThatEntity->GetMinAABB().z);
}