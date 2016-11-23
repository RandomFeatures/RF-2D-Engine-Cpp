#include "RF3d_Global.h"
// Local includes
#include "RF2d_Application.h"
#include "RF2d_Scripting.h"
#include "menuitem.h"
#include "state_menu_Main.h"

stateMenuMain::stateMenuMain(cApp*  pParent)
{
    m_pGame = pParent;
}

void stateMenuMain::initialize()
{
   // load up whatever resources I need (background, sounds, etc)
   char tmpFilePath[256];
  	strcpy(tmpFilePath, g_Game->GetGamePath(FONT));
	strcat(tmpFilePath, "font1.fnt");
	m_MenuFont=new hgeFont(tmpFilePath);
	snd=hge->Effect_Load("resources\\Audio\\SFX\\click7.wav");
	gui=new hgeGUI();
	gui->AddCtrl(new hgeGUIMenuItem(1,m_MenuFont,snd,400,200,0.0f,"Start"));
	gui->AddCtrl(new hgeGUIMenuItem(2,m_MenuFont,snd,400,240,0.1f,"Single Battle"));
	gui->AddCtrl(new hgeGUIMenuItem(3,m_MenuFont,snd,400,280,0.2f,"Multiplayer"));
	gui->AddCtrl(new hgeGUIMenuItem(4,m_MenuFont,snd,400,320,0.3f,"Options"));
	gui->AddCtrl(new hgeGUIMenuItem(5,m_MenuFont,snd,400,360,0.4f,"Credits"));
	gui->AddCtrl(new hgeGUIMenuItem(6,m_MenuFont,snd,400,400,0.5f,"Exit"));

	gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
	//gui->SetCursor(g_Game->m_Cursor);
	gui->Enter();

	strcpy(tmpFilePath, g_Game->GetGamePath(TEXTURE));
	strcat(tmpFilePath, "\\GUI\\logo.png");
	LogoTex=hge->Texture_Load(tmpFilePath);
	LogoSprite=new hgeSprite(LogoTex, 0, 0, 512, 128);
}

void stateMenuMain::shutdown()
{
   // release whatever resources I loaded in initialize
	if (m_MenuFont) delete m_MenuFont;
	if (gui) delete gui;
	hge->Texture_Free(LogoTex);
	hge->Effect_Free(snd);
	delete	LogoSprite;

}


void stateMenuMain::frame()
{
/*****Local Variables Start*****/
  //cApp *g_Game = (cApp*)Ptr;
  float dt;
  int id;
/*****Local Variables End*****/


    dt=hge->Timer_GetDelta();

    // Process a frame of menu
    captureinput(dt);

	id=gui->Update(dt);
	if(id == -1 )
	{
		switch(lastid)
		{
			case 1:
				m_pGame->NewGame();
				break;
			case 2:
				MenuSingleMap();
				//g_Game->ChangeGameState(SPMENU,ALL);
				break;
			case 3:
				//lastid=0;
				//gui->Enter();
				m_pGame->ChangeGameState(MPMENU,CURRENT);
				break;
			case 4:
				gui->Enter();
				break;
			case 5:
				gui->Enter();
				break;
			case 6:
				m_pGame->ChangeGameState(EXIT,CURRENT);
				break;
		}
	  return;
	}
	else if(id) { lastid=id; gui->Leave();}// else lastid=0;




	m_pGame->GetGameObjMangr()->UpdateBeforeRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetWeatherControl()->Update(dt);

    // Render graphics
	hge->Gfx_BeginScene();
	m_pGame->GetMainMap()->Render();
	m_pGame->GetGameObjMangr()->Render(0, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(1, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

	m_pGame->GetWeatherControl()->RenderClouds();
	m_pGame->GetGameObjMangr()->Render(2, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	//ground fog
	m_pGame->GetWeatherControl()->RenderGroundFog();
	m_pGame->GetGameObjMangr()->Render(3, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(4, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(5, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	//full fog
	m_pGame->GetWeatherControl()->RenderFog();
	m_pGame->GetWeatherControl()->RenderSnow();


	LogoSprite->Render(144,84);
	gui->Render();
	m_pGame->GetCursor()->Render();
	hge->Gfx_EndScene();
	m_pGame->GetGameObjMangr()->UpdateAfterRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
}

void stateMenuMain::MenuSingleMap()
{
	//m_GameType = SINGMAP;
	m_pGame->GetGUIManager()->HideGameWindows();
	m_pGame->GetGUIManager()->ShowMapSelectWindow();
	m_pGame->ChangeGameState(GUI, NONE);
};

void stateMenuMain::captureinput(float dt)
{
	float x,y;
	hge->Input_GetMousePos(&x,&y);
    m_pGame->GetCursor()->Update(dt,x,y);


	if (hge->Input_GetKey()==HGEK_ESCAPE)
	{
		//m_pGame->m_StateManager->PopAll(this);
	};

	if (hge->Input_GetKey()==HGEK_SUBTRACT)
	{
		hge->System_Snapshot();
	};
};


