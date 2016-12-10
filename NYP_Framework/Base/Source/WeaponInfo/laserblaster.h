#pragma once

#include "WeaponInfo.h"

class CPlayerInfo;

class CLaserBlaster : public CWeaponInfo
{
public:
	CLaserBlaster();
	virtual ~CLaserBlaster();

	// init instance default values
	void Init(void);
	// discharge weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);

};