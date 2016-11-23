#ifndef MAPSELECTWINDOW_H
#define MAPSELECTWINDOW_H


class cMapSelect_gui :public cBaseWindow
{
public:
	cMapSelect_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd);
	~cMapSelect_gui();
	virtual void DrawWindow(float dt);
	virtual void ResizeWindow();
	virtual void ControlClick(int id);
	virtual bool KeyPress(int key);
	void CreateControls();
	void Init();
	int GetFileList(const char *dir);

private:
	cLabel*				m_IPLabel;
	cScrollListBox*		m_ScrollMaps;
	cButton*			m_BtnNext;
	cButton*			m_BtnBack;
	//cChkBox*			m_DefaultChk;
};

#endif

