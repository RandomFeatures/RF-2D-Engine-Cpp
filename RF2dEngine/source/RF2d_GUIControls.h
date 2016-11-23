#pragma warning(disable:4018)
#include <vector>
#include <stack>
#include <deque>
#include <algorithm>

using namespace std;

#ifndef RF2D_GUICONTROLS_H
#define RF2D_GUICONTROLS_H

typedef deque<std::string> ItemDeq;
typedef vector<std::string> ItemVec;
typedef ItemDeq::iterator DeqItor;
typedef ItemVec::iterator VecItor;

class cBaseWindow;

class cBaseControl
{
public:
	cBaseControl();
	~cBaseControl();
	virtual bool MouseTest(float x, float y);
	virtual bool CharPress(int key){return false;};
	virtual bool KeyPress(int key){return false;}
	virtual void Render(float x, float y, float dt){};
	virtual void MouseMove(float x, float y){};
	virtual void MouseDown(float x, float y){};
	virtual void MouseUp(float x, float y){};
	virtual void Setup(sControlTemplate ct);
	virtual void UseGraphicGUI(HTEXTURE guiTex){};
	virtual void UseGraphicBack(HTEXTURE guiTex){};
	int ID();
	void ID(int i);
	virtual void SetSize(float h, float w);
	float Width();
	void Width(float w);
	float Height();
	void Height(float h);
	void SetPos(float x, float y);
	float X();
	void X(float x);
	float Y();
	void Y(float y);
	void SetParentWindow(cBaseWindow* parent);
	void SetFocus();
	void LostFocus();
	void SetFont(hgeFont* Font);
	void SetText(std::string txt);
	void SetLBorderColor(int red,int green, int blue);
	void SetDBorderColor(int red,int green, int blue);
	void SetFontColor(int red,int green, int blue);
	void SetEnabled(bool state);
	std::string GetText();

protected:
	cBaseWindow* m_Parent;
	bool	m_Focus;
	bool	m_CenterCaption;
	bool	m_MouseOver;
	bool	m_GraphicGUI;
	bool	m_Enabled;
	float	m_Width;
	float	m_Height;
	float	m_XPos;
	float	m_YPos;
	int		m_ID;
	hgeFont* m_Font;
	std::string	m_Text;
	DWORD	m_BorderDColor;
	DWORD	m_BorderLColor;
	DWORD	m_BackColor;
	DWORD	m_FontColor;
	bool	m_Clicked;
};

class cBaseContainer: public cBaseControl
{
public:
	cBaseContainer(){};
	cBaseContainer(float x, float y, float h, float w);
	~cBaseContainer();
	virtual bool CharPress(int key);
	virtual bool KeyPress(int key);
	virtual void MouseMove(float x, float y);
	virtual void MouseDown(float x, float y);
	virtual void MouseUp(float x, float y);
	virtual void Render(float x, float y, float dt);
	virtual void DrawContainer(float x, float y,float dt);
	virtual void DrawBaseContainer(float x, float y);
	void Setup(sControlTemplate ct);
	void DeleteControls();
	void AddControl(cBaseControl* control);
	void ShowBorder(bool show);
	bool ShowBorder();

protected:
	sControlTemplate CntrlTmp;
	bool m_ShowBorder;
	vector<cBaseControl*>  m_Controls;
};

class cLabel: public cBaseControl
{
public:
	cLabel(hgeFont* Font, float x, float y, float h, float w);
	~cLabel(){};
	virtual void Render(float x, float y, float dt);
	void ShowBorder(bool show);
	bool ShowBorder();
private:
	bool m_ShowBorder;
};

class cGroupBox: public cBaseContainer
{
public:
	cGroupBox(hgeFont* Font,float x, float y, float h, float w);
	~cGroupBox();
	void DrawBaseContainer(float x, float y);
	void UseGraphicBack(HTEXTURE guiTex);
private:
	hgeSprite*	m_BckGrnd;
	bool	m_GraphicBack;
};

class cScrollBox: public cBaseControl
{
public:
	cScrollBox(){};
	cScrollBox(hgeFont* Font,float x, float y, float h, float w);
	~cScrollBox();
	virtual void Render(float x, float y, float dt);
	virtual bool KeyPress(int key);
	void SetSize(float h, float w);
	virtual void MouseDown(float x, float y);
	virtual void MouseUp(float x, float y);
	void SetMaxLength(int len);
	void SetItemHeight(int h);
	virtual void ScrollUp();
	virtual void ScrollDown();
	void UseGraphicGUI(HTEXTURE guiTex);
	void PageUp();
	void PageDown();
	int GetMaxLength();
	int GetLength();

protected:
	void Init(float x, float y, float h, float w);
	bool	m_TBtnDown;
	bool	m_BBtnDown;
	int		m_MaxLength;
	int		m_ItemHeight;
	int		m_DisplayCount;
	int		m_DisplayStart;
	int		m_RowCount;
	hgeSprite*	m_imgNormal;
	hgeSprite*	m_imgDown;
	hgeSprite*	m_TopArrow;
	hgeSprite*	m_BotArrow;
	hgeSprite*	m_BckGrnd;

	
};

class cScrollTextBox:public cScrollBox
{
public:
	cScrollTextBox(hgeFont* Font, float x, float y, float h, float w);
	~cScrollTextBox();
	virtual void Render(float x, float y, float dt);
	virtual void ScrollUp();
	virtual void ScrollDown();
	void AddItem(std::string item);
	void Clear();
private:
	ItemDeq  m_Items;
};

class cScrollListBox:public cScrollBox
{
public:
	cScrollListBox(hgeFont* Font, float x, float y, float h, float w);
	~cScrollListBox();
	virtual void Render(float x, float y, float dt);
	virtual void MouseDown(float x, float y);
	void AddItem(std::string item);
	void RemoveItem(int indx);
	void Clear();
	void SelectedIndx(int indx);
	int	SelectedIndx();
	std::string GetSelectedItem();
	std::string GetItemAt(int indx);
	void SetHighlightColor(int red,int green, int blue);

private:
	int m_SelectedIndx;
	ItemVec m_Items;
	DWORD	m_HighlightColor;

};


class cButton:public cBaseControl
{
public:
	cButton(hgeFont* Font, float x, float y, float h, float w);
	~cButton();
	virtual void Render(float x, float y, float dt);
	void MouseDown(float x, float y);
	void MouseUp(float x, float y);
	void CenterCaption();
	void UseGraphicGUI(HTEXTURE guiTex);
	void UseGraphicBack(HTEXTURE guiTex);
private:
	bool	m_BtnDown;
	bool	m_GraphicGUI;
	bool	m_GraphicBack;
	int		m_TextLeft;
	int		m_TextTop;
	hgeSprite*	m_imgNormal;
	hgeSprite*	m_imgOver;
	hgeSprite*	m_imgDown;
	hgeSprite*	m_BckGrnd;
};

class cTextBox:public cBaseControl
{
public:
	cTextBox(hgeFont* Font, float x, float y, float h, float w);
	~cTextBox(){};
	virtual void Render(float x, float y, float dt);
	virtual bool KeyPress(int key);
	virtual bool CharPress(int key);
	virtual void MouseDown(float x, float y);
	void SetText(char *txt);
	void SetFocus();
	void LostFocus();
	int	GetMaxLen();
	void SetMaxLen(int l);
	bool GetFocus();
	std::string GetText();
	void Clear();
private:
	int GetNewDisplayStart(int displaystop);
	int GetNewDisplayStop(int displaystart);
//	bool	m_Focus;
	bool	m_BlinkOn;
	int		m_DisplayStart;
	int		m_DisplayStop;
	int		m_MaxLength;
	int		m_CursorPos;
	float	m_TextTop;
	float	m_BlinkTimer;
	std::string	m_strFullText;
};

class cTabControl: public cBaseControl
{
public:
	cTabControl(hgeFont* Font, float x, float y, float h, float w, float tw, float th);
	~cTabControl();
	virtual bool CharPress(int key);
	virtual bool KeyPress(int key);
	virtual void MouseMove(float x, float y);
	void MouseDown(float x, float y);
	virtual void MouseUp(float x, float y);
	virtual void Render(float x, float y, float dt);
	void DrawTabPage(float x, float y,float dt);
	void DrawTabs(float x, float y);
	void Setup(sControlTemplate ct);
	void DeleteControls();
	void AddControl(cBaseControl* control, int tabindx);
	void AddTab(char* capt);
private:
	sControlTemplate TabTmp;
	int		m_CurrentIndx;
	int		m_TabCount;
	float	m_TabHeight;
	float	m_TabWidth;
	bool	m_Clicked;
	vector<cBaseContainer*>  m_Tabs;
};

class cChkBox: public cBaseControl
{
public:
	cChkBox(hgeFont* Font, float x, float y, float h, float w);
	~cChkBox();
	virtual void Render(float x, float y, float dt);
	void MouseDown(float x, float y);
	void MouseUp(float x, float y);
	void CenterCaption();
	bool Checked();
	void Checked(bool chk);
	void UseGraphicGUI(HTEXTURE guiTex);
private:
	bool	m_Checked;
	bool	m_Clicked;
	bool	m_GraphicGUI;
	int		m_TextLeft;
	int		m_TextTop;
	hgeSprite*	m_imgNormal;
	hgeSprite*	m_imgChecked;
};

#endif

