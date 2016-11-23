#include "resource.h"
#include <vector>

/*
Control the flow through the fight. 
Call AI or pass turn to other player
*/
#pragma warning(disable:4244)
//using namespace std;

#ifndef RF2D_BATTLECONTROL_H
#define RF2D_BATTLECONTROL_H

class cApp;
class cCharacterObject;
class cCharGUI;

class cBattleControl
{
	public:
		cBattleControl(cApp* pParent);
		~cBattleControl();
		void TurnComplete();
		void UserAttack();
		void UserMove(bool AllowUndo);
		void UserCancel();
		void UserPass();
		void UserPassMPly();
		void SetSource(cCharacterObject* source);
		void SetTargetNPC(cCharacterObject* npc);
		void AttackComplete();
		void MoveComplete();
		cCharacterObject* GetSource();
		cCharacterObject* GetCurrentNPC();
		cCharacterObject* GetTargetNPC();
		cCharacterObject* GetNextNPC();
		cCharacterObject* GetChallengerMember(int pos);
		cCharacterObject* GetHomeMember(int pos);
		float GetSourceX();
		float GetSourceY();
		bool GetMoved();
		bool GetAttacked();
		void StartMPlayRound();
		void StartRound();
		void AddHomeTeam(cCharacterObject* npc);
		void AddChallengerTeam(cCharacterObject* npc);
		int GetHomeTeamCount();
		int GetChallengerTeamCount();
		void MonitorActions();
	private:
		cApp*				m_pParent;
		cCharacterObject*	m_SourceNPC;
		cCharacterObject*	m_CurrentNPC;
		cCharacterObject*	m_TargetNPC;
		bool				m_Attacked;
		bool				m_Moved;
		bool				m_BattleOver;
		bool				m_Passed;
		bool				m_Targeting;
		float				m_ScreenX, m_ScreenY;
		int					m_PreSourceX,m_PreSourceY;
		int					m_CrntRndHomeTeam;
		int					m_CrntRndChallenger;
		int					m_NextSide;
		ISODIR				m_PreSourceDir;
		vector<cCharacterObject*> HomeTeamList;
		vector<cCharacterObject*> ChallengerList;
};

#endif