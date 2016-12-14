#include "MachineGun.h"


CMachineGun::CMachineGun()
{
    weight = 3.82;
    muzzleSpeed = 100;
}


CMachineGun::~CMachineGun()
{
}

// Initialise this instance to default values
void CMachineGun::Init(void)
{
    CWeaponInfo::Init();
    // The number of ammunition in a magazine for this weapon
    magRounds = 30;
    // The maximum number of ammunition for this magazine for this weapon
    maxMagRounds = 30;
    // The current total number of rounds currently carried by this player
    totalRounds = 60; // 2 magazines
    // The max total number of rounds currently carried by this player
    maxTotalRounds = 180; //  4 magazines
    // The time between shots
    timeBetweenShots = Math::RandFloatMinMax(0.092f, 0.1333f); // more realistic
    // The elapsed time (between shots)
    elapsedTime = 0.0;
    // Boolean flag to indicate if weapon can fire now
    bFire = true;
}
