#pragma once

#include "WeaponInfo\WeaponInfo.h"

class CMachineGun : public CWeaponInfo
{
public:
    CMachineGun();
    virtual ~CMachineGun();

    // Initialise this instance to default values
    void Init(void);
};

