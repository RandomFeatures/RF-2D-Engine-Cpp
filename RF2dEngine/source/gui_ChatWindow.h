#include <vector>
#include <stack>
/*
    "If there's anything more important than my ego around,\n"
	"I want it hunted and killed now!\n"
    "\n"
    "\n"
    "Only the good die young. That is why immortality is\n"
	"reserved for the greatest of evils.\n"
    "\n"
    "\n"
	"The two rules for success in life are:\n"
	"1) Never tell them everything you know\n"
    "\n"
    "\n"
	"What if the Hokey Pokey is really what it's all about?";
*/
using namespace std;

#ifndef CHATWINDOW_H
#define CHATWINDOW_H


class cMPlayerControl;


class cChatWindow_gui :public cBaseWindow
{
public:
	cChatWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd);
	~cChatWindow_gui();
	virtual void DrawWindow(float dt);
	virtual void ResizeWindow();
	virtual void ControlClick(int id);
	virtual bool KeyPress(int key);
	void CreateControls();
	void Init(){};
	void AddText(unsigned char *msg);
private:
	cScrollListBox* m_ScrollBox;
	cButton* m_BtnSend;
	cTextBox* m_TextBox;
	void SendText();
};
#endif

