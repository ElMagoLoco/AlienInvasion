#pragma once
#include <vector>
#include "BaseObj.h"
#include "Bullet.h"

class Gun// : public BaseObj
{
public:
	friend class Bullet;
	enum GunType {
		GType_Firearm = 0,
		GType_SubMachine,
		GType_AutomaticRifle,
		GType_SniperRifle,
		GType_FlameThrower,
		GType_RocketLauncher,
		GType_GrandeLauncher
	};

// 	virtual void init(Fugui::D2DGrafix* d2d, Fugui::ID2D_RenderTarget* d2dRT, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen);
// 	virtual void render(Fugui::ID2D_RenderTarget* d2dRT);
// 	virtual void update(Fugui::real dt, DirectX::XMFLOAT3& plyrPos);
// 	virtual void shutdown();
	void init(GunType type, Fugui::ID2D_RenderTarget* d2dRT);
	void setGun(GunType type);
	void fire(Fugui::real dt, DirectX::XMFLOAT3& playerPos, DirectX::XMFLOAT3& mousPos);
	void update(Fugui::real dt);
	void render(Fugui::ID2D_RenderTarget* d2dRT);
	void shutdown();

	std::vector<Bullet*>& getBulletList()		{ return m_bulletList; }

	int getMaxClipCount()		{ return m_nClipCount; }
	int getCurrClipCount()		{ return m_nCurrClipCount; }

	Gun();
	virtual ~Gun();

protected:
	std::vector<Bullet*>		m_bulletList;
	std::vector<Bullet*>		m_deadBullets;
	std::vector<Bullet*>		m_aliveBullets;

	static Fugui::ID2D_RenderTarget*		m_pRT;

	GunType		m_eGunType;

	float		m_fDamage;
	float		m_fRateOfFire;
	float		m_fBulletSpeed;
	float		m_fReloadTime;

	unsigned int	m_nClipCount;
	unsigned int	m_nCurrClipCount;
};

