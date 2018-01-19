#pragma once

#include "WeaponInfo.h"

class CRifle :
	public CWeaponInfo
{
public:
	CRifle();
	virtual ~CRifle();

	// Initialise this instance to default values
	void Init(void);
};

