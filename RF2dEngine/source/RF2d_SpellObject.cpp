#include <fstream>
#include <vector>
#include "RF2d_SpellObject.h"
#include "RF2d_EffectObject.h"
#include "RF2d_Projectile.h"
#include "RF2d_Application.h"
#include "RF2d_Scripting.h"

cSpellObject::cSpellObject(const cSpellObject& p)
{
	m_Loading = true;
	m_ActionDone = false;
	m_Cast = NULL;
	m_Effect = NULL;
	m_Projectile = NULL;
	m_Hitsfx = NULL;
	m_Attacksfx = NULL;

	m_pParent = p.m_pParent;
	m_ScriptName = p.m_ScriptName;
	if (p.m_Cast)
		m_Cast = p.m_Cast;
	if (p.m_Effect)
		m_Effect = p.m_Effect;
	if (p.m_Projectile)
		m_Projectile = p.m_Projectile;
	if (p.m_TrgtFileName.length() > 0)
		LoadTarget(p.m_TrgtFileName,p.m_TrgtType,p.m_TrgtStart);
	if (p.m_Hitsfx)
		m_Hitsfx = p.m_Hitsfx;
	if (p.m_Attacksfx)
		m_Attacksfx = p.m_Attacksfx;

	m_GUID = p.m_GUID;
	m_ActionFrame = p.m_ActionFrame;
	m_Casting = false;
	m_Caster = p.m_Caster;
	m_Target = p.m_Target;
	m_CastMode = p.m_CastMode;
	m_TargetList = p.m_TargetList;
}


cSpellObject::cSpellObject(cApp *pParent, std::string filename)
{
	m_pParent = pParent;
	m_Cast = NULL;
	m_Effect = NULL;
	m_Projectile = NULL;
	m_Hitsfx = NULL;
	m_Attacksfx = NULL;
	//Set Some Defaults
	m_Loading = true;
	m_ActionDone = false;
	m_Casting = false;
	m_ScriptName = filename;
	m_CastMode = 0;
	m_Range = 0;
};


cSpellObject::~cSpellObject()
{
	//Dont delete resource points for copies.
	if (m_Cast) delete m_Cast;
	if (m_Effect) delete m_Effect;
	if (m_Projectile) delete m_Projectile;
	m_TargetList.clear();
	m_g1EvenList.clear();
	m_g1OddList.clear();
	m_g2EvenList.clear();
	m_g2OddList.clear();
	m_g3EvenList.clear();
	m_g3OddList.clear();
	m_g4EvenList.clear();
	m_g4OddList.clear();
};

void cSpellObject::SetSpellRange(int rng)
{
	m_Range	= rng;
};


/********Load Cast**********/
void cSpellObject::LoadSpellCast(std::string fileName)
{
	char BaseName[150];
	char tmpBaseName[150];
	char tmpFileName[150];
	int iFrames;
	float iFPS;
	float iWidth, iHeight;
	bool iOneFace;
	int iActionFrame;
	int iMode;
	float iHotx, iHoty;
	int iBlend;
	ifstream inFile;

	hge->System_Log("LoadSpellCast");

	strcpy(tmpFileName,m_pParent->GetGamePath(DATA));
	strcat(tmpFileName,fileName.c_str());

	inFile.open(tmpFileName, ::ios::in);
	if(inFile.bad()) exit(-1);

	//Construct the texture file name
	inFile >> BaseName;
	strcpy(tmpBaseName, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpBaseName, BaseName);
	//Load the animation data
	inFile >> iFrames >> iFPS >> iWidth >> iHeight >> iOneFace >> iActionFrame >> iMode >> iBlend >> iHotx >> iHoty;
	if (iFrames > 0) // Frames are assigned
	{
		m_CastMode = iMode;
		//hge->System_Log("Load:CastMode %i", m_CastMode);
		//Create the HGEAnimation object
		m_Cast = new AniStorage();
		m_Cast->Init(tmpBaseName, iFrames, iFPS, iWidth, iHeight, iOneFace, true);
		if (iBlend = 1) m_Cast->SetBlendMode(BLEND_ALPHAADD);
		m_Cast->SetMode(iMode);
		m_Cast->SetHotSpot(iHotx,iHoty);
	}
	inFile.close(); //Close the file handle

	//Which frame of the cast to act on
	m_ActionFrame = iActionFrame;
};

void cSpellObject::LoadTargetFile(std::string fileName, std::string ttype, std::string tstart)
{
	TRGTTYPE trgtype;
	TRGTSTART trgstrt;
	hge->System_Log("LoadTargetFile");

	if (tstart.compare("mouse")==0)
		trgstrt = MOUSE;
	else
		trgstrt = SOURCE;

	if (ttype.compare("single")==0)
		trgtype = SINGLE;
	else if (ttype.compare("group")==0)
		trgtype = GROUP;
	else
		trgtype = MULTI;

	LoadTarget(fileName,trgtype,trgstrt);

};

void cSpellObject::LoadTarget(std::string fileName, int trgtype, int trgstrt)
{
	ifstream inFile;
	char tmpFileName[150];
	int iLoop;
	int iCount;
	int x, y;
	m_TrgtFileName = fileName;

	strcpy(tmpFileName,m_pParent->GetGamePath(DATA));
	strcat(tmpFileName,fileName.c_str());

	inFile.open(tmpFileName, ::ios::in);
	if(inFile.bad()) exit(-1);

	//Get Targeting Info
	inFile >> iCount;
	m_TrgtType = (TRGTTYPE)trgtype;
	m_TrgtStart = (TRGTSTART)trgstrt;

	if (m_TrgtType != SINGLE)
	{
		//Get the even tiles
		for (iLoop=0;iLoop<iCount;iLoop++)
		{
			inFile >> x >> y;
			AddEvenList(1,x,y);
			x=0;y=0;
		}

		inFile >> iCount;
		//Get the odd tiles
		for (iLoop=0;iLoop<iCount;iLoop++)
		{
			inFile >> x >> y;
			AddOddList(1, x,y);
			x=0;y=0;
		}
	}
	if (m_TrgtType == GROUP)
	{
		for (int jLoop=2;jLoop<5;jLoop++)
		{
			inFile >> iCount;
			//Get the even tiles
			for (iLoop=0;iLoop<iCount;iLoop++)
			{
				inFile >> x >> y;
				AddEvenList(jLoop,x,y);
				x=0;y=0;
			}

			inFile >> iCount;
			//Get the odd tiles
			for (iLoop=0;iLoop<iCount;iLoop++)
			{
				inFile >> x >> y;
				AddOddList(jLoop, x,y);
				x=0;y=0;
			}
		}
	}
	inFile.close(); //Close the file handle
};

void cSpellObject::CreateEffect()
{
	hge->System_Log("CreateEffect");
	if (!m_Effect)
	m_Effect = new cEffectObject(m_pParent);
	l_LoadEffect(this, m_Effect);
};

void cSpellObject::CreateProjectile()
{
	hge->System_Log("CreateProjectile");
	if (!m_Projectile)
	m_Projectile = new cProjectileObject(m_pParent);
	l_LoadProjectile(this, m_Projectile);
};

void cSpellObject::AutoPrepareLaunch()
{
	if (m_Projectile)
		m_Projectile->PrepareLaunch(this);
};


void cSpellObject::SetDirection(ISODIR dir)
{
	m_CurDirection = dir;
	if (m_Cast) m_Cast->SetDirection(dir);
};

void cSpellObject::SwitchDirection(ISODIR dir)
{
	m_LastDirection = m_CurDirection;
	m_CurDirection = dir;
	if (m_Cast) m_Cast->SetDirection(dir);
};


void cSpellObject::SetCastMode(int mode)
{
	if (m_Cast) m_Cast->SetMode(mode);
};

void cSpellObject::PlayCast()
{
	if (m_Cast) m_Cast->Play();
};


void cSpellObject::SimpleCast()
{
	hge->System_Log("SimpleCast");
		if (m_Target)
			m_Caster->FaceTarget(m_Target);
		else
			m_Caster->FaceTarget(m_TargetList[0]);
		SwitchDirection(m_Caster->GetDirection());
		if (m_Caster)
		{
			m_Caster->SetAttackAniMode(m_CastMode);
			m_Caster->DoAction(ATTACK);
		}
		if (m_Cast)
		{
			m_Cast->SetMode(m_CastMode);
			m_Cast->Play();
		}
};

void cSpellObject::SimpleDamage(cCharacterObject *target, int physical, int magical)
{
	hge->System_Log("SimpleDamage");
	int tmpDamage = 0;
	if (target)
	{
		tmpDamage = target->TakeDamage(physical, magical);
		if (tmpDamage == -1)
			target->DoAction(DEATH);
		else
			target->DoAction(PAIN);
	}
};

void cSpellObject::SimpleHit()
{
	hge->System_Log("SimpleHit");
	Hit();
}

void cSpellObject::LoadAttacksfx(std::string filename)
{
	m_Attacksfx = m_pParent->GetSoundManager()->LoadSoundFX(filename,false);
};

void cSpellObject::LoadHitsfx(std::string filename)
{
	m_Hitsfx = m_pParent->GetSoundManager()->LoadSoundFX(filename,false);
};

void cSpellObject::PlayAttacksfx()
{
	if (m_Attacksfx)
		m_pParent->GetSoundManager()->PlaySoundFX(m_Attacksfx);
};

void cSpellObject::PlayHitsfx()
{
	if (m_Hitsfx)
		m_pParent->GetSoundManager()->PlaySoundFX(m_Hitsfx);

};


void cSpellObject::Cast()
{
	if ((m_Cast) && m_Caster->GetCurrentAction() != ATTACK)
	{// Make sure the caster isnt already casting
		m_Casting = true;
  		m_Loading = false;
		m_ActionDone = false;
		l_BeginCast(this);
	}
};


void cSpellObject::Cast(cCharacterObject *caster, cCharacterObject *target)
{
	AssignCaster(caster);
	AssignTarget(target);
	Cast();
};

void cSpellObject::Cast(cCharacterObject *caster, TargetList targetlist)
{
	AssignCaster(caster);
	if (m_TrgtType != MULTI)
		m_Target = targetlist[0];
	else
	{
		m_Target = NULL;
		m_TargetList = targetlist;
	}
	Cast();
};


bool cSpellObject::GetGroupActive(int group)
{
	bool rtn = false;
	switch (group)
	{
	case 1:
		rtn = (m_g1EvenList.size() > 0);
		break;
	case 2:
		rtn = (m_g2EvenList.size() > 0);
		break;
	case 3:
		rtn = (m_g3EvenList.size() > 0);
		break;
	case 4:
		rtn = (m_g4EvenList.size() > 0);
		break;
	}
	return rtn;
};

int cSpellObject::GetTileCount(int group)
{
	int rtn = 0;
	switch (group)
	{
	case 1:
		rtn = m_g1EvenList.size();
		break;
	case 2:
		rtn = m_g2EvenList.size();
		break;
	case 3:
		rtn = m_g3EvenList.size();
		break;
	case 4:
		rtn = m_g4EvenList.size();
		break;
	}
	return rtn;
};

int cSpellObject::GetGroupCount()
{
	int rtn = 0;


	if (m_TrgtType == GROUP)
	{
		if (m_g1EvenList.size() > 0) rtn++;
		if (m_g2EvenList.size() > 0) rtn++;
		if (m_g3EvenList.size() > 0) rtn++;
		if (m_g4EvenList.size() > 0) rtn++;
	}
	else
	{
		rtn = 1;
	}

	return rtn;
};

WorldPoint cSpellObject::GetTargetTile(bool even, int group, unsigned int it)
{
	WorldPoint rtn;
	switch (group)
	{
	case 1:
		if (it < m_g1EvenList.size())
		{
			if (even)
				rtn = m_g1EvenList[it];
			else
				rtn = m_g1OddList[it];
		}
		break;
	case 2:
		if (it < m_g2EvenList.size())
		{
			if (even)
				rtn = m_g2EvenList[it];
			else
				rtn = m_g2OddList[it];
		}
	break;
	case 3:
		if (it < m_g3EvenList.size())
		{
			if (even)
				rtn = m_g3EvenList[it];
			else
				rtn = m_g3OddList[it];
		}
		break;
	case 4:
		if (it < m_g4EvenList.size())
		{
			if (even)
				rtn = m_g4EvenList[it];
			else
				rtn = m_g4OddList[it];
		}
		break;
	}
	return rtn;
};

int cSpellObject::GetTargetGroup(bool even, int x, int y)
{
	int rtn = 0;
	unsigned int iLoop;


if (even)
	{
		for (iLoop=0;iLoop<m_g1EvenList.size();iLoop++)
		{
			if (m_g1EvenList[iLoop].x == x)
				if (m_g1EvenList[iLoop].y == y)
				{
					rtn = 1;
					break;
				}
			if (m_g2EvenList.size() > 0)
			{
				if (m_g2EvenList[iLoop].x == x)
					if (m_g2EvenList[iLoop].y == y)
					{
						rtn = 2;
						break;
					}
				}
			if (m_g3EvenList.size() > 0)
			{
				if (m_g3EvenList[iLoop].x == x)
					if (m_g3EvenList[iLoop].y == y)
					{
						rtn = 3;
						break;
					}
				}
			if (m_g4EvenList.size() > 0)
			{
				if (m_g4EvenList[iLoop].x == x)
					if (m_g4EvenList[iLoop].y == y)
					{
						rtn = 4;
						break;
					}
			}
		}
	}
	else
	{
		for (iLoop=0;iLoop<m_g1OddList.size();iLoop++)
		{
			if (m_g1OddList[iLoop].x == x)
				if (m_g1OddList[iLoop].y == y)
				{
					rtn = 1;
					break;
				}
			if (m_g2OddList.size() > 0)
			{
				if (m_g2OddList[iLoop].x == x)
					if (m_g2OddList[iLoop].y == y)
					{
						rtn = 2;
						break;
					}
			}
			if (m_g3OddList.size() > 0)
			{
				if (m_g3OddList[iLoop].x == x)
					if (m_g3OddList[iLoop].y == y)
					{
						rtn = 3;
						break;
					}
			}
			if (m_g4OddList.size() > 0)
			{
				if (m_g4OddList[iLoop].x == x)
					if (m_g4OddList[iLoop].y == y)
					{
						rtn = 4;
						break;
					}
			}
		}
	}

	return rtn;
}

bool cSpellObject::TargetHitTest(bool even, int x, int y)
{
	bool brtn = false;
	unsigned int iLoop;

	if (even)
	{
		for (iLoop=0;iLoop<m_g1EvenList.size();iLoop++)
		{
			if (m_g1EvenList[iLoop].x == x)
				if (m_g1EvenList[iLoop].y == y)
				{
					brtn = true;
					break;
				}
			if (m_g2EvenList.size() > 0)
			{
				if (m_g2EvenList[iLoop].x == x)
					if (m_g2EvenList[iLoop].y == y)
					{
						brtn = true;
						break;
					}
				}
			if (m_g3EvenList.size() > 0)
			{
				if (m_g3EvenList[iLoop].x == x)
					if (m_g3EvenList[iLoop].y == y)
					{
						brtn = true;
						break;
					}
				}
			if (m_g4EvenList.size() > 0)
			{
				if (m_g4EvenList[iLoop].x == x)
					if (m_g4EvenList[iLoop].y == y)
					{
						brtn = true;
						break;
					}
			}
		}
	}
	else
	{
		for (iLoop=0;iLoop<m_g1OddList.size();iLoop++)
		{
			if (m_g1OddList[iLoop].x == x)
				if (m_g1OddList[iLoop].y == y)
				{
					brtn = true;
					break;
				}
			if (m_g2OddList.size() > 0)
			{
				if (m_g2OddList[iLoop].x == x)
					if (m_g2OddList[iLoop].y == y)
					{
						brtn = true;
						break;
					}
			}
			if (m_g3OddList.size() > 0)
			{
				if (m_g3OddList[iLoop].x == x)
					if (m_g3OddList[iLoop].y == y)
					{
						brtn = true;
						break;
					}
			}
			if (m_g4OddList.size() > 0)
			{
				if (m_g4OddList[iLoop].x == x)
					if (m_g4OddList[iLoop].y == y)
					{
						brtn = true;
						break;
					}
			}
		}
	}

	return brtn;
};


void cSpellObject::Render(float fDeltaTime, float camX, float camY)
{
	float ScreenX;
	float ScreenY;

	if (m_Caster->IsOnScreen())
	{
		//Figure out the exact world cord
		int WorldX = m_Caster->GetxPos() * TILEWIDTH;
		int WorldY = m_Caster->GetyPos() * TILEHALFHEIGHT;

		//if ((WorldX < camX) || (WorldX > (camX + 800 + 32)) ||
		//	(WorldY  < camY) || (WorldY  > (camY + 600 + 128)))
		//{
		//	if (!m_ActionDone) //Allow characters to cast spells off screen;
		//	{
		//		m_ActionDone = true;
		//		if (m_Travel)
		//			Launch(); //if this spell has a projectile the fire it
		//		else //Otherwise Begin the recieving animation
		//			AssignEffect();
		//	}
		//	return;
		//}


		//translate world X,Y to Screen X,Y
		if (m_Caster->GetyPos() % 2 == 0)
			ScreenX =  ((WorldX) - camX)-TILEHALFWIDTH;
		else
			ScreenX =  ((WorldX) - camX);

		ScreenY = ((WorldY) - camY);
		//Translate world X,Y to screen X,Y
		//float ScreenX =  (WorldX - camX)-TILEWIDTH;
		//float ScreenY = (WorldY - camY)-TILEHALFHEIGHT;


		if (m_Casting && (m_Cast)) //Play the cast animation
		{
			if (m_Cast->GetFrame() == m_ActionFrame && !m_ActionDone)
			{
				m_ActionDone = true;
				l_SpellAction(this);
			}
			if (m_Caster) m_Cast->Render(fDeltaTime, ScreenX, ScreenY);
			if (!m_Cast->IsPlaying())
			{
				hge->System_Log("l_EndCast");
				l_EndCast(this);
				m_Casting = false;
			}
		}
	}
};


void cSpellObject::AssignCaster(cCharacterObject *caster)
{
	m_Caster = caster;
	SwitchDirection(m_Caster->GetDirection());
};
void cSpellObject::AssignTarget(cCharacterObject *target)
{
	m_Target = target;
};

void cSpellObject::AddEvenList(int group, int x, int y)
{
	WorldPoint tmp;
	tmp.x = x;
	tmp.y = y;
	switch(group)
	{
	case 1:
		m_g1EvenList.push_back(tmp);
		break;
	case 2:
		m_g2EvenList.push_back(tmp);
		break;
	case 3:
		m_g3EvenList.push_back(tmp);
		break;
	case 4:
		m_g4EvenList.push_back(tmp);
		break;
	}
};

void cSpellObject::AddOddList(int group, int x, int y)
{
	WorldPoint tmp;
	tmp.x = x;
	tmp.y = y;
	switch(group)
	{
	case 1:
		m_g1OddList.push_back(tmp);
		break;
	case 2:
		m_g2OddList.push_back(tmp);
		break;
	case 3:
		m_g3OddList.push_back(tmp);
		break;
	case 4:
		m_g4OddList.push_back(tmp);
		break;
	}

};

void cSpellObject::AssignEffect()
{
	if (m_Effect && m_Target)
	{
		m_Target->AddEffect(m_Effect);
	}
	else
	if (m_Effect && m_TargetList.size() > 0)
	{
		for (unsigned int iLoop=0;iLoop<m_TargetList.size();iLoop++)
		{
			m_TargetList[iLoop]->AddEffect(m_Effect);
		}
	}
	//m_Effect->DeActivate();
};

void cSpellObject::LaunchProjectile()
{
	hge->System_Log("script launch");
	if (m_Projectile)
		m_pParent->GetGameObjMangr()->LaunchProjectile(m_Projectile);
};


void cSpellObject::Hit()
{
	hge->System_Log("SpellHit");
	if (m_Target)
	{
		l_SpellHit(this, m_Target);
	}
	else
	if (m_TargetList.size() > 0)
	{
		for (unsigned int iLoop=0;iLoop<m_TargetList.size();iLoop++)
		{
			l_SpellHit(this, m_TargetList[iLoop]);
		}
	}

};

void cSpellObject::AssignId(int idNo) { m_GUID = idNo; };
void cSpellObject::SetPosition(int x, int y){ m_PosX = x; m_PosY = y; };

int cSpellObject::xPos() { return m_PosX; }
int cSpellObject::yPos() { return m_PosY; }
int cSpellObject::Width() { return 128; }
int cSpellObject::Height() { return 128; }

int cSpellObject::Id() { return m_GUID; };
bool cSpellObject::Finished()
{
	bool finished = false;

	if (!m_Casting && !m_Loading)
	{
		finished = true;


	}
	if (finished)
	{
		if (m_Projectile)
			finished = m_Projectile->Finished();
	}
	if (finished)
	{
		if (m_Effect)
			finished = m_Effect->Finished();
	}

	if (finished)
	hge->System_Log("Finished");

	return finished;
};

int cSpellObject::CastMode(){return m_CastMode;};
ISODIR cSpellObject::Direction() { return m_CurDirection; };
TRGTTYPE cSpellObject::GetTargetType(){ return m_TrgtType; };
TRGTSTART cSpellObject::GetTargetStart(){ return m_TrgtStart; };
int cSpellObject::GetRange(){ return m_Range; };
AniStorage* cSpellObject::GetCastAni(){ return m_Cast; };
cCharacterObject* cSpellObject::GetCaster(){ return m_Caster; };
cCharacterObject* cSpellObject::GetTarget(){ return m_Target; };
std::string  cSpellObject::GetScriptName(){ return m_ScriptName; };
