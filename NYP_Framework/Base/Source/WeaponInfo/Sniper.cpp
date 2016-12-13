#include "Sniper.h"

CSniper::CSniper()
{
	weight = 6.5;
	muzzleSpeed = 20000;
}


CSniper::~CSniper()
{
}

// Initialise this instance to default values
void CSniper::Init(void)
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
	//timeBetweenShots = Math::RandFloatMinMax(0.092f, 0.2333f); // more realistic
	timeBetweenShots = 0.3333;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
    // Set firing rate
    SetFiringRate(30);
}
