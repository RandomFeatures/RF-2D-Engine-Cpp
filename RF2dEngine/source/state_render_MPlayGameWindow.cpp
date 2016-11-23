#include "RF3d_Global.h"
// Local includes
#include "RF2d_Application.h"
#include "RF2d_Scripting.h"
#include "state_render_MPlayGameWindow.h"

stateMPlayGameWindow::stateMPlayGameWindow(cApp*  pParent)
{
    m_pGame = pParent;
}

void stateMPlayGameWindow::initialize()
{
   // load up whatever resources I need (background, sounds, etc)
    hge->System_Log("INIT MPlay Main");
}

void stateMPlayGameWindow::shutdown()
{
   // release whatever resources I loaded in initialize

}


void stateMPlayGameWindow::frame()
{
    float dt;

    m_pGame->StartMPlayGame();

	dt=hge->Timer_GetDelta();
	captureinput(dt);
	m_pGame->GetTargetSystem()->Update();
   	m_pGame->GetWeatherControl()->Update(dt);
	// Render graphics
	m_pGame->GetGameObjMangr()->UpdateBeforeRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());
	hge->Gfx_BeginScene();
	m_pGame->GetMainMap()->Render();
	m_pGame->GetTargetSystem()->Render(dt,0,0);
	m_pGame->GetGameObjMangr()->MouseNPC(m_pGame->GetCursor()->x(), m_pGame->GetCursor()->y());
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
	m_pGame->GetCharGUI()->Render();
	m_pGame->RenderInGameGUI(dt);
	m_pGame->RenderCursor(dt);
	m_pGame->GetFont()->printf(5,5,HGETEXT_LEFT,"MPlayer\ndt:%.3f\nFPS:%d", dt, hge->Timer_GetFPS());
	m_pGame->GetFont()->printf(300,5,HGETEXT_LEFT,"Mouse( X:%i Y:%i )", m_pGame->GetCursor()->WorldX(), m_pGame->GetCursor()->WorldY());
	hge->Gfx_EndScene();
	m_pGame->GetGameObjMangr()->UpdateAfterRender(dt,m_pGame->GetMainMap()->ScreenX(),m_pGame->GetMainMap()->ScreenY());

	m_pGame->GetMPlayerControl()->Update(); //Process RakNet messages

}


void stateMPlayGameWindow::captureinput(float dt)
{

/*****Local Variables Start*****/
	int		tmpKey;
	bool	bFollowing = false;
	float	x, y;
	bool	bUIClicked = false;
/*****Local Variables End*****/

	hge->Input_GetMousePos(&x,&y);
	m_pGame->GetCursor()->Update(dt,x,y);

	movetimer += dt;

	m_pGame->GetGUIManager()->GUIMouseMove(x,y);

	if (hge->Input_GetKeyState(HGEK_LBUTTON))
        m_pGame->GetGUIManager()->GUIMouseDown(x,y);
	else m_pGame->GetGUIManager()->GUIMouseUp(x,y);

	if (movetimer > 0.01f) movetimer = 0;


    if (!m_pGame->GetGUIManager()->GUICharPress(hge->Input_GetChar()))
	{
		tmpKey = hge->Input_GetKey();
		if (!m_pGame->GetGUIManager()->GUIKeyPress(tmpKey))
		switch (tmpKey)
		{
			case HGEK_ESCAPE:
				m_pGame->GetGUIManager()->HideAllWindows();
				m_pGame->ChangeGameState(EXIT, ALL);
				//m_StateManager.Push(SPlayerMenuFrame, this);
				break;
			case HGEK_LBUTTON:
				{
					//only of UI is visible
					if (m_pGame->GetCharGUI()->isShown())
						bUIClicked = m_pGame->GetCharGUI()->UserClick();
						//only if target is active and UI was not clicked
					if (m_pGame->GetTargetSystem()->isShown() && !bUIClicked)
						m_pGame->GetTargetSystem()->UserClick();
					break;
				}
			case HGEK_RBUTTON:
				{
					//only of UI is visible
					if (m_pGame->GetCharGUI()->isShown())
						bUIClicked = m_pGame->GetCharGUI()->UserClick();
					//only if target is active and UI was not clicked
					if (m_pGame->GetTargetSystem()->isShown() && !bUIClicked)
						m_pGame->GetTargetSystem()->Cancel();
					break;
				}
			case HGEK_1:
 				 m_pGame->GetBattleControl()->GetSource()->SetAction(ATTACK);//attack
				break;
			case HGEK_2:
 				m_pGame->GetBattleControl()->GetSource()->SetAction(WALK);//walk
				break;
			case HGEK_3:
 				m_pGame->GetBattleControl()->GetSource()->SetAction(DEATH);//death
				break;
			case HGEK_4:
 				m_pGame->GetBattleControl()->GetSource()->SetAction(PAIN);//pain
				break;
			case HGEK_SUBTRACT:
				hge->System_Snapshot();
				break;
		}
	}


}


