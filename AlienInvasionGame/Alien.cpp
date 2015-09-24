#include "Alien.h"

using namespace DirectX;
using namespace Fugui;

Alien::Alien()
{
	m_fLife = 50.0f;
	m_fDamage = 10.0f;
}


Alien::~Alien()
{
}

void Alien::init(Fugui::D2DGrafix* d2d, Fugui::ID2D_RenderTarget* d2dRT, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen)
{
	m_vPos = pos;
	m_vAccel = accel;
	m_vVelo = velo;
	m_vColor = color;
	m_fDimen = dimen;
	//m_fSpeed = 200.0f;
	//m_fMaxSpeed = 111140.0f;
	//m_vTarget = XMFLOAT3(m_vPos.x, m_vPos.y, 0.0f);

	m_brush.createSolidColorBrush(D2D1::ColorF(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w), d2dRT, 1.0f);
}

void Alien::render(Fugui::ID2D_RenderTarget* d2dRT)
{
	float top = m_vPos.y - m_fDimen;
	float left = m_vPos.x - m_fDimen;

	m_brush.drawRectangle(d2dRT, top, left, m_fDimen, m_fDimen);
}

void Alien::update(real dt)
{
	XMVECTOR dir;
	XMVECTOR finalPos;
	XMVECTOR pos = XMLoadFloat3(&m_vPos);
	XMVECTOR target = XMLoadFloat3(&m_vTarget);
	XMVECTOR velo = XMLoadFloat3(&m_vVelo);
	XMVECTOR accel = XMLoadFloat3(&m_vAccel);
// 	XMVECTOR parentPos = XMLoadFloat3(&parent->data.m_vPos);
// 	XMVECTOR distToParent = XMVector3Length(pos - parentPos);


	float time = (dt * dt) * 0.5f;

	dir = target - pos; // pos - target;
	XMVECTOR distance = XMVector2Length(dir);
	dir = XMVector2Normalize(dir);

	velo *= dir * -m_fSpeed;
	//velo *= -m_fSpeed;

	if (XMVectorGetX(distance) > m_fDimen && XMVectorGetY(distance) > m_fDimen) {
		// check to see if we need to adjust the math
		if (XMVectorGetX(target) > XMVectorGetX(pos)) {
			float tempX = XMVectorGetX(velo);
			tempX *= -1.0f;
			velo = XMVectorSetX(velo, tempX);
		}

		if (XMVectorGetY(target) > XMVectorGetY(pos)) {
			float tempY = XMVectorGetY(velo);
			tempY *= -1.0f;
			velo = XMVectorSetY(velo, tempY);
		}

		// check if we are past our velocity bounds
		// on x
		if (XMVectorGetX(velo) > m_fMaxSpeed)
			velo = XMVectorSetX(velo, m_fMaxSpeed);
		else if (XMVectorGetX(velo) < -m_fMaxSpeed)
			velo = XMVectorSetX(velo, -m_fMaxSpeed);
		// on y
		if (XMVectorGetY(velo) > m_fMaxSpeed)
			velo = XMVectorSetY(velo, m_fMaxSpeed);
		else if (XMVectorGetY(velo) < -m_fMaxSpeed)
			velo = XMVectorSetY(velo, -m_fMaxSpeed);
	}
// 	else
// 		velo *= 0.25f;
	


	finalPos = pos + velo * dt;// +accel * time;

	XMStoreFloat3(&m_vPos, finalPos);
	XMStoreFloat3(&m_vVelo, dir);
}

void Alien::shutdown()
{
}
