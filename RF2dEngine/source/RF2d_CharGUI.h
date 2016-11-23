#include "hgesprite.h"
#include "resource.h"

/*
renders character action icons.
Acttack, Move, Pass

Respond to player clicks to notify
the system that the player has choose to Attack, Move, or Pass
*/
#pragma warning(disable:4244)
//using namespace std;

#ifndef RF2D_CHARGUI_H
#define RF2D_CHARGUI_H

class cApp;
class cCharacterObject;

class cCharGUI
{
	public:
		cCharGUI(cApp* pParent);
		~cCharGUI();
		void Render();
		void Update(float dt, float x, float y);
		bool UserClick();
		bool UserClickMPly();
		void ShowMenu();
		void HideMenu();
		bool isShown();
		void AddTeam(cCharacterObject* npc);

	private:
		cApp*				m_pParent;
		hgeSprite*			m_AttackSprite;
		hgeSprite*			m_MoveSprite;
		hgeSprite*			m_CancelSprite;
		hgeSprite*			m_StatusSprite;
		hgeSprite*			m_HealthBar;
		bool				m_Show;
		int					m_Selected;
		float				m_ScreenX, m_ScreenY;
		vector<cCharacterObject*> TeamList;

};

#endif