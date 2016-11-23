#ifndef HOSTWINDOW_H
#define HOSTWINDOW_H





class cHostWindow_gui :public cBaseWindow
{
public:
	cHostWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd);
	~cHostWindow_gui();
	virtual void DrawWindow(float dt);
	virtual void ResizeWindow();
	virtual void ControlClick(int id);
	virtual bool KeyPress(int key);
	void CreateControls();
	void Init();

private:
	cLabel*				m_PassLabel;
	cLabel*				m_NameLabel;
	cTextBox*			m_NameTextBox;
	cTextBox*			m_PassTextBox;
	cButton*			m_BtnStart;
	cButton*			m_BtnBack;
};

#endif

