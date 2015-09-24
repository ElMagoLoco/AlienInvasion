#pragma once
#include <vector>
#include "Alien.h"
#include "Bullet.h"


class Swarm
{
public:
	struct SwarmNode {
		SwarmNode* ltChild;
		SwarmNode* rtChild;
		SwarmNode* parent;
		Alien data;

		SwarmNode() :
			ltChild(nullptr),
			rtChild(nullptr),
			parent(nullptr)
		{}
	};

	enum SwarmType {
		Swarm_Linear = 0,
		Swarm_Spearhead,
		Swarm_Double_Spearhead,
		Swarm_Triple_Spearhead
	};

	void setTarget(DirectX::XMFLOAT3& playerPos)	{ m_pLead->data.m_vTarget = playerPos; }

	void initSwarm(int num, SwarmType type, Fugui::ID2D_RenderTarget* d2dRT, DirectX::XMFLOAT3& initPos, DirectX::XMFLOAT3& initVelo, DirectX::XMFLOAT3& initAccel, DirectX::XMFLOAT3& target, float initSpeed, float initDimen, float initMaxSpeed);
	void renderSwarm(Fugui::ID2D_RenderTarget* d2dRT);
	void updateSwarm(Fugui::real dt, BaseObj& collision, std::vector<Bullet*> bulletList);
	void shutdownSwarm();

	bool checkCollision(BaseObj& obj);

	Swarm();
	~Swarm();

private:
	//Fugui::D2DGeometry	m_brush;

	SwarmNode*	m_pLead;

	int		m_nNumAliens;

	SwarmType		m_eSwarmType;

	void createSwarm(Fugui::ID2D_RenderTarget* d2dRT, SwarmNode* leadAlien, SwarmType swarmType);
	void renderSwarm(Fugui::ID2D_RenderTarget* d2dRT, SwarmNode* next);
	void updateSwarm(Fugui::real dt, SwarmNode* next, BaseObj& collision, std::vector<Bullet*> bulletList);
	void shutdownSwarm(SwarmNode* next);
};

