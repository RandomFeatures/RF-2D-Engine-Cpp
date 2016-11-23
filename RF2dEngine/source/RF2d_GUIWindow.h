#include "RF3d_Global.h"
#include <vector>

/*
render base GUI window
trap all user events within the window and pass them on to the controls
*/
using namespace std;

#ifndef RF2D_GUIWINDOW_H
#define RF2D_GUIWINDOW_H

class cBaseControl;
class cApp;

struct sWindowControlOptions
{
	bool Visible;
	bool Enabled;
	bool Click;
	bool MouseOver;
	DWORD Color;
	DWORD ShadowColor;
	DWORD BColor;
	hgeSprite* Image;
	std::string Text;
};

struct sControlTemplate
{
	hgeFont* Font;
	DWORD	FontColor;
	DWORD	BorderDColor;
	DWORD	BorderLColor;
	DWORD	BackColor;
};

class cBaseWindow
{
public:
	cBaseWindow();
	cBaseWindow(cApp* GameParent, float x, float y, float w, float h, int blend);
	~cBaseWindow();
	float Width();
	float Height();
	float Left();
	float Top();
	bool MouseTest(float x, float y);
	bool Visible();
	bool GotFocus();
	int GetZPos();
	void Width(float w);
	void Height(float h);
	void SetSize(float h, float w);
	void SetMinSize(float h, float w);
	void SetMaxSize(float h, float w);
	void SetBGColor(int blend, int red, int green, int blue);
	void SetBorderColor(int red, int green, int blue);
	void SetPos(float x, float y);
	void SetBaseFont(hgeFont* Font);
	void SetCaption(std::string strText);
	void SetupCloseButton(bool enabled, bool visible);
	void SetupControlBox(bool enabled, bool visible);
	void Show();
	void Hide();
	void SetFocus();
	void SetZPos(int z);
	void SetFontColor(int red, int green, int blue);
	void LostFocus();
	void ShowAllParts();
	void HideAllParts();
	void HideWindow();
	void ShowWindow();




	virtual void MouseMove(float x, float y);
	virtual void MouseDown(float x, float y);
	virtual void MouseUp(float x, float y);
	virtual bool CharPress(int key);
	virtual bool KeyPress(int key);
	virtual void ControlClick(int id){};
	void AlignTop();
	void AlignBottom();
	void AlignLeft();
	void AlignRight();
	void AlignCenter();
	void ToggleVisible();
	void Render(float dt);
	void DrawBaseWindow();
	void DrawGraphicWindow();
	void BringFront();
	void Reorder(int maxVal);
	void CenterCaption();
	void DeleteControls();
	void LoadGUISprites();
	void AddControl(cBaseControl* control);
	void UseGraphicGUI();
	virtual void DrawWindow(float dt);
	virtual void ResizeWindow(){};
	virtual bool CanClose(){return true;};

	sWindowControlOptions Caption;
	sWindowControlOptions Grip;
	sWindowControlOptions ControlBox;
	sWindowControlOptions CloseButton;
	sWindowControlOptions Border;
	sWindowControlOptions BackGround;
	sControlTemplate	  ControlTemp;



protected:
	cApp*		m_GameParent;
	int			m_ZPos;
	int			m_Red;
	int			m_Green;
	int			m_Blue;
	int			m_Blend;
	float		m_Width;
	float		m_Height;
	float		m_XPos;
	float		m_YPos;
	float		m_MinWidth;
	float		m_MinHeight;
	float		m_MaxWidth;
	float		m_MaxHeight;
	float		m_CaptStart;
	float		m_CaptClickX;
	float		m_CaptClickY;

	DWORD		m_FontColor;
	bool		m_Visible;
	bool		m_Focus;
	bool		m_MouseDown;
	bool		m_GraphicGUI;
	hgeFont*	m_BFont;

	hgeSprite*	m_TopLeft;
	hgeSprite*	m_TopRight;
	hgeSprite*	m_BottomLeft;
	hgeSprite*	m_BottomRight;
	hgeSprite*	m_CenterLeft;
	hgeSprite*	m_CenterRight;
	hgeSprite*	m_CenterTop;
	hgeSprite*	m_CenterBottom;
	hgeSprite*	m_Close1;
	hgeSprite*	m_Close2;
	hgeSprite*	m_Close3;

	vector<cBaseControl*>  m_Controls;


};


#endif
