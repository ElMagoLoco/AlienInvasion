#pragma once
#include "D3DUtils.h"
#include "D2DGrafix.h"
#include "D2DGeometry.h"

class BaseObj
{
public:
	virtual void init(Fugui::D2DGrafix* d2d, Fugui::ID2D_RenderTarget* d2dRT, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen) = 0;
	virtual void render(Fugui::ID2D_RenderTarget* d2dRT) = 0;
	virtual void update(Fugui::real dt) = 0;
	virtual void shutdown() = 0;

	inline void checkCollision(BaseObj& obj);

	DirectX::XMFLOAT3& getPos()	{ return m_vPos; }
	float getLife()		{ return m_fLife; }
	float getDamage()	{ return m_fDamage; }
	void reduceHealth(float damage)		{ m_fLife -= damage; }
	void increaseHealth(float heal)			{ m_fLife += heal; }

	BaseObj() :
		m_vPos(0.0f, 0.0f, 0.0f),
		m_vVelo(0.0f, 0.0f, 0.0f),
		m_vAccel(0.0f, 0.0f, 0.0f),
		m_vColor(0.0f, 0.0f, 0.0f, 1.0f),
		m_fDimen(0.0f),
		m_fSpeed(0.0f),
		m_nTexId(0),
		m_nMeshId(0),
		m_vTarget(0.0f, 0.0f, 0.0f),
		m_bHit(false)
	{}
	BaseObj(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen) :
		m_vPos(pos),
		m_vVelo(velo),
		m_vAccel(accel),
		m_vColor(color),
		m_fDimen(dimen),
		m_fSpeed(0.0f),
		m_nTexId(0),
		m_nMeshId(0),
		m_vTarget(0.0f, 0.0f, 0.0f),
		m_bHit(false)
	{}
	virtual ~BaseObj() = 0 {}

protected:
	//Fugui::D2DGeometry	m_brush;

	DirectX::XMFLOAT4		m_vColor;

	DirectX::XMFLOAT3		m_vPos;
	DirectX::XMFLOAT3		m_vVelo;
	DirectX::XMFLOAT3		m_vAccel;
	DirectX::XMFLOAT3		m_vTarget;

	float		m_fDimen;
	float		m_fSpeed;
	float		m_fMaxSpeed;
	float		m_fLife;
	float		m_fDamage;

	UINT		m_nTexId;
	UINT		m_nMeshId;

	bool		m_bHit;
};

inline void BaseObj::checkCollision(BaseObj& obj)
{ 
	// first to see if they are close enough to each other
	DirectX::XMVECTOR distance = DirectX::XMVectorAbs(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->m_vPos), DirectX::XMLoadFloat3(&obj.m_vPos))));
	if (DirectX::XMVectorGetX(distance) < this->m_fDimen + obj.m_fDimen || DirectX::XMVectorGetY(distance) < this->m_fDimen + obj.m_fDimen) {
		// then we might be hitting the object, do further calculations to make sure
		// TODO: hack, do the calculations
		this->m_bHit = true;
		obj.m_bHit = true;
		//return;
	}
	//if (this->m_vPos.x)

	// the object is too far
	//return;
}