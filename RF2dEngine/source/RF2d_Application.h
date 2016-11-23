#ifndef RF2D_APPLICATION_H_INCLUDED
#define RF2D_APPLICATION_H_INCLUDED

#include "hgefont.h"
#include "resource.h"
#include "hgeanim.h"
#include <libsqlitewrapped.h>

extern "C"
{
 #include <lua.h>
 #include <lauxlib.h>
 #include <lualib.h>
}
// luabind
#include "luabind/luabind.hpp"


class cCursor;
class cMPlayerControl;
class cGameObjectManager;
class cTargetObject;
class cCharGUI;
class cWeatherControl;
class cIniFile;
class cStateManager;
class cGUIManager;
class cSoundManager;
class cMapObject;
class cBattleControl;
class cStaticTextures;
class hgeFont;

class stateGameWindow;
class stateLoadScreen;
class stateMPlayGameWindow;
class stateSingleGUIWindow;
class stateTeamSelectWindow;

class cApp
{
  protected:

    cIniFile*			m_inifile;
	cCursor*			m_Cursor;
	lua_State*          m_luaVM;
	cStateManager*		m_StateManager;
	cGUIManager*		m_WindowManager;
	cMPlayerControl*    m_MPlayerControl;
	cGameObjectManager* m_GameObjMangr;
	cMapObject*			m_MainMap;
	cTargetObject*		m_TargetSystem;
	cCharGUI*			m_CharGUI;
	cSoundManager*		m_SoundManager;
	cWeatherControl*	m_WeatherControl;
	cBattleControl*		m_BattleControl;
    cStaticTextures*    m_StaticTextures;
    Query*              m_SQLHelper;

	int					m_iLoadStatus;
	int					m_iWidth;
    int					m_iHeight;
	bool				m_bIsGameRunning;
	bool				m_bFullScreen;
	bool				m_bShutDown;
	bool				m_bAllowMPlayer;
	bool				m_bUseShadows;

	//HGE Stuff
	hgeFont*		    m_Font;

	GAMETYPE			m_GameType;
	MAPTYPE				m_MapType;

	//paths
	std::string			m_BasePath;
	std::string         m_ParticlePath;
	std::string         m_FontPath;
	std::string			m_TexturePath;
	std::string			m_LevelFileName;
	std::string			m_SFXPath;
	std::string			m_ThemePath;
	std::string			m_AmbPath;
	std::string			m_MenuMapPath;
	std::string			m_SingleMapPath;
	std::string			m_MultiMapPath;
	std::string			m_ScriptPath;
	std::string			m_DataPath;
	std::string			m_CharPath;
	std::string			m_ModulePath;

    stateGameWindow*            m_stateGame;
    stateLoadScreen*            m_stateLoadScreen;
    stateMPlayGameWindow*       m_stateMPlayGame;
    stateSingleGUIWindow*       m_stateSingleGUI;
    stateTeamSelectWindow*      m_stateTeamSelect;


	void LoadPaths();
	void LoadSettings();
	bool Shutdown();
	//Init Function... inorder
	bool InitRenderer();
	bool LoadUIGlobals();
    bool CreateGameObjects();
	bool SetupMPlayer();
	void GetGameWindowSettings();

  public:

	/**** Local Constructor ****/
	cApp();
	int					MakeTextureGrayScale(HTEXTURE tex);
	bool				Init();
	bool				Frame();
	bool				Run();
	bool				UseShadows();
	void				NewGame();
	void				ConnectGame(const char *ip);
	void				ChangeMap(std::string fileName, int side);
	void				StartMPlayGame();
	void				StartSingleMapGame();
	void				StartMPlayHostGame();
	void				StartMPlayConnectGame();
	int					GetLoadStatus();
	int					GetScreenHeight();
	int					GetScreenWidth();
	void				SetLoadStatus(int stat);
	void				RenderCursor(float dt);
	void				RenderInGameGUI(float dt);
	void				SetLuaVM(lua_State* luaVM);
	void				ChangeGameState(SCREENTYPE screen, CLEARTYPE clear);
	const char*			GetGamePath(PATHTYPE path);
	cCursor*			GetCursor();
	hgeFont*			GetFont();
	lua_State*			GetLuaVM();
	cGameObjectManager* GetGameObjMangr();
	cMapObject*			GetMainMap();
	cTargetObject*		GetTargetSystem();
	cCharGUI*			GetCharGUI();
	cGUIManager*		GetGUIManager();
	cMPlayerControl*	GetMPlayerControl();
	cSoundManager*		GetSoundManager();
	cWeatherControl*	GetWeatherControl();
	cBattleControl*		GetBattleControl();
	cStaticTextures*    GetStaticTexture();
	GAMETYPE			GetGameType();
	Query*              GetSQLHelper();

};

extern cApp *g_Game;



#endif // RF2D_APPLICATION_H_INCLUDED
