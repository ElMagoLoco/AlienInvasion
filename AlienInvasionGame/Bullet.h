#pragma once
#include "BaseObj.h"

class Bullet : public BaseObj
{
public:
	friend class Gun;
	enum BulletState {
		BState_Idle,
		BState_Fired,
		BState_Hit,
		BState_Offscreen
	};

	//BulletState getState()	{ return m_eState; }

	Bullet();
	virtual ~Bullet();

	virtual void init(Fugui::D2DGrafix* d2d, Fugui::ID2D_RenderTarget* d2dRT, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen);
	virtual void render(Fugui::ID2D_RenderTarget* d2dRT);
	virtual void update(Fugui::real dt);
	virtual void shutdown();

	void setState(BulletState state)	{ m_eState = state; }
	//virtual bool checkCollision(BaseObj& obj1, BaseObj& obj2);

	Fugui::D2DGeometry	m_brush;

	BulletState		m_eState;
};

