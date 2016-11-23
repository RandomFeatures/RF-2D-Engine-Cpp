#include "RF3d_Global.h"
#include "RF2d_Anistorage.h"

using namespace std;

#ifndef CHARSELECTITEM_H
#define CHARSELECTITEM_H


class cCharSelectItem_gui_support
{
public:
	cCharSelectItem_gui_support(float phy, float mag, int defrate, int hitrate, int hps, int attrate, int min, int max, int moverate, int dir, int x, int y, int type, int spellID, int movement, std::string guid, std::string file, std::string name, std::string texpath);
	~cCharSelectItem_gui_support();
	float Physical();
	float Magical();
	int HitPoints();
	int AttackMin();
	int AttackMax();
	int SpellID();
	int AttackRating();
	int HitRating();
	int DefRating();
	int MoveRating();
	int Movement();
	int Dir();
	int X();
	int Y();
	int Type();
	std::string	GUID();
	std::string	FileName();
	std::string	Name();
	bool IsLoaded();
	bool IsSelected();
	void SetDebug();
	void X(int x);
	void Y(int y);
	void Select(bool sel);
	void SetDirection(ISODIR dir);
	void SetColor(DWORD color);
	void LoadStand();
	void RenderInWindow(float fDeltaTime, float x, float y);
	void RenderInGame(float fDeltaTime, float camX, float camY);


private:
	float			m_PhyResist;
	float			m_MagResist;
	int				m_HitPoints;
	int				m_AttackMin;
	int				m_AttackMax;

	int				m_AttackRating;
	int				m_HitRating;
	int				m_DefRating;

	int				m_MoveRating;
	int				m_Movement;
	int				m_Dir;
	int				m_X;
	int				m_Y;
	int				m_Type;
	int				m_SpellID;
	std::string		m_GUID;
	std::string		m_FileName;
	std::string		m_Name;
	std::string		m_TexPath;
	bool			m_Loaded;
	bool			m_Debug;
	bool			m_Selected;
	AniStorage*		m_Stand;

};


#endif
