#include "Bullet.h"

using namespace Fugui;
using namespace DirectX;

Bullet::Bullet()
{
	m_eState = BState_Idle;
}


Bullet::~Bullet()
{
}

void Bullet::init(D2DGrafix* d2d, ID2D_RenderTarget* d2dRT, XMFLOAT3& pos, XMFLOAT3& velo, XMFLOAT3& accel, XMFLOAT4& color, float dimen)
{
	m_vPos = pos;
	m_vVelo = velo;
	m_vAccel = accel;
	m_fDimen = dimen;
	m_vColor = color;

	m_brush.createSolidColorBrush(D2D1::ColorF(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w), d2dRT, 0.5f);

	m_eState = BState_Fired;
}

void Bullet::render(ID2D_RenderTarget* d2dRT)
{
	if (BState_Fired == m_eState) {
		m_brush.drawRectangle(d2dRT, m_vPos.y, m_vPos.x, 5.0f, 5.0f);
	}
}

void Bullet::update(real dt)
{
	switch (m_eState)
	{
	case BState_Idle:
	{
		// reset the bullet
		m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vAccel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vVelo = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		m_fDimen = 0.0f;
		m_brush.destroySCB(); // needdsfsdfdsfdsfds to do this otherwise new brush is created on init() everytime a bullet is fired causing a mem leak or crash
	}
 		break;
	case BState_Fired:
	{
		// do the math...
		float time = (dt * dt) * 0.5f;

		XMVECTOR accel = XMLoadFloat3(&m_vAccel);
		XMVECTOR	pos = XMLoadFloat3(&m_vPos);
		XMVECTOR velo = XMLoadFloat3(&m_vVelo);
		XMVECTOR finalPos = pos + velo * dt + accel * time;
		// TODO: clamp movement to the window
		XMStoreFloat3(&m_vPos, finalPos);

		if (m_vPos.x < 0) {
			m_eState = BState_Offscreen;
			return; // skip the next checks
		}
		if (m_vPos.y < 0) {
			m_eState = BState_Offscreen;
			return;
		}
		if (m_vPos.x > 1024) {
			m_eState = BState_Offscreen;
			return; // skip the next checks
		}
		if (m_vPos.y > 768) {
			m_eState = BState_Offscreen;
			return; // skip the next checks
		}
	}
		break;
	case BState_Hit:
	{
		m_eState = BState_Idle;
	}
		break;
	case BState_Offscreen:
	{
		m_eState = BState_Idle;
	}
		break;
	default:
		break;
	}
}

void Bullet::shutdown()
{
}
