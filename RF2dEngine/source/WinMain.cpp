/**************************************************
RF3d_WinMain.cpp
Random Features 3D RPG Main Window

by Allen Halsted (12 June 2002)

Required libraries:

**************************************************/
//#include "RF3d_Global.h"

// Local includes
#include "WinMain.h"
#include "RF2d_Application.h"
#include "RF2d_GameObjectManager.h"
#include "RF2d_MapObject.h"
#include "hge.h"


// This function will be called by HGE
// when the application receives focus.
// We use it here to update the render
// target's texture handle that may change
// when the focus is lost.
bool FocusGainFunc()
{
	if (g_Game)
	{

		if (g_Game->GetGameObjMangr())
		{
			g_Game->GetGameObjMangr()->GainFocus();
		}
		if (g_Game->GetMainMap()){
            g_Game->GetMainMap()->GainFocus();
		}
	}
//	g_Game->tar->SetTexture(hge->Target_GetTexture(g_Game->target));
	return false;
}


//Call back for HGE to update each frame
bool FrameFunc()
{
	//false to keep going
	//true to end
	return !g_Game->Frame();
}


//Global Ref to the Application
cApp  *g_Game;
HGE *hge=0;


int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
   	//Create Game Object
	cApp App;
	//App.SetLuaVM(luaVM); //Give The Game a pointer to Lua
  return App.Run(); //When this is done... we are done
}


