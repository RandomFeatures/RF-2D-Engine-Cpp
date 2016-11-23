/**************************************************
RF3d_WinMain.cpp
Random Features 3D RPG Main Window

by Allen Halsted (12 June 2002)

Required libraries:

**************************************************/
//#include "RF3d_Global.h"

// Local includes
#include <ini.h>
#include <stdio.h>
#include <sqlite3.h>
#include <libsqlitewrapped.h>
#include "WinMain.h"
#include "RF2d_Application.h"
#include "StateManager.h"
#include "RF2d_Scripting.h"

#include "hge.h"
#include "hgefont.h"
#include "hgegui.h"

#include "RF2d_GameObjectManager.h"
#include "RF2d_MapObject.h"
#include "RF2d_UsedResourceTracker.h"
#include "RF2d_GUIWindow.h"
#include "RF2d_GUIControls.h"
#include "RF2d_GUIManager.h"
#include "RF2d_MPlayer.h"
#include "RF2d_Cursor.h"
#include "RF2d_TargetObject.h"
#include "RF2d_CharGUI.h"
#include "RF2d_WeatherControl.h"
#include "RF2d_BattleControl.h"
#include "RF2d_StaticTextures.h"

#include "state_render_TeamSelectWindow.h"
#include "state_render_SingleGUIWindow.h"
#include "state_render_MPlayGameWindow.h"
#include "state_render_LoadScreen.h"
#include "state_render_GameWindow.h"

//---------------------------------------------------------------
//Read the IniFile for the window size and state
//Set up the frame rate -- not used now
//Set global pointer
cApp::cApp()
{
	//Assign default values
	m_iWidth = 800;
	m_iHeight = 600;
	m_bFullScreen = false;
	m_bIsGameRunning = false;
	m_bAllowMPlayer = false;

	m_GameType = CAMP;
	m_MapType = MTMENU;
	m_inifile = new cIniFile(".\\rf2d.ini");
	//? Full screen | windowed
	m_bFullScreen = m_inifile->ReadBool("settings","fullscreen");

	//get the screen size and setup the window
	if (m_bFullScreen)
	{
		m_iWidth = m_inifile->ReadInteger("fullscreen","width",800);
		m_iHeight = m_inifile->ReadInteger("fullscreen","height",600);
	}
	else
	{
		m_iWidth = m_inifile->ReadInteger("window","width",800);
		m_iHeight = m_inifile->ReadInteger("window","height",600);
	}
	m_bUseShadows = m_inifile->ReadBool("settings","shadows");
//	m_bUseShadows = false;
	//Set The Global
	g_Game = (cApp*)this;
}

//Create the HGE engine
//Get Dir Info and mouse cursor
//Init State Manager
bool cApp::Init()
{
	if (!InitRenderer()) //Create HGE
        return false;
	LoadPaths();

	if (!LoadUIGlobals())//load the mouse cursor and other stuff
        return false;

	if (!CreateGameObjects())
        return false;
	LoadSettings();
	m_bAllowMPlayer = SetupMPlayer();
	m_bShutDown = false;

	//register all of the available script functions
	l_bindClasses(m_luaVM);
	//execute the startup script
	l_LoadScriptFile(true, m_luaVM, "resources\\scripts\\startup.lua");
	l_LoadScriptFile(false, m_luaVM, "resources\\scripts\\DefaultAI.lua");
	//push the main menu onto the stack
	StartSingleMapGame();
//	ChangeGameState(MMENU,NONE);
	hge->Random_Seed(45);
	return TRUE;
}


bool cApp::Shutdown()
{
	// clean up state manager
    //m_StateManager->PopAll(this);
    StateManager::getInstance()->popAll();
	m_MPlayerControl->Disconnect();
	// Delete created objects and free loaded resources
	if (m_Cursor) delete m_Cursor;
	if (m_Font) delete m_Font;
	if (m_inifile) delete m_inifile;

	if (m_SoundManager) delete m_SoundManager;

	delete resourcemanager;
	delete m_MainMap;
    //delete m_StateManager;
	delete m_GameObjMangr;
	delete m_TargetSystem;
	delete m_CharGUI;
	delete m_MPlayerControl;
	delete m_WindowManager;
	delete m_WeatherControl;
	delete m_BattleControl;
    delete m_StaticTextures;
	delete m_stateGame;
    delete m_stateLoadScreen;
    delete m_stateMPlayGame;
    delete m_stateSingleGUI;
    delete m_stateTeamSelect;
    delete m_SQLHelper;

	//Get rid of HGE
	hge->System_Shutdown();
	hge->Release();
	return true;
}

//called by HGE
bool cApp::Frame()
{
  if (m_bShutDown)
	StateManager::getInstance()->popAll();

  return StateManager::getInstance()->frame();

}

void cApp::LoadPaths()
{
	//get the resource data from the ini file
	m_BasePath = hge->Ini_GetString("settings","basepath","");
	m_ParticlePath = m_BasePath + hge->Ini_GetString("settings","particles","");
	m_TexturePath = m_BasePath + hge->Ini_GetString("settings","textures","");
	m_FontPath = m_BasePath + hge->Ini_GetString("settings","fonts","");
	m_SingleMapPath = m_BasePath + hge->Ini_GetString("settings","singlemaps","");
	m_MultiMapPath = m_BasePath + hge->Ini_GetString("settings","multimaps","");
	m_MenuMapPath = m_BasePath + hge->Ini_GetString("settings","menumaps","");
	m_ModulePath = m_BasePath + hge->Ini_GetString("settings","module","");
	m_SFXPath = m_BasePath + hge->Ini_GetString("settings","sfx","");
	m_ThemePath = m_BasePath + hge->Ini_GetString("settings","theme","");
	m_AmbPath = m_BasePath + hge->Ini_GetString("settings","amb","");
	m_ScriptPath = m_BasePath + hge->Ini_GetString("settings","scripts","");
	m_DataPath = m_BasePath + hge->Ini_GetString("settings","data","");
	m_CharPath = m_BasePath + hge->Ini_GetString("settings","character","");

}

void cApp::LoadSettings()
{
	int iTmp;
	iTmp = hge->Ini_GetInt("sound","musicvol",20);
	m_SoundManager->MusicVolume(iTmp);
	iTmp = hge->Ini_GetInt("sound","sfxvol",50);
	m_SoundManager->SFXVolume(iTmp);
	iTmp = hge->Ini_GetInt("sound","abmvol",40);
	m_SoundManager->AbmVolume(iTmp);

};



void cApp::StartMPlayGame()
{
	if (!m_bIsGameRunning)
	{
		if (m_MPlayerControl->IsGameReady())
		{
			m_BattleControl->StartMPlayRound();
			m_bIsGameRunning = true;
			//ChangeGameState(MPGAME, CURRENT);
		}
	}
};

void cApp::StartSingleMapGame()
{
	m_GameType = SINGMAP;
	m_MapType = MTSINGLE;
	m_WindowManager->HideGameWindows();
	m_WindowManager->ShowMapSelectWindow();
	ChangeGameState(GUI, CURRENT);
};

void cApp::StartMPlayHostGame()
{
	m_GameType = MPLAY;
	m_WindowManager->HideGameWindows();
	m_WindowManager->ShowMapSelectWindow();
	ChangeGameState(GUI, CURRENT);
};

void cApp::StartMPlayConnectGame()
{
	m_GameType = MPLAY;
	std::string ipaddres;
	std::string username;


	//get name and IP
	username = hge->Ini_GetString("join","name","");
	ipaddres = hge->Ini_GetString("join","ip","");

	m_MPlayerControl->SetPlayerData(username, "");
	m_MPlayerControl->UseDefaultArmy(false);
	m_MPlayerControl->JoinGame(ipaddres.c_str());

	m_WindowManager->HideGameWindows();
	m_WindowManager->ShowConnectWindow();
	ChangeGameState(GUI,CURRENT);
};


int cApp::GetLoadStatus(){return m_iLoadStatus;};
void cApp::SetLoadStatus(int stat){m_iLoadStatus = stat; Frame();};
void cApp::ChangeMap(std::string fileName, int side)
{
	//hge->System_Log("ChangeMap") ;
	char tmpFilePath[256];
	int iEncounterID;
	//Load Cursor Texture

	switch (m_MapType)
	{
		case MTMULTI:
			strcpy(tmpFilePath, m_MultiMapPath.c_str());
			break;
		case MTMENU:
			strcpy(tmpFilePath, m_MenuMapPath.c_str());
			break;
		case MTSINGLE:
			strcpy(tmpFilePath, m_SingleMapPath.c_str());
			break;
		case MTMOD:
			strcpy(tmpFilePath, m_ModulePath.c_str());
			break;
	}

	strcat(tmpFilePath, fileName.c_str());

	ChangeGameState(LOAD, NONE);
	SetLoadStatus(2);
	resourcemanager->ResetUsage();
	m_SoundManager->ResetUsage();
	m_WeatherControl->NoWeather();
	SetLoadStatus(5);

	//hge->System_Log("ClearGameObjectList");
	m_GameObjMangr->ClearGameObjectList();
	m_SoundManager->ClearAmbientSounds();
	m_SoundManager->FadeSoundOut();

    //Get EncounterID
    m_SQLHelper->get_result("SELECT  [EncounterID] FROM  [tblISO_Encounter] WHERE [MapName] =" + fileName );

    if (m_SQLHelper->fetch_row())
    {
		iEncounterID = m_SQLHelper->getval();
        m_SQLHelper->free_result();

        SetLoadStatus(10);
        //hge->System_Log("ClearMap");
        m_MainMap->ClearMap();
        SetLoadStatus(15);
        //hge->System_Log("LoadMap");
        m_GameObjMangr->LoadMap(tmpFilePath, side);
        //Keep track of the current map name
        //hge->System_Log("LoadTiles");
        strcpy(tmpFilePath, fileName.c_str());
        m_MainMap->MapName(tmpFilePath);
        //hge->System_Log("Clean Up");
        SetLoadStatus(100);
        resourcemanager->ClearUnusedTex();
        m_SoundManager->ClearUnusedSFX();

        //	ChangeGameState(GAME, CURRENT);
        //StateManager::getInstance()->popState();
   	}

};

//Called from Init
bool cApp::LoadUIGlobals()
{

    char tmpFilePath[128];

	m_StaticTextures = new cStaticTextures(this);

    strcpy(tmpFilePath, m_FontPath.c_str());
	strcat(tmpFilePath, "font2.fnt");
	//hge->System_Log(tmpFilePath);
	m_Font=new hgeFont(tmpFilePath);

	m_Cursor = new cCursor(this);

	return 1;
}

bool cApp::SetupMPlayer()
{
	//This is my wrapper for multiplayer stuff
	//see RF2d_MPlayer.h/.cpp
	//My current thinking is to have each indiviual object that needs to call the MPlayerControl
	//to call it directly. I am using the MPlayerControler as an intermediate class so that all of my
	//other game object dont have to know about RakNet
	//TODO - GameObjectManager needs pointer to m_PlayerControl
	//Now that everyone knows about everyone else its safe to connect to the server
	return false;
};

GAMETYPE cApp::GetGameType(){ return m_GameType; };

//Called from Init
bool cApp::InitRenderer()
{
 // Initialize the graphics device and set display mode
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, "RF2D.Log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_FOCUSGAINFUNC, FocusGainFunc);
	hge->System_SetState(HGE_TITLE, "Principal Adversaries");
	hge->System_SetState(HGE_WINDOWED, !m_bFullScreen);
	hge->System_SetState(HGE_SCREENWIDTH, m_iWidth);
	hge->System_SetState(HGE_SCREENHEIGHT, m_iHeight);
	hge->System_SetState(HGE_SCREENBPP, 16);
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_ICON, MAKEINTRESOURCE(101));
	hge->System_SetState(HGE_INIFILE, "rf2d.ini");
//	hge->System_SetState(HGE_DONTSUSPEND,true);
    //If HGE initized then continue on
	return hge->System_Initiate();
}


//Called from Init
bool cApp::CreateGameObjects()
{
	/***** Database Stuff *******/
	Database::Mutex mutex; // not really necessary here at all
	StderrLog log;
	Database db(mutex, m_DataPath + hge->Ini_GetString("Database","file","RF2dEngine.db3"), &log);
    m_SQLHelper = new Query(db);


	/******* LUA / LUABIND *******/
	m_luaVM = lua_open(); // initialize mLuaState for Lua
	luaL_openlibs(m_luaVM);
	luabind::open(m_luaVM);

	m_GameObjMangr = new cGameObjectManager(this);
	m_MainMap = new cMapObject(this);
    m_TargetSystem = new cTargetObject(this);
	m_CharGUI = new cCharGUI(this);
	m_WindowManager = new cGUIManager(this);
	m_MPlayerControl = new cMPlayerControl(this);
	m_SoundManager = new cSoundManager(this);
	m_WeatherControl = new cWeatherControl(this);
	m_BattleControl = new cBattleControl(this);
    m_stateGame = new stateGameWindow(this);
    m_stateLoadScreen = new stateLoadScreen(this);
    m_stateMPlayGame = new stateMPlayGameWindow(this);
    m_stateSingleGUI = new stateSingleGUIWindow(this);
    m_stateTeamSelect = new stateTeamSelectWindow(this);



	m_WindowManager->Setup();
	m_WindowManager->CreateWindows();

	m_MainMap->SetScreenDim(m_iHeight, m_iWidth);
	return true;
}

void cApp::NewGame()
{
	m_GameType = SINGMAP;

	//m_bIsGameRunning = true;
	l_LoadScriptFile(true, m_luaVM, "resources\\scripts\\startup.lua");
	ChangeGameState(GAME, CURRENT);
	m_BattleControl->StartRound();
}

void cApp::ConnectGame(const char *ip)
{
	//m_bIsGameRunning = true;
	m_GameType = MPLAY;
	m_MPlayerControl->JoinGame(ip);
};

void cApp::RenderCursor(float dt)
{
	m_Cursor->Render();
};

void cApp::RenderInGameGUI(float dt)
{
	m_WindowManager->Render(dt);
};


int cApp::MakeTextureGrayScale(HTEXTURE tex)
{
   unsigned char *surface = (unsigned char *)hge->Texture_Lock(tex, false);

   if (surface == NULL) return -1;

   int w, h;

   w = hge->Texture_GetWidth(tex);
   h = hge->Texture_GetHeight(tex);

   // As were updating the whole texture we need no regard for X,Y
   // points - just the amount of data

   int pixels = w * h * 4;  // each pixel is 32 bits ARGB

   int i;
   int b1, b2, b3, res;

   for (i = 0; i < pixels; i += 4) // skip along 4 bytes at a time
   {
      // Since reading byte at a time...byte ordering is important
      // but because of what were doing only the alpha has to be left

      b1 = (int)surface[i];
      b2 = (int)surface[i + 1];
      b3 = (int)surface[i + 2];

      res = (b1 + b2 + b3) / 3;

      surface[i] = (unsigned char)res;
      surface[i + 1] = (unsigned char)res;
      surface[i + 2] = (unsigned char)res;
   }

   // Free surface so it gets put back into video memory.

   hge->Texture_Unlock(tex);

        return 0;
}

void cApp::SetLuaVM(lua_State* luaVM){ m_luaVM = luaVM; };

bool cApp::UseShadows() { return m_bUseShadows; };
cCursor* cApp::GetCursor(){ return m_Cursor; };
hgeFont* cApp::GetFont(){ return m_Font; };
lua_State* cApp::GetLuaVM(){ return m_luaVM; };
cGameObjectManager* cApp::GetGameObjMangr(){ return m_GameObjMangr; };
cMapObject*	cApp::GetMainMap() { return m_MainMap; };
cTargetObject* cApp::GetTargetSystem() { return m_TargetSystem; };
cCharGUI* cApp::GetCharGUI() { return m_CharGUI; };
cGUIManager* cApp::GetGUIManager() { return m_WindowManager; };
cMPlayerControl* cApp::GetMPlayerControl() { return m_MPlayerControl; };
cSoundManager*	cApp::GetSoundManager(){ return m_SoundManager; };
cWeatherControl* cApp::GetWeatherControl(){ return m_WeatherControl; };
cBattleControl*	cApp::GetBattleControl(){ return m_BattleControl; };
cStaticTextures* cApp::GetStaticTexture(){ return m_StaticTextures; };
int cApp::GetScreenHeight() { return m_iHeight; };
int cApp::GetScreenWidth() { return m_iWidth; };
Query* cApp::GetSQLHelper() { return m_SQLHelper; };


const char* cApp::GetGamePath(PATHTYPE path)
{
	const char* rtnpath = 0;
	switch (path)
	{
	case BASE:
		rtnpath = m_BasePath.c_str();
		break;
	case PARTICLE:
		rtnpath = m_ParticlePath.c_str();
		break;
	case TEXTURE:
		rtnpath = m_TexturePath.c_str();
		break;
	case FONT:
		rtnpath = m_FontPath.c_str();
		break;
	case SINGLEMAP:
		rtnpath = m_SingleMapPath.c_str();
		break;
	case MULTIMAP:
		rtnpath = m_MultiMapPath.c_str();
		break;
	case MENUMAP:
		rtnpath = m_MenuMapPath.c_str();
		break;
	case SFX:
		rtnpath = m_SFXPath.c_str();
		break;
	case THEME:
		rtnpath = m_ThemePath.c_str();
		break;
	case AMB:
		rtnpath = m_AmbPath.c_str();
		break;
	case SCRIPT:
		rtnpath = m_ScriptPath.c_str();
		break;
	case DATA:
		rtnpath = m_DataPath.c_str();
		break;
	case CHARACTER:
		rtnpath = m_CharPath.c_str();
		break;
	case MODULE:
		rtnpath = m_ModulePath.c_str();
		break;
	}
	return rtnpath;
}

void cApp::ChangeGameState(SCREENTYPE screen, CLEARTYPE clear)
{
	switch (clear)
	{
	case CURRENT:
		//m_StateManager->Pop(this);
		StateManager::getInstance()->popState();
		break;
	case ALL:
		//m_StateManager->PopAll(this);
        StateManager::getInstance()->popAll();
		break;
	};
//TODO JAH
	switch (screen)
	{
	case LOAD:
		//m_StateManager->Push(LoadScreenFrame, this);
		 StateManager::getInstance()->pushState(m_stateLoadScreen);
		break;
	case MMENU:
		//m_StateManager->Push(MainMenuFrame,this);
		break;
	case SPMENU:
		//m_StateManager->Push(SPlayerMenuFrame,this);
		break;
	case MPMENU:
		//m_StateManager->Push(MPlayerMenuFrame,this);
		break;
	case MPGAME:
		//m_StateManager->Push(MainGameFrameMPly, this);
		 StateManager::getInstance()->pushState(m_stateMPlayGame);
		break;
	case GAME:
		//m_StateManager->Push(MainGameFrame, this);
		 StateManager::getInstance()->pushState(m_stateGame);
		break;
	case LOBBY:
		break;
	case GUI:
		//m_StateManager->Push(SingleGUIWindowFrame, this);
		 StateManager::getInstance()->pushState(m_stateSingleGUI);
		break;
	case TEAM:
		//m_StateManager->Push(TeamSelectFrame, this);
		 StateManager::getInstance()->pushState(m_stateTeamSelect);
		break;
	};
};


bool cApp::Run()
{
	if (Init())
	{
		// Let's rock now!
		hge->System_Start();
	}
	if( m_luaVM )
	{
		lua_close(m_luaVM );
		m_luaVM = 0;
	}
	Shutdown();
	return 0;
}

