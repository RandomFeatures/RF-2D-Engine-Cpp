#include "RF3d_Global.h"
// Local includes
#include "RF2d_Application.h"
#include "RF2d_Scripting.h"
#include "state_render_LoadScreen.h"

stateLoadScreen::stateLoadScreen(cApp*  pParent)
{
    m_pGame = pParent;
}

void stateLoadScreen::initialize()
{
   // load up whatever resources I need (background, sounds, etc)
   	char tmpFilePath[256];
	strcpy(tmpFilePath, g_Game->GetGamePath(TEXTURE));
	strcat(tmpFilePath, "\\GUI\\logo.png");
	LoadBGtex=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, g_Game->GetGamePath(TEXTURE));
	strcat(tmpFilePath, "\\GUI\\statusbar.png");
	StatusBartex=hge->Texture_Load(tmpFilePath);

	BGSprite=new hgeSprite(LoadBGtex, 0, 0, 512, 128);
	StatusBarSprite=new hgeSprite(StatusBartex, 0, 16, 256, 16);
	StatusBar= new hgeAnimation(StatusBartex, 256, 0, 0, 0, 1, 16);
}

void stateLoadScreen::shutdown()
{
   // release whatever resources I loaded in initialize
    hge->Texture_Free(LoadBGtex);
	hge->Texture_Free(StatusBartex);
	delete	BGSprite;
	delete	StatusBarSprite;
	delete  StatusBar;
}


void stateLoadScreen::frame()
{
    /*****Local Variables Start*****/
    float dt;
    float prcntdone;

    /*****Local Variables End*****/

    dt=hge->Timer_GetDelta();
	//g_Game->CaptureGameInput(dt);
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(ARGB(255,10,10,10));
	m_pGame->GetFont()->printf(5,15, HGETEXT_LEFT, "Loading: %i", g_Game->GetLoadStatus());
	BGSprite->Render(144,64);
	StatusBarSprite->Render(272,450);

	if (m_pGame->GetLoadStatus()>1)
	prcntdone = (m_pGame->GetLoadStatus()/100.f)*256;
	for(int iloop =0;iloop<prcntdone;iloop++)
	{
		StatusBar->SetFrame(iloop);
		StatusBar->Render(272+iloop,450);
	}
	m_pGame->GetFont()->printf(5,5, HGETEXT_LEFT,"Done:%i",m_pGame->GetLoadStatus());

	hge->Gfx_EndScene();
}


void stateLoadScreen::captureinput(float dt)
{

}





