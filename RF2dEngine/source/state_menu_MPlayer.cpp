#include "RF3d_Global.h"
// Local includes
#include "RF2d_Application.h"
#include "RF2d_Scripting.h"
#include "menuitem.h"
#include "state_menu_MPlayer.h"

stateMenuMPlayer::stateMenuMPlayer(cApp*  pParent)
{
    m_pGame = pParent;
}

void stateMenuMPlayer::initialize()
{
   // load up whatever resources I need (background, sounds, etc)
    char tmpFilePath[256];
		strcpy(tmpFilePath, g_Game->GetGamePath(FONT));
		strcat(tmpFilePath, "font1.fnt");
		m_MenuFont1=new hgeFont(tmpFilePath);
		snd=hge->Effect_Load("resources\\Audio\\SFX\\click7.wav");
		gui1=new hgeGUI();
		gui1->AddCtrl(new hgeGUIMenuItem(1,m_MenuFont1,snd,400,200,0.0f,"Host A Game"));
		gui1->AddCtrl(new hgeGUIMenuItem(2,m_MenuFont1,snd,400,240,0.1f,"Join A Game"));
		gui1->AddCtrl(new hgeGUIMenuItem(3,m_MenuFont1,snd,400,280,0.2f,"Main Menu"));
		gui1->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		//gui->SetCursor(g_Game->m_Cursor);
		gui1->Enter();

		strcpy(tmpFilePath, g_Game->GetGamePath(TEXTURE));
		strcat(tmpFilePath, "\\GUI\\logo.png");
		LogoTex=hge->Texture_Load(tmpFilePath);
		LogoSprite=new hgeSprite(LogoTex, 0, 0, 512, 128);
}

void stateMenuMPlayer::shutdown()
{
   // release whatever resources I loaded in initialize
	if (gui1) delete gui1;
		gui1 = NULL;
  		if (m_MenuFont1) delete m_MenuFont1;
		m_MenuFont1 = NULL;
		hge->Texture_Free(LogoTex);
		hge->Effect_Free(snd);
		delete	LogoSprite;
		LogoSprite = NULL;

}


void stateMenuMPlayer::frame()
{
/*****Local Variables Start*****/
	float dt;
	int id;
/*****Local Variables End*****/


    dt=hge->Timer_GetDelta();

	// Process a frame of menu
	captureinput(dt);

    m_pGame->GetMPlayerControl()->Update(); //Process RakNet messages
	//an update from the server to change maps will cause these objects to be deleted so
	//no need to continue
    if (!gui1) return;
    if (!m_MenuFont1) return;

	id=gui1->Update(dt);

	if(id == -1)
	{
		switch(lastid)
		{
			case 0:
				break;
			case 1:
				lastid=0;
				MenuHost();
				break;
			case 2:
				lastid=0;
				MenuConnect();
				break;
			case 3:
				m_pGame->ChangeGameState(MMENU, CURRENT);
				break;
		}

	  return;
	}
	else if(id) { lastid=id; gui1->Leave();}// else lastid=0;

    // Render graphics
	m_pGame->GetGameObjMangr()->UpdateBeforeRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetWeatherControl()->Update(dt);

	hge->Gfx_BeginScene();
	m_pGame->GetMainMap()->Render();
	m_pGame->GetGameObjMangr()->Render(0, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(1, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

	m_pGame->GetWeatherControl()->RenderClouds();

	m_pGame->GetGameObjMangr()->Render(2, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetWeatherControl()->RenderGroundFog();
	m_pGame->GetGameObjMangr()->Render(3, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(4, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(5, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetWeatherControl()->RenderFog();
	m_pGame->GetWeatherControl()->RenderSnow();

		//g_Game->GetCharGUI()->Render();
	if (LogoSprite) LogoSprite->Render(144,84);
	if (gui1) gui1->Render();
	m_pGame->GetCursor()->Render();
	//g_Game->GetFont()->printf(5,5,"dt:%.3f\nFPS:%d", dt, hge->Timer_GetFPS());
	hge->Gfx_EndScene();
	m_pGame->GetGameObjMangr()->UpdateAfterRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

}


void stateMenuMPlayer::captureinput(float dt)
{
	float x,y;
	hge->Input_GetMousePos(&x,&y);
	m_pGame->GetCursor()->Update(dt,x,y);

	if (hge->Input_GetKey()==HGEK_ESCAPE)
	{
		m_pGame->ChangeGameState(MMENU, CURRENT);
	};
}

void stateMenuMPlayer::MenuConnect()
{
	//m_GameType = MPLAY;
	m_pGame->GetGUIManager()->HideGameWindows();
	m_pGame->GetGUIManager()->ShowConnectWindow();
	m_pGame->ChangeGameState(GUI,CURRENT);
};


void stateMenuMPlayer::MenuHost()
{
	//m_GameType = MPLAY;
	m_pGame->GetGUIManager()->HideGameWindows();
	m_pGame->GetGUIManager()->ShowMapSelectWindow();
	m_pGame->ChangeGameState(GUI, CURRENT);
};
