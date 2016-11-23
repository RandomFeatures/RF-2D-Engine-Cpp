#include "RF2d_CharGUI.h"
#include "RF2d_GameObject.h"
#include "RF2d_BattleControl.h"
#include "RF2d_StaticTextures.h"
#include "RF2d_Application.h"

/*
renders character action icons.
Acttack, Move, Pass

Respond to player clicks to notify
the system that the player has choose to Attack, Move, or Pass
*/

cCharGUI::cCharGUI(cApp* pParent)
{
	m_pParent = pParent;
	m_AttackSprite = new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 64, 0, 32, 32);
	m_MoveSprite = new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 96, 0, 32, 32);
	m_CancelSprite = new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 64, 32, 32, 32);
	m_StatusSprite = new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 96, 32, 32, 32);
	m_HealthBar = new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 0, 40, 32, 4);
	m_AttackSprite->SetHotSpot(0,0);
	m_MoveSprite->SetHotSpot(0,0);
	m_CancelSprite->SetHotSpot(0,0);
	m_StatusSprite->SetHotSpot(0,0);
	m_Show = false;
	m_Selected = 0;

};

cCharGUI::~cCharGUI()
{
	if (m_AttackSprite) delete m_AttackSprite;
	if (m_MoveSprite) delete m_MoveSprite;
	if (m_CancelSprite) delete m_CancelSprite;
	if (m_StatusSprite) delete m_StatusSprite;
	if (m_HealthBar) delete m_HealthBar;
	TeamList.clear();
};

void cCharGUI::Render()
{
	char tip[7];
	float x = m_pParent->GetCursor()->x();
	float y = m_pParent->GetCursor()->y();
	int x1, x2, y1, y2;
	float hits, damage = 0.0f;
	float perc;
	cCharacterObject* NPC = NULL;

	strcpy(tip,"");
	m_ScreenX = m_pParent->GetBattleControl()->GetSourceX();
	m_ScreenY = m_pParent->GetBattleControl()->GetSourceY();

	m_AttackSprite->SetColor(ARGB(100,255,255,255));
	m_MoveSprite->SetColor(ARGB(100,255,255,255));
	m_CancelSprite->SetColor(ARGB(100,255,255,255));
	m_StatusSprite->SetColor(ARGB(100,255,255,255));
	m_Selected = 0;

	for (int iLoop=0;iLoop<TeamList.size();iLoop++)
	{
		x1 = (48*iLoop+5);
		y1 = 5;
		x2 = x1 + 43;
		y2 = y1 + 43;
		NPC = TeamList[iLoop];
		if (!NPC->IsDead())
		{
			NPC->RenderPortrait(x1,y1,x2,y2);
			hits = NPC->GetHitPoints();
			damage = NPC->GetDamage();
			perc = 43-((damage/hits)*43);
			m_HealthBar->RenderStretch(x1,y2-6,x1+perc,y2);
		}

		if (m_pParent->GetBattleControl()->GetSource() == NPC)
		{
			//top
			hge->Gfx_RenderLine(x1-1,y1-1,x2+1,y1-1, ARGB(255,100,255,100));
			//bottom
			hge->Gfx_RenderLine(x1-1,y2+1,x2+1,y2+1, ARGB(255,100,255,100));
			//right
			hge->Gfx_RenderLine(x2+1,y1-1,x2+1,y2+1, ARGB(255,100,255,100));
			//left
			hge->Gfx_RenderLine(x1-1,y1-1,x1-1,y2+2, ARGB(255,100,255,100));
		}
	}

	//mouse over the attack icon
	if ((x > m_ScreenX-16) && (x < m_ScreenX+16))
	{
		if ((y > m_ScreenY-108) && (y < m_ScreenY-76))
		{
			if (!m_pParent->GetBattleControl()->GetAttacked())
				m_AttackSprite->SetColor(ARGB(255,255,255,255));
			strcpy(tip,"Attack");
			m_Selected = 1;
		}

		//if ((y > m_ScreenY-44) && (y < m_ScreenY-12))
		//{
		//	m_StatusSprite->SetColor(ARGB(255,255,255,255));
		//	strcpy(tip,"Status");
		//	m_Selected = 2;
		//}
	}
	//mouse over icons
	if ((y > m_ScreenY-76) && (y < m_ScreenY-44))
	{
		//over move icon
		if ((x > m_ScreenX-48) && (x < m_ScreenX-16))
		{
			if (!m_pParent->GetBattleControl()->GetMoved())
			{
				m_MoveSprite->SetColor(ARGB(255,255,255,255));
				strcpy(tip," Move");
			} else
			{
				if (!m_pParent->GetBattleControl()->GetAttacked() && m_pParent->GetGameType() != MPLAY)
				{
					m_StatusSprite->SetColor(ARGB(255,255,255,255));
					strcpy(tip," Undo");
				} else
				  strcpy(tip," Move");

			}

			m_Selected = 3;
		}
		//over pass icon
		if ((x > m_ScreenX+16) && (x < m_ScreenX+48))
		{
			m_CancelSprite->SetColor(ARGB(255,255,255,255));
			strcpy(tip," Pass");
			m_Selected = 4;
		}
	}
	//current status... decide which icon to show
	if (m_Show && m_pParent->GetBattleControl()->GetSource())
	{
		if ( m_pParent->GetBattleControl()->GetSource()->GetCurrentAction() == STAND)
		{//render icons
			//TODO -- see if the character is at the top of the screen and render the attack icon lower
			m_AttackSprite->Render(m_ScreenX-16,m_ScreenY-108);

			if(m_pParent->GetBattleControl()->GetMoved() && !m_pParent->GetBattleControl()->GetAttacked())
				m_StatusSprite->Render(m_ScreenX-48,m_ScreenY-76);
			else
				m_MoveSprite->Render(m_ScreenX-48,m_ScreenY-76);

			m_CancelSprite->Render(m_ScreenX+16,m_ScreenY-76);
			//m_StatusSprite->Render(m_ScreenX-16,m_ScreenY-44);
			m_pParent->GetFont()->SetColor(ARGB(255,255,255,255));
			if (strlen(tip) > 4)
			m_pParent->GetFont()->printf(m_ScreenX-16,m_ScreenY-12,HGETEXT_LEFT,tip);
		}
	}
};

bool cCharGUI::UserClick()
{

	bool rtn = false;
	if (m_Show)
	{
		switch(m_Selected)
		{
		case 1: //Attack
			m_pParent->GetBattleControl()->UserAttack();
			break;
		case 2: //cancel
			m_pParent->GetBattleControl()->UserCancel();
			//m_Show=true;
			break;
		case 3://move
			m_pParent->GetBattleControl()->UserMove(true);
			break;
		case 4://pass
			m_pParent->GetBattleControl()->UserPass();
			break;
		}
		rtn = true;
	}
	return rtn;
};



bool cCharGUI::UserClickMPly()
{
	bool rtn = false;
	if (m_Show)
	{
		switch(m_Selected)
		{
		case 1: //Attack
			m_pParent->GetBattleControl()->UserAttack();
			break;
		case 2:
			m_pParent->GetBattleControl()->UserCancel();
			break;
		case 3: //Move
			m_pParent->GetBattleControl()->UserMove(false);
			break;
		case 4: //PASS
			{
				m_pParent->GetBattleControl()->UserPassMPly();
				HideMenu();
			}
			break;
		}
		rtn = true;
	}
	return rtn;
};
void  cCharGUI::AddTeam(cCharacterObject* npc)
{
	TeamList.push_back(npc);
};


void cCharGUI::ShowMenu(){ m_Show = true; };
void cCharGUI::HideMenu(){ m_Show = false; };
bool cCharGUI::isShown(){ return m_Show; };
