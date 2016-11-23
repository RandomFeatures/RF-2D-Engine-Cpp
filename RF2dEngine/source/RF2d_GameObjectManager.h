#include <vector>
#include <map>
#include <string>
#include "resource.h"
#include "NetworkTypes.h" //defines the Packet data type

/*
track, update, move and render all game objects
*/

using namespace std;

#ifndef RF2D_GAMEOBJECTMANAGER_H
#define RF2D_GAMEOBJECTMANAGER_H

class cApp;
class cCharacterObject;
class cSpellObject;
class CGameObject;
class cProjectileObject;
class cLight;
class cSound;

typedef vector<cCharacterObject*> TargetList;
typedef map<int, string> LookupList;
typedef LookupList::value_type llValType;
typedef LookupList::iterator llMapItor;

class cGameObjectManager
{
	public:
		cGameObjectManager(cApp* pParent);
		~cGameObjectManager();
		static cGameObjectManager *Instance();
		void Render(int layer, float fDeltaTime, float camX, float camY);
		void UpdateBeforeRender(float fDeltaTime, float camX, float camY);
		void UpdateAfterRender(float fDeltaTime, float camX, float camY);
		void RenderNoNPC(int layer, float fDeltaTime, float camX, float camY);
		void AddNpc(int dir, int side, int action, int x, int y, int hp, int imin, int imax, int move, int spellID, float phy, float mag, const char *guid, const char *fileName, const char *name);
		void AddNpc(int dir, int side, int action, int x, int y, const char *guid, const char *fileName, const char *name);
		void AddSpell(int id, std::string fileName, std::string name);
		void AddSprite(int offx, int offy,  int x, int y, char *group, char *guid, char *fileName, char *name);
		void AddStatic(int offx, int offy,  int x, int y, int length, int width, int layer, bool shadow, char *frame, char *group, char *guid, char *fileName);
		void AddLight(int offx, int offy, int x, int y, float width, float height, DWORD color, char *group, char *guid, int flickcount);
		void AddSound(int pan, int interval, int rand, int loop, char *guid, char* filename);
		void CastSpell(bool local, int spellID, cCharacterObject *caster, cCharacterObject *target);
		void CastSpell(bool local, int spellID, char *casterID, char *targetID);
		void CastSpell(bool local, cSpellObject *spell, cCharacterObject *caster, TargetList targetlist);
		void CastSpell(bool local, cSpellObject *spell, cCharacterObject *caster, cCharacterObject *target);
		void ScriptCastSpell(int spellID, cCharacterObject *caster, cCharacterObject *target);

		void MeleeAttack(bool local, cCharacterObject *attacker, cCharacterObject *target);
		void MeleeAttack(bool local, cCharacterObject *attacker,  TargetList targetlist);
		void MeleeAttack(bool local, char *attackerID, char *targetID);
		void LaunchProjectile(cProjectileObject *projectile);
		void MoveSprite(ISODIR dir, char *GUID);
		void MoveSelectedNPC(int x, int y);
		void PathSelectedNPC(int x, int y);
		void PathNPC(bool local, char *charID, int x, int y);
		void PathNPC(bool local, cCharacterObject *charID, int x, int y);
		void MouseNPC(int x, int y);
		void CleanUpInActiveSpells(bool forced);
		void CleanUpInActiveProjectiles(bool forced);
		void LoadMap(char *fileName, int side);
		void LoadEncounter(int iEncounterID, int side);
		void ClearGameObjectList();
		void UpdateMovingCharacters(float fDeltaTime);
		void SelectNPC(int x, int y);
		void SetAmbientColor(int red, int green, int blue);
		void SetAmbientColor(DWORD ambientcolor);
		void UpdateCharacterCollision();
		int GetArmySize();
		int GetFacing();
		cCharacterObject* GetNPCAt(int x, int y);
		cCharacterObject* GetWeakestNPC(cCharacterObject* source);
		cCharacterObject* GetStrongestNPC(cCharacterObject* source);
		cCharacterObject* GetHighPhyNPC(cCharacterObject* source);
		cCharacterObject* GetHighMagNPC(cCharacterObject* source);
		cCharacterObject* GetLowPhyNPC(cCharacterObject* source);
		cCharacterObject* GetLowMagNPC(cCharacterObject* source);
		cCharacterObject* GetNearestNPC(cCharacterObject* source);
		cCharacterObject* GetSelectNPC();
		cCharacterObject* GetNPC(char *GUID);
		cSpellObject* GetSpell(int spellID);
		bool CheckCollision(WorldPoint wrld, bool includeNPC);
		WorldPoint GetTileInRange(cCharacterObject* source, cCharacterObject* target);
		void MoveToAttackRange(cCharacterObject* source, cCharacterObject* target);
		void IdleWander(cCharacterObject* source);
		void GainFocus();

	public: //MPlayer Stuff See RF2d_GameObjectManagerMPly.cpp
		void ProcessPacket(Packet *p, unsigned char packetIdentifier);
		void rcv_PathNPC(Packet *p);
		void rcv_CastSpell(Packet *p);
		void rcv_MeleeAttack(Packet *p);
		void rcv_AddNPC(Packet *p);
		void rcv_JoinGame(Packet *p);
		void rcv_CharTurn(Packet *p);
		void rcv_TurnDone(Packet *p);

		void xmit_PathNPC(char *charID, int x, int y);
		void xmit_CastSpell(int spellID, char *casterID, char *targetID);
		void xmit_MeleeAttack(char *attackerID, char *targetID);
		void xmit_AddNpc(int dir, int side, int action, int x, int y, char *guid, char *fileName, char *name);
		void xmit_AllNPCs();
	private:
		//vector<spritePos> npcPosition;
		bool m_UseShadows;
		bool m_FocusLost;
		int m_NPCCount;
		char m_TexPath[150];
		cApp*  m_pParent;
		DWORD m_AmbientColor;
		int m_ArmySize;
		int m_Face;
		cCharacterObject *m_SelectedNPC;
		vector<CGameObject*> MasterObjectList;
		vector<cCharacterObject*> NPCList;
		vector<cProjectileObject*> ProjectileList;
		vector<CGameObject*> LayerZeroList;
		vector<CGameObject*> LayerOneList;
		vector<CGameObject*> LayerTwoList;
		vector<CGameObject*> LayerThreeList;
		vector<CGameObject*> SpriteList;
		vector<cLight*> MasterLightList;
		vector<cSound*> MasterSFXList;
		vector<cSpellObject*> MasterSpellList;
		vector<cSpellObject*> ActiveSpellList;
		LookupList SpellLookup;

};

#endif
