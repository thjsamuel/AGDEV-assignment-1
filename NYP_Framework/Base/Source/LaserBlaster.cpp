#include "LaserBlaster.h"

CLaserBlaster::CLaserBlaster()
{

}

CLaserBlaster::~CLaserBlaster()
{

}

void CLaserBlaster::Init(void)
{
	CWeaponInfo::Init();
	magRounds = 5;
	maxMagRounds = 5;
	totalRounds = 10;
	maxTotalRounds = 10;

	timeBetweenShots = 0.01667;
	elapsedTime = 0.0;
	bFire = true;
}

void CLaserBlaster::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalized();
			CLaser* aLaser = Create::Laser("laser", position, _direction, 10.0f, 2.0f, 100.0f, _source);
			//aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
            aLaser->SetIsLaser(true);
            aLaser->SetCollider(false);
			bFire = false;
			magRounds--;
		}
	}
}