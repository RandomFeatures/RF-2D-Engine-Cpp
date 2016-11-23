#include "RF2d_Projectile.h"
#include "RF2d_Application.h"
#include "RF2d_CallBackHeader.h"
#include "RF2d_StaticTextures.h"

cProjectileObject::cProjectileObject(cApp *pParent)
{
	m_PartSprite = NULL;
	m_psi = NULL;
	m_GlowSprite = NULL;
	m_Hit = NULL;
	m_Travel = NULL;
	m_TmpTravel1 = NULL;
	m_TmpTravel2 = NULL;
	m_Loading = true;
	m_AStar = NULL;
	m_Moving = false;
	m_MoveAgain = false;
	m_Traveling = false;
	m_Hitting = false;
	m_MovingOffSetX = 0;
	m_MovingOffSetY = 0;
	m_Particlefps = 50.0f;
	m_OnScreen = false;
	m_Type = PROJ;
	m_pParent = pParent;
	m_Time = 0;
	m_GlowColor = ARGB(255,0,0,0);
};

cProjectileObject::cProjectileObject(const cProjectileObject& p)
{
	m_PartSprite = NULL;
	m_psi = NULL;
	m_GlowSprite = NULL;
	m_Hit = NULL;
	m_Travel = NULL;
	m_TmpTravel1 = NULL;
	m_TmpTravel2 = NULL;
	m_Time = 0;
	m_Loading = true;
	m_AStar = NULL;
	m_Traveling = false;
	m_Hitting = false;
	m_Moving = false;
	m_MoveAgain = false;
	m_MovingOffSetX = 0;
	m_MovingOffSetY = 0;
	m_OnScreen = false;
	m_Type = PROJ;

	m_pParent = p.m_pParent;

	if (p.m_Hit)
		m_Hit = p.m_Hit;

	if (p.m_TmpTravel1 && p.m_TmpTravel2)
	{
		m_TmpTravel2 = p.m_TmpTravel2;
		m_TmpTravel1 = p.m_TmpTravel1;
	}
	else
		if (p.m_Travel)
			m_Travel = p.m_Travel;

	if (p.m_psi) LoadPSI(p.m_psiFileName, p.m_Particlefps);

	GlowEffect(p.m_GlowColor);
};



cProjectileObject::~cProjectileObject()
{
	m_Target = NULL;
	m_Caster = NULL;

	if (m_Hit) delete m_Hit;
	m_Hit = NULL;

	if (m_TmpTravel1 && m_TmpTravel2)
	{
		delete m_TmpTravel1;
		delete m_TmpTravel2;
	}
	else
		if (m_Travel) delete m_Travel;

	m_TmpTravel1 = NULL;
	m_TmpTravel2 = NULL;
	m_Travel = NULL;
	if (m_AStar) delete m_AStar;
	m_AStar = NULL;
	if (m_GlowSprite) delete m_GlowSprite;
	m_GlowSprite = NULL;
	//m_psi->Stop();
	if (m_psi) delete m_psi;
	m_psi = NULL;
	if (m_PartSprite) delete m_PartSprite;
	m_PartSprite = NULL;
	m_Traveling = false;
	m_Hitting = false;
};

void cProjectileObject::PrepareLaunch(cSpellObject *p)
{

	hge->System_Log("AutoPrepareLaunch");
	m_SourceSpell = p;
	m_Target = m_SourceSpell->GetTarget();
	m_Caster = m_SourceSpell->GetCaster();
	GetStartPosFromCaster();
};

void cProjectileObject::AssignCaster(cCharacterObject *caster)
{
	m_Caster = caster;
};
void cProjectileObject::AssignTarget(cCharacterObject *target)
{
	m_Target = target;
};
void cProjectileObject::AssignSpell(cSpellObject *spell)
{
	m_SourceSpell = spell;
};


void cProjectileObject::Launch()
{
	m_Traveling = true;
	m_Loading = false;
	if (m_Target)
	{
		//SetDirection(m_Target->GetDirection());
		if (!PathFindTo(m_Target->GetxPos(),m_Target->GetyPos()))
		hge->System_Log("No Path");
	}
	else
		hge->System_Log("No Target");
	if (m_Travel)
		m_Travel->Play();
	if (m_psi)
		m_psi->Fire();
};

void  cProjectileObject::Hit()
{
	hge->System_Log("ProjectileHit");
	m_Hitting = true;
	m_Traveling = false;
	if (m_Hit)
		m_Hit->Play();
	if (m_psi)
		m_psi->Stop();
	if (m_SourceSpell)
		m_SourceSpell->Hit();
};

void cProjectileObject::GetStartPosFromCaster()
{
	hge->System_Log("GetStartPosFromCaster");

	if (m_Caster)
	{
		if (m_Caster->GetyPos()  % 2 == 0)
		{
			switch(m_Caster->GetDirection())
			{
			case iNE:
				SetPosition(m_Caster->GetxPos(),m_Caster->GetyPos()-1);
				break;
			case iNW:
				SetPosition(m_Caster->GetxPos()-1,m_Caster->GetyPos()-1);
				break;
			case iSE:
				SetPosition(m_Caster->GetxPos(),m_Caster->GetyPos()+1);
				break;
			case iSW:
				SetPosition(m_Caster->GetxPos()-1,m_Caster->GetyPos()+1);
				break;
			}
		}
		else
		{
			switch(m_Caster->GetDirection())
			{
			case iNE:
				SetPosition(m_Caster->GetxPos()+1,m_Caster->GetyPos()-1);
				break;
			case iNW:
				SetPosition(m_Caster->GetxPos(),m_Caster->GetyPos()-1);
				break;
			case iSE:
				SetPosition(m_Caster->GetxPos()+1,m_Caster->GetyPos()+1);
				break;
			case iSW:
				SetPosition(m_Caster->GetxPos(),m_Caster->GetyPos()+1);
				break;
			}
		}
	}
};

void cProjectileObject::SetDirection(DIR dir)
{
	m_CurDirection = dir;
	if (m_TmpTravel1 && m_TmpTravel2)
	{
		switch (dir)
		{
		case NW:
			m_Travel = m_TmpTravel1;
			m_Travel->SetDirection(iNW);
			break;
		case NN:
			m_Travel = m_TmpTravel2;
			m_Travel->SetDirection(iNE);
			break;
		case NE:
			m_Travel = m_TmpTravel1;
			m_Travel->SetDirection(iNE);
			break;
		case EE:
			m_Travel = m_TmpTravel2;
			m_Travel->SetDirection(iNW);
			break;
		case SE:
			m_Travel = m_TmpTravel1;
			m_Travel->SetDirection(iSE);
			break;
		case SS:
			m_Travel = m_TmpTravel2;
			m_Travel->SetDirection(iSW);
			break;
		case SW:
			m_Travel = m_TmpTravel1;
			m_Travel->SetDirection(iSW);
			break;
		case WW:
			m_Travel = m_TmpTravel2;
			m_Travel->SetDirection(iSE);
			break;
		}
	}else
	{
	//	if (m_Travel) m_Travel->SetDirection(dir);
	//	if (m_Hit) m_Hit->SetDirection(dir);
	}
};

bool cProjectileObject::PathFindTo(int x, int y)
{
//	int WorldX = ((x + mainmap->ScreenX())/TILEWIDTH);
//	int WorldY = ((y + mainmap->ScreenY())/TILEHALFHEIGHT);
	if (!m_AStar) m_AStar = new cAStar();
	m_AStar->SetCollisionCall(ptCheckCollision);
	m_DestX = x;
	m_DestY = y;
	if (m_PosX == m_DestX && m_PosY == m_DestY)
		 return false;

	m_AStar->FindPath(m_PosX,m_PosY,m_DestX,m_DestY);
	if(m_AStar->GetPathStatus() == FOUND)
	{
		GetNextPath();
		m_Moving = true;
		return true;
	}
	else
	 return false;
};

void cProjectileObject::GetNextPath()
{
	DIR dir;
	if(m_AStar->GetPathStatus() == FOUND)
	{
		m_AStar->ReadPath(m_PosX,m_PosY);
		m_NextDestX = m_AStar->GetNextX();
		m_NextDestY = m_AStar->GetNextY();
		m_Halfway =false;

		dir = NW;

		if(m_PosY%2==0 && m_PosY - 1 == m_NextDestY && m_PosX == m_NextDestX)
			dir = NE;
		else
		if(m_PosY%2!=0 && m_PosY - 1 == m_NextDestY && m_PosX + 1 == m_NextDestX)
			dir = NE;
		else
		if(m_PosY%2==0 && m_PosY + 1 == m_NextDestY && m_PosX == m_NextDestX)
			dir = SE;
		else
		if(m_PosY%2!=0 && m_PosY + 1 == m_NextDestY && m_PosX +1== m_NextDestX)
			dir = SE;
		else
		if(m_PosY%2==0 && m_PosY+1 == m_NextDestY && m_PosX-1 == m_NextDestX)
			dir = SW;
		else
		if(m_PosY%2!=0 && m_PosY+1 == m_NextDestY && m_PosX == m_NextDestX)
			dir = SW;
		else
		if(m_PosY%2==0 && m_PosY-1 == m_NextDestY && m_PosX-1 == m_NextDestX)
			dir = NW;
		else
		if(m_PosY%2!=0 && m_PosY -1== m_NextDestY && m_PosX == m_NextDestX)
			dir = NW;
		else
		if(m_PosY - 2 == m_NextDestY && m_PosX == m_NextDestX)
			dir = NN;
		else
		if(m_PosY + 2 == m_NextDestY && m_PosX == m_NextDestX)
			dir = SS;
		else
		if(m_PosY == m_NextDestY && m_PosX + 1 == m_NextDestX)
			dir = EE;
		else
		if(m_PosY == m_NextDestY && m_PosX - 1 == m_NextDestX)
			dir = WW;

		SetDirection(dir);
	}

};

void cProjectileObject::UpdateMove(float fDeltaTime)
{
	m_Time += fDeltaTime;

	if (m_Time > .03f)
	{
		switch(m_CurDirection)
		{
			case NW:
			{
				if (m_Halfway)
				{
					if( !(getOffsetY() <= 0) ) addOffsetY(-HALFPROJSPEEDY);
					if( !(getOffsetX() <= 0) ) addOffsetX(-PROJSPEEDX);

					if( getOffsetX()<=0 && getOffsetY()<=0 )
					{
						m_Halfway =false;
						GetNextPath();
					}
				}
				else
				{
					if( !(getOffsetY() <= -QUARTERTILEHEIGHT) ) addOffsetY(-HALFPROJSPEEDY);
					if( !(getOffsetX() <= -QUARTERTILEWIDTH) ) addOffsetX(-PROJSPEEDX);

					if( getOffsetX()<=-QUARTERTILEWIDTH && getOffsetY()<=-QUARTERTILEHEIGHT )
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetX = QUARTERTILEWIDTH;
							m_MovingOffSetY = QUARTERTILEHEIGHT;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
			case NE:
			{
				if (m_Halfway)
				{
					if( (getOffsetY() >= 0) ) addOffsetY(-HALFPROJSPEEDY);
					if( getOffsetX() <= 0) addOffsetX(PROJSPEEDX);

					if( getOffsetX()>=0 && getOffsetY()<=0 )
					{
						m_Halfway = false;
						GetNextPath();
					}
				}
				else
				{
					if( !(getOffsetY() <= -QUARTERTILEHEIGHT) ) addOffsetY(-HALFPROJSPEEDY);
					if( getOffsetX() <= QUARTERTILEWIDTH ) addOffsetX(PROJSPEEDX);

					if( getOffsetX()>=QUARTERTILEWIDTH && getOffsetY()<=-QUARTERTILEHEIGHT )
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetY = QUARTERTILEHEIGHT;
							m_MovingOffSetX = -QUARTERTILEWIDTH;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
			case SW:
			{
				if (m_Halfway)
				{
					if( !(getOffsetY() >= 0) ) addOffsetY(HALFPROJSPEEDY);
					if( !(getOffsetX() <= 0) ) addOffsetX(-PROJSPEEDX);

					if( getOffsetX()<=0 && getOffsetY()<=0)
					{
						m_HasMoved = false;
						GetNextPath();
					}
				}
				else
				{
					if( !(getOffsetY() >= QUARTERTILEHEIGHT) ) addOffsetY(HALFPROJSPEEDY);
					if( !(getOffsetX() <= -QUARTERTILEWIDTH) ) addOffsetX(-PROJSPEEDX);

					if( getOffsetX()<=-QUARTERTILEWIDTH && getOffsetY()<=QUARTERTILEHEIGHT )
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetY = -QUARTERTILEHEIGHT;
							m_MovingOffSetX = QUARTERTILEWIDTH;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
			case SE:
			{
				if (m_Halfway)
				{
					if( getOffsetY() <= 0 ) addOffsetY(HALFPROJSPEEDY);
					if( getOffsetX() <= 0 ) addOffsetX(PROJSPEEDX);

					if( getOffsetX()>=0 && getOffsetY()>=0 )
					{
						m_HasMoved = false;
						GetNextPath();
					}
				}
				else
				{
					if( getOffsetY() <= QUARTERTILEHEIGHT ) addOffsetY(HALFPROJSPEEDY);
					if( getOffsetX() <= QUARTERTILEWIDTH ) addOffsetX(PROJSPEEDX);

					if( getOffsetX()>=QUARTERTILEWIDTH && getOffsetY()>=QUARTERTILEHEIGHT )
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetY = -QUARTERTILEHEIGHT;
							m_MovingOffSetX = -QUARTERTILEWIDTH;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
			case NN:
			{
				if (m_Halfway)
				{
					if(getOffsetY() >= 0) addOffsetY(-HALFPROJSPEEDY);

					if(getOffsetY()<=0)
					{
						m_Halfway =false;
						GetNextPath();
					}
				}
				else
				{
					if(getOffsetY() >= -HALFTILEHEIGHT) addOffsetY(-HALFPROJSPEEDY);

					if( getOffsetY()<=-HALFTILEHEIGHT )
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetX = 0;
							m_MovingOffSetY = HALFTILEHEIGHT;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
			case EE:
			{
				if (m_Halfway)
				{
					if( getOffsetX() <= 0 ) addOffsetX(PROJSPEEDX);

					if( getOffsetX()>=0 && getOffsetY()>=0 )
					{
						m_HasMoved = false;
						GetNextPath();
					}
				}
				else
				{
					if( getOffsetX() <= HALFTILEWIDTH ) addOffsetX(PROJSPEEDX);

					if(getOffsetX()>=HALFTILEWIDTH)
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetY = 0;
							m_MovingOffSetX = -HALFTILEWIDTH;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
			case SS:
			{
				if (m_Halfway)
				{
					if(getOffsetY() <= 0) addOffsetY(HALFPROJSPEEDY);

					if(getOffsetY()>=0)
					{
						m_Halfway =false;
						GetNextPath();
					}
				}
				else
				{
					if(getOffsetY() <= HALFTILEHEIGHT) addOffsetY(HALFPROJSPEEDY);

					if(getOffsetY()>=HALFTILEHEIGHT)
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetX = 0;
							m_MovingOffSetY = -HALFTILEHEIGHT;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
			case WW:
			{
				if (m_Halfway)
				{
					if(getOffsetX() >= 0) addOffsetX(-PROJSPEEDX);

					if(getOffsetX()<=0)
					{
						m_HasMoved = false;
						GetNextPath();
					}
				}
				else
				{
					if(getOffsetX() >= -HALFTILEWIDTH) addOffsetX(-PROJSPEEDX);

					if(getOffsetX()<=-HALFTILEWIDTH)
					{
						if (ShouldStop())
							Stop();
						else
						{
							SetPosition(m_NextDestX,m_NextDestY);
							m_MovingOffSetY = 0;
							m_MovingOffSetX = HALFTILEWIDTH;
							m_HasMoved = true;
							m_Halfway =true;
						}
					}
				}
				break;
			}
		}
		m_Time = 0;
	}
	m_MoveAgain = false;
}

bool cProjectileObject::ShouldStop()
{
 if (m_NextDestX == m_DestX && m_NextDestY == m_DestY)
	 return true;
 else
	 return false;
};

void cProjectileObject::Stop()
{
		hge->System_Log("projectileStop");
		m_Moving = false;
		m_MovingOffSetX = 0;
		m_MovingOffSetY = 0;
		if (!m_MoveAgain)
		{
			Hit();
		}
};

void cProjectileObject::AddGlowEffect(int red, int green, int blue)
{
	hge->System_Log("AddGlowEffect");
	m_GlowColor = ARGB(255,red,green,blue);
	GlowEffect(m_GlowColor);
}

void cProjectileObject::GlowEffect(DWORD glowcolor)
{
	m_GlowColor = glowcolor;
	if (m_GlowColor != ARGB(255,0,0,0))
	{
		//hge->System_Log("glow color %i",m_GlowColor);
		if (!m_GlowSprite)
		m_GlowSprite=new hgeSprite(m_pParent->GetStaticTexture()->GetGlowTexture(),0, 0, 256, 256);
		m_GlowColor = SETA(m_GlowColor,0);
		m_GlowSprite->SetColor(m_GlowColor);
		m_GlowSprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD);
	}
};

void cProjectileObject::LoadPSI(std::string fileName, float fps)
{
	char tmpFilePath[256];
	hge->System_Log("LoadPSI");
	m_Particlefps = fps;
	//Load Particle Texture
	if (!m_PartSprite)
	m_PartSprite=new hgeSprite(m_pParent->GetStaticTexture()->GetParticleTexture(), 0, 0, 32, 32);
	m_PartSprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD);
	m_PartSprite->SetHotSpot(4,4);
	m_psiFileName = fileName;
	strcpy(tmpFilePath, m_pParent->GetGamePath(PARTICLE));
	strcat(tmpFilePath, fileName.c_str());
	if (!m_psi)
	m_psi=new hgeParticleSystem(tmpFilePath,m_PartSprite);
	//m_psi->info.nEmission=120;
};

/********Load Travel**********/
void cProjectileObject::LoadTravel(std::string fileName)
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
	if (iFrames > 0)// Frames are assigned
	{
	hge->System_Log("LoadTravel");
		//Create the HGEAnimation object
		m_Travel = new AniStorage();
		m_Travel->Init(tmpBaseName, iFrames, iFPS, iWidth, iHeight, true, true);
		if (iBlend == 1) m_Travel->SetBlendMode(BLEND_ALPHAADD);
		m_Travel->SetMode(iMode);
		m_Travel->SetHotSpot(iHotx,iHoty);
		m_Travel->SetDividePoint(0);
	}
	inFile.close(); //Close the file handle
};

/********Load Travel**********/
void cProjectileObject::LoadTravel2(std::string firstFile, std::string secondFile)
{
	char BaseName[150];
	char tmpBaseName[150];
	char tmpFileName[150];
	int iFrames = 0;
	float iFPS;
	float iWidth, iHeight;
	bool iOneFace;
	int iActionFrame;
	int iMode;
	float iHotx, iHoty;
	int iBlend;
	ifstream inFile;

	hge->System_Log("LoadTravel2");
	strcpy(tmpFileName,m_pParent->GetGamePath(DATA));
	strcat(tmpFileName,firstFile.c_str());

	inFile.open(tmpFileName, ::ios::in);
	if(inFile.bad()) exit(-1);
	//Construct the texture file name
	inFile >> BaseName;
	strcpy(tmpBaseName, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpBaseName, BaseName);
	//Load the animation data
	inFile >> iFrames >> iFPS >> iWidth >> iHeight >> iOneFace >> iActionFrame >> iMode >> iBlend >> iHotx >> iHoty;
	if (iFrames > 0)// Frames are assigned
	{
		//Create the HGEAnimation object
		m_TmpTravel1 = new AniStorage();
		m_TmpTravel1->Init(tmpBaseName, iFrames, iFPS, iWidth, iHeight, iOneFace, true);
		if (iBlend == 1) m_TmpTravel1->SetBlendMode(BLEND_ALPHAADD);
		m_TmpTravel1->SetMode(iMode);
		m_TmpTravel1->SetHotSpot(iHotx,iHoty);
	}
	inFile.close(); //Close the file handle
	inFile.clear();
	iFrames = 0;

	strcpy(tmpFileName,m_pParent->GetGamePath(DATA));
	strcat(tmpFileName,secondFile.c_str());

	inFile.open(tmpFileName, ::ios::in);
	if(inFile.bad()) exit(-1);

	//Construct the texture file name
	inFile >> BaseName;
	strcpy(tmpBaseName, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpBaseName, BaseName);
	//Load the animation data
	inFile >> iFrames >> iFPS >> iWidth >> iHeight >> iOneFace >> iActionFrame >> iMode >> iBlend >> iHotx >> iHoty;
	if (iFrames > 0)// Frames are assigned
	{
		//Create the HGEAnimation object
		m_TmpTravel2 = new AniStorage();
		m_TmpTravel2->Init(tmpBaseName, iFrames, iFPS, iWidth, iHeight, iOneFace, true);
		if (iBlend == 1) m_TmpTravel2->SetBlendMode(BLEND_ALPHAADD);
		m_TmpTravel2->SetMode(iMode);
		m_TmpTravel2->SetHotSpot(iHotx,iHoty);
	}
	inFile.close(); //Close the file handle
	inFile.clear();
	m_Travel = m_TmpTravel1;

};


/********Load Hit**********/
void cProjectileObject::LoadHit(std::string fileName)
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


	hge->System_Log("LoadHit");
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
	if (iFrames > 0)// Frames are assigned
	{
		//Create the HGEAnimation object
		m_Hit = new AniStorage();
		m_Hit->Init(tmpBaseName, iFrames, iFPS, iWidth, iHeight, iOneFace, true);
		if (iBlend == 1) m_Hit->SetBlendMode(BLEND_ALPHAADD);
		m_Hit->SetMode(iMode);
		m_Hit->SetHotSpot(iHotx,iHoty);
	}
	inFile.close(); //Close the file handle
};

void cProjectileObject::Render(float fDeltaTime, float camX, float camY)
{
	int red, green, blue;
	DWORD tmpColor;

	//Figure out the exact world cord
	float ScreenX;
	float ScreenY;

	//Figure out the exact world cord
	int WorldX = m_PosX * TILEWIDTH;
	int WorldY = m_PosY * TILEHALFHEIGHT;

	m_OnScreen = true;
	//See if the object is onscreen before trying to draw
	if (((WorldX + TILEWIDTH) < camX) || (WorldX > (camX + SCREENWIDTH + TILEWIDTH)) ||
		((WorldY + TILEHEIGHT) < camY) || (WorldY  > (camY + SCREENHEIGHT + 128)))
	{
		m_OnScreen = false;
		return; //Not onscreen... get out
	}
	//translate world X,Y to Screen X,Y
	if (m_PosY % 2 == 0)
		ScreenX =  ((WorldX) - camX)-TILEHALFWIDTH;
	else
		ScreenX =  ((WorldX) - camX);

	ScreenY = ((WorldY) - camY);

	//ScreenX += TILEHALFWIDTH;
	ScreenY -= TILEHEIGHT+ TILEHALFHEIGHT;

    ScreenX += m_MovingOffSetX;
	ScreenY += m_MovingOffSetY;

	if (m_Traveling && (m_Travel))
	{
		if (m_GlowSprite)
		{
			int alpha = GETA(m_GlowColor) +4;
			if (alpha > 255) alpha = 255;
			m_GlowColor = SETA(m_GlowColor,alpha);

			m_FlickTimer += fDeltaTime;
			if (m_FlickTimer > 0.05f)
			{
				m_FlickTimer = 0;
				red = GETR(m_GlowColor);
				green = GETG(m_GlowColor);
				blue = GETB(m_GlowColor);

				red += hge->Random_Int(-20,20);
				green += hge->Random_Int(-20,20);
				blue +=  hge->Random_Int(-20,20);

				if (red < 0) red = 0;
				if (green < 0) green = 0;
				if (blue < 0) blue = 0;
				if (red > 255) red = 255;
				if (green > 255) green = 255;
				if (blue > 255) blue = 255;

				tmpColor = ARGB(alpha,red,green,blue);


				m_GlowSprite->SetColor(tmpColor);
			}



		//	m_GlowColor = SETA(m_GlowColor,alpha);
		//	m_GlowSprite->SetColor(m_GlowColor);
		//	m_GlowSprite->RenderStretch(ScreenX-16,ScreenY+48,ScreenX+96,ScreenY+96);
			m_GlowSprite->Render(ScreenX-120,ScreenY-120);

			//m_GlowSprite->SetColor(ARGB(alpha,255,255,255));
			//m_GlowSprite->Render(ScreenX-120,ScreenY-120);
		}

		if (m_psi)
		{
			m_psi->MoveTo(ScreenX,ScreenY);
			m_psi->Update(fDeltaTime);
			m_psi->Render();
		}
		m_Travel->Render(fDeltaTime, ScreenX, ScreenY);


	}
	if (m_Hitting && (m_Hit))
	{
		if (m_GlowSprite)
		{
			int alpha = GETA(m_GlowColor) -4;
			if (alpha < 0) alpha = 0;
			m_GlowColor = SETA(m_GlowColor,alpha);
			m_GlowSprite->SetColor(m_GlowColor);
		//	m_GlowSprite->RenderStretch(ScreenX-16,ScreenY+48,ScreenX+96,ScreenY+96);
			m_GlowSprite->Render(ScreenX-120,ScreenY-120);

		}
		if (m_Target)
			m_Hit->Render(fDeltaTime, m_Target->GetScreenX(), m_Target->GetScreenY()-(TILEHEIGHT+ TILEHALFHEIGHT));
		else
			m_Hit->Render(fDeltaTime, ScreenX, ScreenY);

		if (m_psi)
		{
			m_psi->Update(fDeltaTime);
			m_psi->Render();
		}

		if (!m_Hit->IsPlaying())
		{
			m_Hitting = false;
		}
	}
};
void cProjectileObject::SetPosition(int x, int y)
{
	hge->System_Log("SetPosition");
	m_LastPosX = m_PosX;
	m_LastPosY = m_PosY;
	m_PosX = x;
	m_PosY = y;
}


int cProjectileObject::addOffsetX(int x) { return m_MovingOffSetX+=x; }
int cProjectileObject::addOffsetY(int y) { return m_MovingOffSetY+=y; }
int cProjectileObject::getOffsetX() { return m_MovingOffSetX; };
int cProjectileObject::getOffsetY() { return m_MovingOffSetY; };
void cProjectileObject::ResetMoved(){ m_HasMoved = false; };
bool cProjectileObject::GetMoving() { return m_Moving; };
bool cProjectileObject::HasMoved(){ return m_HasMoved; };
bool cProjectileObject::Finished() {return !(m_Traveling || m_Hitting || m_Loading); };
int cProjectileObject::Id() { return m_GUID; };
