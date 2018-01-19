#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "Collider\Collider.h"
#include "GenericEntity.h"
#include "Spatial Partition\SpatialPartition.h"

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition);
	bool RemoveEntity(EntityBase* _existingEntity);

	bool pointtoAABB(Vector3 pos, Vector3 forward, GenericEntity *ThatEntity);
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check for overlap
	//bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	//bool CheckAABBCollision(CCollider *ThisEntity, CCollider *ThatEntity);
	// Check if any Collider is colliding with another Collider
	// Custom Code
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	bool CheckForCollision(void);

	bool MarkForDeletion(EntityBase*_existingEntity);

	void SetSpatialPartition(CSpatialPartition*theSpatialPartition);

	std::list<EntityBase*> &GetEntityList()
	{
		return entityList;
	}
private:
	EntityManager();
	virtual ~EntityManager();

	Vector3 getMinBox(EntityBase * _entity);
	Vector3 getMaxBox(EntityBase * _entity);

	std::list<EntityBase*> entityList;
	// Handler to Spatial Partition
	CSpatialPartition* theSpatialPartition = nullptr;
};

#endif // ENTITY_MANAGER_H