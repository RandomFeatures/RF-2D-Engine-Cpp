#include <vector>
#include "gui_ChatWindow.h"
#include "gui_LobbyWindow.h"
#include "gui_ConnectWindow.h"
#include "gui_HostWindow.h"
#include "gui_MapSelect.h"
#include "gui_CharSelect.h"
#include "NetworkTypes.h" //defines the Packet data type
using namespace std;


#ifndef RF2D_GUIMANAGER_H
#define RF2D_GUIMANAGER_H

class cApp;
class cBaseWindow;

class cGUIManager
{
private:
	cApp*				m_pParent;
	hgeSprite*			m_BckGrnd;
	hgeSprite*			m_CloseBtn;
	hgeSprite*			m_CtrlBtn;
	hgeFont*		    m_BaseFont;
	HEFFECT				m_ClickSFX;
	vector<cBaseWindow*> m_WindowList;
	cBaseWindow*		m_CurrentWindow;


public:
	cGUIManager(cApp* pParent);
	~cGUIManager();
	bool GUIMouseDown(float x, float y);
	void GUIMouseMove(float x, float y);
	void GUIMouseUp(float x, float y);
	bool GUIKeyPress(int key);
	bool GUICharPress(int key);
	void CreateWindows();
	void ClearFocus();
	void ShowChatWindow();
	void HideChatWindow();
	void ShowMPlayGameWindows();
	void HideMPlayGameWindows();
	void ShowGameWindows();
	void HideGameWindows();
	void ShowLobbyWindow();
	void HideLobbyWindow();
	void ShowConnectWindow();
	void HideConnectWindow();
	void ShowHostWindow();
	void HideHostWindow();
	void ShowMapSelectWindow();
	void HideMapSelectWindow();
	void ShowCharSelectWindow();
	void HideCharSelectWindow();
	void HideAllWindows();
	void Setup();
	void Render(float dt);
	void ProcessPacket(Packet *p, unsigned char packetIdentifier);
	void ReceiveChatMessage(Packet *p);


	cChatWindow_gui*		ChatWindow;
	//cBaseWindow*		    StatWindow;
	//cLobbyWindow_gui*     LobbyWindow;
	cConnectWindow_gui*     ConnectWindow;
	cHostWindow_gui*		HostWindow;
	cMapSelect_gui*	        MapSelectWindow;
	cCharSelect_gui*	    CharSelectWindow;
	bool				    m_Clicked;

};

#endif
