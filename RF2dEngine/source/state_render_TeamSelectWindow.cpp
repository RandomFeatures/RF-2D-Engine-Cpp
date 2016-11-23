#include "RF3d_Global.h"
// Local includes
#include "RF2d_Application.h"
#include "RF2d_Scripting.h"
#include "state_render_TeamSelectWindow.h"

stateTeamSelectWindow::stateTeamSelectWindow(cApp*  pParent)
{
    m_pGame = pParent;
}

void stateTeamSelectWindow::initialize()
{
   // load up whatever resources I need (background, sounds, etc)
    hge->System_Log("INIT TeamSelect Frame");
}

void stateTeamSelectWindow::shutdown()
{
   // release whatever resources I loaded in initialize

}


void stateTeamSelectWindow::frame()
{
    float dt;

	dt=hge->Timer_GetDelta();
	captureinput(dt);

	m_pGame->GetGameObjMangr()->UpdateBeforeRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	// Render graphics
	hge->Gfx_BeginScene();
	m_pGame->GetMainMap()->Render();
	m_pGame->GetMainMap()->RenderTeamSides();
	m_pGame->GetGameObjMangr()->MouseNPC(m_pGame->GetCursor()->x(), m_pGame->GetCursor()->y());
	m_pGame->GetGameObjMangr()->RenderNoNPC(0, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->RenderNoNPC(1, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->RenderNoNPC(2, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

	m_pGame->GetGameObjMangr()->RenderNoNPC(3, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	m_pGame->GetGameObjMangr()->RenderNoNPC(4, dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

	m_pGame->RenderInGameGUI(dt);
	m_pGame->RenderCursor(dt);
	//g_Game->GetFont()->printf(5,5,"Team Select\ndt:%.3f\nFPS:%d", dt, hge->Timer_GetFPS());
	//g_Game->GetFont()->printf(300,5,"Mouse( X:%i Y:%i )", g_Game->GetCursor()->WorldX(), g_Game->GetCursor()->WorldY());

	hge->Gfx_EndScene();
	m_pGame->GetGameObjMangr()->UpdateAfterRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	if (m_pGame->GetGameType() == MPLAY)
		m_pGame->GetMPlayerControl()->Update(); //Process RakNet messages
}


void stateTeamSelectWindow::captureinput(float dt)
{
/*****Local Variables Start*****/
	int		tmpKey;
	bool	bFollowing = false;
	float	x, y;
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
		if (!m_pGame->GetGUIManager()->GUIKeyPress(tmpKey))
		switch (tmpKey)
		{
			case HGEK_ESCAPE:
				m_pGame->GetGUIManager()->HideCharSelectWindow();
				m_pGame->GetGUIManager()->ShowMapSelectWindow();
				StateManager::getInstance()->popState();
				break;
			case HGEK_LBUTTON:
				if (!m_pGame->GetGUIManager()->CharSelectWindow->MouseTest(x,y))
				m_pGame->GetGUIManager()->CharSelectWindow->PlaceCharacter(HGEK_LBUTTON);
				break;
			case HGEK_RBUTTON:
				if (!m_pGame->GetGUIManager()->CharSelectWindow->MouseTest(x,y))
				m_pGame->GetGUIManager()->CharSelectWindow->PlaceCharacter(HGEK_RBUTTON);
				break;
			case HGEK_SUBTRACT:
				hge->System_Snapshot();
				break;
		}
	}

}


