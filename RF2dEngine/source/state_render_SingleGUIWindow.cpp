#include "RF3d_Global.h"
// Local includes
#include "RF2d_Application.h"
#include "state_render_SingleGUIWindow.h"

stateSingleGUIWindow::stateSingleGUIWindow(cApp*  pParent)
{
    m_pGame = pParent;
}

void stateSingleGUIWindow::initialize()
{
   // load up whatever resources I need (background, sounds, etc)
    char tmpFilePath[256];
	strcpy(tmpFilePath, m_pGame->GetGamePath(TEXTURE));
	strcat(tmpFilePath, "\\GUI\\logo.png");
	LogoTex=hge->Texture_Load(tmpFilePath);
	LogoSprite=new hgeSprite(LogoTex, 0, 0, 512, 128);
}

void stateSingleGUIWindow::shutdown()
{
   // release whatever resources I loaded in initialize
   hge->Texture_Free(LogoTex);
   delete	LogoSprite;
   LogoSprite = NULL;
}


void stateSingleGUIWindow::frame()
{
     float dt;

    // render our background, etc.
    dt=hge->Timer_GetDelta();
    captureinput(dt);
   	m_pGame->GetWeatherControl()->Update(dt);
	m_pGame->GetGameObjMangr()->UpdateBeforeRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
    // Render graphics
	hge->Gfx_BeginScene();
	m_pGame->GetMainMap()->Render();

    m_pGame->GetGameObjMangr()->Render(0, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(1, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(2, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

	m_pGame->GetWeatherControl()->RenderClouds();
	m_pGame->GetWeatherControl()->RenderGroundFog();
	m_pGame->GetWeatherControl()->RenderWater();

	m_pGame->GetGameObjMangr()->Render(3, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(4, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->Render(5, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

	m_pGame->GetWeatherControl()->RenderFog();
	m_pGame->GetWeatherControl()->RenderSnow();


	//g_Game->GetCharGUI()->Render();
	if (LogoSprite) LogoSprite->Render(144,84);

	m_pGame->RenderInGameGUI(dt);
	m_pGame->GetCursor()->Render();
	//m_pGame->GetFont()->printf(5,5,"dt:%.3f\nFPS:%d", dt, hge->Timer_GetFPS());
	//m_pGame->GetFont()->printf(300,5,"Mouse( X:%i Y:%i )", m_pGame->GetCursor()->WorldX(), m_pGame->GetCursor()->WorldY());

	hge->Gfx_EndScene();
	m_pGame->GetGameObjMangr()->UpdateAfterRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	//m_pGame->GetMPlayerControl()->Update(); //Process RakNet messages
}


void stateSingleGUIWindow::captureinput(float dt)
{

/*****Local Variables Start*****/
	int tmpKey;
	float x,y;
/*****Local Variables End*****/
	hge->Input_GetMousePos(&x,&y);
	m_pGame->GetCursor()->Update(dt,x,y);

    m_pGame->GetGUIManager()->GUIMouseMove(x,y);
	if (hge->Input_GetKeyState(HGEK_LBUTTON))
        m_pGame->GetGUIManager()->GUIMouseDown(x,y);
	else m_pGame->GetGUIManager()->GUIMouseUp(x,y);


    if (!m_pGame->GetGUIManager()->GUICharPress(hge->Input_GetChar()))
	{
		tmpKey = hge->Input_GetKey();
		m_pGame->GetGUIManager()->GUIKeyPress(tmpKey);
		if (tmpKey==HGEK_ESCAPE)
		{
			StateManager::getInstance()->popState();
		};
	}


}


