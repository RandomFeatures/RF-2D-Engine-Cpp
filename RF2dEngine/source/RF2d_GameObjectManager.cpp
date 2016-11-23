#include "RF2d_Application.h"
#include "Sorts.h"
#include "RF3d_Global.h"
#include "RF2d_Scripting.h"
#include "RF2d_CallBackHeader.h"
/*
track, update, move and render all game objects
*/

//Default Constructor
cGameObjectManager::cGameObjectManager(cApp*  pParent)
{
	m_SelectedNPC = NULL;
	m_pParent =  pParent;
	m_ArmySize = 5;
	m_Face = 2;
	strcpy(m_TexPath, pParent->GetGamePath(TEXTURE));
	m_UseShadows = m_pParent->UseShadows();
	m_FocusLost = false;
};

//Default Destructor
cGameObjectManager::~cGameObjectManager()
{
	unsigned int iloop;

	hge->System_Log("delete game objects");
	CGameObject *GameObj;
	for (iloop=0;iloop<MasterObjectList.size();iloop++)
	{
		GameObj = MasterObjectList[iloop];

		if	(GameObj->GetType() == NPC)
			delete (cCharacterObject*)GameObj;
		else
			delete (CGameObject*)GameObj;

	}
	MasterObjectList.clear();

	hge->System_Log("delete spells");
	cSpellObject *spell;
	for (iloop=0;iloop<MasterSpellList.size();iloop++)
	{
		spell = MasterSpellList[iloop];
		delete spell;
	}
	MasterSpellList.clear();

	hge->System_Log("delete lights");
	cLight *light;
	for (iloop=0;iloop<MasterLightList.size();iloop++)
	{
		light = MasterLightList[iloop];
		delete light;
	}
	MasterLightList.clear();
	hge->System_Log("delete sounds");
	cSound *sound;
	for (iloop=0;iloop<MasterSFXList.size();iloop++)
	{
		sound = MasterSFXList[iloop];
		delete sound;
	}
	MasterSFXList.clear();


	hge->System_Log("clear list");
	SpriteList.clear();
	LayerZeroList.clear();
	LayerOneList.clear();
	LayerTwoList.clear();
	LayerThreeList.clear();
	NPCList.clear();
	ProjectileList.clear();

	hge->System_Log("clean inactive projectiles");
	//Clear spell copies
	//CleanUpInActiveProjectiles(true);
	hge->System_Log("clean inactive spells");
	//CleanUpInActiveSpells(true);
	hge->System_Log("done");
};

//Add an NPC to the Master Object list
void cGameObjectManager::AddNpc(int dir, int side, int action, int x, int y, int hp, int imin, int imax, int move, int spellID, float phy, float mag, const char *guid, const char *fileName, const char *name)
{
	m_NPCCount++;
	cCharacterObject *npc = new cCharacterObject(m_pParent, dir, action, fileName, m_TexPath);
	npc->SetGUID(guid);
	npc->SetDisplayName(name);
	npc->SetId(m_NPCCount);
	npc->SetSide(side);
	npc->SetPosition(x, y);
	npc->SetType(NPC);
	npc->DoAction((ACTION)action);
	npc->SetShadows(m_UseShadows);
	npc->SetAmbientColor(m_AmbientColor);
	npc->SetSide(side);
	npc->SetHitPoints(hp);
	npc->SetAttackMin(imin);
	npc->SetAttackMax(imax);
	npc->SetPhyResist(phy);
	npc->SetMagResist(mag);
	npc->AssignCurrentSpellID(spellID);
	npc->SetWalkDistance(move);
	MasterObjectList.push_back(npc);
	NPCList.push_back(npc);
	LayerTwoList.push_back(npc);
	UpdateCharacterCollision();
	if (side == HOMETEAM)
	{
		m_pParent->GetBattleControl()->AddHomeTeam(npc);
		m_pParent->GetCharGUI()->AddTeam(npc);
	}
	else
		m_pParent->GetBattleControl()->AddChallengerTeam(npc);
};

//Add an NPC to the Master Object list
void cGameObjectManager::AddNpc(int dir, int side, int action, int x, int y, const char *guid, const char *fileName, const char *name)
{
	m_NPCCount++;
	cCharacterObject *npc = new cCharacterObject(m_pParent, dir, action, fileName, m_TexPath);
	npc->SetGUID(guid);
	npc->SetDisplayName(name);
	npc->SetId(m_NPCCount);
	npc->SetSide(side);
	npc->SetPosition(x, y);
	npc->SetType(NPC);
	npc->DoAction((ACTION)action);
	npc->SetShadows(m_UseShadows);
	npc->SetAmbientColor(m_AmbientColor);
	npc->SetSide(side);

	MasterObjectList.push_back(npc);
	NPCList.push_back(npc);
	LayerTwoList.push_back(npc);
	UpdateCharacterCollision();
	if (side == HOMETEAM)
	{
		m_pParent->GetBattleControl()->AddHomeTeam(npc);
		m_pParent->GetCharGUI()->AddTeam(npc);
	}
	else
		m_pParent->GetBattleControl()->AddChallengerTeam(npc);
};

//Add an Sprite to the Master Object list
void cGameObjectManager::AddSprite(int offx, int offy,  int x, int y, char *group, char *guid, char *fileName, char *name)
{
	//spritePos temp;
	CGameObject *sprite = new CGameObject(false, false, m_TexPath);
	//npc->SetMoveScript(movescript);
	sprite->LoadSprite(fileName,name);
	//sprite->AssignGUID(guid);
	sprite->SetPosition(x, y);
	sprite->SetOffset(offx,offy);
	sprite->SetShadows(m_UseShadows);
	sprite->SetAmbientColor(m_AmbientColor);
	sprite->SetGameParent(m_pParent);
	MasterObjectList.push_back(sprite);
	SpriteList.push_back(sprite);
	LayerTwoList.push_back(sprite);
	//npcPosition.push_back(temp);
};


//Add an Sprite to the Master Object list
void cGameObjectManager::AddStatic(int offx, int offy,  int x, int y, int length, int width, int layer, bool shadow, char *frame, char *group, char *guid, char *fileName)
{
//	hge->System_Log("%i %i",offx, offy);

	//spritePos temp;
	CGameObject *statc = new CGameObject(false, false, m_TexPath);
	//npc->SetMoveScript(movescript);
	statc->LoadStatic(frame, fileName);
	//sprite->AssignGUID(guid);
	statc->SetPosition(x, y);
	statc->SetOffset(offx,offy);
	if (m_UseShadows && shadow == true)
		statc->SetShadows(true);
	else
		statc->SetShadows(false);

	statc->SetAmbientColor(m_AmbientColor);
	statc->SetGameParent(m_pParent);
	MasterObjectList.push_back(statc);
	switch(layer)
	{
		case 0:
			LayerZeroList.push_back(statc);
			break;
		case 1:
			LayerOneList.push_back(statc);
			break;
		case 2:
			LayerTwoList.push_back(statc);
			break;
		case 3:
			LayerThreeList.push_back(statc);
			break;
	}
	//npcPosition.push_back(temp);
};

//Add a spell to the MasterSpell list
void cGameObjectManager::AddSpell(int id, std::string fileName, std::string name)
{//This should only be called from the startup script
	SpellLookup.insert(llValType(id,fileName));
};

void cGameObjectManager::AddLight(int offx, int offy, int x, int y, float width, float height, DWORD color, char *group, char *guid, int flickcount)
{
	//spritePos temp;
	cLight *light = new cLight(m_pParent);
	light->SetPosition(x, y);
	light->SetOffset(offx,offy);
	light->SetColor(color);
	light->SetSize(width,height);
	light->SetFlickerCount(flickcount);
	MasterLightList.push_back(light);
}

void cGameObjectManager::AddSound(int pan, int interval, int rand, int loop, char *guid, char* filename)
{
	cSound *sound = new cSound(m_pParent);
	sound->LoadSFX(filename);
	sound->SetPan(pan);
	if(rand)
		sound->SetInterval(true,interval);
	else
		sound->SetInterval(false,interval);

	sound->SetGUID(guid);
	if (loop)
	{
		sound->LoopSFX();
		sound->PlayNow();
	}
	MasterSFXList.push_back(sound);
};


//CastSpell from script using caster and target GUID rather than a pointer
void cGameObjectManager::CastSpell(bool local, int spellID, char *casterID, char *targetID)
{//This should only be called from the scripting system
	bool bCasterFound = false;
	bool bTargetFound = false;
    int iTargetID = 0;
    int iCasterID = 0;

	//Seach the Master Object list and find a pointer to the caster
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if	(NPCList[iloop]->GetType() == NPC) //Dont check walls and crap
			if (strcmp(NPCList[iloop]->GetGUID(), casterID) == 0)
			{
				bCasterFound = true;
				iCasterID = iloop;
				break;
			}
	}

	//Seach the Master NPC list and find a pointer to the target
	for (unsigned int jloop=0;jloop<NPCList.size();jloop++)
	{
		if	(NPCList[jloop]->GetType() == NPC)
			if (strcmp(NPCList[jloop]->GetGUID(), targetID) == 0)
			{
				bTargetFound = true;
				iTargetID = jloop;
				break;
			}
	}

	if (bTargetFound &&  bCasterFound)
	{
		//hge->System_Log(((cCharacterObject*)MasterObjectList[iloop])->GetGUID());
		CastSpell(local,spellID,((cCharacterObject*)NPCList[iCasterID]),((cCharacterObject*)NPCList[iTargetID]));
	}else
	{//Log if one of them was not found
		if (!bTargetFound) hge->System_Log("CastSpell: Target Not Found: %i", targetID);
		if (!bCasterFound) hge->System_Log("CastSpell: Caster Not Found: %i", casterID);
	}

};


void cGameObjectManager::ScriptCastSpell(int spellID, cCharacterObject *caster, cCharacterObject *target)
{
	CastSpell(true, spellID, caster, target);
}

//Cast a spell for engine code
void cGameObjectManager::CastSpell(bool local, int spellID, cCharacterObject *caster, cCharacterObject *target)
{
	bool SpellFound = false;
	std::string spellfile;

	for (unsigned int iloop=0;iloop<MasterSpellList.size();iloop++)
	{
			if (MasterSpellList[iloop]->Id() == spellID)
			{
				CastSpell(local,MasterSpellList[iloop],caster,target);
				SpellFound = true;
				break;
			}
	}
	if (!SpellFound)
	{

		//Load Spell
		spellfile = m_pParent->GetGamePath(SCRIPT);
		spellfile += hge->Ini_GetString("spells",IntToStr(spellID).c_str(),"spells\\StdMelee.lua");
		cSpellObject *spell = new cSpellObject(m_pParent,spellfile);
		spell->AssignId(spellID);
		l_LoadSpell(spell);
		MasterSpellList.push_back(spell);
		CastSpell(local,spell,caster,target);
	}

};

void cGameObjectManager::CastSpell(bool local, cSpellObject *spell, cCharacterObject *caster, TargetList targetlist)
{
	ActiveSpellList.push_back(spell);
	caster->SetCurrentTarget(targetlist[0]);
	targetlist[0]->SetLastAttacker(caster);
	//Cast the spell
	spell->Cast(caster, targetlist);
	if (local && m_pParent->GetGameType() == MPLAY)
		xmit_CastSpell(spell->Id(),caster->GetGUID(),targetlist[0]->GetGUID());
};

void cGameObjectManager::CastSpell(bool local, cSpellObject *spell, cCharacterObject *caster, cCharacterObject *target)
{

	ActiveSpellList.push_back(spell);
	caster->SetCurrentTarget(target);
	target->SetLastAttacker(caster);
	//Cast the spell
	spell->Cast(caster, target);
	if (local && m_pParent->GetGameType() == MPLAY)
		xmit_CastSpell(spell->Id(),caster->GetGUID(),target->GetGUID());
};



cSpellObject* cGameObjectManager::GetSpell(int spellID)
{
	bool SpellFound = false;
	std::string spellfile;
	hge->System_Log("GetSpell: %i", spellID);
	for (unsigned int iloop=0;iloop<MasterSpellList.size();iloop++)
	{
			if (MasterSpellList[iloop]->Id() == spellID)
			{
				return MasterSpellList[iloop];
				SpellFound = true;
				break;
			}
	}

	if (!SpellFound)
	{

		spellfile = m_pParent->GetGamePath(SCRIPT);
		spellfile += hge->Ini_GetString("spells",IntToStr(spellID).c_str(),"spells\\StdMelee.lua");
		cSpellObject *spell = new cSpellObject(m_pParent,spellfile);
		//Load Spell
		//hge->System_Log("GetSpell: %s", SpellLookup[spellID].c_str());
		//cSpellObject *spell = new cSpellObject(m_pParent,SpellLookup[spellID]);
		spell->AssignId(spellID);
		l_LoadSpell(spell);
		MasterSpellList.push_back(spell);
		return spell;
	}

	return NULL;
};

//remove spell copies from memory once they are done casting
void cGameObjectManager::CleanUpInActiveSpells(bool forced)
{   //when necessary the forced pram will clear the list no matter what

	if (forced)
	{
        ActiveSpellList.clear();
	}else
	{

        cSpellObject *spell;
        //Loop through the ActiveSpellList looking for spells that are finshed
        //In a turn based game this list should never have more than 1 or 2 items to compare
        for (vector<cSpellObject*>::iterator iter = ActiveSpellList.begin();iter != ActiveSpellList.end();)
        {
            if ((*iter)->Finished())
            {
                //ActiveSpellList.erase(spell);
                iter = ActiveSpellList.erase(iter);
            }else
                iter++;
        }


	}
};

//remove spell copies from memory once they are done casting
void cGameObjectManager::CleanUpInActiveProjectiles(bool forced)
{   //when necessary the forced pram will clear the list no matter what

	CGameObject *spell;

	for (vector<CGameObject*>::iterator iter = LayerTwoList.begin();iter != LayerTwoList.end();)
	{
		if ((*iter)->GetType() == PROJ)
		{
			spell = *iter;
			if (((cProjectileObject*)spell)->Finished() || forced)
			{
				//LayerTwoList.erase(iter);
				iter = LayerTwoList.erase(iter);
			}else
                iter++;
		}else
            iter++;

	}


	//Loop through the projectile looking for projectiles that are finshed
	//In a turn based game this list should never have more than 1 or 2 items to compare
	for (vector<cProjectileObject*>::iterator iter2 = ProjectileList.begin();iter2 != ProjectileList.end();)
	{
		if ((*iter2)->Finished() || forced)
		{
			//ProjectileList.erase(iter2);
			iter2 = ProjectileList.erase(iter2);
		}else
            iter2++;
	}
};


//MeleeAttack from script using attacker and target GUID rather than a pointer
void cGameObjectManager::MeleeAttack(bool local, char *attackerID, char *targetID)
{//This should only be called from the scripting system
	bool bAttackerFound = false;
	bool bTargetFound = false;
    int iCasterID = 0;
    int iTargetID = 0;
	//Seach the Master Object list and find a pointer to the caster
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if	(NPCList[iloop]->GetType() == NPC) //Dont check walls and crap
			if (strcmp(NPCList[iloop]->GetGUID(), attackerID) == 0)
			{
				bAttackerFound = true;
				iCasterID = iloop;
				break;
			}
	}

	//Seach the Master NPC list and find a pointer to the target
	for (unsigned int jloop=0;jloop<NPCList.size();jloop++)
	{
		if	(NPCList[jloop]->GetType() == NPC)
			if (strcmp(NPCList[jloop]->GetGUID(), targetID) == 0)
			{
				bTargetFound = true;
				iTargetID = jloop;
				break;
			}
	}

	if (bTargetFound &&  bAttackerFound)
	{
		//hge->System_Log(((cCharacterObject*)MasterObjectList[iloop])->GetGUID());
		MeleeAttack(local,(cCharacterObject*)NPCList[iCasterID],(cCharacterObject*)NPCList[iTargetID]);
	}else
	{//Log if one of them was not found
		if (!bTargetFound) hge->System_Log("MeleeAttack: Target Not Found: %i", targetID);
		if (!bAttackerFound) hge->System_Log("MeleeAttack: Attacker Not Found: %i", attackerID);
	}
};

void cGameObjectManager::MeleeAttack(bool local, cCharacterObject *attacker, cCharacterObject *target)
{
	attacker->FaceTarget(target);
	attacker->SetCurrentTarget(target);
	target->SetLastAttacker(attacker);
	target->FaceTarget(attacker);
	attacker->DoAction(ATTACK);
	if (local && m_pParent->GetGameType() == MPLAY)
		xmit_MeleeAttack(attacker->GetGUID(),target->GetGUID());
	//TODO - target take damage
};

void cGameObjectManager::MeleeAttack(bool local, cCharacterObject *attacker,  TargetList targetlist)
{
	int iLoop;
	attacker->FaceTarget(targetlist[0]);
	attacker->SetCurrentTarget(targetlist[0]);
	for(iLoop=0;iLoop<targetlist.size();iLoop++)
	{
		targetlist[iLoop]->FaceTarget(attacker);
		targetlist[iLoop]->SetLastAttacker(attacker);
	}
	attacker->DoAction(ATTACK);
	//if (local && m_pParent->GetGameType() = MPLAY)
	//	 xmit_MeleeAttack(attacker->GetGUID(),target->GetGUID());
	//TODO - target take damage
	//for(iLoop=0;iLoop<targetlist.size();iLoop++)
	//{
	//	targetlist[iLoop]->//target take damage
	//}
};

void cGameObjectManager::LaunchProjectile(cProjectileObject *projectile)
{
	hge->System_Log("LaunchProjectile");
	projectile->Launch();
	ProjectileList.push_back(projectile);
	//LayerTwoList.push_back(projectile);
};


void cGameObjectManager::MouseNPC(int x, int y)
{
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		NPCList[iloop]->MouseOver(x,y);
	}
};

//Select NPC to walk to a location
void cGameObjectManager::PathSelectedNPC(int x, int y)
{
	if (m_SelectedNPC)
	{
		PathNPC(true, m_SelectedNPC, x, y);
	}


};

//allow selected NPC to follow the mouse
void cGameObjectManager::MoveSelectedNPC(int x, int y)
{
	int newx;
	int newy;
	ISODIR dir;
	if (m_SelectedNPC)
	{
		//Figure out the direction to face for our next destiation tile
		dir = iNW;
		if (x <= m_SelectedNPC->GetScreenX() && y <= m_SelectedNPC->GetScreenY())
			dir = iNW;
		if (x > m_SelectedNPC->GetScreenX() && y < m_SelectedNPC->GetScreenY())
			dir = iNE;
		if (x <= m_SelectedNPC->GetScreenX() && y >= m_SelectedNPC->GetScreenY())
			dir = iSW;
		if (x > m_SelectedNPC->GetScreenX() && y > m_SelectedNPC->GetScreenY())
			dir = iSE;

		//get the actual X,Y of the next tile based on direction
		switch ( dir )
		{
			case iNE:
				if (m_SelectedNPC->GetyPos() % 2 == 0)
				{
					newx = m_SelectedNPC->GetxPos();
					newy = m_SelectedNPC->GetyPos() - 1;
				}else
				{
					newx = m_SelectedNPC->GetxPos() + 1;
					newy = m_SelectedNPC->GetyPos() - 1;
				}
				break;
			case iNW:
				if (m_SelectedNPC->GetyPos() % 2 == 0)
				{
					newx = m_SelectedNPC->GetxPos() - 1;
					newy = m_SelectedNPC->GetyPos() - 1;
				}else
				{
					newx = m_SelectedNPC->GetxPos();
					newy = m_SelectedNPC->GetyPos() - 1;
				}
				break;
			case iSE:
				if (m_SelectedNPC->GetyPos() % 2 == 0)
				{
					newx = m_SelectedNPC->GetxPos() ;
					newy = m_SelectedNPC->GetyPos() + 1;
				}else
				{
					newx = m_SelectedNPC->GetxPos() + 1;
					newy = m_SelectedNPC->GetyPos() + 1;
				}
				break;
			case iSW:
				if (m_SelectedNPC->GetyPos() % 2 == 0)
				{
					newx = m_SelectedNPC->GetxPos() - 1;
					newy = m_SelectedNPC->GetyPos() + 1;
				}else
				{
					newx = m_SelectedNPC->GetxPos() ;
					newy = m_SelectedNPC->GetyPos() + 1;
				}
				break;
		}

		//if its free then walk to that tile
		if (!m_pParent->GetMainMap()->CheckCollision(newx,newy))
		  m_SelectedNPC->MoveTo(newx,newy,dir);

	}
};

void cGameObjectManager::SelectNPC(int x, int y)
{
	m_SelectedNPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		NPCList[iloop]->SetSelected(false);
		if (NPCList[iloop]->IsMouseOver())
		{
			NPCList[iloop]->SetSelected(true);
			m_SelectedNPC = NPCList[iloop];
		}
	}
};

cCharacterObject* cGameObjectManager::GetNPCAt(int x, int y)
{
	cCharacterObject* NPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		//hge->System_Log("iloop %i",iloop);
		if ((NPCList[iloop]->GetxPos() == x) && (NPCList[iloop]->GetyPos() == y))
		{
			NPC = NPCList[iloop];
			if (NPC->IsDead())
			{
				NPC = NULL;
			}
			break;
		}
		//hge->System_Log("iloop %i",iloop);
	}
	return NPC;
};

cCharacterObject* cGameObjectManager::GetNPC(char *GUID)
{
	cCharacterObject* chrobj = NULL;
	//Seach the Master Object list and find a pointer to the caster
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if	(NPCList[iloop]->GetType() == NPC) //Dont check walls and crap
			if (strcmp(NPCList[iloop]->GetGUID(), GUID) == 0)
			{
				chrobj = NPCList[iloop];
				break;
			}
	}
	return chrobj;
};


void cGameObjectManager::PathNPC(bool local, char *charID, int x, int y)
{
	bool bCharFound = false;
    unsigned int iloop;
	//Seach the Master NPC list and find a pointer to the character
	for (iloop=0;iloop<NPCList.size();iloop++)
	{
		if	(NPCList[iloop]->GetType() == NPC) //Dont check walls and crap
			if (strcmp(NPCList[iloop]->GetGUID(), charID) == 0)
			{
				bCharFound = true;
				break;
			}
	}

	if (bCharFound)
	{
		//hge->System_Log(((cCharacterObject*)NPCList[iloop])->GetGUID());
		PathNPC(local,(cCharacterObject*)NPCList[iloop], x, y);
	}else
	{//Log if one of them was not found
		 hge->System_Log("PathNPC: Character Not Found: %s.", charID);
	}
};

void cGameObjectManager::PathNPC(bool local, cCharacterObject *charID, int x, int y)
{//Tell this character to find its way to the new x, y

	if (charID->PathFindTo(x,y))
	{
		if (local && m_pParent->GetGameType() == MPLAY)
			xmit_PathNPC(charID->GetGUID(),x,y);
	}else
	{
		hge->System_Log("NOPATH9");
	}
};

void cGameObjectManager::MoveSprite(ISODIR dir, char *GUID)
{
	bool bSpriteFound = false;
	int newx;
	int newy;
	unsigned int iloop;

	//Seach the Master Object list and find a pointer to the sprite
	for (iloop=0;iloop<NPCList.size();iloop++)
	{
		if (strcmp(NPCList[iloop]->GetGUID(), GUID) == 0)
		{
			bSpriteFound = true;

			break;
		}
	}
	if (bSpriteFound)
	{
		switch ( dir )
		{
			case iNE:
				if (NPCList[iloop]->GetyPos() % 2 == 0)
				{
					newx = NPCList[iloop]->GetxPos();
					newy = NPCList[iloop]->GetyPos() - 1;
				}else
				{
					newx = NPCList[iloop]->GetxPos() + 1;
					newy = NPCList[iloop]->GetyPos() - 1;
				}
				break;
			case iNW:
				if (NPCList[iloop]->GetyPos() % 2 == 0)
				{
					newx = NPCList[iloop]->GetxPos() - 1;
					newy = NPCList[iloop]->GetyPos() - 1;
				}else
				{
					newx = NPCList[iloop]->GetxPos();
					newy = NPCList[iloop]->GetyPos() - 1;
				}
				break;
			case iSE:
				if (NPCList[iloop]->GetyPos() % 2 == 0)
				{
					newx = NPCList[iloop]->GetxPos() ;
					newy = NPCList[iloop]->GetyPos() + 1;
				}else
				{
					newx = NPCList[iloop]->GetxPos() + 1;
					newy = NPCList[iloop]->GetyPos() + 1;
				}
				break;
			case iSW:
				if (NPCList[iloop]->GetyPos() % 2 == 0)
				{
					newx = NPCList[iloop]->GetxPos() - 1;
					newy = NPCList[iloop]->GetyPos() + 1;
				}else
				{
					newx = NPCList[iloop]->GetxPos() ;
					newy = NPCList[iloop]->GetyPos() + 1;
				}
				break;
		}
		NPCList[iloop]->MoveTo(newx,newy,dir);
	//	QuickSort(LayerTwoList,0,LayerTwoList.size(),CompareY);
	}
};

void cGameObjectManager::UpdateCharacterCollision()
{
	unsigned int iloop;

	m_pParent->GetMainMap()->ClearCharacterCollision();

	for (iloop=0;iloop<NPCList.size();iloop++)
	{
		if (!NPCList[iloop]->IsDead())
		{
			m_pParent->GetMainMap()->SetCharacterCollision(NPCList[iloop]->GetxPos(),NPCList[iloop]->GetyPos());
		}
	}
}

void cGameObjectManager::UpdateMovingCharacters(float fDeltaTime)
{
	bool doSort = false;
	unsigned int iloop;
	for (iloop=0;iloop<NPCList.size();iloop++)
	{
		if (NPCList[iloop]->GetMoving())
		{
			NPCList[iloop]->UpdateMove(fDeltaTime);
		}
		if (NPCList[iloop]->HasMoved())
		{
			doSort = true;
			NPCList[iloop]->ResetMoved();
		}
	}

	for (iloop=0;iloop<ProjectileList.size();iloop++)
	{
		if (ProjectileList[iloop]->GetMoving())
		{
			//hge->System_Log("UpdateMove");
			ProjectileList[iloop]->UpdateMove(fDeltaTime);
		}
		//else
		//	ProjectileList[iloop]->Hit();

		if (ProjectileList[iloop]->HasMoved())
		{
			doSort = true;
			ProjectileList[iloop]->ResetMoved();
		}
	}

	if (doSort)
		QuickSort(LayerTwoList,0,LayerTwoList.size(),CompareY);

};

void cGameObjectManager::UpdateBeforeRender(float fDeltaTime, float camX, float camY)
{
	m_pParent->GetBattleControl()->MonitorActions();
	if(m_FocusLost)
	{
		m_FocusLost = false;
		for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
		{
			NPCList[iloop]->RestorPortrait();
		}
	}
};
void cGameObjectManager::UpdateAfterRender(float fDeltaTime, float camX, float camY)
{
	UpdateMovingCharacters(fDeltaTime);
	m_pParent->GetSoundManager()->FadeSound(fDeltaTime);
	for (unsigned int iloop=0;iloop<MasterSFXList.size();iloop++)
		MasterSFXList[iloop]->Update(fDeltaTime);
};

void cGameObjectManager::Render(int layer, float fDeltaTime, float camX, float camY)
{
	switch (layer)
	{
	case 0://sub
			for (unsigned int iloop=0;iloop<LayerZeroList.size();iloop++)
				LayerZeroList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	case 1://ground
			for (unsigned int iloop=0;iloop<LayerOneList.size();iloop++)
				LayerOneList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	case 2://life
			//Loop through all the NPCs and Render them.
			//Each NPC is responsible for determining if they are on screen and
			//need to be rendered
			for (unsigned int iloop=0;iloop<LayerTwoList.size();iloop++)
				LayerTwoList[iloop]->Render(fDeltaTime, camX, camY, 1);

		break;
	case 3://life
			//Loop through all the NPCs and Render them.
			//Each NPC is responsible for determining if they are on screen and
			//need to be rendered
			for (unsigned int iloop=0;iloop<LayerTwoList.size();iloop++)
				LayerTwoList[iloop]->Render(fDeltaTime, camX, camY, 2);


			//Loop through all the Active Spells and render them
			//Each Spell is responsible for determining if it is on screen and
			//needs to be rendered
			for (unsigned int iloop=0;iloop<ActiveSpellList.size();iloop++)
				ActiveSpellList[iloop]->Render(fDeltaTime, camX, camY);

			for (unsigned int iloop=0;iloop<ProjectileList.size();iloop++)
				ProjectileList[iloop]->Render(fDeltaTime, camX, camY);

			//Clean up spells that are finished casting
			CleanUpInActiveProjectiles(false);
			CleanUpInActiveSpells(false);

		break;

	case 4: //Light
			for (unsigned int iloop=0;iloop<MasterLightList.size();iloop++)
				MasterLightList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	case 5://roof
			for (unsigned int iloop=0;iloop<LayerThreeList.size();iloop++)
				LayerThreeList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	}
};

void cGameObjectManager::RenderNoNPC(int layer, float fDeltaTime, float camX, float camY)
{
	switch (layer)
	{
	case 0://sub
			for (unsigned int iloop=0;iloop<LayerZeroList.size();iloop++)
				LayerZeroList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	case 1://ground
			for (unsigned int iloop=0;iloop<LayerOneList.size();iloop++)
				LayerOneList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	case 2://life
			//Loop through all the NPCs and Render them.
			//Each NPC is responsible for determining if they are on screen and
			//need to be rendered
			for (unsigned int iloop=0;iloop<LayerTwoList.size();iloop++)
				if (LayerTwoList[iloop]->GetType() != NPC)
					LayerTwoList[iloop]->Render(fDeltaTime, camX, camY);
			break;
	case 3: //Light
			for (unsigned int iloop=0;iloop<MasterLightList.size();iloop++)
				MasterLightList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	case 4://roof
			for (unsigned int iloop=0;iloop<LayerThreeList.size();iloop++)
				LayerThreeList[iloop]->Render(fDeltaTime, camX, camY);
		break;
	}
};


void cGameObjectManager::SetAmbientColor(int red, int green, int blue)
{
	SetAmbientColor(ARGB(255,red,green,blue));
};

void cGameObjectManager::SetAmbientColor(DWORD ambientcolor)
{
	CGameObject *GameObj;
	for (unsigned int iloop=0;iloop<MasterObjectList.size();iloop++)
	{
		GameObj = MasterObjectList[iloop];
		GameObj->SetAmbientColor(ambientcolor);
	}
	m_pParent->GetMainMap()->SetAmbientColor(ambientcolor);
	m_AmbientColor = ambientcolor;
};


void cGameObjectManager::ClearGameObjectList()
{

	CGameObject *GameObj;
	for (unsigned int iloop=0;iloop<MasterObjectList.size();iloop++)
	{
		GameObj = MasterObjectList[iloop];

		if	(GameObj->GetType() == NPC)
			delete (cCharacterObject*)GameObj;
		else
			delete (CGameObject*)GameObj;

	}

	cLight *LightObj;
	for (unsigned int iloop=0;iloop<MasterLightList.size();iloop++)
	{
		LightObj = MasterLightList[iloop];
		delete (cLight*)LightObj;

	}

	cSound *SoundObj;
	for (unsigned int iloop=0;iloop<MasterSFXList.size();iloop++)
	{
		SoundObj = MasterSFXList[iloop];
		delete (cSound*)SoundObj;

	}

	MasterSFXList.clear();
	MasterLightList.clear();
	MasterObjectList.clear();
	SpriteList.clear();
	LayerZeroList.clear();
	LayerOneList.clear();
	LayerTwoList.clear();
	LayerThreeList.clear();
	NPCList.clear();
	ProjectileList.clear();
};

void cGameObjectManager::LoadEncounter(int iEncounterID, int side)
{
	int tmpX, tmpY, tmpJnk;
	int tmpoffX, tmpoffY, tmpDir;
	int tmpLength, tmpWidth, tmpHeight, tmpLayer;
	int tmpCount, tmpStatus, tmpShadow, tmpSide,tmpType;
	int tmpAlpha, tmpred, tmpgreen, tmpblue;
	int tmpPan, tmpRand, tmpInter, tmpLoop, tmpWheather;
	float tmpfPhy, tmpfMag;
	int tmpiSpellID, tmpiHps, tmpiMin, tmpiMax, tmpiMove;
	DWORD tmpColor;
	int tmpshadow;
	char tmpGUID[20];
	char tmpGroupID[20];
	char tmpFileName[100];
	char tmpMusicFileName[100];
	char tmpSpriteName[20];
	char tmpFrame[20];
	char tmpLineID[100];
	char tmpMiscString[100];
    std:string strSQL;

	tmpStatus = 15;
	m_NPCCount = 0;
	//hge->System_Log(fileName);
	//Resource Block
	strSQL = "SELECT  [EncounterID],[MapType],[MapName],[Description],[AmbientR],[AmbientG],[AmbientB],[ArmySize],[Face],[WalkSFX],";
    strSQL = strSQL + "[UseShadows],[CloudBlend],[WindDir],[Snow],[FogType],[FogBlend],[FogR],[FogG],[FogB],[FogTextureID],[ScriptFile] ";
    strSQL = strSQL + "FROM  [tblISO_Encounter] WHERE [EncounterID] =" +  IntToStr(iEncounterID);

    m_pParent->GetSQLHelper()->get_result(strSQL);

    if (m_pParent->GetSQLHelper()->fetch_row())
    {
            //Ambient Lignt
            tmpred = m_pParent->GetSQLHelper()->getval("AmbientR");
            tmpgreen = m_pParent->GetSQLHelper()->getval("AmbientG");
            tmpblue = m_pParent->GetSQLHelper()->getval("AmbientB");
            SetAmbientColor((tmpred, tmpgreen, tmpblue);
            //Shadows
            tmpshadow = m_pParent->GetSQLHelper()->getval("UseShadows");
            if (m_pParent->UseShadows()==0)
            {
                m_UseShadows = false;
            }else
            {
                if (tmpshadow == 0)
                    m_UseShadows = false;
                else
                    m_UseShadows = true;
            }
            //Clouds
            tmpWheather = m_pParent->GetSQLHelper()->getval("CloudBlend");
			m_pParent->GetWeatherControl()->CreateClouds(tmpWheather);
            //Wind Direction
            tmpWheather = m_pParent->GetSQLHelper()->getval("WindDir");
			m_pParent->GetWeatherControl()->SetWindDir((DIR)tmpWheather);
            //Snow
            tmpWheather = m_pParent->GetSQLHelper()->getval("Snow");
			m_pParent->GetWeatherControl()->CreateSnow(tmpWheather);
            //Fog
            tmpLayer = m_pParent->GetSQLHelper()->getval("FogTextureID");
            tmpAlpha = m_pParent->GetSQLHelper()->getval("FogBlend");
            tmpred = m_pParent->GetSQLHelper()->getval("FogR");
            tmpgreen = m_pParent->GetSQLHelper()->getval("FogG");
            tmpblue = m_pParent->GetSQLHelper()->getval("FogB");
            tmpType = m_pParent->GetSQLHelper()->getval("FogType");

		    if (tmpLayer)
            {
                m_pParent->GetWeatherControl()->CreateFog(tmpAlpha,tmpred,tmpgreen,tmpblue,tmpType);
            }
            else
                m_pParent->GetWeatherControl()->CreateGroundFog(tmpAlpha,tmpred,tmpgreen,tmpblue,tmpType);


			//inFile >> tmpAlpha >> tmpred >> tmpgreen >> tmpblue;
			//m_pParent->GetWeatherControl()->CreateWater(tmpAlpha,tmpred,tmpgreen,tmpblue);
            //Army Settings
			m_ArmySize = m_pParent->GetSQLHelper()->getval("ArmySize");
			m_Face = m_pParent->GetSQLHelper()->getval("Face");

            //Walk SFX
			tmpMiscString= m_pParent->GetSQLHelper()->getval("WalkSFX");
			m_pParent->GetSoundManager()->LoadWalkSFX(tmpMiscString);

		//TODO From here on
		if (strcmp(tmpLineID,"MUSIC") == 0)
			inFile >> tmpMusicFileName;
			m_pParent->GetSoundManager()->PlayMusic(tmpMusicFileName);
		if (strcmp(tmpLineID,"SOUND") == 0)
			inFile >> tmpMiscString;
			m_pParent->GetSoundManager()->PlayAmbientSound(tmpMiscString);

		if (strcmp(tmpLineID,"SCRIPT") == 0)
			inFile >> tmpMiscString;
			l_LoadScriptFile(true, m_pParent->GetLuaVM(), tmpMiscString);

	}




	//Sprite Block
	if (strcmp(tmpLineID,"SB") == 0)
	{
		inFile >> tmpCount;
		tmpCount = 55/tmpCount;
		while (strcmp(tmpLineID,"EB") != 0)
		{
			//GUID GroupID Frame WorldX WorldY OffsetX OffsetY TileLength TileWidth StaticName
			if (strcmp(tmpLineID,"STATIC") == 0)
			{
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpFrame;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpoffX;
				inFile >> tmpoffY;
				inFile >> tmpLength;
				inFile >> tmpWidth;
				inFile >> tmpLayer;
				inFile >> tmpShadow;
				inFile >> tmpFileName;
				AddStatic(tmpoffX, tmpoffY, tmpX, tmpY, tmpLength, tmpWidth, tmpLayer, (bool)tmpShadow, tmpFrame ,tmpGroupID, tmpGUID, tmpFileName);
			}
			if (strcmp(tmpLineID,"NPC") == 0)
			{//NPC Facing GUID GroupID X Y FileName SpriteName
				inFile >> tmpDir;
				inFile >> tmpSide;
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpFileName;
				inFile >> tmpSpriteName;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpiSpellID;
				inFile >> tmpfPhy;
				inFile >> tmpfMag;
				inFile >> tmpiHps;
				inFile >> tmpiMin;
				inFile >> tmpiMax;
				inFile >> tmpiMove;

				switch (side)
				{
				case HOMETEAM:
					if (tmpSide == 0)
						AddNpc(tmpDir, HOMETEAM, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					break;
				case CHALLENGER:
					if (tmpSide == 1)
						AddNpc(tmpDir, CHALLENGER, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					break;
				case CUSTTEAM:
					if (m_pParent->GetGameType() != MPLAY)
					{
						if (tmpSide == 1)
							AddNpc(tmpDir, CHALLENGER, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					}
					break;
				case DEFAULTTEAM:
					if (m_pParent->GetGameType() != MPLAY)
					{
						if (tmpSide == 0)
							AddNpc(tmpDir, HOMETEAM, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
						if (tmpSide == 1)
							AddNpc(tmpDir, CHALLENGER, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					}
					break;
				default:
					AddNpc(tmpDir, tmpSide, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID,tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					break;
				}
			}
			if (strcmp(tmpLineID,"SPRITE") == 0)
			{//Sprite GUID GroupID X Y OffsetX OffsetY FileName SpriteName
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpoffX;
				inFile >> tmpoffY;
				inFile >> tmpFileName;
				inFile >> tmpSpriteName;
				AddSprite(tmpoffX, tmpoffY, tmpX, tmpY, tmpGroupID, tmpGUID, tmpFileName, tmpSpriteName);
			}
			if (strcmp(tmpLineID,"START") == 0)
			{//Start Facing GUID GroupID X Y

			}
			if (strcmp(tmpLineID,"LIGHT") == 0)
			{
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpoffX;
				inFile >> tmpoffY;
				inFile >> tmpWidth;
				inFile >> tmpHeight;
				inFile >> tmpAlpha;
				inFile >> tmpred;
				inFile >> tmpgreen;
				inFile >> tmpblue;
				inFile >> tmpCount;
				tmpColor = ARGB(tmpAlpha,tmpred,tmpgreen,tmpblue);
				AddLight(tmpoffX, tmpoffY, tmpX, tmpY, tmpWidth, tmpHeight, tmpColor ,tmpGroupID, tmpGUID, tmpCount);
			}
			if (strcmp(tmpLineID,"SOUND") == 0)
			{
				inFile >> tmpGUID;
				inFile >> tmpPan;
				inFile >> tmpRand;
				inFile >> tmpInter;
				inFile >> tmpLoop;
				inFile >> tmpFileName;
				AddSound(tmpPan,tmpInter,tmpRand,tmpLoop,tmpGUID,tmpFileName);
			}


			inFile >> tmpLineID;
			if (tmpStatus < 70)
			tmpStatus+= tmpCount;


			m_pParent->SetLoadStatus(tmpStatus);
		}

		m_pParent->SetLoadStatus(70);
		m_pParent->GetMainMap()->Load(inFile);
		m_pParent->SetLoadStatus(90);
		hge->System_Log("Load Complete");
	}
}

void cGameObjectManager::LoadMap(char *fileName, int side)
{
	int tmpX, tmpY, tmpJnk;
	int tmpoffX, tmpoffY, tmpDir;
	int tmpLength, tmpWidth, tmpHeight, tmpLayer;
	int tmpCount, tmpStatus, tmpShadow, tmpSide,tmpType;
	int tmpAlpha, tmpred, tmpgreen, tmpblue;
	int tmpPan, tmpRand, tmpInter, tmpLoop;
	float tmpfPhy, tmpfMag;
	int tmpiSpellID, tmpiHps, tmpiMin, tmpiMax, tmpiMove;
	DWORD tmpColor;
	int tmpshadow;
	char tmpGUID[20];
	char tmpGroupID[20];
	char tmpFileName[100];
	char tmpMusicFileName[100];
	char tmpSpriteName[20];
	char tmpFrame[20];
	char tmpLineID[100];
	char tmpMiscString[100];

	::ifstream inFile;
	tmpStatus = 15;
	m_NPCCount = 0;
	inFile.open(fileName);
	//hge->System_Log(fileName);
	//Resource Block
	inFile >> tmpLineID;
	if (strcmp(tmpLineID,"RB") == 0)
	{
		while (strcmp(tmpLineID,"EB") != 0)
		{
			//RefID FileName
			inFile >> tmpLineID;

		}
		inFile >> tmpLineID;
	}
	//Light Block
	if (strcmp(tmpLineID,"LB") == 0)
	{

		while (strcmp(tmpLineID,"EB") != 0)
		{
			//RefID FileName
			inFile >> tmpLineID;
			if (strcmp(tmpLineID,"AMBIENT") == 0)
			{
				inFile >> tmpred >> tmpgreen >> tmpblue;
				SetAmbientColor(tmpred, tmpgreen, tmpblue);
			}
			if (strcmp(tmpLineID,"SHADOW") == 0)
			{
				inFile >> tmpshadow;
				if (m_pParent->UseShadows()==0)
				{
					m_UseShadows = false;
				}else
				{
					if (tmpshadow == 0)
						m_UseShadows = false;
					else
						m_UseShadows = true;
				}
			}

			if (strcmp(tmpLineID,"CLOUD") == 0)
			{
				inFile >> tmpshadow;
				m_pParent->GetWeatherControl()->CreateClouds(tmpshadow);
			}

			if (strcmp(tmpLineID,"WIND") == 0)
			{
				inFile >> tmpshadow;
				m_pParent->GetWeatherControl()->SetWindDir((DIR)tmpshadow);
			}

			if (strcmp(tmpLineID,"SNOW") == 0)
			{
				inFile >> tmpshadow;
				m_pParent->GetWeatherControl()->CreateSnow(tmpshadow);
			}

			if (strcmp(tmpLineID,"FOG") == 0)
			{
				inFile >> tmpLayer >> tmpAlpha >> tmpred >> tmpgreen >> tmpblue >> tmpType;
				if (tmpLayer)
				{
					m_pParent->GetWeatherControl()->CreateFog(tmpAlpha,tmpred,tmpgreen,tmpblue,tmpType);
				}
				else
					m_pParent->GetWeatherControl()->CreateGroundFog(tmpAlpha,tmpred,tmpgreen,tmpblue,tmpType);
			}

			if (strcmp(tmpLineID,"WATER") == 0)
			{
				inFile >> tmpAlpha >> tmpred >> tmpgreen >> tmpblue;
				m_pParent->GetWeatherControl()->CreateWater(tmpAlpha,tmpred,tmpgreen,tmpblue);
			}


			//if (strcmp(tmpLineID,"WEATHER") == 0)
			//{
			//	inFile >> tmpshadow;
			//}
			if (strcmp(tmpLineID,"ARMYSIZE") == 0)
			{
				inFile >> m_ArmySize;
			}
			if (strcmp(tmpLineID,"FACE") == 0)
			{
				inFile >> m_Face;
			}
			if (strcmp(tmpLineID,"MUSIC") == 0)
			{
				inFile >> tmpMusicFileName;
				m_pParent->GetSoundManager()->PlayMusic(tmpMusicFileName);
			}
			if (strcmp(tmpLineID,"SOUND") == 0)
			{
				inFile >> tmpMiscString;
				m_pParent->GetSoundManager()->PlayAmbientSound(tmpMiscString);
			}
			if (strcmp(tmpLineID,"WALK") == 0)
			{
				inFile >> tmpMiscString;
				m_pParent->GetSoundManager()->LoadWalkSFX(tmpMiscString);
			}

			if (strcmp(tmpLineID,"SCRIPT") == 0)
			{//load map specific script file
				inFile >> tmpMiscString;
				l_LoadScriptFile(true, m_pParent->GetLuaVM(), tmpMiscString);
			}


		}
		//inFile >> tmpred >> tmpgreen >> tmpblue;
		//SetAmbientColor(tmpred, tmpgreen, tmpblue);
		//inFile >> tmpshadow;
		//if (m_UseShadows && (tmpshadow == 0))
		//	m_UseShadows = false;

		//inFile >> tmpLineID; //EB
		inFile >> tmpLineID; //SB
	}
	//Sprite Block
	if (strcmp(tmpLineID,"SB") == 0)
	{
		inFile >> tmpCount;
		tmpCount = 55/tmpCount;
		while (strcmp(tmpLineID,"EB") != 0)
		{
			//GUID GroupID Frame WorldX WorldY OffsetX OffsetY TileLength TileWidth StaticName
			if (strcmp(tmpLineID,"STATIC") == 0)
			{
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpFrame;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpoffX;
				inFile >> tmpoffY;
				inFile >> tmpLength;
				inFile >> tmpWidth;
				inFile >> tmpLayer;
				inFile >> tmpShadow;
				inFile >> tmpFileName;
				AddStatic(tmpoffX, tmpoffY, tmpX, tmpY, tmpLength, tmpWidth, tmpLayer, (bool)tmpShadow, tmpFrame ,tmpGroupID, tmpGUID, tmpFileName);
			}
			if (strcmp(tmpLineID,"NPC") == 0)
			{//NPC Facing GUID GroupID X Y FileName SpriteName
				inFile >> tmpDir;
				inFile >> tmpSide;
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpFileName;
				inFile >> tmpSpriteName;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpJnk;
				inFile >> tmpiSpellID;
				inFile >> tmpfPhy;
				inFile >> tmpfMag;
				inFile >> tmpiHps;
				inFile >> tmpiMin;
				inFile >> tmpiMax;
				inFile >> tmpiMove;

				switch (side)
				{
				case HOMETEAM:
					if (tmpSide == 0)
						AddNpc(tmpDir, HOMETEAM, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					break;
				case CHALLENGER:
					if (tmpSide == 1)
						AddNpc(tmpDir, CHALLENGER, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					break;
				case CUSTTEAM:
					if (m_pParent->GetGameType() != MPLAY)
					{
						if (tmpSide == 1)
							AddNpc(tmpDir, CHALLENGER, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					}
					break;
				case DEFAULTTEAM:
					if (m_pParent->GetGameType() != MPLAY)
					{
						if (tmpSide == 0)
							AddNpc(tmpDir, HOMETEAM, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
						if (tmpSide == 1)
							AddNpc(tmpDir, CHALLENGER, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID, tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					}
					break;
				default:
					AddNpc(tmpDir, tmpSide, 0, tmpX, tmpY, tmpiHps, tmpiMin, tmpiMax, tmpiMove, tmpiSpellID,tmpfPhy, tmpfMag, tmpGUID, tmpFileName, tmpSpriteName);
					break;
				}
			}
			if (strcmp(tmpLineID,"SPRITE") == 0)
			{//Sprite GUID GroupID X Y OffsetX OffsetY FileName SpriteName
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpoffX;
				inFile >> tmpoffY;
				inFile >> tmpFileName;
				inFile >> tmpSpriteName;
				AddSprite(tmpoffX, tmpoffY, tmpX, tmpY, tmpGroupID, tmpGUID, tmpFileName, tmpSpriteName);
			}
			if (strcmp(tmpLineID,"START") == 0)
			{//Start Facing GUID GroupID X Y

			}
			if (strcmp(tmpLineID,"LIGHT") == 0)
			{
				inFile >> tmpGUID;
				inFile >> tmpGroupID;
				inFile >> tmpX;
				inFile >> tmpY;
				inFile >> tmpoffX;
				inFile >> tmpoffY;
				inFile >> tmpWidth;
				inFile >> tmpHeight;
				inFile >> tmpAlpha;
				inFile >> tmpred;
				inFile >> tmpgreen;
				inFile >> tmpblue;
				inFile >> tmpCount;
				tmpColor = ARGB(tmpAlpha,tmpred,tmpgreen,tmpblue);
				AddLight(tmpoffX, tmpoffY, tmpX, tmpY, tmpWidth, tmpHeight, tmpColor ,tmpGroupID, tmpGUID, tmpCount);
			}
			if (strcmp(tmpLineID,"SOUND") == 0)
			{
				inFile >> tmpGUID;
				inFile >> tmpPan;
				inFile >> tmpRand;
				inFile >> tmpInter;
				inFile >> tmpLoop;
				inFile >> tmpFileName;
				AddSound(tmpPan,tmpInter,tmpRand,tmpLoop,tmpGUID,tmpFileName);
			}


			inFile >> tmpLineID;
			if (tmpStatus < 70)
			tmpStatus+= tmpCount;


			m_pParent->SetLoadStatus(tmpStatus);
		}

		m_pParent->SetLoadStatus(70);
		m_pParent->GetMainMap()->Load(inFile);
		m_pParent->SetLoadStatus(90);
		hge->System_Log("Load Complete");
	}
	inFile.close();
};

cCharacterObject* cGameObjectManager::GetSelectNPC(){ return m_SelectedNPC; };
int cGameObjectManager::GetArmySize() { return m_ArmySize; };
int cGameObjectManager::GetFacing() { return m_Face; };


cCharacterObject* cGameObjectManager::GetWeakestNPC(cCharacterObject* source)
{
	cCharacterObject* NPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if ((!NPCList[iloop]->IsDead()) && (strcmp(NPCList[iloop]->GetGUID(), source->GetGUID()) == 0))
		{
			if (NPCList[iloop]->GetSide() != source->GetSide())
			{
				if (NPC)
				{
					if (NPCList[iloop]->GetHitPoints() < NPC->GetHitPoints())
					{
						NPC = NPCList[iloop];
					}
				}
				else
				{
					NPC = NPCList[iloop];
				}
			}
		}
	}
	return NPC;
};

cCharacterObject* cGameObjectManager::GetStrongestNPC(cCharacterObject* source)
{
	cCharacterObject* NPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if ((!NPCList[iloop]->IsDead()) && (strcmp(NPCList[iloop]->GetGUID(), source->GetGUID()) == 0))
		{
			if (NPCList[iloop]->GetSide() != source->GetSide())
			{
				if (NPC)
				{
					if (NPCList[iloop]->GetHitPoints() > NPC->GetHitPoints())
					{
						NPC = NPCList[iloop];
					}
				}
				else
				{
					NPC = NPCList[iloop];
				}
			}
		}
	}
	return NPC;
};

cCharacterObject* cGameObjectManager::GetHighPhyNPC(cCharacterObject* source)
{
	cCharacterObject* NPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if ((!NPCList[iloop]->IsDead()) && (strcmp(NPCList[iloop]->GetGUID(), source->GetGUID()) == 0))
		{
			if (NPCList[iloop]->GetSide() != source->GetSide())
			{
				if (NPC)
				{
					if (NPCList[iloop]->GetPhyResist() > NPC->GetPhyResist())
					{
						NPC = NPCList[iloop];
					}
				}
				else
				{
					NPC = NPCList[iloop];
				}
			}
		}
	}
	return NPC;
};

cCharacterObject* cGameObjectManager::GetHighMagNPC(cCharacterObject* source)
{
	cCharacterObject* NPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if ((!NPCList[iloop]->IsDead()) && (strcmp(NPCList[iloop]->GetGUID(), source->GetGUID()) == 0))
		{
			if (NPCList[iloop]->GetSide() != source->GetSide())
			{
				if (NPC)
				{
					if (NPCList[iloop]->GetMagResist() > NPC->GetMagResist())
					{
						NPC = NPCList[iloop];
					}
				}
				else
				{
					NPC = NPCList[iloop];
				}
			}
		}
	}
	return NPC;
};

cCharacterObject* cGameObjectManager::GetLowPhyNPC(cCharacterObject* source)
{
	cCharacterObject* NPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if ((!NPCList[iloop]->IsDead()) && (strcmp(NPCList[iloop]->GetGUID(), source->GetGUID()) == 0))
		{
			if (NPCList[iloop]->GetSide() != source->GetSide())
			{
				if (NPC)
				{
					if (NPCList[iloop]->GetPhyResist() < NPC->GetPhyResist())
					{
						NPC = NPCList[iloop];
					}
				}
				else
				{
					NPC = NPCList[iloop];
				}
			}
		}
	}
	return NPC;
};

cCharacterObject* cGameObjectManager::GetLowMagNPC(cCharacterObject* source)
{
	cCharacterObject* NPC = NULL;
	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if ((!NPCList[iloop]->IsDead()) && (strcmp(NPCList[iloop]->GetGUID(), source->GetGUID()) == 0))
		{
			if (NPCList[iloop]->GetSide() != source->GetSide())
			{
				if (NPC)
				{
					if (NPCList[iloop]->GetMagResist() < NPC->GetMagResist())
					{
						NPC = NPCList[iloop];
					}
				}
				else
				{
					NPC = NPCList[iloop];
				}
			}
		}
	}
	return NPC;
};

cCharacterObject* cGameObjectManager::GetNearestNPC(cCharacterObject* source)
{
	cIsoAStar *m_AStar;
	cCharacterObject* NPC = NULL;
	int iStepCount = 0;

	m_AStar = new cIsoAStar();

    m_AStar->SetCollisionCall(ptCheckCollision, ptCheckCharCollision);

	for (unsigned int iloop=0;iloop<NPCList.size();iloop++)
	{
		if ((!NPCList[iloop]->IsDead()) && (strcmp(NPCList[iloop]->GetGUID(), source->GetGUID()) != 0))
		{
			if (NPCList[iloop]->GetSide() != source->GetSide())
			{
				m_AStar->FindPath(source->GetxPos(),source->GetyPos(),NPCList[iloop]->GetxPos(),NPCList[iloop]->GetyPos());
				if(m_AStar->GetPathStatus() == FOUND)
				{
					if (iStepCount>0)
					{
						if (m_AStar->GetPathLength() < iStepCount)
						{
							iStepCount = m_AStar->GetPathLength();
							NPC = NPCList[iloop];
						}
					}else
					{
						iStepCount = m_AStar->GetPathLength();
						NPC = NPCList[iloop];
					}
				}
			}
		}
	}
	delete m_AStar;
	return NPC;
};


WorldPoint cGameObjectManager::GetTileInRange(cCharacterObject* source, cCharacterObject* target)
{
	int iLoop;
	int jLoop;
	bool even;
	int iStepCount = 0;
	WorldPoint tmpWld;
	WorldPoint trgtPoint;
	cIsoAStar *m_AStar;
	cSpellObject* SourceSpell = NULL;
	TRGTTYPE TrgtType;
	TRGTSTART TrgtStart;

	if (!target) hge->System_Log("No Target");

	trgtPoint.x = 0;
	trgtPoint.y = 0;

	SourceSpell = this->GetSpell(source->GetCurrentSpellID());
	if (SourceSpell)
	{
		TrgtType = SourceSpell->GetTargetType();
		TrgtStart = SourceSpell->GetTargetStart();
	}
	//atleast start with a valid location
	trgtPoint.x = source->GetxPos();
	trgtPoint.y = source->GetyPos();
	m_AStar = new cIsoAStar();
	m_AStar->SetCollisionCall(ptCheckCollision, ptCheckCharCollision);


	if (SourceSpell && (TrgtStart == SOURCE))
	{
		even = (target->GetyPos() % 2 == 0);
		//work our way through each group to find the clossest
		for(jLoop=0;jLoop<SourceSpell->GetGroupCount();jLoop++)
		{
			//start with the further tile from the target for that group
			for(iLoop=SourceSpell->GetTileCount(jLoop+1)-1;iLoop>-1;iLoop--)
			{
				tmpWld = SourceSpell->GetTargetTile(even,jLoop+1,iLoop);
				tmpWld.x += target->GetxPos();
				tmpWld.y += target->GetyPos();

				//if its not blocked get its distance
				if ((!this->CheckCollision(tmpWld, true)) || ((source->GetxPos() == tmpWld.x) && (source->GetyPos() == tmpWld.y)))
				{
					if ((source->GetxPos() == tmpWld.x) && (source->GetyPos() == tmpWld.y))
					{//best location is right where I am
						iStepCount = 1;
						trgtPoint.x = tmpWld.x;
						trgtPoint.y = tmpWld.y;
					}else
					{  //get the distance to this one
						m_AStar->FindPath(source->GetxPos(),source->GetyPos(),tmpWld.x ,tmpWld.y);
						if(m_AStar->GetPathStatus() == FOUND)
						{
							if (iStepCount>0)
							{
								if (m_AStar->GetPathLength() < iStepCount)
								{
									iStepCount = m_AStar->GetPathLength();
									trgtPoint.x = tmpWld.x;
									trgtPoint.y = tmpWld.y;
								}
							}else
							{
								iStepCount = m_AStar->GetPathLength();
								trgtPoint.x = tmpWld.x;
								trgtPoint.y = tmpWld.y;
							}
						}
					}
					break;//get us out of this loop
				}
				//if it is blocked continue on and check the next tile in
			}
		}
	}
	delete m_AStar;
	return trgtPoint;
};

bool cGameObjectManager::CheckCollision(WorldPoint wrld, bool includeNPC)
{
	bool rtn = true;

	if (includeNPC)
	{
		if (!m_pParent->GetMainMap()->CheckCollision(wrld.x,wrld.y))
			{
				if (!m_pParent->GetMainMap()->CheckCharacterCollision(wrld.x,wrld.y))
					rtn = false;
			}
	}else
	{
		if (!m_pParent->GetMainMap()->CheckCollision(wrld.x,wrld.y))
			{
				rtn = false;
			}
	}

	return rtn;
};

void cGameObjectManager::MoveToAttackRange(cCharacterObject* source, cCharacterObject* target)
{
	WorldPoint trgtpoint;
	trgtpoint = GetTileInRange(source,target);
	if (trgtpoint.x == 0 && trgtpoint.y == 0)
	{
		source->SetStatus(NOPATH);
	}else
	{
		PathNPC(true,source->GetGUID(),trgtpoint.x, trgtpoint.y);
	}

};

void cGameObjectManager::IdleWander(cCharacterObject* source)
{
	WorldPoint trgtpoint;
	trgtpoint.x = source->GetxPos() + hge->Random_Int(-2,2);
	trgtpoint.y = source->GetyPos() + hge->Random_Int(-2,2);
	if (trgtpoint.x == 0 && trgtpoint.y == 0)
	{
		source->SetStatus(NOPATH);
	}else
	{
		PathNPC(true,source->GetGUID(),trgtpoint.x, trgtpoint.y);
	}

};

void cGameObjectManager::GainFocus()
{
	m_FocusLost = true;
};
