#include <vector>
#include <stack>
using namespace std;

#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H


class cMPlayerControl;


class cLobbyWindow_gui :public cBaseWindow
{
public:
	cLobbyWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd);
	~cLobbyWindow_gui();
	virtual void DrawWindow(float dt);
	virtual void ResizeWindow();
	virtual void ControlClick(int id);
	virtual bool KeyPress(int key);
	void CreateControls();
	void Init(){};

private:
	cTabControl*		m_ChatGroup;
	cButton*			m_BtnSend;
	cScrollTextBox*		m_ScrollChatBox;
	cScrollListBox*		m_ScrollFriends;
	cScrollListBox*		m_ScrollRoom;
	cTextBox*			m_TextBox;
	cChkBox*			m_CheckBox;
};

#endif

