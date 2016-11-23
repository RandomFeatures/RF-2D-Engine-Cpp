#include "RF2d_Application.h"
#include "RF3d_Global.h"
#include "RF2d_BattleControl.h"
#include "RF2d_Scripting.h"


/*
renders character action icons.
Acttack, Move, Pass

Respond to player clicks to notify
the system that the player has choose to Attack, Move, or Pass
*/

cBattleControl::cBattleControl(cApp* pParent)
{
	m_pParent = pParent;
	m_Attacked = false;
	m_Moved = false;
	m_BattleOver = false;
	m_Passed = false;
	m_Targeting = false;
	m_CurrentNPC = 0;
	m_TargetNPC = 0;
};

cBattleControl::~cBattleControl()
{
	HomeTeamList.clear();
	ChallengerList.clear();
};

void cBattleControl::MonitorActions()
{
	CHARSTATUS	CrntStat = IDLE;
	CHARSTATUS	TrgtStat = IDLE;
	//get the current status
	if (m_CurrentNPC)
		CrntStat = m_CurrentNPC->GetStatus();
    if (m_TargetNPC)
		TrgtStat = m_TargetNPC->GetStatus();


	if (m_Passed)
	{

		if (((TrgtStat == DONEHURTING || TrgtStat == DONEDYING)  && CrntStat == DONEATTACKING) ||  //make sure attack and pain/death are both done
			(CrntStat != ATTACKING && CrntStat != WALKING && CrntStat != HURTING && CrntStat != DYING))//passed and current not doing anything else
				TurnComplete();

	}else
	{

		if (m_TargetNPC)
		{
			if (TrgtStat == DONEHURTING || TrgtStat == DONEDYING) //target has done pain/death ani
			{
				if (m_Moved && m_Attacked) //current is out of actions
				{
					hge->System_Log("TurnComplete");
					TurnComplete();
				}
			}
		}

		if (m_CurrentNPC)
		{
			if (CrntStat == DONEATTACKING && !m_Attacked)
				AttackComplete();
			if (CrntStat == DONEWALKING && !m_Moved)
				MoveComplete();
			if (CrntStat == NOPATH && !m_Moved)
				MoveComplete();
			//determin if UI should be shown again or goto next
			if (m_Moved && m_Attacked)
			{//moved and attacked so lets get out of here
				//!!Not a good place for this!!!
				//TurnComplete();
			}else
			{//more to do still
				if (m_NextSide == CHALLENGER && CrntStat == WAITING)//meaning current is HOME
				{
					if (!m_pParent->GetCharGUI()->isShown() && !m_Targeting)
						m_pParent->GetCharGUI()->ShowMenu();
				}else
				{
					//TODO CALL AI for next action
					if ((CrntStat == WAITING) && m_Moved && !m_Attacked && m_TargetNPC)
						l_AttackTarget(m_CurrentNPC, m_TargetNPC);
						//m_pParent->GetGameObjMangr()->CastSpell(true,m_CurrentNPC->GetCurrentSpellID(), m_CurrentNPC->GetGUID() ,m_TargetNPC->GetGUID());
				}
			}
		}
	}
};


bool cBattleControl::GetMoved()
{
	return m_Moved;
};

bool cBattleControl::GetAttacked()
{
	return m_Attacked;
};

void cBattleControl::AttackComplete()
{
	m_Attacked = true;
	m_Targeting = false;
	m_CurrentNPC->SetStatus(WAITING);
};

void cBattleControl::MoveComplete()
{
	m_Moved = true;
	m_Targeting = false;
	m_CurrentNPC->SetStatus(WAITING);
};

cCharacterObject* cBattleControl::GetCurrentNPC()
{
	return m_CurrentNPC;
};


cCharacterObject* cBattleControl::GetSource()
{
	return m_SourceNPC;
};


float cBattleControl::GetSourceX()
{
	float rtn = 0.0f;

	if (m_SourceNPC)
	{
		rtn = m_SourceNPC->GetScreenX();
	}

	return rtn;
};

float cBattleControl::GetSourceY()
{
	float rtn = 0.0f;

	if (m_SourceNPC)
	{
		rtn = m_SourceNPC->GetScreenY();
	}

	return rtn;
};

void cBattleControl::SetTargetNPC(cCharacterObject* npc)
{
	m_TargetNPC = npc;
	m_TargetNPC->SetStatus(WAITING);
};
cCharacterObject* cBattleControl::GetTargetNPC(){ return m_TargetNPC; };

void cBattleControl::SetSource(cCharacterObject* source)
{
	if (source)
	{
		//hge->System_Log("Source Set");
		m_SourceNPC = source;
		m_CurrentNPC = source;
		m_CurrentNPC->SetStatus(WAITING);
		m_PreSourceX = m_SourceNPC->GetxPos();
		m_PreSourceY = m_SourceNPC->GetyPos();
		m_PreSourceDir = m_SourceNPC->GetDirection();
	}
};

void cBattleControl::TurnComplete()
{

	if (m_CurrentNPC)
			m_CurrentNPC->SetStatus(IDLE);
	if (m_TargetNPC)
			m_TargetNPC->SetStatus(IDLE);

	m_Attacked = false;
	m_Moved = false;
	m_Passed = false;
	m_Targeting = false;

	m_CurrentNPC = GetNextNPC();
	if (m_CurrentNPC && !m_BattleOver)
	{
		if (m_NextSide == CHALLENGER)
		{
			SetSource(m_CurrentNPC);
			m_pParent->GetCharGUI()->ShowMenu();
		}else
		{

			//hge->System_Log("AI Attack1");
			//TODO AI Code here
			l_StartTurn(m_CurrentNPC);
			//hge->System_Log("AI Attack2");
			//m_TargetNPC = m_pParent->GetGameObjMangr()->GetNearestNPC(m_CurrentNPC);
			//m_pParent->GetGameObjMangr()->MoveToAttackRange(m_CurrentNPC,m_TargetNPC);
		}//call AI
	}else		//do end battle stuff
		hge->System_Log("end battle");
};

void cBattleControl::UserAttack()
{
	if (!m_Attacked)
	{
		m_pParent->GetTargetSystem()->SetSource(m_SourceNPC);
		m_pParent->GetTargetSystem()->SetMode(TMATTACK);
		m_Targeting = true;
		m_pParent->GetCharGUI()->HideMenu();
	}
};

void cBattleControl::UserMove(bool AllowUndo)
{
	if (!m_Moved)
	{
		m_pParent->GetTargetSystem()->SetSource(m_SourceNPC);
		m_pParent->GetTargetSystem()->SetMode(TMWALK);
		m_Targeting = true;
		m_pParent->GetCharGUI()->HideMenu();
	} else
	{
		if (!m_Attacked && AllowUndo)
		{
			m_SourceNPC->SetPosition(m_PreSourceX,m_PreSourceY);
			m_SourceNPC->SetDirection(m_PreSourceDir);
			m_Moved = false;
		}
	}
};

void cBattleControl::UserCancel()
{
	//m_pParent->GetCharGUI()->ShowMenu();
	m_Targeting = false;
	m_CurrentNPC->SetStatus(WAITING);
};

void cBattleControl::UserPass()
{
	m_Targeting = false;
	m_pParent->GetCharGUI()->HideMenu();
	m_Passed = true;
};

void cBattleControl::UserPassMPly()
{
	if (m_pParent->GetGameType() == MPLAY)
	{
		if (m_pParent->GetMPlayerControl()->IsGameHost())
		{
			cCharacterObject* NPC = GetNextNPC();
			if (NPC->GetSide() == m_pParent->GetMPlayerControl()->GetSide()) //npc on my side
				SetSource(NPC);
			else //npc on other side
			{
				m_pParent->GetMPlayerControl()->TransmitCharTurn(NPC->GetGUID());
				m_pParent->GetCharGUI()->HideMenu();
			}
		}
		else
		{
			m_pParent->GetMPlayerControl()->TransmitTurnDone();
			m_pParent->GetCharGUI()->HideMenu();
		}
	}

};


void cBattleControl::StartRound()
{
	hge->System_Log("StartRound");
	m_CrntRndHomeTeam = 0;
	m_CrntRndChallenger = 0;
	m_NextSide = HOMETEAM;
	SetSource(GetNextNPC());
	m_pParent->GetCharGUI()->ShowMenu();
	m_BattleOver = false;
};

void cBattleControl::StartMPlayRound()
{
	hge->System_Log("StartMPlayRound");
	if (!m_pParent->GetMPlayerControl()->IsGameHost()) return;
	m_CrntRndHomeTeam = 0;
	m_CrntRndChallenger = 0;
	m_NextSide = CHALLENGER;
	m_pParent->GetMPlayerControl()->TransmitCharTurn(GetNextNPC()->GetGUID());
	m_pParent->GetCharGUI()->HideMenu();
};

cCharacterObject* cBattleControl::GetNextNPC()
{
	cCharacterObject* NPC = NULL;
	int iLoop;
	bool bWinner = false;
	bool bChanllengerLive = false;
	bool bHomeLive = false;

	//see if any challengers still live
	for (iLoop=0;iLoop<ChallengerList.size();iLoop++)
	{
		NPC = ChallengerList[iLoop];
		if (!NPC->IsDead())
			bChanllengerLive = true;
	}
	//see if any of home team still live
	for (iLoop=0;iLoop<HomeTeamList.size();iLoop++)
	{
		NPC = HomeTeamList[iLoop];
		if (!NPC->IsDead())
			bHomeLive = true;
	}
	//somebody is all dead so we are done
	if (!bChanllengerLive || !bHomeLive)
	{
		bWinner = true;
		m_BattleOver = true;
	}

	NPC = NULL;

	if (!bWinner)
	{ //both teams still kicken so figure out who is next
		if ((m_NextSide == CHALLENGER) && (ChallengerList.size() > 0))
		{
			do
			{
				NPC = ChallengerList[m_CrntRndChallenger];
				if (m_CrntRndChallenger >= ChallengerList.size()-1)
					m_CrntRndChallenger = 0;
				else
					m_CrntRndChallenger++;
			}while (NPC->IsDead());
			m_NextSide = HOMETEAM;
		} else if  (HomeTeamList.size() > 0)
		{
			do
			{
				NPC = HomeTeamList[m_CrntRndHomeTeam];
				if (m_CrntRndHomeTeam >= HomeTeamList.size()-1)
					m_CrntRndHomeTeam = 0;
				else
					m_CrntRndHomeTeam++;
			}while (NPC->IsDead());
			m_NextSide = CHALLENGER;
		}
	}
	return NPC;
};

void cBattleControl::AddHomeTeam(cCharacterObject* npc)
{
	HomeTeamList.push_back(npc);
};

void cBattleControl::AddChallengerTeam(cCharacterObject* npc)
{
	ChallengerList.push_back(npc);
};

int cBattleControl::GetHomeTeamCount()
{
	return	HomeTeamList.size();
};

int cBattleControl::GetChallengerTeamCount()
{
	return ChallengerList.size();
};

cCharacterObject* cBattleControl::GetChallengerMember(int pos)
{
	cCharacterObject* NPC = NULL;
	if (pos < ChallengerList.size())
        NPC = ChallengerList[pos];

	return NPC;
};

cCharacterObject* cBattleControl::GetHomeMember(int pos)
{
	cCharacterObject* NPC = NULL;

	if (pos < HomeTeamList.size())
		NPC = HomeTeamList[pos];

	return NPC;
};
