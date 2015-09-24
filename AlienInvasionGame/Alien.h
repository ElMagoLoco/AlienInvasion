#pragma once
#include "BaseObj.h"

class Alien : public BaseObj
{
public:
	// needed for ease of access, for a swarm IS NOT an alien, but has many aliens and accessor usage for the swarm AI for each alien is inefficient
	friend class Swarm;

	// to be used to determine when the swarm goes off on "its own"
	enum ThreatLevel {
		TLevel_None = 0,
		TLevel_Low,
		TLevel_Med,
		TLevel_High,
		TLevel_Critical
	};
	
	enum SpacePref {
		SPref_None = 0,
		SPref_Near,
		SPref_Far,
		SPref_Avg
	};

	virtual void init(Fugui::D2DGrafix* d2d, Fugui::ID2D_RenderTarget* d2dRT, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen);
	virtual void render(Fugui::ID2D_RenderTarget* d2dRT);
	virtual void update(Fugui::real dt);
	virtual void shutdown();
	//virtual bool checkCollision(BaseObj& obj1, BaseObj& obj2);

	Alien();
	Alien(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& velo, DirectX::XMFLOAT3& accel, DirectX::XMFLOAT4& color, float dimen);
	virtual ~Alien();

protected:
	Fugui::D2DGeometry	m_brush;

	ThreatLevel	m_eThreatLvl;
	SpacePref		m_eSpaceRef;

	float		m_fInfSphere;
	float		m_fNear;
	float		m_fFar;
	float		m_fAvg;

	bool		m_bAlive;
};

