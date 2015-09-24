#include "Gun.h"

using namespace Fugui;
using namespace DirectX;
using namespace std;

ID2D_RenderTarget* Gun::m_pRT = nullptr;

Gun::Gun()
{
}


Gun::~Gun()
{
}

// void Gun::init(D2DGrafix* d2d, ID2D_RenderTarget* d2dRT, XMFLOAT3& pos, XMFLOAT3& velo, XMFLOAT3& accel, XMFLOAT4& color, float dimen)
// {
// }
// 
// void Gun::render(ID2D_RenderTarget* d2dRT)
// {
// }
// 
// void Gun::update(Fugui::real plyrPos, DirectX::XMFLOAT3& plyrPos)
// {
// }
// 
// void Gun::shutdown()
// {
// }

void Gun::init(GunType type, Fugui::ID2D_RenderTarget* d2dRT)
{
	// we need to capture this so that later we can create bullets during a call to update()
	if (nullptr == m_pRT)
		m_pRT = d2dRT;

	switch (type)
	{
	case GType_Firearm:
	{
		m_fBulletSpeed = 1000.0f;
		m_fReloadTime = 3.0f;
		m_fDamage = 25.0f;
		m_fRateOfFire = 0.50f;

		m_eGunType = GType_Firearm;

		m_nClipCount = 15;
		m_nCurrClipCount = m_nClipCount;
	}
		break;
	case GType_SubMachine:
		break;
	case GType_AutomaticRifle:
		break;
	case GType_SniperRifle:
		break;
	case GType_FlameThrower:
		break;
	case GType_RocketLauncher:
		break;
	case GType_GrandeLauncher:
		break;
	default:
		break;
	}
}


void Gun::fire(Fugui::real dt, DirectX::XMFLOAT3& playerPos, DirectX::XMFLOAT3& mousPos)
{
	static float canShoot = m_fRateOfFire;
	static float reloadTime = m_fReloadTime;
	//static int	currClipCount = m_nClipCount;

	if (m_nCurrClipCount <= 0) {
		reloadTime -= dt;

		if (reloadTime <= 0.0f) {
			m_nCurrClipCount = m_nClipCount;
			reloadTime = m_fReloadTime;
		}
		
		return;
	}

	canShoot += dt;

	if (canShoot >= m_fRateOfFire) {
		XMVECTOR plyrPos = XMLoadFloat3(&playerPos);
		XMVECTOR target = XMLoadFloat3(&mousPos);
		//XMVECTOR velo = XMLoadFloat3(&m_vVelo);

		XMVECTOR dir = plyrPos - target;

		dir = XMVector2Normalize(dir);

		dir *= -m_fBulletSpeed;

		XMFLOAT3 tempVelo;
		XMStoreFloat3(&tempVelo, dir);

		// TODO: need to clean this up later
		Bullet* newBullet = new Bullet;
		if (nullptr == newBullet)
			return;
		// fix where the bullets shoot out so they don't always generate from the bottom right of the player
		XMFLOAT3 finalPos = playerPos;
		if (mousPos.y < playerPos.y) {
			finalPos.y -= 18.0f;
			finalPos.x -= 12.0f;
		}
		else if (mousPos.x > playerPos.x) {
			//finalPos = playerPos;
 			finalPos.y -= 12.0f;
// 			finalPos.x += 12.0f;
		}
		else if (mousPos.x < playerPos.x) {
			finalPos.y -= 12.0f;
			finalPos.x -= 12.0f;
		}
		newBullet->init(NULL, m_pRT, finalPos, tempVelo, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 2.0f);
		newBullet->m_fDamage = m_fDamage;

		--m_nCurrClipCount;

		m_bulletList.push_back(newBullet);

		canShoot = 0.0f;
	}
}

void Gun::setGun(GunType type)
{
	switch (type)
	{
	case GType_Firearm:
	{
		m_fBulletSpeed = 5.0f;
		m_fReloadTime = 3.0f;
		m_fDamage = 25.0f;
		m_fRateOfFire = 2.0f;

		m_eGunType = GType_Firearm;

		m_nClipCount = 15;
	}
	break;
	case GType_SubMachine:
		break;
	case GType_AutomaticRifle:
		break;
	case GType_SniperRifle:
		break;
	case GType_FlameThrower:
		break;
	case GType_RocketLauncher:
		break;
	case GType_GrandeLauncher:
		break;
	default:
		break;
	}
}

void Gun::update(real dt)
{
	for (UINT i = 0; i < m_bulletList.size(); ++i) {
		if (Bullet::BState_Fired == m_bulletList[i]->m_eState)
			m_bulletList[i]->update(dt);
	}
}

void Gun::render(ID2D_RenderTarget* d2dRT)
{
	for (UINT i = 0; i < m_bulletList.size(); ++i){
		if (Bullet::BState_Fired == m_bulletList[i]->m_eState)
			m_bulletList[i]->render(d2dRT);
	}
}
void Gun::shutdown()
{
	for (UINT i = 0; i < m_bulletList.size(); ++i){
		if (m_bulletList[i]) {
			Bullet* del = m_bulletList[i];
			delete del;
			del = nullptr;
		}
	}

	m_bulletList.clear();
}
