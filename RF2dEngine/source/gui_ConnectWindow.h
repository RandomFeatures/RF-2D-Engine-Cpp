#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H





class cConnectWindow_gui :public cBaseWindow
{
public:
	cConnectWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd);
	~cConnectWindow_gui();
	virtual void DrawWindow(float dt);
	virtual void ResizeWindow();
	virtual void ControlClick(int id);
	virtual bool KeyPress(int key);
	void CreateControls();
	void Init();

private:
	cLabel*				m_IPLabel;
	cLabel*				m_PassLabel;
	cLabel*				m_NameLabel;
	cTextBox*			m_IPTextBox;
	cTextBox*			m_NameTextBox;
	cTextBox*			m_PassTextBox;
	cButton*			m_BtnConnect;
	cButton*			m_BtnBack;
	cChkBox*			m_DefaultChk;

};

#endif

