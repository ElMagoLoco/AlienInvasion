#include <string>
#include "Player.h"
#include "DXInput.h"

using std::wstring;
using namespace Fugui;
using namespace DirectX;

Player::Player()
{
	m_nLives = 3;
	m_fPoints = 0.0f;
	m_fLife = 100.0f;
	m_fDamage = 25.0f;
}


Player::~Player()
{
	shutdown();
}

void Player::init(D2DGrafix* d2d, ID2D_RenderTarget* d2dRT, XMFLOAT3& pos, XMFLOAT3& velo, XMFLOAT3& accel, XMFLOAT4& color, float dimen)
{
	m_vPos = pos;
	m_vAccel = accel;
	m_vVelo = velo;
	m_vColor = color;
	m_fDimen = dimen;
	m_fSpeed = 200.0f;
	m_fMaxSpeed = 111140.0f;
	m_vTarget = XMFLOAT3(m_vPos.x, m_vPos.y, 0.0f);

	//m_mainGun.init(d2d, d2dRT, m_vPos, m_vVelo, m_vAccel, m_vColor, 3.0f);

	m_brush.createSolidColorBrush(D2D1::ColorF(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w), d2dRT, 1.0f);
}

void Player::render(ID2D_RenderTarget* d2dRT)
{
	//m_txtLives.renderText(d2dRT);

	float top = m_vPos.y - m_fDimen;
	float left = m_vPos.x - m_fDimen;

	m_brush.drawRectangle(d2dRT, top, left, m_fDimen, m_fDimen);

	m_mainGun.render(d2dRT);
}

void Player::update(real dt)
{
	if (m_fLife < 0.0f) {
		--m_nLives;
		m_fLife = 100.0f;
	}

	XMFLOAT3 currPos;

	currPos.x = (float)g_input->getMousePosX() + 20.0f;
	currPos.y = (float)g_input->getMousePosY() + 20.0f;
	currPos.z = 1.0f;

	if (g_input->mouseBtnDown(LT_MOUSE_BTN))/*g_input->mouseBtnDown(LT_MOUSE_BTN)*/
		m_mainGun.fire(dt, m_vPos, currPos);

	if (g_input->mouseBtnDown(RT_MOUSE_BTN))
		m_altGun.fire(dt, m_vPos, currPos);

	// get keyboard input
	if (g_input->keyDown(DIK_W) || g_input->keyDown(DIK_UPARROW))
		m_vVelo.y -= m_fSpeed;
	if (g_input->keyDown(DIK_S) || g_input->keyDown(DIK_DOWNARROW))
		m_vVelo.y += m_fSpeed;
	if (g_input->keyDown(DIK_A) || g_input->keyDown(DIK_LEFTARROW))
		m_vVelo.x -= m_fSpeed;
	if (g_input->keyDown(DIK_D) || g_input->keyDown(DIK_RIGHTARROW))
		m_vVelo.x += m_fSpeed;
	// check if we are past our bounds
	// on x
	if (m_vVelo.x > m_fMaxSpeed)
		m_vVelo.x = m_fMaxSpeed;
	else if (m_vVelo.x < -m_fMaxSpeed)
		m_vVelo.x = -m_fMaxSpeed;
	// on y
	if (m_vVelo.y > m_fMaxSpeed)
		m_vVelo.y = m_fMaxSpeed;
	else if (m_vVelo.y < -m_fMaxSpeed)
		m_vVelo.y = -m_fMaxSpeed;

	// do the math...
	float time = (dt * dt) * 0.5f;
	
	XMVECTOR accel = XMLoadFloat3(&m_vAccel);
	XMVECTOR	pos = XMLoadFloat3(&m_vPos);
	XMVECTOR velo = XMLoadFloat3(&m_vVelo);
	XMVECTOR finalPos = pos + velo * dt + accel * time;
	// TODO: clamp movement to the window
	XMStoreFloat3(&m_vPos, finalPos);
	velo = XMVectorZero();
	XMStoreFloat3(&m_vVelo, velo);

	// update the bullets for the guns
	m_mainGun.update(dt);
	m_altGun.update(dt);

// 	wstring temp = L"Lives: " + m_nLives;
// 	m_txtLives.setText(temp.c_str());
}

void Player::shutdown()
{
	m_mainGun.shutdown();
}
