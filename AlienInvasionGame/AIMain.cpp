#include <string>
#include "D3DUtils.h"
#include "D3DApp.h"
#include "Camera.h"
#include "D2DGrafix.h"
#include "D2DGeometry.h"
#include "DXInput.h"
#include "Player.h"
#include "Swarm.h"
#include "Sound.h"

#pragma comment(lib, "D3DApp.lib")
#pragma comment(lib, "D2DGraphics.lib")
#pragma comment(lib, "DXInput.lib")
#pragma comment(lib, "Sound.lib")

using std::wstring;
using namespace DirectX;
using namespace Fugui;

class InvasionApp : public D3DApp, public D2DGrafix
{

public:
	enum GameStates {
		GState_MainMenu = 0,
		GState_InGame,
		GState_Pause,
		GState_Credits,
		GState_Options,
		GState_Quit
	};

	enum CursorPos {
		CPos_Play,
		CPos_Options,
		CPos_Credits,
		CPos_Quit
	};

	Camera	camera;
	Swarm	swarms[12];
	Player	player;

	D2DText		txtHealth;
	D2DText		txtLives;
	D2DText		txtPoints;
	D2DText		txtBullets;

	TD2D_RectF	menuCursorRect;
	TD2D_RectF	cursorPos;

	GameStates	currentState;
	CursorPos		menuCursorPos;

	int		backgroundBmpID;
	int		arrowBmpID;
	int		cursorBmpID;

	FMOD::Sound*	mainMenuLoopID;
	FMOD::Sound*	ingameLoopID;
	int		firearmSfxID;
	int		playerHitSfxID;
	int		alienHitSfxID;
	int		playerDeathSfxID;
	int		alienDeathSfxID;

	virtual void initApp(TCHAR* title, HINSTANCE hinst, UINT x, UINT y, UINT dx, UINT dy, bool fs);
	virtual void onResetDevice() {}
	virtual void onLostDevice() {}

	virtual void updateApp(real dt);
	virtual void renderApp();

	void handleCursor();
	void initUI();
	void renderUI();
	void updateUI();

	// Initialize the window and Direct3D
	InvasionApp(TCHAR* title, HINSTANCE hinst, int x, int y, int dx, int dy, bool fs);	
	virtual ~InvasionApp();
};

InvasionApp::InvasionApp(TCHAR* title, HINSTANCE hinst, int x, int y, int dx, int dy, bool fs) :
D3DApp(title, hinst, x, y, dx, dy, fs)
{
//	loadCursor(L"../../../../../Development/Assets/AlienInvasion/CenterTarget.png");
	// initialize modules with app specific values
	initApp(title, hinst, x, y, dx, dy, fs);
}

InvasionApp::~InvasionApp()
{
	swarms[0].shutdownSwarm();
	swarms[1].shutdownSwarm();
	swarms[2].shutdownSwarm();
	g_sound->shutdown();
	g_input->shutdown();
}

void InvasionApp::initApp(TCHAR* title, HINSTANCE hinst, UINT x, UINT y, UINT dx, UINT dy, bool fs)
{
	//----------------------------------------------------------------------
	// initialize APIs
	//----------------------------------------------------------------------
	g_input->init(m_hWnd, hinst, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	initD2D(m_pDXGISwapChain);
	camera.pos() = XMFLOAT4(0.0f, 2.0f, -5.0f, 1.0f);
	camera.setLens(3.14f * 0.25, m_vOutputSize.x / m_vOutputSize.y, 0.01f, 100.0f);
	g_sound->init();

	//----------------------------------------------------------------------
	// load bitmaps / textures / sounds
	//----------------------------------------------------------------------
	backgroundBmpID = m_bmpCtrlr.loadBmpFromFile(m_pD2DRenderTarget, m_pWicImagingFactory, L"Assets/Materials/MainMenuBackground.png", 1024.0f, 768.0f);
	arrowBmpID = m_bmpCtrlr.loadBmpFromFile(m_pD2DRenderTarget, m_pWicImagingFactory, L"Assets/Materials/GreenArrow.png", 10.0f, 10.0f);
	cursorBmpID = m_bmpCtrlr.loadBmpFromFile(m_pD2DRenderTarget, m_pWicImagingFactory, L"Assets/Materials/CenterTarget.png", 10.0f, 10.0f);
	mainMenuLoopID = g_sound->loadStream("Assets/SFX/Influken-Luke_Sto-10002_hifi.mp3");
	ingameLoopID = g_sound->loadStream("Assets/SFX/action prt 2.mp3");

	//----------------------------------------------------------------------
	// initialize game objects
	//----------------------------------------------------------------------
	player.init(this, m_pD2DRenderTarget, XMFLOAT3(200.0f, 100.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 20.0f);
	player.getMainGun().init(Gun::GType_Firearm, m_pD2DRenderTarget);
	player.getAltGun().init(Gun::GType_SubMachine, m_pD2DRenderTarget);
	swarms[0].initSwarm(13, Swarm::SwarmType::Swarm_Linear, m_pD2DRenderTarget, XMFLOAT3(300.0f, 800.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), player.getPos(), 100.0f, 15.0f, 150.0f);
	swarms[1].initSwarm(20, Swarm::SwarmType::Swarm_Linear, m_pD2DRenderTarget, XMFLOAT3(-30.0f, -25.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), player.getPos(), 150.0f, 15.0f, 200.0f);
	swarms[2].initSwarm(20, Swarm::SwarmType::Swarm_Linear, m_pD2DRenderTarget, XMFLOAT3(-300.0f, -250.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), player.getPos(), 150.0f, 15.0f, 200.0f);

	menuCursorRect = { 85.0f, 353.0f, 110.0f, 378.0f };
	cursorPos = { 1024 << 2, 768 << 2, (1024 << 2) + 32, (768 << 2) + 32 };//{ g_input->getMousePosX(), g_input->getMousePosY(), g_input->getMousePosX() + 32.0f, g_input->getMousePosY() + 32.0f };
	g_input->setSensitivity(0.25f);

	//----------------------------------------------------------------------
	// init the UI
	//----------------------------------------------------------------------
	initUI();

	//----------------------------------------------------------------------
	// Set initial game sates
	//----------------------------------------------------------------------
	currentState = GState_MainMenu;
	menuCursorPos = CPos_Play;

	g_sound->playStream(mainMenuLoopID);
	g_sound->update();

}

void InvasionApp::updateApp(real dt)
{
	g_sound->update();
	g_input->pollBothTypes();
	camera.update(dt, 0.0f);
	cursorPos = { (float)g_input->getMousePosX(), (float)g_input->getMousePosY(), (float)g_input->getMousePosX() + 32.0f, (float)g_input->getMousePosY() + 32.0f };

	switch (currentState)
	{
	case GState_MainMenu:
	{
		handleCursor();
	}
	break;
	case GState_Options:
	{}
	break;
	case GState_Credits:
	{}
	break;
	case GState_InGame:
	{
		//g_sound->playStream(ingameLoopID);
		g_sound->update();
		player.update(dt);
		swarms[0].setTarget(player.getPos());
		swarms[0].updateSwarm(dt, player, player.getMainGun().getBulletList());
		swarms[1].setTarget(player.getPos());
		swarms[1].updateSwarm(dt, player, player.getMainGun().getBulletList());
		swarms[2].setTarget(player.getPos());
		swarms[2].updateSwarm(dt, player, player.getMainGun().getBulletList());
	//	for (swarms[0].)

		if (g_input->keyDownOnce(DIK_SPACE))
			currentState = GState_Pause;

		updateUI();
	}
	break;
	case GState_Pause:
	{
		if (g_input->keyDownOnce(DIK_SPACE))
			currentState = GState_InGame;
	}
	break;
	case GState_Quit:
	{
		m_bAppRun = false;
	}
	break;
	} // end switch(currentState)
}

void InvasionApp::renderApp()
{
	m_pD2DRenderTarget->BeginDraw();
	switch (currentState)
	{
	case GState_MainMenu:
	{
		//g_sound->playStream(mainMenuLoopID);
		m_bmpCtrlr.drawBackground(m_pD2DRenderTarget, backgroundBmpID, 1024.0f, 768.0f);
		m_bmpCtrlr.drawBmp(m_pD2DRenderTarget, arrowBmpID, menuCursorRect);
		m_bmpCtrlr.drawBmp(m_pD2DRenderTarget, cursorBmpID, cursorPos);
	}
	break;
	case GState_Options:
	{}
	break;
	case GState_Credits:
	{}
	break;
	case GState_InGame:
	{
		//g_sound->playStream(mainMenuLoopID);

		m_bmpCtrlr.drawBmp(m_pD2DRenderTarget, cursorBmpID, cursorPos);
		player.render(m_pD2DRenderTarget);
 		swarms[0].renderSwarm(m_pD2DRenderTarget);
		swarms[1].renderSwarm(m_pD2DRenderTarget);
		swarms[2].renderSwarm(m_pD2DRenderTarget);

		renderUI();
	}
	break;
	case GState_Pause:
	{
		player.render(m_pD2DRenderTarget);
 		swarms[0].renderSwarm(m_pD2DRenderTarget);
		swarms[1].renderSwarm(m_pD2DRenderTarget);
		swarms[2].renderSwarm(m_pD2DRenderTarget);

		renderUI();
	}
	break;
	case GState_Quit:
	{
		m_bAppRun = false;
	}
	break;
	} // end switch(currentState)
	m_pD2DRenderTarget->EndDraw();
}

void InvasionApp::handleCursor()
{
	switch (currentState)
	{
	case GState_MainMenu: {
		if (g_input->keyDownOnce(DIK_DOWNARROW)) {
			switch (menuCursorPos)
			{
			case CPos_Play: menuCursorPos = CPos_Options; break;
			case CPos_Options: menuCursorPos = CPos_Quit; break;
			case CPos_Quit: menuCursorPos = CPos_Play; break;
			} // end switch(cursorPos)
		}

		if (g_input->keyDownOnce(DIK_UPARROW)) {
			switch (menuCursorPos)
			{
			case CPos_Play: menuCursorPos = CPos_Quit; break;
			case CPos_Options: menuCursorPos = CPos_Play; break;
			case CPos_Quit: menuCursorPos = CPos_Options; break;
			}
		}

		switch (menuCursorPos)
		{
		case CPos_Play: menuCursorRect = { 85.0f, 353.0f, 110.0f, 378.0f }; break;
		case CPos_Options: menuCursorRect = { 85.0f, 422.0f, 110.0f, 446.0f }; break;
		case CPos_Quit: menuCursorRect = { 85.0f, 492.0f, 110.0f, 517.0f }; break;
		} // end switch(cursorPos)

		if (g_input->keyDownOnce(DIK_RETURN)) {
			switch (menuCursorPos)
			{
			case CPos_Play: currentState = GState_InGame; g_sound->stopStream(mainMenuLoopID);  g_sound->playStream(ingameLoopID);  break;
			case CPos_Options: currentState = GState_Options; break;
			case CPos_Credits: currentState = GState_Credits; break;
			case CPos_Quit: currentState = GState_Quit; break;
			}
		}
	} // end case GState_MainMenu
	} // switch(currentState)
}

void InvasionApp::initUI()
{
	txtLives.initText(m_pD2DRenderTarget, L"Verdana", 20.0f, XMFLOAT3(0.70f, 0.0f, 0.0f));
	txtHealth.initText(m_pD2DRenderTarget, L"Verdana", 20.0f, XMFLOAT3(0.70f, 0.0f, 0.0f));
	txtBullets.initText(m_pD2DRenderTarget, L"Verdana", 20.0f, XMFLOAT3(0.90f, 0.90f, 0.90f));
	txtPoints.initText(m_pD2DRenderTarget, L"Verdana", 20.0f, XMFLOAT3(0.70f, 0.70f, 0.70f));

	wstring temp = L"Lives: "/* + m_nLives*/;
	wchar_t  t[10];
	_itow_s(player.getLives(), t, 10);
	temp += t;

	TD2D_RectF rect;
	rect.top = 10.0f;
	rect.left = 5.0f;
	rect.right = 100.0f;
	rect.bottom = 35.0f;
	
	txtLives.setTextRect(rect);
	txtLives.setText(temp.c_str());

	temp = L"Health: ";
	_itow_s((int)player.getLife(), t, 10);
	temp += t;
	rect.top = 35.0f;
	rect.right = 125.0f;
	rect.bottom = 70.0f;

	txtHealth.setText(temp.c_str());
	txtHealth.setTextRect(rect);

	_itow_s(player.getMainGun().getMaxClipCount(), t, 10);
	rect.top = 70.0f;
	rect.bottom = 105.0f;
	rect.right = 50.0f;

	txtBullets.setText(t);
	txtBullets.setTextRect(rect);

	temp = L"Score: ";
	_itow_s((int)player.getPoints(), t, 10);
	temp += t;
	rect.top = 110.0f;
	rect.bottom = 140.0f;
	rect.right = 80.0f;

	txtPoints.setText(temp.c_str());
	txtPoints.setTextRect(rect);
}

void InvasionApp::renderUI()
{
	txtLives.renderText(m_pD2DRenderTarget);
	txtHealth.renderText(m_pD2DRenderTarget);
	txtBullets.renderText(m_pD2DRenderTarget);
	txtPoints.renderText(m_pD2DRenderTarget);
}

void InvasionApp::updateUI()
{
	wstring temp = L"Lives: "/* + m_nLives*/;
	wchar_t  t[10];
	_itow_s(player.getLives(), t, 10);
	temp += t;

	txtLives.setText(temp.c_str());

	temp = L"Health: ";
	_itow_s((int)player.getLife(), t, 10);
	temp += t;

	txtHealth.setText(temp.c_str());

	_itow_s(player.getMainGun().getCurrClipCount(), t, 10);

	txtBullets.setText(t);

	temp = L"Score: ";
	_itow_s((int)player.getPoints(), t, 10);
	temp += t;

	txtPoints.setText(temp.c_str());
}

//----------------------------------------------------------------------
// Entry Point....
//----------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	InvasionApp myGame(L"Alien Invasion v3.0", hInstance, (GetSystemMetrics(SM_CXSCREEN) >> 1) - (1024 >> 1), (GetSystemMetrics(SM_CYSCREEN) >> 1) - (768 >> 1), 1024, 768, false);
	if (myGame.isAppRunning())
		myGame.runApp();

	return -1;
}