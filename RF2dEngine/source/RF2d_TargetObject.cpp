#include "RF2d_TargetObject.h"
#include "RF2d_Application.h"
#include "AStar.h"
#include "RF2d_CallBackHeader.h"
/*
render and control attact targeting system.
*/


cTargetObject::cTargetObject(cApp* pParent)
{
	char tmpFilePath[256];
	m_TrgtMode = TMNONE;
	m_ValidTarget = false;
	m_pParent = pParent;
	m_SourceNPC = NULL;
	m_TargetNPC = NULL;
	m_TargetWldPos.clear();
	m_TargetScrnPos.clear();
	m_SourceWldPos.clear();
	m_SourceScrnPos.clear();

	//Load Particle Texture
	strcpy(tmpFilePath, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpFilePath, "\\gui\\targettiles.png");
	m_TrgtTex=hge->Texture_Load(tmpFilePath);

	m_RedSprite=new hgeSprite(m_TrgtTex, 0, 0, 64, 32);
	m_RedSprite->SetHotSpot(0,0);

	m_YellowSprite=new hgeSprite(m_TrgtTex, 64, 0, 64, 32);
	m_YellowSprite->SetHotSpot(0,0);

	m_BlueSprite=new hgeSprite(m_TrgtTex, 128, 0, 64, 32);
	m_BlueSprite->SetHotSpot(0,0);

	m_GreySprite=new hgeSprite(m_TrgtTex, 192, 0, 64, 32);
	m_GreySprite->SetHotSpot(0,0);
};

cTargetObject::~cTargetObject()
{
	if (m_TrgtSprite) m_RedSprite = NULL;
	if (m_RedSprite) delete m_RedSprite;
	if (m_YellowSprite) delete m_YellowSprite;
	if (m_BlueSprite) delete m_BlueSprite;
	if (m_GreySprite) delete m_GreySprite;

	if (m_TrgtTex) hge->Texture_Free(m_TrgtTex);
};

hgeSprite* cTargetObject::GetRedSprite(){ return m_RedSprite; };
hgeSprite* cTargetObject::GetYellowSprite(){ return m_YellowSprite; };
hgeSprite* cTargetObject::GetBlueSprite(){ return m_BlueSprite; };
hgeSprite* cTargetObject::GetGreySprite(){ return m_GreySprite; };

void cTargetObject::TargetType(TRGTTYPE tp)
{
	m_TrgtType = tp;
};

TRGTTYPE cTargetObject::TargetType()
{
	return m_TrgtType;
};

void cTargetObject::SetSource(int x, int y)
{
	SetSource(m_pParent->GetGameObjMangr()->GetNPCAt(x,y));
};


void cTargetObject::SetSource(cCharacterObject* source)
{
	if (source)
	{
		m_SourceNPC = source;
		m_SourceWldPos.x  = m_SourceNPC->GetxPos();
		m_SourceWldPos.y = m_SourceNPC->GetyPos();

		m_AttackType = m_SourceNPC->GetAttackType();
		m_SourceScrnPos = GetScrnPos(m_SourceWldPos);
		m_TrgtMode = TMNONE;
		switch (m_AttackType)
		{
		case MELEE:
			{
				m_TrgtType = SINGLE;
				m_TrgtStart = SOURCE;
				m_SourceSpell = NULL;
				break;
			}
		case SPELL:
			{
				m_SourceSpell = m_pParent->GetGameObjMangr()->GetSpell(m_SourceNPC->GetCurrentSpellID());
				if (m_SourceSpell)
				{
					m_TrgtType = m_SourceSpell->GetTargetType();
					m_TrgtStart = m_SourceSpell->GetTargetStart();
				}
				break;
			}
		case RANGED:
			break;
		}
	}
	else
		Reset();
};

void cTargetObject::SetMode(TRGTMODE mode)
{
	if (m_SourceNPC)
	{
		m_TrgtMode = mode;
		BuildHighlightList(m_SourceNPC->GetWalkDistance(),m_SourceWldPos,TMWALK);
		switch (m_AttackType)
		{
		case MELEE:
			{
				BuildHighlightList(2,m_SourceWldPos,TMATTACK);
				break;
			}
		case SPELL:
			{
				if (m_SourceSpell)
				{
					if (m_TrgtType == GROUP)
						BuildGroupHighlightList(m_SourceWldPos,TMATTACK);
					else
						BuildHighlightList(m_SourceSpell->GetRange(),m_SourceWldPos,TMATTACK);
				}
				break;
			}
		case RANGED:
			break;
		}
	}
	else
		m_TrgtMode = TMNONE;
};

bool cTargetObject::isShown() { return m_TrgtMode != TMNONE; };

void cTargetObject::SetTarget(int x, int y)
{
	//TODO--Allow for Character Target and Ground Target
	if (!m_TargetNPC) m_TargetNPC = m_pParent->GetGameObjMangr()->GetNPCAt(x,y);
	m_TargetList.push_back(m_pParent->GetGameObjMangr()->GetNPCAt(x,y));
	m_TargetWldPos.x = x;
	m_TargetWldPos.y = y;
};

void cTargetObject::SetTarget(cCharacterObject* target)
{
	if (!m_TargetNPC) m_TargetNPC = target;
	m_TargetList.push_back(target);
};

void cTargetObject::Reset()
{
	m_TargetList.clear();
	m_HighlightWlkList.clear();
	m_HighlightAttkList.clear();
	m_SourceNPC = NULL;
	m_TargetNPC = NULL;
	m_SourceSpell = NULL;
	m_TargetWldPos.clear();
	m_TargetScrnPos.clear();
	m_SourceWldPos.clear();
	m_SourceScrnPos.clear();
	m_TrgtMode = TMNONE;
};

void cTargetObject::Cancel()
{
	Reset();
	m_pParent->GetBattleControl()->UserCancel();
};

ScreenPoint cTargetObject::GetMouseTileScrnPos()
{
	ScreenPoint rtn;
	int StartX=0;
	int StartY=0;

	if (m_pParent->GetCursor()->WorldY() % 2 == 0) StartX = 0; // Even Line //
	else   StartX =  TILEHALFWIDTH; // Odd Line  //

	rtn.x = float((StartX + (m_pParent->GetCursor()->WorldX() * TILEWIDTH) - m_pParent->GetMainMap()->ScreenX()) -TILEWIDTH);
	rtn.y = float((StartY + (m_pParent->GetCursor()->WorldY() * TILEHALFHEIGHT) - m_pParent->GetMainMap()->ScreenY()) -TILEHALFHEIGHT);

	return rtn;
};

ScreenPoint cTargetObject::GetScrnPos(WorldPoint wrld)
{
	return GetScrnPos(wrld.x, wrld.y);
};

ScreenPoint cTargetObject::GetScrnPos(int wrldx, int wrldy)
{
	int StartX=0;
	int StartY=0;
	ScreenPoint rtn;

	if (wrldy % 2 == 0) StartX = 0; // Even Line //
	else   StartX =  TILEHALFWIDTH; // Odd Line  //

	rtn.x = float((StartX + (wrldx * TILEWIDTH) - m_pParent->GetMainMap()->ScreenX()) -TILEWIDTH);
	rtn.y = float((StartY + (wrldy * TILEHALFHEIGHT) - m_pParent->GetMainMap()->ScreenY()) -TILEHALFHEIGHT);

	return rtn;
};

WorldPoint cTargetObject::GetOffSetTileWrldPos(int scrnx, int scrny)
{
	WorldPoint rtn;
	float ScreenX = m_pParent->GetMainMap()->ScreenX() + scrnx;
	float ScreenY = m_pParent->GetMainMap()->ScreenY() + scrny;
	rtn.y = ScreenY/TILEHALFHEIGHT;

	if (rtn.y % 2 == 0)
		// Even Line //
		rtn.x = ((ScreenX+TILEHALFWIDTH)/TILEWIDTH);
	else
		// Odd Line  //
		rtn.x = ((ScreenX)/TILEWIDTH);

	return rtn;
};

float cTargetObject::GetOffSetTilePosScrnX(float start, int count)
{
	float x = start;
	for (int iLoop=0;iLoop<count;iLoop++)
	{
		x +=  TILEHALFWIDTH;
	}
	return x;
};

float cTargetObject::GetOffSetTileNegScrnX(float start, int count)
{
	float x = start;
	for (int iLoop=0;iLoop<count;iLoop++)
	{
		x -=  TILEHALFWIDTH;
	}
	return x;
};

float cTargetObject::GetOffSetTilePosScrnY(float start, int count)
{
	float y = start;
	for (int iLoop=0;iLoop<count;iLoop++)
	{
		y +=  TILEHALFHEIGHT;
	}
	return y;
};

float cTargetObject::GetOffSetTileNegScrnY(float start, int count)
{
	float y = start;
	for (int iLoop=0;iLoop<count;iLoop++)
	{
		y -=  TILEHALFHEIGHT;
	}
	return y;

};

void cTargetObject::AddTargetToList(WorldPoint wldtrgt, WorldPoint offset)
{
	WorldPoint tmp;
	tmp.x = wldtrgt.x + offset.x;
	tmp.y = wldtrgt.y + offset.y;
	AddTargetToList(tmp.x,tmp.y);
};

void cTargetObject::AddTargetToList(int x, int y)
{
	cCharacterObject* tmpNPC = NULL;
	bool bInList = false;
	tmpNPC = m_pParent->GetGameObjMangr()->GetNPCAt(x,y);

	if (tmpNPC != NULL)
	{

		for (unsigned int iloop=0;iloop<m_TargetList.size();iloop++)
		{
			if (m_TargetList[iloop]->GetId() == tmpNPC->GetId())
			{
				bInList = true;
				break;
			}
		}
		if (!bInList)
			m_TargetList.push_back(tmpNPC);
	}

};


void cTargetObject::FindTargetsInSpellArea()
{
	WorldPoint mouse;
	int	iLoop;
	mouse.y = m_pParent->GetCursor()->WorldY();
	mouse.x = m_pParent->GetCursor()->WorldX();

	if (m_SourceSpell)
	{
		if (mouse.y % 2 == 0)
		{
			for(iLoop=0;iLoop<m_SourceSpell->GetTileCount(1);iLoop++)
			{
				AddTargetToList(mouse,m_SourceSpell->GetTargetTile(true,1,iLoop));
			}
		}
		else
		{
			for(iLoop=0;iLoop<m_SourceSpell->GetTileCount(1);iLoop++)
			{
				AddTargetToList(mouse,m_SourceSpell->GetTargetTile(false,1,iLoop));
			}
		}
	}

};

bool cTargetObject::MouseInHighLightList(TRGTMODE mode)
{
	WorldPoint mouse;
	bool rtn = false;
	mouse = m_pParent->GetCursor()->GetWorldPoint();

	switch (mode)
	{
	case TMWALK:
		if( m_HighlightWlkList.size() > 0)
		for (unsigned int iLoop=0;iLoop<m_HighlightWlkList.size();iLoop++)
		{
			if (m_HighlightWlkList[iLoop].x == mouse.x)
				if (m_HighlightWlkList[iLoop].y == mouse.y)
				{
					rtn = true;
					break;
				}
		}
		break;
	case TMATTACK:
		if( m_HighlightAttkList.size() > 0)
		for (unsigned int iLoop=0;iLoop<m_HighlightAttkList.size();iLoop++)
		{
			if (m_HighlightAttkList[iLoop].x == mouse.x)
				if (m_HighlightAttkList[iLoop].y == mouse.y)
				{
					rtn = true;
					break;
				}
		}
		break;
	}
	return rtn;

};

void cTargetObject::GetTargets()
{
	WorldPoint wldstart;
	WorldPoint trgtpoint;
	bool even;
	wldstart = m_pParent->GetCursor()->GetWorldPoint();

	m_TargetList.clear();
	even = (wldstart.y % 2 == 0);

	switch (m_TrgtType)
	{
	case SINGLE:
		AddTargetToList(wldstart.x,wldstart.y);
		break;
	case MULTI:
		{
			AddTargetToList(wldstart.x,wldstart.y);
			if (m_SourceSpell)
			for(int iLoop=0;iLoop<m_SourceSpell->GetTileCount(1);iLoop++)
			{
				trgtpoint = m_SourceSpell->GetTargetTile(even,1,iLoop);
				AddTargetToList(wldstart.x + trgtpoint.x, wldstart.y + trgtpoint.y);
			}
			break;
		}
	case GROUP:
		{
			if (m_SourceSpell)
			{
				even = (m_SourceWldPos.y % 2 == 0);
				int group = m_SourceSpell->GetTargetGroup(even, wldstart.x - m_SourceWldPos.x, wldstart.y - m_SourceWldPos.y);
				for(int iLoop=0;iLoop<m_SourceSpell->GetTileCount(group);iLoop++)
				{
					trgtpoint = m_SourceSpell->GetTargetTile(even,group,iLoop);
					AddTargetToList(m_SourceWldPos.x + trgtpoint.x, m_SourceWldPos.y + trgtpoint.y);
				}
			}
			break;
		}
	};
};


bool cTargetObject::CheckCollision(WorldPoint wrld, TRGTMODE mode)
{
	bool rtn = true;
	switch (mode)
	{
	case TMWALK:
		{
			if (!m_pParent->GetMainMap()->CheckCollision(wrld.x,wrld.y))
			{
				if (!m_pParent->GetMainMap()->CheckCharacterCollision(wrld.x,wrld.y))
					rtn = false;
			}
			break;
		}
	case TMATTACK:
		{
			if (!m_pParent->GetMainMap()->CheckCollision(wrld.x,wrld.y))
			{
				rtn = false;
			}
			break;
		}
	}
	return rtn;
};

void cTargetObject::BuildHighlightList(int count, WorldPoint center, TRGTMODE mode)
{
	int iLoop;
	int jLoop;
	WorldPoint tmpWld;
	cIsoAStar *m_AStar;

	m_AStar = new cIsoAStar();
	m_AStar->SetCollisionCall(ptCheckCollision, ptCheckCharCollision);
	switch (mode)
	{
	case TMWALK:
		m_HighlightWlkList.clear();
		break;
	case TMATTACK:
		m_HighlightAttkList.clear();
		break;
	}

	for (iLoop=0;iLoop<count;iLoop++)
	{
		for (jLoop=0;jLoop<count;jLoop++)
		{
			//++
			tmpWld.x = center.x + iLoop;
			tmpWld.y = center.y + jLoop;
			if (iLoop+jLoop > 0)
			{
				if (!CheckCollision(tmpWld, mode))
				{ //open tile
					//check path count
					m_AStar->FindPath(center.x,center.y,tmpWld.x,tmpWld.y);
					if(m_AStar->GetPathStatus() == FOUND)
					{
						if (m_AStar->GetPathLength() < count)
						{
							switch (mode)
							{
							case TMWALK:
								m_HighlightWlkList.push_back(tmpWld);
								break;
							case TMATTACK:
								m_HighlightAttkList.push_back(tmpWld);
								break;
							}
						}
					}
				}
			}
			//--
			tmpWld.x = center.x - iLoop;
			tmpWld.y = center.y - jLoop;
			if (iLoop + jLoop > 0)
			{
				if (!CheckCollision(tmpWld, mode))
				{ //open tile
					//check path count
					m_AStar->FindPath(center.x,center.y,tmpWld.x,tmpWld.y);
					if(m_AStar->GetPathStatus() == FOUND)
					{
						if (m_AStar->GetPathLength() < count)
						{
							switch (mode)
							{
							case TMWALK:
								m_HighlightWlkList.push_back(tmpWld);
								break;
							case TMATTACK:
								m_HighlightAttkList.push_back(tmpWld);
								break;
							}
						}
					}
				}
			}
			//+-
			tmpWld.x = center.x + iLoop;
			tmpWld.y = center.y - jLoop;
			if (iLoop + jLoop > 0)
			{
				if (!CheckCollision(tmpWld, mode))
				{ //open tile
					//check path count
					m_AStar->FindPath(center.x,center.y,tmpWld.x,tmpWld.y);
					if(m_AStar->GetPathStatus() == FOUND)
					{
						if (m_AStar->GetPathLength() < count)
						{
							switch (mode)
							{
							case TMWALK:
								m_HighlightWlkList.push_back(tmpWld);
								break;
							case TMATTACK:
								m_HighlightAttkList.push_back(tmpWld);
								break;
							}
						}
					}
				}
			}
			//-+
			tmpWld.x = center.x - iLoop;
			tmpWld.y = center.y + jLoop;
			if (iLoop + jLoop > 0)
			{
				if (!CheckCollision(tmpWld, mode))
				{ //open tile
					//check path count
					m_AStar->FindPath(center.x,center.y,tmpWld.x,tmpWld.y);
					if(m_AStar->GetPathStatus() == FOUND)
					{
						if (m_AStar->GetPathLength() < count)
						{
							switch (mode)
							{
							case TMWALK:
								m_HighlightWlkList.push_back(tmpWld);
								break;
							case TMATTACK:
								m_HighlightAttkList.push_back(tmpWld);
								break;
							}

						}
					}
				}
			}
		}
	}
	delete m_AStar;
};

void cTargetObject::BuildGroupHighlightList(WorldPoint center, TRGTMODE mode)
{
	int iLoop;
	int jLoop;
	bool even;
	WorldPoint tmpWld;

	m_HighlightAttkList.clear();

	if (m_SourceSpell)
	{
		even = (center.y % 2 == 0);
		for(jLoop=0;jLoop<m_SourceSpell->GetGroupCount();jLoop++)
		{
			for(iLoop=0;iLoop<m_SourceSpell->GetTileCount(jLoop+1);iLoop++)
			{
				tmpWld = m_SourceSpell->GetTargetTile(even,jLoop+1,iLoop);
				tmpWld.x += center.x;
				tmpWld.y += center.y;
				if (!CheckCollision(tmpWld, mode))
				{
					m_HighlightAttkList.push_back(tmpWld);
				}
				else
                    break;
			}
		}
	}
};

void cTargetObject::UserClick()
{
	if (m_SourceNPC && MouseInHighLightList(m_TrgtMode))
	{
		switch (m_TrgtMode)
		{
		case TMWALK:
			{
				m_pParent->GetGameObjMangr()->PathNPC(true,m_SourceNPC->GetGUID(),m_pParent->GetCursor()->WorldX(), m_pParent->GetCursor()->WorldY());
				Reset();
				break;
			}
		case TMATTACK:
			{
				switch (m_AttackType)
				{
				case MELEE:
					{
						GetTargets();
						CastSpell();
						//MeleeAttack();
						Reset();
						break;
					}
				case SPELL:
					{
						GetTargets();
						CastSpell();
						Reset();
						break;
					}
				case RANGED:
					break;
				};
				break;
			}
		case TMNONE:
			break;
		}
	}
}

void cTargetObject::CastSpell()
{
	if (m_SourceSpell)
	{
		if (m_TargetList.size() == 0)
			Cancel();
		if (m_TargetList.size() > 0) //identify the primart target for battle control status
			m_pParent->GetBattleControl()->SetTargetNPC(m_TargetList[0]);
		if (m_TargetList.size() == 1)
			m_pParent->GetGameObjMangr()->CastSpell(true,m_SourceSpell->Id(), m_SourceNPC->GetGUID() ,m_TargetList[0]->GetGUID());
		if (m_TargetList.size() > 1)
			m_pParent->GetGameObjMangr()->CastSpell(true,m_SourceSpell, m_SourceNPC,m_TargetList);
	}
};

void cTargetObject::MeleeAttack()
{
	if (m_SourceNPC)
	{
		if (m_TargetList.size() > 0)//identify the primart target for battle control status
			m_pParent->GetBattleControl()->SetTargetNPC(m_TargetList[0]);
		if (m_TargetList.size() == 1)
			m_pParent->GetGameObjMangr()->MeleeAttack(true,m_SourceNPC->GetGUID(),m_TargetList[0]->GetGUID());
		if (m_TargetList.size() > 1)
			m_pParent->GetGameObjMangr()->MeleeAttack(true,m_SourceNPC,m_TargetList);
	}
};


void cTargetObject::Update()
{
	if ((m_ScreenX != m_pParent->GetMainMap()->ScreenX()) ||  (m_ScreenY != m_pParent->GetMainMap()->ScreenY()))
	{
		m_ScreenX = m_pParent->GetMainMap()->ScreenX();
		m_ScreenY = m_pParent->GetMainMap()->ScreenY();
		if (m_SourceNPC)
		    m_SourceScrnPos = GetScrnPos(m_SourceWldPos);
	}
};

void cTargetObject::Render(float fDeltaTime, float camX, float camY)
{

	m_MouseScrnPos = GetMouseTileScrnPos();

	switch (m_TrgtMode)
	{
	case TMWALK:
		{
			RenderHighLightWalk(m_BlueSprite);
			//show attack patter too
			RenderHighLightAttack(m_YellowSprite);

			if (MouseInHighLightList(TMWALK))
			{
					m_RedSprite->Render(m_MouseScrnPos.x,m_MouseScrnPos.y);
					m_ValidTarget = true;
			}
			else
			{
					m_GreySprite->Render(m_MouseScrnPos.x,m_MouseScrnPos.y);
					m_ValidTarget = false;
			}
			break;
		}
	case TMATTACK:
		{
			switch (m_AttackType)
			{
			case MELEE:
				{
					RenderHighLightAttack(m_YellowSprite);
					if (MouseInHighLightList(TMATTACK))
					{
						m_RedSprite->Render(m_MouseScrnPos.x,m_MouseScrnPos.y);
						m_ValidTarget = true;
					}
					else
					{
						m_GreySprite->Render(m_MouseScrnPos.x,m_MouseScrnPos.y);
						m_ValidTarget = false;
					}
					break;
				}
			case SPELL:
				{
					RenderHighLightAttack(m_YellowSprite);
					switch(m_TrgtType)
					{
					case GROUP:
						RenderGroupSpellArea(m_RedSprite);
						break;
					case MULTI:
						{
							if (MouseInHighLightList(TMATTACK))
								RenderSpellArea(m_RedSprite);
							else
								RenderSpellArea(m_GreySprite);
							break;
						}
					case SINGLE:
						{
							if (MouseInHighLightList(TMATTACK))
							{
								m_RedSprite->Render(m_MouseScrnPos.x,m_MouseScrnPos.y);
								m_ValidTarget = true;
							}
							else
							{
								m_GreySprite->Render(m_MouseScrnPos.x,m_MouseScrnPos.y);
								m_ValidTarget = false;
							}
							break;
						}
					}
					break;
				}
			case RANGED:
				break;
			};
			break;
		}
	case TMNONE:
		break;
	}
};


void cTargetObject::RenderHighLightWalk(hgeSprite* sprt)
{
	float maxX = m_pParent->GetMainMap()->ScreenX() + SCREENWIDTH;
	float maxY = m_pParent->GetMainMap()->ScreenY() + SCREENHEIGHT;
	ScreenPoint tmpScrn;
	for (unsigned int iLoop=0;iLoop<m_HighlightWlkList.size();iLoop++)
	{
		tmpScrn = GetScrnPos(m_HighlightWlkList[iLoop]);
		if (tmpScrn.x < maxX)
			if (tmpScrn.y < maxY)
				sprt->Render(tmpScrn.x, tmpScrn.y);
	}
}

void cTargetObject::RenderHighLightAttack(hgeSprite* sprt)
{
	float maxX = m_pParent->GetMainMap()->ScreenX() + SCREENWIDTH;
	float maxY = m_pParent->GetMainMap()->ScreenY() + SCREENHEIGHT;
	ScreenPoint tmpScrn;
	for (unsigned int iLoop=0;iLoop<m_HighlightAttkList.size();iLoop++)
	{
		tmpScrn = GetScrnPos(m_HighlightAttkList[iLoop]);
		if (tmpScrn.x < maxX)
			if (tmpScrn.y < maxY)
				sprt->Render(tmpScrn.x, tmpScrn.y);
	}
}


void cTargetObject::RenderSpellArea(hgeSprite* sprt)
{
	WorldPoint wldstart;
	WorldPoint trgtpoint;
	ScreenPoint screen;
	bool even;
	int iLoop;

	wldstart.y = m_pParent->GetCursor()->WorldY();
	wldstart.x = m_pParent->GetCursor()->WorldX();

	if (m_TrgtStart != MOUSE)
	{
		wldstart.y = m_SourceWldPos.y;
		wldstart.x = m_SourceWldPos.x;
	}

	if (m_SourceSpell)
	{
		even = (wldstart.y % 2 == 0);

		if (m_TrgtType != GROUP)
		{ //only do group 1
			for(iLoop=0;iLoop<m_SourceSpell->GetTileCount(1);iLoop++)
			{
				trgtpoint = m_SourceSpell->GetTargetTile(even,1,iLoop);
				screen = GetScrnPos(wldstart.x + trgtpoint.x, wldstart.y + trgtpoint.y);
				sprt->Render(screen.x,screen.y);
			}
		}

	}

};


void cTargetObject::RenderGroupSpellArea(hgeSprite* sprt)
{
	WorldPoint wldstart;
	WorldPoint trgtpoint;
	ScreenPoint screen;
	bool even;
	unsigned int group;
	int iLoop;

	wldstart.y = m_pParent->GetCursor()->WorldY();
	wldstart.x = m_pParent->GetCursor()->WorldX();

	if (m_SourceSpell)
	{
		even = (m_SourceWldPos.y % 2 == 0);
		if (m_TrgtType == GROUP)
		{ //only do one group

			group = m_SourceSpell->GetTargetGroup(even, wldstart.x - m_SourceWldPos.x, wldstart.y - m_SourceWldPos.y);

			for(iLoop=0;iLoop<m_SourceSpell->GetTileCount(group);iLoop++)
			{
				trgtpoint = m_SourceSpell->GetTargetTile(even,group,iLoop);
				trgtpoint.x +=  m_SourceWldPos.x;
				trgtpoint.y +=	m_SourceWldPos.y;
				if (!CheckCollision(trgtpoint, TMATTACK))
				{
					screen = GetScrnPos(trgtpoint.x, trgtpoint.y);
					sprt->Render(screen.x,screen.y);
				}
				else
					break;
			}
		}

	}

};






