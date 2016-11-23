#include "gui_support_CharSelectItem.h"


#ifndef CHARSELECTWINDOW_H
#define CHARSELECTWINDOW_H

class cCharSelect_gui :public cBaseWindow
{
public:
	cCharSelect_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd);
	~cCharSelect_gui();
	virtual void DrawWindow(float dt);
	virtual void ResizeWindow();
	virtual void ControlClick(int id);
	virtual bool KeyPress(int key);
	void CreateControls();
	void Init(){};
	void Clear();
	void LoadCharData();
	void PlaceCharacter(int key);
	void AddCharItem(std::string guid, std::string name, int type, int defrate, int hitrate, int attrate, int moverate, std::string file, int spellID, float phy, float mag, int hps, int imin, int imax, int movement);
	void AddCharsToGame();
	std::string LookupClass(int type);
	std::string LookupStat(int stat);

private:
	cLabel*				m_ClassLabel;
	cLabel*				m_HitLabel;
	cLabel*				m_AttackLabel;
	cLabel*				m_DefenseLabel;
	cLabel*				m_MoveLabel;
	cLabel*				m_ScrollLabel;
	cLabel*				m_ArmySizeLabel;
	cGroupBox*			m_Group;
	cScrollListBox*		m_ScrollChar;
	cButton*			m_BtnChar;
	cButton*			m_BtnStart;
	cButton*			m_BtnBack;
	cButton*			m_BtnRes;
	cCharSelectItem_gui_support*	m_SelectedChar;
	int					m_CurrIndx;
	std::string			m_ArmySize;
	vector<cCharSelectItem_gui_support*> MasterCharList;
	vector<cCharSelectItem_gui_support*> SelectedCharList;
};

#endif

