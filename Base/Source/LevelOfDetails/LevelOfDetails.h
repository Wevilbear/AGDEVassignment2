#ifndef LEVEL_OF_DETAILS
#define LEVEL_OF_DETAILS

#include "Vector3.h"
#include "Mesh.h"

class CLevelOfDetails
{
public:
	CLevelOfDetails();
	virtual ~CLevelOfDetails();

	Mesh* modelMesh_HighDetails;
	Mesh* modelMesh_MidDetails;
	Mesh* modelMesh_LowDetails;

	enum DETAIL_LEVEL
	{
		NO_DETAILS = 0,
		HIGH_DETAILS,
		MID_DETAILS,
		LOW_DETAILS,
		NUM_DETAIL_LEVEL,
	};

	bool InitLOD(const std::string& _meshName_High, const std::string& _meshName_Mid, const std::string&_meshName_Low);

	bool DestroyLOD(void);

	void SetLODStatus(const bool bActive);
	bool GetLODStatus(void)const;

	bool SetLODMesh(Mesh* theMesh, const DETAIL_LEVEL theDetailLevel);
	Mesh* GetLODMesh(void)const;
	Mesh* GetLODmesh(const DETAIL_LEVEL theDetailLevel)const;
	int GetDetailLevel(void)const;
	bool SetDetailLevel(const DETAIL_LEVEL theDetailLevel);

private:
	bool m_bActive;

protected:
	DETAIL_LEVEL theDetailLevel;

};



#endif // !LEVEL_OF_DETAILS
