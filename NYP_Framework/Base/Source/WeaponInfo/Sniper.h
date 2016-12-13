#ifndef SNIPER_H
#define SNIPER_H

#include "WeaponInfo.h"


class CSniper : public CWeaponInfo
{
public:
	CSniper();
	virtual ~CSniper();

	// Initialise this instance to default values
	void Init(void);
};



#endif