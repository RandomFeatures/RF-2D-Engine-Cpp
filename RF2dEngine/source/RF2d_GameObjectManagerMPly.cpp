#include "RF3d_Global.h"
#include "RF2d_Application.h"
#include "Sorts.h"
//My Files
#include "MPlayerStructures.h"

/*
transmit and recieve game object updates from/to other players
*/


//Process Packets that have been identified for the GUI Manager
void cGameObjectManager::ProcessPacket(Packet *p, unsigned char packetIdentifier)
{
switch (packetIdentifier)
	{
		case ID_PATH_NPC:
			rcv_PathNPC(p);
			break;
		case ID_CAST_SPELL:
			rcv_CastSpell(p);
			break;
		case ID_ADD_NPC:
			rcv_AddNPC(p);
			break;
		case ID_MELEE_ATTACK:
			rcv_MeleeAttack(p);
			break;
		case ID_JOIN_GAME:
			rcv_JoinGame(p);
			break;
		case ID_TURN_DONE:
			rcv_TurnDone(p);
			break;
		case ID_CHAR_TURN:
			rcv_CharTurn(p);
			break;
		default:
			hge->System_Log("Unknown packet type in cGameObjectManager::ProcessPacket.  Type %i with length %i", packetIdentifier, p->length);
			//assert(1);
			break;
	}
};


void cGameObjectManager::rcv_TurnDone(Packet *p)
{
	hge->System_Log("rcv_TurnDone");
	m_pParent->GetBattleControl()->TurnComplete();
	//m_pParent->GetCharGUI()->ShowMenu();
};

void cGameObjectManager::rcv_CharTurn(Packet *p)
{
	hge->System_Log("rcv_CharTurn");
	cCharacterObject* NPC;
	CharTurnStruct s = *((CharTurnStruct *) p->data);
	NPC = GetNPC(s.charID);
	m_pParent->GetBattleControl()->SetSource(NPC);
	m_pParent->GetCharGUI()->ShowMenu();
};


void cGameObjectManager::rcv_PathNPC(Packet *p)
{
	hge->System_Log("rcv_PathNPC");
	MoveCharacterStruct s = *((MoveCharacterStruct *) p->data);
	PathNPC(false,s.charId, s.destX, s.destY);
};

void cGameObjectManager::rcv_CastSpell(Packet *p)
{
	hge->System_Log("rcv_CastSpell");
	CastSpellStruct s = *((CastSpellStruct *) p->data);
	CastSpell(false,s.spellId,s.sourcID,s.trgtID);
};

void cGameObjectManager::rcv_MeleeAttack(Packet *p)
{
	hge->System_Log("rcv_MeleeAttack");
	MeleeAttackStruct s = *((MeleeAttackStruct *) p->data);
	MeleeAttack(false,s.sourcID,s.trgtID);
};

void cGameObjectManager::rcv_AddNPC(Packet *p)
{
	AddNPCStruct s = *((AddNPCStruct *) p->data);
	AddNpc(s.dir,s.side,s.action,s.x,s.y,s.guid,s.fileName,s.name);
};

void cGameObjectManager::rcv_JoinGame(Packet *p)
{
	//transmit my side NPC
	hge->System_Log("rcv_JoinGame");
	xmit_AllNPCs();//send all for inital testing
};

void cGameObjectManager::xmit_PathNPC(char *charID, int x, int y)
{
	hge->System_Log("xmit_PathNPC");
	if (m_pParent->GetMPlayerControl()->IsConnected())
		m_pParent->GetMPlayerControl()->TransmitMoveCharacter(charID,x,y);
};

void cGameObjectManager::xmit_CastSpell(int spellID, char *casterID, char *targetID)
{
	hge->System_Log("xmit_CastSpell");
	if (m_pParent->GetMPlayerControl()->IsConnected())
		m_pParent->GetMPlayerControl()->TransmitCastSpell(spellID, casterID, targetID);
};

void cGameObjectManager::xmit_MeleeAttack(char *attackerID, char *targetID)
{
	hge->System_Log("xmit_MeleeAttack");
	if (m_pParent->GetMPlayerControl()->IsConnected())
		m_pParent->GetMPlayerControl()->TransmitMeleeAttack(attackerID, targetID);
};

void cGameObjectManager::xmit_AddNpc(int dir, int side, int action, int x, int y, char *guid, char *fileName, char *name)
{
	hge->System_Log("xmit_AddNpc");
	if (m_pParent->GetMPlayerControl()->IsConnected())
		m_pParent->GetMPlayerControl()->TransmitAddNPC(dir,action,x,y,guid,fileName,name);
};

void  cGameObjectManager::xmit_AllNPCs()
{
	cCharacterObject* NPC = NULL;

	if (m_pParent->GetMPlayerControl()->IsConnected())
	{
		if (m_pParent->GetMPlayerControl()->GetSide()== HOMETEAM)
		{
			for (unsigned int iloop=0;iloop<m_pParent->GetBattleControl()->GetHomeTeamCount();iloop++)
			{
				NPC	= m_pParent->GetBattleControl()->GetHomeMember(iloop);
				m_pParent->GetMPlayerControl()->TransmitAddNPC(NPC->GetDirection(),NPC->GetCurrentAction(),NPC->GetxPos(),NPC->GetyPos(),NPC->GetGUID(),NPC->FileName(),NPC->GetDisplayName());
			}
		}
		else
		{
			for (unsigned int iloop=0;iloop<m_pParent->GetBattleControl()->GetChallengerTeamCount();iloop++)
			{
				NPC	= m_pParent->GetBattleControl()->GetChallengerMember(iloop);
				m_pParent->GetMPlayerControl()->TransmitAddNPC(NPC->GetDirection(),NPC->GetCurrentAction(),NPC->GetxPos(),NPC->GetyPos(),NPC->GetGUID(),NPC->FileName(),NPC->GetDisplayName());
			}
		}
	}

};
