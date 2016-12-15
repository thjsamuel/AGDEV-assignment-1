#pragma once
#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "Collider/Collider.h"
#include <vector>
using std::vector;

class CPlayerInfo : public EntityBase, public CCollider
{
protected:
	static CPlayerInfo *s_instance;
	CPlayerInfo(void);

public:
	static CPlayerInfo *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~CPlayerInfo(void);

    enum WEAPONS
    {
        PISTOL,
        MACHINEGUN,
        GRENADE,
        SNIPERRIFLE,
        MAX_WEAPON
    };

	// Initialise this class instance
	void Init(void);
	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's vertical movement
	void StopVerticalMovement(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set m_dJumpSpeed of the player
	void SetJumpSpeed(const double m_dJumpSpeed);
	// Set m_dJumpAcceleration of the player
	void SetJumpAcceleration(const double m_dJumpAcceleration);
	// Set Fall Speed of the player
	void SetFallSpeed(const double m_dFallSpeed);
	// Set Fall Acceleration of the player
	void SetFallAcceleration(const double m_dFallAcceleration);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);
    // set aabb
    void SetAABB(Vector3 maxAABB, Vector3 minAABB);
    // set player barricade/blockade boundary
    // Ideally, go through list of player-bumpable objects, lessened using spatial-partioning then check for collision
    bool CollisionCourse(Vector3 pos, Vector3 scale); // the parameters will become invalid later when the list is made
    bool CollisionCourseBack(Vector3 pos, Vector3 scale); // the parameters will become invalid later when the list is made
    bool CollisionCourseSide(Vector3 pos, Vector3 scale); // the parameters will become invalid later when the list is made
    void CollideFront(Vector3 pos, Vector3 scale);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get Jump Speed of the player
	double GetJumpSpeed(void) const;
	// Get Jump Acceleration of the player
	double GetJumpAcceleration(void) const;
	// Get Fall Speed of the player
	double GetFallSpeed(void) const;
	// Get Fall Acceleration of the player
	double GetFallAcceleration(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);
    WEAPONS GetWeaponType(void);
    const CWeaponInfo GetPrimaryWeapon(void);
    const CWeaponInfo GetSecondaryWeapon(void);
    const CWeaponInfo GetTertiaryWeapon(void);

	// Update Jump Upwards
	void UpdateJumpUpwards(double dt = 0.0333f);
	// Update FreeFall
	void UpdateFreeFall(double dt = 0.0333f);
	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);

	// Handling Camera
	void AttachCamera(FPSCamera* _cameraPtr);
	void DetachCamera();

private:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 position, target, up;
	Vector3 maxBoundary, minBoundary;
    vector<Vector3> posScale;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
    float lookSpeed;
	double m_dAcceleration;

	bool m_bJumpUpwards;
	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	double m_dFallSpeed;
	bool m_bFallDownwards;
	double m_dFallAcceleration;

	FPSCamera* attachedCamera;

	CWeaponInfo* primaryWeapon;
	CWeaponInfo* secondaryWeapon;
    CWeaponInfo* tertiaryWeapon;
    WEAPONS activeWeapon;
};
