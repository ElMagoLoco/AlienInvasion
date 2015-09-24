#include "Swarm.h"
#include "Player.h"

using namespace Fugui;
using namespace DirectX;

Swarm::Swarm()
{
}


Swarm::~Swarm()
{
}

void Swarm::initSwarm(int num, SwarmType type, ID2D_RenderTarget* d2dRT, XMFLOAT3& initPos, XMFLOAT3& initVelo, XMFLOAT3& initAccel, XMFLOAT3& target, float initSpeed, float initDimen, float initMaxSpeed)
{
	m_pLead = new SwarmNode;
	if (nullptr == m_pLead)
		exit(EXIT_FAILURE);	// TODO: throw an error msg

	m_nNumAliens = num;
	m_eSwarmType = type;

	m_pLead->data.init(NULL, d2dRT, initPos, initVelo, initAccel, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), initDimen);
	m_pLead->data.m_fSpeed = initSpeed;
	m_pLead->data.m_fMaxSpeed = initMaxSpeed;
	m_pLead->data.m_vTarget = target;

	createSwarm(d2dRT, m_pLead, m_eSwarmType);
}

void Swarm::createSwarm(ID2D_RenderTarget* d2dRT, SwarmNode* leadAlien, SwarmType swarmType)
{
	static int count = 1;	// 1 since the lead was created already
	// exit condition for recursion
	if (count >= m_nNumAliens) {
		count = 1;
		return;
	}

	switch (swarmType) 
	{
	case Swarm_Linear:
	{
		SwarmNode* alien = new SwarmNode;
		if (nullptr == alien)
			exit(EXIT_FAILURE);

		alien->parent = leadAlien;
		alien->data.m_vPos = leadAlien->data.m_vPos;
		alien->data.m_vPos.y += 19.0f;
		alien->data.m_fSpeed = leadAlien->data.m_fSpeed;
		alien->data.m_fMaxSpeed = leadAlien->data.m_fMaxSpeed;

		alien->data.init(NULL, d2dRT, alien->data.m_vPos, leadAlien->data.m_vVelo, leadAlien->data.m_vAccel, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), leadAlien->data.m_fDimen);

		leadAlien->ltChild = alien;

		++count;

		createSwarm(d2dRT, alien, swarmType);

	} // end case Swarm_Linear
	break;
	case Swarm_Spearhead:
	{
		if (count >= m_nNumAliens) {
			count = 1;
			return;
		}

		SwarmNode* ltAlien = new SwarmNode;
		if (nullptr == ltAlien)
			exit(EXIT_FAILURE);

		ltAlien->parent = leadAlien;
		ltAlien->data.m_vPos = leadAlien->data.m_vPos;
		ltAlien->data.m_vPos.y += 19.0f;
		ltAlien->data.m_vPos.x += 19.0f;
		ltAlien->data.m_fSpeed = leadAlien->data.m_fSpeed;
		ltAlien->data.m_fMaxSpeed = leadAlien->data.m_fMaxSpeed;

		ltAlien->data.init(NULL, d2dRT, ltAlien->data.m_vPos, leadAlien->data.m_vVelo, leadAlien->data.m_vAccel, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), leadAlien->data.m_fDimen);

		leadAlien->ltChild = ltAlien;

		++count;


		SwarmNode* rtAlien = new SwarmNode;
		if (nullptr == rtAlien)
			exit(EXIT_FAILURE);

		rtAlien->parent = leadAlien;
		rtAlien->data.m_vPos = leadAlien->data.m_vPos;
		rtAlien->data.m_vPos.y += 19.0f;
		rtAlien->data.m_vPos.x -= 19.0f;
		rtAlien->data.m_fSpeed = leadAlien->data.m_fSpeed;
		rtAlien->data.m_fMaxSpeed = leadAlien->data.m_fMaxSpeed;

		rtAlien->data.init(NULL, d2dRT, rtAlien->data.m_vPos, leadAlien->data.m_vVelo, leadAlien->data.m_vAccel, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), leadAlien->data.m_fDimen);

		leadAlien->rtChild = rtAlien;

		++count;
		
		createSwarm(d2dRT, rtAlien, swarmType);
		createSwarm(d2dRT, ltAlien, swarmType);

	} // end case Swarm_Spearhead
	break;
	} // end switch(m_eSwarmType
}


void Swarm::updateSwarm(Fugui::real dt, BaseObj& collision, std::vector<Bullet*> bulletList)
{
	if (m_pLead) {
		if (m_pLead->data.m_bAlive) {
			m_pLead->data.update(dt);
			m_pLead->data.checkCollision(collision);

			if (m_pLead->data.m_bHit)
				m_pLead->data.m_fLife -= collision.getDamage();

			if (m_pLead->data.m_fLife < 0.0f) {
				dynamic_cast<Player&>(collision).increasePoints(5.0f);
				m_pLead->data.m_bAlive = false;
				//collision.increasePoints(20.0f);
			}
		}

		// check if a bullet hit the alien
		for (unsigned int i = 0; i < bulletList.size(); ++i) {
			m_pLead->data.checkCollision(*bulletList[i]);

			if (m_pLead->data.m_bHit && m_pLead->data.m_bAlive) {
				m_pLead->data.reduceHealth(bulletList[i]->getDamage());
				bulletList[i]->setState(Bullet::BState_Hit);
			}

			if (m_pLead->data.m_fLife < 0.0f) {
				dynamic_cast<Player&>(collision).increasePoints(25.0f);
				m_pLead->data.m_bAlive = false;
			}
		}

		if (m_pLead->ltChild) 
			updateSwarm(dt, m_pLead->ltChild, collision, bulletList);
		if (m_pLead->rtChild)
			updateSwarm(dt, m_pLead->rtChild, collision, bulletList);
	}
}

void Swarm::updateSwarm(real dt, SwarmNode* next, BaseObj& collision, std::vector<Bullet*> bulletList)
{
	if (next->data.m_bAlive) {
		XMVECTOR pos = XMLoadFloat3(&next->data.m_vPos);
		XMVECTOR playerTarget = XMLoadFloat3(&m_pLead->data.m_vTarget);
		XMVECTOR distance = XMVector3Length(pos - playerTarget);

		if (XMVectorGetX(distance) < 120.0f || XMVectorGetY(distance) < 120.0f || false == m_pLead->data.m_bAlive)
			next->data.m_vTarget = m_pLead->data.m_vTarget;
		else
			next->data.m_vTarget = next->parent->data.m_vPos;

		// check if we hit the player
		next->data.update(dt);
		next->data.checkCollision(collision);

		if (next->data.m_bHit)
			next->data.reduceHealth(collision.getDamage());

		if (next->data.m_fLife < 0.0f) {
			dynamic_cast<Player&>(collision).increasePoints(5.0f);
			collision.reduceHealth(next->data.m_fDamage);
			next->data.m_bAlive = false;
			//collision.increasePoints(20.0f);
		}

		// check if a bullet hit the alien
		for (unsigned int i = 0; i < bulletList.size(); ++i) {
			next->data.checkCollision(*bulletList[i]);

			if (next->data.m_bHit && next->data.m_bAlive) {
				next->data.reduceHealth(bulletList[i]->getDamage());
				bulletList[i]->setState(Bullet::BState_Hit);
			}
			
			if (next->data.m_fLife < 0.0f) {
				dynamic_cast<Player&>(collision).increasePoints(25.0f);
				next->data.m_bAlive = false;
			}
		}
	}

	if (next->ltChild)
		updateSwarm(dt, next->ltChild, collision, bulletList);
	if (next->rtChild)
		updateSwarm(dt, next->rtChild, collision, bulletList);
}


void Swarm::renderSwarm(ID2D_RenderTarget* d2dRT)
{
	if (m_pLead) {
		if (m_pLead->data.m_bAlive)
			m_pLead->data.render(d2dRT);

		if (m_pLead->ltChild)
			renderSwarm(d2dRT, m_pLead->ltChild);
		if (m_pLead->rtChild)
			renderSwarm(d2dRT, m_pLead->rtChild);
	}
}

void Swarm::renderSwarm(ID2D_RenderTarget* d2dRT, SwarmNode* next)
{
	if (next->data.m_bAlive)
		next->data.render(d2dRT);

	if (next->ltChild) 
		renderSwarm(d2dRT, next->ltChild);
	if (next->rtChild)
		renderSwarm(d2dRT, next->rtChild);
}

void Swarm::shutdownSwarm()
{
	if (m_pLead) {
		if (m_pLead->ltChild)
			shutdownSwarm(m_pLead->ltChild);
		if (m_pLead->rtChild)
			shutdownSwarm(m_pLead->rtChild);
	}

	delete m_pLead;
	m_pLead = nullptr;
}

void Swarm::shutdownSwarm(SwarmNode* next)
{
	if (next->ltChild)
		shutdownSwarm(next->ltChild);
	if (next->rtChild)
		shutdownSwarm(next->rtChild);
	
	delete next;
	next = nullptr;
}

bool Swarm::checkCollision(BaseObj& obj)
{
	return false;
}

