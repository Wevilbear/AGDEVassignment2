#include "Enemy3D.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../PlayerInfo/PlayerInfo.h"

CEnemy3D::CEnemy3D(Mesh* _modelMesh)
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_fElapsedTimeBeforeUpdate(0.0f)
	,m_iWayPointIndex(-1)
{
	EntityBase::setType(EntityBase::ENEMY);
	this->modelMesh = _modelMesh;
	setBoxSizeAABB(Vector3(15, 15, 15));
	listOfWaypoints.clear();
}


CEnemy3D::~CEnemy3D()
{
}

void CEnemy3D::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set up the waypoints
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	m_iWayPointIndex = 0;

	// Set the current values
	//position.Set(position.x, position.y, position.z);
	target.Set(10.0f, 0.0f, 10);
	CWaypoint* nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
		target = nextWaypoint->GetPosition();
	else
		target = Vector3(0, 0, 0);
	//cout << "Next target: " << target << endl;
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(10, 10, 10);
	minBoundary.Set(-10, -10, -10);

	// Set speed
	m_dSpeed = 10.0;

	//Initialise the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	//Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

}

// Reset this player instance to default
void CEnemy3D::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy3D::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy3D::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy3D::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy3D::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy3D::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy3D::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy3D::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy3D::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy3D::GetTerrain(void)
{
	return m_pTerrain;
}

CWaypoint * CEnemy3D::GetNextWaypoint(void)
{
	if ((int)listOfWaypoints.size() > 0)
	{
		m_iWayPointIndex++;
		if (m_iWayPointIndex >= (int)listOfWaypoints.size())
			m_iWayPointIndex = 0;
		return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
	}
	else
		return NULL;
}

// Update
void CEnemy3D::Update(double dt)
{
	//Vector3 viewVector = (target - playerInfo->s_instance->position).Normalized();
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;

	//cout << position << "..." << viewVector << endl;

	// Constrain the position
	//Constrain();

	//Update the target
	/*
	if(position.z > 400.0f)
	target.z = position.z * -1;
	else if(position.z < -400.0f)
	target.z = position.z * -1;
	*/
	if ((target - position).LengthSquared() < 25.0f)
	{
		CWaypoint * nextWaypoint = GetNextWaypoint();
		if (nextWaypoint)
			target = nextWaypoint->GetPosition();
		else
			target = Vector3(0, 0, 0);
		//cout << "Next target: " << target << endl;
	}
	// This is RealTime Loop control
	// Update the target once every 5 seconds. 
	// Doing more of these calculations will not affect the outcome.
	m_fElapsedTimeBeforeUpdate += dt;
	if (m_fElapsedTimeBeforeUpdate > 5.0f)
	{
		//cout << m_fElapsedTimeBeforeUpdate << endl;
		m_fElapsedTimeBeforeUpdate = 0.0f;
		if (position.z > 400.0f)
			target.z = position.z * -1;
		else if (position.z < -400.0f)
			target.z = position.z * -1;
	}
	SetPAABB(Vector3(scale.x, scale.y, scale.z), position);
}

void CEnemy3D::Update2(CCollider player, std::list<GenericEntity*> obj, double dt)
{
	Vector3 viewVector = (target - position).Normalized();
	//cout << position << "..." << viewVector << endl;

	// Constrain the position
	//Constrain();

	//Update AABB
	SetPAABB(Vector3(scale.x, scale.y, scale.z), position);

	//Check Collision w other obj
	bool collision = false;
	for (auto &i : obj)
	{
		if (!i->IsDone())
			collision = EntityManager::GetInstance()->CheckOverlap(GetMinAABB(), GetMaxAABB(), i->GetMinAABB(), i->GetMaxAABB());

		if (collision)
		{
			SetIsDone(true);
			i->SetIsDone(true);
			return;
		}
	}

	//Collision w player
	collision = EntityManager::GetInstance()->CheckOverlap(GetMinAABB(), GetMaxAABB(), player.GetMinAABB(), player.GetMaxAABB());
	if (!collision)
		position += viewVector * (float)m_dSpeed * (float)dt;

	// This is RealTime Loop control
	// Update the target once every 5 seconds. 
	// Doing more of these calculations will not affect the outcome.
	m_fElapsedTimeBeforeUpdate += dt;
	if (m_fElapsedTimeBeforeUpdate > 5.0f)
	{
		//cout << m_fElapsedTimeBeforeUpdate << endl;
		m_fElapsedTimeBeforeUpdate = 0.0f;

		if (position.z > 400.0f)
			target.z = position.z * -1;
		else if (position.z < -400.0f)
			target.z = position.z * -1;
	}
}

// Constrain the position within the borders
void CEnemy3D::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy3D::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus())
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(modelMesh);
		}
	}
	modelStack.PopMatrix();
}

CEnemy3D* Create::Enemy3D(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CEnemy3D* result = new CEnemy3D(modelMesh);
	result->Init();
	result->SetPos(_position);
	if (_meshName == "windmill_fan")
	{
		//cout << result->getpos << endl;
	}
	result->SetScale(_scale);
	result->SetCollider(false);
	result->setType(EntityBase::ENEMY);
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}

