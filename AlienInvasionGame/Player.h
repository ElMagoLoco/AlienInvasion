#pragma once
//#include "BaseObj.h"
#include "D2DText.h"
#include "Gun.h"

class Player :	public BaseObj
{
public:
	Gun& getMainGun()		{ return m_mainGun; }
	Gun& getAltGun()		{ return m_altGun; }

	int getLives()		{ return m_nLives; }
	float getPoints()	{ return m_fPoints; }
	void increasePoints(float points)		{ m_fPoints += points; }
	void decreaseLives()	{ --m_nLives; }
	void increaseLives()		{ ++m_nLives; }

	virtual void init(Fugui::D2DGrafix* d2d, Fugui::ID2D_RenderTarget* d2dRT, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen);
	virtual void render(Fugui::ID2D_RenderTarget* d2dRT);
	virtual void update(Fugui::real dt);
	virtual void shutdown();
	//virtual bool checkCollision(BaseObj& obj1, BaseObj& obj2);

	Player();
	Player(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen);
	Player(const BaseObj& obj) { *this = obj; }
	virtual ~Player();

protected:
	Fugui::D2DGeometry	m_brush;

	Gun	m_mainGun;
	Gun	m_altGun;

	float	m_fPoints;

	UINT m_nLives;
};

