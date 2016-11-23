#include <fstream>
#include <vector>
#include "RF2d_GameObject.h"
#include "RF2d_EffectObject.h"
#include "RF2d_UsedresourceTracker.h"
#include "RF2d_Application.h"
#include "RF2d_CallBackHeader.h"

/*
game character and ansister objects
*/

CGameObject::CGameObject(bool blend, bool locked, char* TexPath)
{
	m_Type = OBJ;
	strcpy(m_TexPath, TexPath);
	m_Blended = blend;
	m_Locked = locked;
	m_UseShadows = true;
	m_Frame = -1;
	m_PosX = 0;
	m_PosY = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_TileLength = 1;
	m_TileWidth = 1;
	m_DefaultAni = NULL;
	m_DefaultSprite = NULL;
	m_MouseOver = false;
	strcpy(m_GUID,"cGameObject");
	m_AmbientColor = ARGB(255,255,255,255);
};

CGameObject::CGameObject()
{
	m_Type = OBJ;
	m_Blended = false;
	m_Locked = false;
	m_UseShadows = true;
	m_Frame = -1;
	m_PosX = 0;
	m_PosY = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_TileLength = 1;
	m_TileWidth = 1;
	m_DefaultAni = NULL;
	m_DefaultSprite = NULL;
	m_AmbientColor = ARGB(255,255,255,255);
	m_ScreenX = 0.0f;
	m_ScreenY = 0.0f;
};

CGameObject::~CGameObject()
{
	if (m_DefaultAni) m_DefaultAni->Stop();
	if (m_DefaultAni) delete m_DefaultAni;
	if (m_DefaultSprite) delete m_DefaultSprite;
	//hge->System_Log("Base Destory %s", m_GUID);
};

void CGameObject::SetGameParent(cApp* GameParent)
{
	m_GameParent = GameParent;
};

void CGameObject::LoadStatic(char* frame, char* fileName)
{
	m_Type = STATIC;
	strcpy(m_GUID,"Static");
	//Sprite data file
	ifstream inFile;
	char BaseName[150];
	char tmpBaseName[150];
	char itemName[20];
	float x, y, iWidth, iHeight;
	int  offx, offy, len, wid, div;
	int iLoop, iCount;


//	m_Frame = frame; //Which Frame in the animation to draw;

	inFile.open(fileName, ::ios::in);
	if(inFile.bad()) exit(-1);

	//Image File
	inFile >> BaseName; //Editor Root Name
	inFile >> BaseName;
	//inFile.getline(BaseName, 100, '\n');//Image File
	strcpy(tmpBaseName, m_TexPath);
	strcat(tmpBaseName, BaseName);
	inFile >> iCount;
	for(iLoop = 0;iLoop<iCount;iLoop++)
	{
		inFile >> itemName;
		inFile >> x >> y >> iWidth >> iHeight >> offx >> offy >> len >> wid >> div;
		if (strcmp(itemName,frame)==0)
		   break;
	}
	m_DividePoint = div;
	m_Width = iWidth;
	m_Height = iHeight;
	LoadDefaultTexture(tmpBaseName);
	m_TileLength = len;
	m_TileWidth = wid;
	if (offx < 0 )offx = offx*-1;
	if (offy < 0 )offy = offy*-1;


	m_DefaultSprite = new hgeSprite(m_DefaultTex, x, y, iWidth, iHeight);
	//m_DefaultSprite->SetHotSpot(0,iHeight);
	m_HotX = offx+16;
	m_HotY = iHeight-offy;

	m_DefaultSprite->SetHotSpot(m_HotX,m_HotY);
	inFile.close();
};

void CGameObject::LoadSprite(char* fileName, char* spriteName)
{
	m_Type = SPRITE;
	strcpy(m_GUID,spriteName);
	//Sprite data file
	ifstream inFile;
	char BaseName[150];
	char tmpBaseName[150];
	int iFrames;
	float iFPS, iWidth, iHeight;
	int OneFacing; //1=true,0=false
	int Mode;// 0=fwd,1=rev,2=pingpong,4=loop
	int Blend;// 1=true, 0=false
	int div;
	inFile.open(fileName, ::ios::in);
	if(inFile.bad()) exit(-1);

	//Image File
	inFile.getline(BaseName, 100, '\n');//Image File
	strcpy(tmpBaseName, m_TexPath);
	strcat(tmpBaseName, BaseName);

	inFile >> iFrames >> iFPS >> iWidth >> iHeight; //Image Info
	inFile >> OneFacing >> Mode >> Blend >> div ; //Image Info
	m_Width = iWidth;
	m_Height = iHeight;
	m_DividePoint = div;
	LoadDefaultTexture(tmpBaseName);
	//m_DefaultTex=hge->Texture_Load(tmpBaseName);//Load the Texture
	switch(Blend)
	{
	case 1:
		m_Blended = true;
		break;
	case 0:
		m_Blended = false;
		break;
	}
	m_DefaultAni= new hgeAnimation(m_DefaultTex, iFrames, iFPS, 0, 0, iWidth, iHeight);
	m_HotX = 0;
	m_HotY = iHeight;

	m_DefaultAni->SetHotSpot(m_HotX,m_HotY);
	if (Blend == 1) m_DefaultAni->SetBlendMode(BLEND_ALPHAADD);
	//m_DefaultAni.SetMode(HGEANIM_LOOP);
	//m_DefaultAni->SetColor(ARGB(250,255,255,255));
	inFile.close();
	m_DefaultAni->Play();
};


void CGameObject::LoadDefaultTexture(char * fileName)
{
	resourcemanager->GetTexture(fileName, m_DefaultTex);
}
void CGameObject::Render(float fDeltaTime, float camX, float camY, int mode)
{
	//Figure out the exact world cord
	float ScreenX;
	float ScreenY;
	int WorldX = (m_PosX * TILEWIDTH);
	int WorldY = (m_PosY * TILEHALFHEIGHT);
	m_OnScreen = true;
	//See if the object is onscreen before trying to draw
	if (((WorldX + TILEWIDTH) < camX) || (WorldX > (camX + 800 + TILEWIDTH)) ||
		((WorldY + TILEHEIGHT) < camY) || (WorldY  > (camY + 600 + 128)))
	{
		m_OnScreen = false;
		return; //Not onscreen... get out
	}
	//translate world X,Y to Screen X,Y
	if (m_PosY % 2 == 0)
		ScreenX =  ((WorldX) - camX)-TILEWIDTH;// + m_xOffset;
	else
		ScreenX =  ((WorldX) - camX)-TILEHALFWIDTH;// + m_xOffset;

	ScreenY = ((WorldY) - camY);//- m_yOffset;

	m_ScreenX = ScreenX;
	m_ScreenY = ScreenY;

	if (m_DefaultAni)
	{
		if (!m_Locked && (mode!=1)) m_DefaultAni->Update(fDeltaTime);

		//if (m_MouseOver)
		//	m_DefaultAni->SetColor(ARGB(255,180,180,255));
		//else
		//	m_DefaultAni->SetColor(ARGB(255,255,255,255));
		if (!m_Blended && m_UseShadows && (mode!=1))
		{
			m_DefaultAni->SetColor(ARGB(150,10,10,10));
			m_DefaultAni->Render4V(ScreenX+32,ScreenY-m_DefaultAni->GetHeight(),ScreenX+m_DefaultAni->GetWidth()+32,ScreenY-m_DefaultAni->GetHeight(),ScreenX+m_DefaultAni->GetWidth(),ScreenY,ScreenX,ScreenY);
			m_DefaultAni->SetColor(m_AmbientColor);

		}

		switch (mode)
		{
		case 0:
			m_DefaultAni->Render(ScreenX,ScreenY);
			break;
		case 1:
			if (m_DividePoint > 0)
				RenderBottom(m_DefaultAni,ScreenX,ScreenY);
			else
				m_DefaultAni->Render(ScreenX,ScreenY);
			break;
		case 2:
			if (m_DividePoint > 0)
				RenderTop(m_DefaultAni,ScreenX,ScreenY);
			break;
		}
	}

	if (m_DefaultSprite)
	{
		//if (m_MouseOver)
		//	m_DefaultSprite->SetColor(ARGB(255,180,180,255));
		//else
		//	m_DefaultSprite->SetColor(ARGB(255,255,255,255));
		if (!m_Blended && m_UseShadows && (mode!=1))
		{
			//m_HotY
			//m_HotX

			float halfwidth = m_DefaultSprite->GetWidth()/2;
			float x0 = ScreenX+32-m_HotX;
			float y0 = ScreenY-m_HotY;
			float x1 = ScreenX+m_HotX+64;
			float y1 = ScreenY-m_HotY;
			float x2 = ScreenX+m_HotX;
			float y2 = ScreenY+(m_DefaultSprite->GetHeight()-m_HotY);
			float x3 = ScreenX-m_HotX;
			float y3 = ScreenY+(m_DefaultSprite->GetHeight()-m_HotY);
			m_DefaultSprite->SetColor(ARGB(80,10,10,10));
			m_DefaultSprite->Render4V(x0,y0,x1,y1,x2,y2,x3,y3);
			//m_DefaultSprite->RenderEx(ScreenX,ScreenY,5.00f);
			m_DefaultSprite->SetColor(m_AmbientColor);
		}
		switch (mode)
		{
		case 0:
			m_DefaultSprite->Render(ScreenX,ScreenY);
			break;
		case 1:
			if (m_DividePoint > 0)
				RenderBottom(m_DefaultSprite,ScreenX,ScreenY);
			else
				m_DefaultSprite->Render(ScreenX,ScreenY);
			break;
		case 2:
			if (m_DividePoint > 0)
				RenderTop(m_DefaultSprite,ScreenX,ScreenY);
			break;
		}
	}
};

void CGameObject::RenderBottom(hgeSprite* Spri, float x, float y)
{


   float origX,origY,origW,origH;

   if(Spri)
   {
		Spri->GetTextureRect(&origX,&origY,&origW,&origH);
		Spri->SetTextureRect(origX,origY+(origH-m_DividePoint),origW,origH-(origH-m_DividePoint));
		Spri->Render(x,y+(origH-m_DividePoint));
		Spri->SetTextureRect(origX,origY,origW,origH);
   }

}

void CGameObject::RenderTop(hgeSprite* Spri, float x, float y)
{

   float origX,origY,origW,origH;

   if(Spri)
   {
		Spri->GetTextureRect(&origX,&origY,&origW,&origH);
		Spri->SetTextureRect(origX,origY,origW,origH-m_DividePoint);
		Spri->Render(x,y);
		Spri->SetTextureRect(origX,origY,origW,origH);
   }

}



void CGameObject::RenderDynamicShadow(int degree)
{
	float rAngel =	0.0174532f * degree;

	if (m_DefaultSprite)
	{
		if (!m_Blended && m_UseShadows)
		{


			m_DefaultSprite->SetColor(ARGB(80,10,10,10));
			m_DefaultSprite->RenderEx(m_ScreenX,m_ScreenY,rAngel,0.0f);
			m_DefaultSprite->SetColor(m_AmbientColor);
		}
	}

	if (m_DefaultAni)
	{
		if (!m_Blended && m_UseShadows)
		{
			m_DefaultAni->SetColor(ARGB(150,10,10,10));
			m_DefaultAni->RenderEx(m_ScreenX,m_ScreenY,rAngel,0.0f);
			m_DefaultAni->SetColor(m_AmbientColor);

		}
	}

}


void CGameObject::MouseOver(int x, int y)
{
	m_MouseOver = false;
	if (x > m_ScreenX && x < (m_ScreenX+m_Width))
		if (y > m_ScreenY && y < (m_ScreenY+m_Height))
			m_MouseOver = true;
};

void CGameObject::SetAmbientColor(int red, int green, int blue)
{
	SetAmbientColor(ARGB(255,red,green,blue));
};

void  CGameObject::SetAmbientColor(DWORD ambientcolor)
{
	m_AmbientColor = ambientcolor;
	if (m_DefaultAni && !m_Blended) m_DefaultAni->SetColor(m_AmbientColor);
	if (m_DefaultSprite && !m_Blended) m_DefaultSprite->SetColor(m_AmbientColor);
};

void CGameObject::SetShadows(bool shadow)
{
	m_UseShadows = shadow;
};


char *CGameObject::GetTexPath(){return m_TexPath;};
char *CGameObject::GetDisplayName(){return m_DisplayName;};
char *CGameObject::GetGUID(){return m_GUID;};
int CGameObject::GetxPos() {return m_PosX;};
int CGameObject::GetyPos(){return m_PosY;};
int CGameObject::GetxOffset(){return m_xOffset;};
int CGameObject::GetyOffset(){return m_yOffset;};
int CGameObject::GetTileLength(){return m_TileLength;};
int	CGameObject::GetTileWidth(){return m_TileWidth;};
int	CGameObject::GetFrame(){return m_Frame;};
int CGameObject::GetId() { return m_ID; }
int CGameObject::GetScreenX(){return m_ScreenX;};
int CGameObject::GetScreenY(){return m_ScreenY;};

bool CGameObject::GetEnabled(){return m_Enabled;};  //Show the sprite;
bool CGameObject::GetBlended(){return m_Blended;}; //Blend the Sprite
bool CGameObject::GetLocked(){return m_Locked;};  // Specific action lock
bool CGameObject::IsOnScreen(){return m_OnScreen;};
bool CGameObject::GetSelected() {return m_Selected;};
bool CGameObject::IsMouseOver() {return m_MouseOver;};
OBJTYPE CGameObject::GetType(){return m_Type;};  // Object Type

void CGameObject::SetDisplayName(const char *displayname){strcpy(m_DisplayName, displayname);};
void CGameObject::SetTextPath(char *textpath){strcpy(m_TexPath, textpath);};
void CGameObject::SetGUID(const char *guid){strcpy(m_GUID, guid);};
void CGameObject::SetId(int idNo){m_ID = idNo;}
void CGameObject::SetEnabled(bool enabled){m_Enabled = enabled;};
void CGameObject::SetBlended(bool blend){m_Blended = blend;};
void CGameObject::SetLocked(bool lock){m_Blended = lock;};
void CGameObject::SetPosition(int x, int y){ m_PosX = x; m_PosY = y; }
void CGameObject::SetTileSpace(int length, int width) {m_TileLength = length; m_TileWidth = width;}
void CGameObject::SetOffset(int x, int y){m_xOffset = x; m_yOffset = y;}
void CGameObject::SetType(OBJTYPE mtype){m_Type = mtype;};
void CGameObject::SetSelected(bool sel) { m_Selected = sel;};
/****************End cGameObject***********************************/

/****************Start cLiveObject********************************/
CLiveObject::CLiveObject()
{
	m_Blended = false;
	m_Locked = false;
	m_UseShadows = true;
	m_Frame = -1;
	m_PosX = 0;
	m_PosY = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_TileLength = 1;
	m_TileWidth = 1;
	m_DefaultAni = NULL;
	m_DefaultSprite = NULL;
	m_Painsfx = NULL;
	m_Deathsfx = NULL;
	m_AmbientColor = ARGB(255,255,255,255);
	m_ScreenX = 0.0f;
	m_ScreenY = 0.0f;
	m_HitPoints = 10;
	m_Damage = 0;
	m_Dead = false;
};

CLiveObject::~CLiveObject(){};

int CLiveObject::TakeDamage(int physical, int magical)
{
	int rtn;

	m_Damage += physical + magical;
	rtn = m_Damage; //return the actual amount of damage taken
	if (m_Damage > m_HitPoints)	m_Dead = true;
	if (m_Dead) rtn = -1; //-1 if damage killed me
	hge->System_Log("Damage: %i Hits: %i", m_Damage,m_HitPoints);
	return rtn;
};

int CLiveObject::GetHitPoints() {return m_HitPoints;}
int CLiveObject::GetDamage() {return m_Damage;}
bool CLiveObject::IsDead() {return m_Dead;}
void CLiveObject::SetHitPoints(int HitPoints){m_HitPoints = HitPoints;};
void CLiveObject::SetDamage(int damage){m_Damage = damage;};
/****************End cLiveObject***********************************/

/****************Start cMobileObject***************************/
CMobileObject::CMobileObject()
{
	m_Blended = false;
	m_Locked = false;
	m_UseShadows = true;
	m_Frame = -1;
	m_PosX = 0;
	m_PosY = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_TileLength = 1;
	m_TileWidth = 1;
	m_DefaultAni = NULL;
	m_DefaultSprite = NULL;
	m_Painsfx = NULL;
	m_Deathsfx = NULL;
	m_AmbientColor = ARGB(255,255,255,255);
	m_ScreenX = 0.0f;
	m_ScreenY = 0.0f;
	m_HitPoints = 10;
	m_Damage = 0;
	m_Dead = false;
	m_Speed = 50;
	m_Distance = 0.0f;
	m_AStar = NULL;
};
CMobileObject::~CMobileObject()
{
	if (m_AStar) delete m_AStar;
	m_AStar = NULL;
};

void CMobileObject::MoveTo(int x, int y, ISODIR dir)
{
	if (!m_Moving) //dont interupt mid move
	{
		SetDirection(dir);
		m_Moving = true;
		m_NextDestX = x;
		m_NextDestY = y;
		m_DestX = x;
		m_DestY = y;
		m_Halfway =false;
	}
	else
	m_MoveAgain = true;
};

bool CMobileObject::PathFindTo(int x, int y)
{
//	int WorldX = ((x + mainmap->ScreenX())/TILEWIDTH);
//	int WorldY = ((y + mainmap->ScreenY())/TILEHALFHEIGHT);
	if (!m_AStar) m_AStar = new cIsoAStar();
	m_AStar->SetCollisionCall(ptCheckCollision, ptCheckCharCollision);


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
	{
		hge->System_Log("NO PATH");
		return false;
	}
};

void CMobileObject::GetNextPath()
{
	ISODIR dir;
	if(m_AStar->GetPathStatus() == FOUND)
	{
		m_AStar->ReadPath(m_PosX,m_PosY);
		m_NextDestX = m_AStar->GetNextX();
		m_NextDestY = m_AStar->GetNextY();
		m_Halfway =false;

		dir = iNW;

		if(m_PosY%2==0 && m_PosY - 1 == m_NextDestY && m_PosX == m_NextDestX)
			dir = iNE;
		else
		if(m_PosY%2!=0 && m_PosY - 1 == m_NextDestY && m_PosX + 1 == m_NextDestX)
			dir = iNE;
		else
		if(m_PosY%2==0 && m_PosY + 1 == m_NextDestY && m_PosX == m_NextDestX)
			dir = iSE;
		else
		if(m_PosY%2!=0 && m_PosY + 1 == m_NextDestY && m_PosX +1== m_NextDestX)
			dir = iSE;
		else
		if(m_PosY%2==0 && m_PosY+1 == m_NextDestY && m_PosX-1 == m_NextDestX)
			dir = iSW;
		else
		if(m_PosY%2!=0 && m_PosY+1 == m_NextDestY && m_PosX == m_NextDestX)
			dir = iSW;
		else
		if(m_PosY%2==0 && m_PosY-1 == m_NextDestY && m_PosX-1 == m_NextDestX)
			dir = iNW;
		else
		if(m_PosY%2!=0 && m_PosY -1== m_NextDestY && m_PosX == m_NextDestX)
			dir = iNW;

		SetDirection(dir);
	}

};

void CMobileObject::UpdateMove(float fDeltaTime)
{
	m_Time += fDeltaTime;
	if (m_Time > .03f)
	{
		switch(m_CurDirection)
		{
			case iNW:
			{
				if (m_Halfway)
				{
					if( ! (getOffsetY() <= 0)  ) addOffsetY(-HALFSPRSPEEDY);
					if( ! (getOffsetX() <= 0) ) addOffsetX(-SPRSPEEDX);

					if( getOffsetX()<=0 && getOffsetY()<=0 ) {
						m_Halfway =false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( ! (getOffsetY() <= -QUARTERTILEHEIGHT)  ) addOffsetY(-HALFSPRSPEEDY);
					if( ! (getOffsetX() <= -QUARTERTILEWIDTH) ) addOffsetX(-SPRSPEEDX);

					if( getOffsetX()<=-QUARTERTILEWIDTH && getOffsetY()<=-QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetX = QUARTERTILEWIDTH;
						m_MovingOffSetY = QUARTERTILEHEIGHT;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
			case iNE:
			{
				if (m_Halfway)
				{
					if( (getOffsetY() >= 0)  ) addOffsetY(-HALFSPRSPEEDY);
					if( getOffsetX() <= 0) addOffsetX(SPRSPEEDX);

					if( getOffsetX()>=0 && getOffsetY()<=0 ) {
						m_Halfway = false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( !(getOffsetY() <= -QUARTERTILEHEIGHT)  ) addOffsetY(-HALFSPRSPEEDY);
					if( getOffsetX() <= QUARTERTILEWIDTH ) addOffsetX(SPRSPEEDX);

					if( getOffsetX()>=QUARTERTILEWIDTH && getOffsetY()<=-QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetY = QUARTERTILEHEIGHT;
						m_MovingOffSetX = -QUARTERTILEWIDTH;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
			case iSW:
			{
				if (m_Halfway)
				{
					if( ! (getOffsetY() >= 0)  ) addOffsetY(HALFSPRSPEEDY);
					if( ! (getOffsetX() <= 0) ) addOffsetX(-SPRSPEEDX);

					if( getOffsetX()<=0 && getOffsetY()<=0) {
						m_HasMoved = false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( ! (getOffsetY() >= QUARTERTILEHEIGHT)  ) addOffsetY(HALFSPRSPEEDY);
					if( ! (getOffsetX() <= -QUARTERTILEWIDTH) ) addOffsetX(-SPRSPEEDX);

					if( getOffsetX()<=-QUARTERTILEWIDTH && getOffsetY()<=QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetY = -QUARTERTILEHEIGHT;
						m_MovingOffSetX = QUARTERTILEWIDTH;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
			case iSE:
			{
				if (m_Halfway)
				{
					if( getOffsetY() <= 0 ) addOffsetY(HALFSPRSPEEDY);
					if( getOffsetX() <= 0 ) addOffsetX(SPRSPEEDX);

					if( getOffsetX()>=0 && getOffsetY()>=0 ) {
						m_HasMoved = false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( getOffsetY() <= QUARTERTILEHEIGHT ) addOffsetY(HALFSPRSPEEDY);
					if( getOffsetX() <= QUARTERTILEWIDTH ) addOffsetX(SPRSPEEDX);

					if( getOffsetX()>=QUARTERTILEWIDTH && getOffsetY()>=QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetY = -QUARTERTILEHEIGHT;
						m_MovingOffSetX = -QUARTERTILEWIDTH;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
		}
		m_Time = 0;
	}
	m_MoveAgain = false;
}

bool CMobileObject::ShouldStop()
{
 if (m_PosX == m_DestX && m_PosY == m_DestY)
	 return true;
 else
	 return false;
};

void CMobileObject::FaceTarget(int x, int y)
{
	ISODIR dir;
	//Figure out the direction to face
	dir = iNW;
	if (m_PosY%2==0)
	{
		if (x <= m_PosX && y < m_PosY)
			dir = iNW;

		if (x >= m_PosX && y <= m_PosY)
			dir = iNE;

		if (x < m_PosX && y >= m_PosY)
			dir = iSW;

		if (x >= m_PosX && y > m_PosY)
			dir = iSE;
	}
	else
	{
		if (x <= m_PosX && y < m_PosY)
			dir = iNW;

		if (x > m_PosX && y <= m_PosY)
			dir = iNE;

		if (x <= m_PosX && y >= m_PosY)
			dir = iSW;

		if (x > m_PosX && y > m_PosY)
			dir = iSE;
	}
	SetDirection(dir);
};

void CMobileObject::SetDirection(ISODIR dir)
{
	m_CurDirection = dir;
};

void CMobileObject::SwitchDirection(ISODIR dir)
{
	m_LastDirection = m_CurDirection;
	m_CurDirection = dir;
};

void CMobileObject::Stop()
{
	m_Moving = false;
	m_MovingOffSetX = 0;
	m_MovingOffSetY = 0;
};

ISODIR CMobileObject::GetDirection() { return m_CurDirection; }
bool CMobileObject::GetMoving() { return m_Moving; };
bool CMobileObject::HasMoved(){ return m_HasMoved; };

int CMobileObject::addOffsetX(int x) { return m_MovingOffSetX+=x; }
int CMobileObject::addOffsetY(int y) { return m_MovingOffSetY+=y; }
int CMobileObject::getOffsetX() { return m_MovingOffSetX; };
int CMobileObject::getOffsetY() { return m_MovingOffSetY; };

float CMobileObject::GetSpeed(){return m_Speed;};
float CMobileObject::GetDistance(){return m_Distance;};

void CMobileObject::ResetMoved(){ m_HasMoved = false; };
void CMobileObject::SetMoving(bool move) { m_Moving = move; };
void CMobileObject::SetSpeed(float speed){m_Speed = speed;};
void  CMobileObject::SetDistance(float distance){m_Distance= distance;};



/****************End cMobileObject***************************/


/****************Start cCharacterObject***************************/
cCharacterObject::cCharacterObject(cApp* GameParent, int dir, int action, const char* fileName, char* TexPath)
{
    m_Blended = false;
	m_Locked = false;
	m_UseShadows = true;
	m_Frame = -1;
	m_PosX = 0;
	m_PosY = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_TileLength = 1;
	m_TileWidth = 1;
	m_DefaultAni = NULL;
	m_DefaultSprite = NULL;
	m_AmbientColor = ARGB(255,255,255,255);
	m_EffectColor = 0;
	m_ScreenX = 0.0f;
	m_ScreenY = 0.0f;
	m_HitPoints = 10;
	m_PhyResist = 0.0f;
	m_MagResist = 0.0f;
	m_DamageMin = 1;
	m_DamageMax = 5;
	m_Damage = 0;
	m_Dead = false;
	m_Speed = 50;
	m_Distance = 0.0f;
	m_AStar = NULL;
	m_Painsfx = NULL;
	m_Deathsfx = NULL;
	m_Moving = false;
	m_MoveAgain = false;
	m_Selected = false;
	m_MovingOffSetX = 0;
	m_MovingOffSetY = 0;
	m_WalkDistance = 5;
	m_CurrentSpellID = 0;
	m_Time = 0;
	m_AttackType = SPELL;
	SetGameParent(GameParent);
	SetType(NPC);
	//Character data file
	ifstream inFile;
	char BaseName[150];
	char tmpBaseName[150];
	int iFrames;
	int iPortraitFrame;
	float iFPS;
	float iWidth, iHeight;
//	int itmp;

	//hge->System_Log("Character Load 1");
	SetTextPath(TexPath);
	FileName(fileName);
	inFile.open(fileName, ::ios::in);
	if(inFile.bad()) exit(-1);

	//hge->System_Log("chr file: %s",fileName);

	//Image File
	inFile >> BaseName;
	//Load Stand
	strcpy(tmpBaseName, m_TexPath);
	strcat(tmpBaseName, BaseName);
	inFile >> iPortraitFrame >> iFrames >> iFPS >> iWidth >> iHeight;
	hge->System_Log("port: %i",iPortraitFrame);
	if (iFPS == 1)
		iFPS = hge->Random_Float(0.05f, 0.20f);

	m_Stand = new AniStorage();
	m_Stand->Init(tmpBaseName, iFrames, iFPS, iWidth, iHeight, false, false);
	CreatePortrait(iPortraitFrame);
	m_Stand->SetMode(HGEANIM_LOOP);
	m_Stand->SetHotSpot(iWidth/2,iHeight - 24);
	m_Stand->SetDividePoint(53);
	//Load Walk
	m_Walk = new AniStorage();
	m_Walk->Init();

	for (int iLoop=0;iLoop<4;iLoop++)
	{
		inFile >> BaseName >> iFrames >> iFPS >> iWidth >> iHeight;
		strcpy(tmpBaseName, m_TexPath);
		strcat(tmpBaseName, BaseName);
		strcpy(BaseName, tmpBaseName);
		m_Walk->AddFacing(BaseName, iFrames, iFPS, iWidth, iHeight, GetDirFromFileName(BaseName), false);
	}
	m_Walk->SetMode(HGEANIM_LOOP);
	m_Walk->SetHotSpot(iWidth/2,iHeight - 24);
	m_Walk->SetDividePoint(53);


	//Load Attack
	m_Attack = new AniStorage();
	m_Attack->Init();
	for (int iLoop=0;iLoop<4;iLoop++)
	{
		inFile >> BaseName >> iFrames >> iFPS >> iWidth >> iHeight;
		strcpy(tmpBaseName, m_TexPath);
		strcat(tmpBaseName, BaseName);
		strcpy(BaseName, tmpBaseName);
		m_Attack->AddFacing(BaseName, iFrames, iFPS, iWidth, iHeight, GetDirFromFileName(BaseName), false);
	}
	m_Attack->SetMode(HGEANIM_FWD);
	m_Attack->SetHotSpot(iWidth/2,iHeight - 24);
	m_Attack->SetDividePoint(53);

	//Load Death
	m_Death = new AniStorage();
	m_Pain = new AniStorage();
	m_Death->Init();
	m_Pain->Init();
	m_Pain->SetDividePoint(53);

	for (int iLoop=0;iLoop<4;iLoop++)
	{
		inFile >> BaseName >> iFrames >> iFPS >> iWidth >> iHeight;
		strcpy(tmpBaseName, m_TexPath);
		strcat(tmpBaseName, BaseName);
		strcpy(BaseName, tmpBaseName);
		m_Death->AddFacing(BaseName, iFrames, iFPS, iWidth, iHeight, GetDirFromFileName(BaseName), false);
		m_Pain->AddFacing(BaseName, 3, iFPS, iWidth, iHeight, GetDirFromFileName(BaseName), false);
	}
	m_Death->SetMode(HGEANIM_FWD);
	m_Death->SetHotSpot(iWidth/2,iHeight - 24);
	m_Death->SetDividePoint(0);

	m_Pain->SetMode(HGEANIM_PINGPONG);
	m_Pain->SetHotSpot(iWidth/2,iHeight - 24);
	//load basic SFx
	inFile >> tmpBaseName;
	LoadPainsfx(tmpBaseName);
	inFile >> tmpBaseName;
	LoadDeathsfx(tmpBaseName);
	inFile.close();

	SetDirection((ISODIR)dir);
	//hge->System_Log("Action %i", action);
	m_CurAction = STRT;
	DoAction((ACTION)action);
	m_Status = IDLE;
	//LoadSprite("resources\\Data\\sprites\\charselect.dat",m_GUID);
	//hge->System_Log("Character Load 2");

};


cCharacterObject::~cCharacterObject()
{
	delete m_Attack;
	delete m_Stand;
	delete m_Walk;
	delete m_Death;
	delete m_Pain;

	ClearTarget();
	ClearSpellList();
	ClearEffectList(true);
	if (m_target) hge->Target_Free(m_target);
	if (m_Portrait) delete m_Portrait;

};

DIR	cCharacterObject::GetDirFromFileName(std::string filename)
{
	string::size_type loc;
	DIR dir = SW;

	loc = filename.find( "_NE_", 0 );
	if( loc != string::npos )
		dir = NE;

	loc = filename.find( "_NW_", 0 );
	if( loc != string::npos )
		dir = NW;

	loc = filename.find( "_SE_", 0 );
	if( loc != string::npos )
		dir = SE;

	loc = filename.find( "_SW_", 0 );
	if( loc != string::npos )
		dir = SW;

	return dir;
};

void cCharacterObject::SetDirection(ISODIR dir)
{
	CMobileObject::SetDirection(dir);
	m_Attack->SetDirection(dir);
	m_Stand->SetDirection(dir);
	m_Walk->SetDirection(dir);
	m_Death->SetDirection(dir);
	m_Pain->SetDirection(dir);
};

void cCharacterObject::SwitchDirection(ISODIR dir)
{
	CMobileObject::SwitchDirection(dir);
	m_Attack->SetDirection(dir);
	m_Stand->SetDirection(dir);
	m_Walk->SetDirection(dir);
	m_Death->SetDirection(dir);
	m_Pain->SetDirection(dir);
};

void cCharacterObject::SetAction(ACTION action)
{
	m_CurAction = action;

	//hge->System_Log("SetAction: %i",m_CurAction);
};

void cCharacterObject::SwitchAction(ACTION action)
{
	if (m_CurAction != DEAD) // Dead is dead
	{
		m_LastAction = m_CurAction;
		m_CurAction = action;
	}
};

ACTION cCharacterObject::GetCurrentAction()
{
 return m_CurAction;
};

void cCharacterObject::PlayAction(std::string action)
{
	if (action.compare("stand") == 0)
		DoAction(STAND);
	if (action.compare("attack") == 0)
		DoAction(ATTACK);
	if (action.compare("walk") == 0)
		DoAction(WALK);
	if (action.compare("death") == 0)
		DoAction(DEATH);
	if (action.compare("pain") == 0)
		DoAction(PAIN);

};

void cCharacterObject::DoAction(ACTION action)
{
	if (m_CurAction != DEAD && m_CurAction != action) // Dead is dead
	{
		SwitchAction(action);

		StopAction();

		switch (action)
		{
			case STAND:
				m_Stand->Play();
				break;
			case ATTACK:
				//hge->System_Log("Test: Attack");
				m_Attack->Play();
				m_Status = ATTACKING;
				break;
			case WALK:
				//hge->System_Log("Test: Walk");
				m_Walk->Play();
				m_Status = WALKING;
				break;
			case DEATH:
				{
					//hge->System_Log("Test: Death");
					m_Death->Play();
					PlayDeathsfx();
					m_Status = DYING;

				}
				break;
			case PAIN:
				{
					//hge->System_Log("Test: Pain");
					m_Pain->Play();
					PlayPainsfx();
					m_Status = HURTING;
				}
				break;
		}
	}
};

void cCharacterObject::Stop()
{
	if (m_CurAction != DEAD) // Dead is dead
	{
		m_Moving = false;
		m_MovingOffSetX = 0;
		m_MovingOffSetY = 0;
		//m_Attack->Stop();
		if (!m_MoveAgain)
		{
			//stop walk sound
			m_GameParent->GetSoundManager()->StopWalkSFX();
			m_Walk->Stop();
			DoAction(STAND);
			m_Status = DONEWALKING;
			m_GameParent->GetGameObjMangr()->UpdateCharacterCollision();
			//m_GameParent->GetBattleControl()->MoveComplete(this);
		}
	}

};

void cCharacterObject::StopAction()
{
	if (m_CurAction != DEAD) // Dead is dead
	{
		m_Attack->Stop();
		m_Walk->Stop();
		m_Pain->Stop();
	}
};


void cCharacterObject::Render(float fDeltaTime, float camX, float camY, int mode)
{

	//Figure out the exact world cord
	float ScreenX;
	float ScreenY;

	//Figure out the exact world cord
	int WorldX = m_PosX * TILEWIDTH;
	int WorldY = m_PosY * TILEHALFHEIGHT;

	//Update Effects
	for (unsigned int a=0;a<m_EffectList.size();a++)
		m_EffectList[a]->Update(fDeltaTime);

	m_OnScreen = true;
	//See if the object is onscreen before trying to draw
	if (((WorldX + TILEWIDTH) < camX) || (WorldX > (camX + 800 + TILEWIDTH)) ||
		((WorldY + TILEHEIGHT) < camY) || (WorldY  > (camY + 600 + 128)))
	{
		m_OnScreen = false;
		return; //Not onscreen... get out
	}
	//translate world X,Y to Screen X,Y
	if (GetyPos() % 2 == 0)
		ScreenX =  ((WorldX) - camX)-TILEHALFWIDTH;
	else
		ScreenX =  ((WorldX) - camX);

	ScreenY = ((WorldY) - camY);

    ScreenX += m_MovingOffSetX;
	ScreenY += m_MovingOffSetY;

	m_ScreenX = ScreenX;
	m_ScreenY = ScreenY;

	if (m_Selected)
	{
		//m_DefaultAni->SetColor(ARGB(200,255,255,255));
		//m_DefaultAni->Update(fDeltaTime);
		m_DefaultAni->Render(ScreenX-HALFTILEWIDTH,ScreenY+HALFTILEHEIGHT);
	}

	switch (m_CurAction)
	{
		case STAND:
			if (m_EffectColor > 0)
				m_Stand->SetColor(m_EffectColor);
			else
				m_Stand->SetColor(m_AmbientColor);
			m_Stand->Render(fDeltaTime, ScreenX, ScreenY, mode);
			m_Width = m_Stand->GetFrameWidth();
			m_Height = m_Stand->GetFrameHeight();
			break;
		case ATTACK:
			//When the attack animation is done go back to stand
			if (!m_Attack->IsPlaying())
			{
				//m_GameParent->GetBattleControl()->AttackComplete(this);
				DoAction(STAND);
				m_Status = DONEATTACKING;
			}
			if (m_EffectColor > 0)
				m_Attack->SetColor(m_EffectColor);
			else
				m_Attack->SetColor(m_AmbientColor);
			m_Attack->Render(fDeltaTime,ScreenX, ScreenY, mode);
			m_Width = m_Attack->GetFrameWidth();
			m_Height = m_Attack->GetFrameHeight();
			break;
		case WALK:
			if (m_EffectColor > 0)
				m_Walk->SetColor(m_EffectColor);
			else
				m_Walk->SetColor(m_AmbientColor);
			m_Walk->Render(fDeltaTime, ScreenX, ScreenY, mode);
			m_Width = m_Walk->GetFrameWidth();
			m_Height = m_Walk->GetFrameHeight();
			break;
		case DEATH:
			if (m_Death->GetFrame() == 7)
			{ //Stop on the last frame of death and be dead
				m_Death->Stop();
				SetAction(DEAD);
				m_Status = DONEDYING;
			}
			if (m_EffectColor > 0)
				m_Death->SetColor(m_EffectColor);
			else
				m_Death->SetColor(m_AmbientColor);
			m_Death->Render(fDeltaTime, ScreenX, ScreenY, mode);
			m_Width = m_Death->GetFrameWidth();
			m_Height = m_Death->GetFrameHeight();
			break;
		case PAIN:
			if (!m_Pain->IsPlaying())
			{
				DoAction(STAND);
				m_Status = DONEHURTING;
			}
			if (m_EffectColor > 0)
				m_Pain->SetColor(m_EffectColor);
			else
				m_Pain->SetColor(m_AmbientColor);
			m_Pain->Render(fDeltaTime, ScreenX, ScreenY, mode);
			m_Width = m_Pain->GetFrameWidth();
			m_Height = m_Pain->GetFrameHeight();
			//m_GameParent->GetFont()->SetColor(ARGB(255,100,100,225));
			m_GameParent->GetFont()->printf(ScreenX-10,ScreenY-100,HGETEXT_LEFT, "%i",m_LastDamage);
			break;
		case DEAD:
			if (m_EffectColor > 0)
				m_Death->SetColor(m_EffectColor);
			else
				m_Death->SetColor(m_AmbientColor);
			m_Death->Render(fDeltaTime, ScreenX, ScreenY, mode);
			m_Width = m_Death->GetFrameWidth();
			m_Height = m_Death->GetFrameHeight();
			break;
	}

    //Render Effects
	m_EffectColor = 0;
	for (unsigned int b=0;b<m_EffectList.size();b++)
	{
		m_EffectList[b]->Render(fDeltaTime, ScreenX, ScreenY);
		m_EffectColor = m_EffectList[b]->GetColor();
	}
	//Clean up finished effects
	ClearEffectList(false);

};

void cCharacterObject::ClearTarget()
{
	m_TargetList.clear();
	/*
	vector<cCharacterObject*>::iterator iternpc;
	for (iternpc = m_TargetList.begin();iternpc< m_TargetList.end();iternpc++)
	{
		m_TargetList.erase(iternpc);
	}
	*/

};

void cCharacterObject::AddTarget(cCharacterObject* npc)
{
	m_TargetList.push_back(npc);
};


void cCharacterObject::AssignCurrentSpellID(int spell)
{
	hge->System_Log("my spellID: %i", spell);
	m_CurrentSpellID = spell;
};

int cCharacterObject::GetCurrentSpellID()
{
	return m_CurrentSpellID;
};
int cCharacterObject::GetWalkDistance()
{
	return m_WalkDistance;
};

void cCharacterObject::SetWalkDistance(int walk)
{
	m_WalkDistance = walk;
};

ATTACKTYPE cCharacterObject::GetAttackType()
{
	return m_AttackType;
};
void cCharacterObject::SetAttackType(ATTACKTYPE at)
{
	m_AttackType = at;
};
void cCharacterObject::SetAttackType(int at)
{
	switch(at)
	{
	case 0:
		m_AttackType = MELEE;
		break;
	case 1:
		m_AttackType = SPELL;
		break;
	case 2:
		m_AttackType = RANGED;
		break;
	}
};


void cCharacterObject::AddSpellList(int spell)
{
	m_SpellList.push_back(spell);
};

void cCharacterObject::ClearSpellList()
{
	m_SpellList.clear();
};


void cCharacterObject::AddEffect(cEffectObject *effect)
{
	hge->System_Log("CharacterAddEffect");
	if (!UpdateCurrentEffect(effect->Id()))
	{
		//Create a new instance of the effect
		cEffectObject *myeffect = new cEffectObject(effect);
		myeffect->Activate();
		myeffect->AssignTarget(this);
		m_EffectList.push_back(myeffect);
	}
   //hge->System_Log("Effect List size: %i",m_EffectList.size());
};

bool cCharacterObject::UpdateCurrentEffect(int effectID)
{ //If the effect already exists the reset its duration
	bool rtnCode = false;

	vector<cEffectObject*>::iterator iter;
	for (iter = m_EffectList.begin();iter< m_EffectList.end();iter++)
	{
		if ((*iter)->Id() == effectID)
		{
			(*iter)->ResetDuration();
			rtnCode = true;
			break;
		}
	}
	return rtnCode;
};

void cCharacterObject::ClearEffectList(bool forced)
{

	cEffectObject *effect;
	vector<cEffectObject*>::iterator iter;
	for (iter = m_EffectList.begin();iter< m_EffectList.end();iter++)
	{
		if ((*iter)->Finished() || forced)
		{
			effect = *iter;
			delete effect;

		}
	}
	m_EffectList.clear();
};

void cCharacterObject::SetAttackAniMode(int mode)
{
	if (m_Attack) m_Attack->SetMode(mode);
};

void cCharacterObject::MoveTo(int x, int y, ISODIR dir)
{
	CMobileObject::MoveTo(x, y, dir);
	DoAction(WALK);
};

bool cCharacterObject::PathFindTo(int x, int y)
{
	if (CMobileObject::PathFindTo(x, y))
	{
		DoAction(WALK);
		m_GameParent->GetSoundManager()->PlayWalkSFX();
	}else
	{
		hge->System_Log("NOPATH8");
		m_Status = NOPATH;
		return false;
	}
	//play walk sound
	return true;
};

void cCharacterObject::SetShadows(bool shadow)
{
	m_UseShadows = shadow;
	m_Stand->SetShadows(shadow);
	m_Walk->SetShadows(shadow);
	m_Attack->SetShadows(shadow);
	m_Death->SetShadows(shadow);
};

void cCharacterObject::UpdateMove(float fDeltaTime)
{
	m_Time += fDeltaTime;
	if (m_Time > .03f)
	{
		switch(m_CurDirection)
		{
			case iNW:
			{
				if (m_Halfway)
				{
					if( ! (getOffsetY() <= 0)  ) addOffsetY(-HALFNPCSPEEDY);
					if( ! (getOffsetX() <= 0) ) addOffsetX(-NPCSPEEDX);

					if( getOffsetX()<=0 && getOffsetY()<=0 ) {
						m_Halfway =false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( ! (getOffsetY() <= -QUARTERTILEHEIGHT)  ) addOffsetY(-HALFNPCSPEEDY);
					if( ! (getOffsetX() <= -QUARTERTILEWIDTH) ) addOffsetX(-NPCSPEEDX);

					if( getOffsetX()<=-QUARTERTILEWIDTH && getOffsetY()<=-QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetX = QUARTERTILEWIDTH;
						m_MovingOffSetY = QUARTERTILEHEIGHT;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
			case iNE:
			{
				if (m_Halfway)
				{
					if( (getOffsetY() >= 0)  ) addOffsetY(-HALFNPCSPEEDY);
					if( getOffsetX() <= 0) addOffsetX(NPCSPEEDX);

					if( getOffsetX()>=0 && getOffsetY()<=0 ) {
						m_Halfway = false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( !(getOffsetY() <= -QUARTERTILEHEIGHT)  ) addOffsetY(-HALFNPCSPEEDY);
					if( getOffsetX() <= QUARTERTILEWIDTH ) addOffsetX(NPCSPEEDX);

					if( getOffsetX()>=QUARTERTILEWIDTH && getOffsetY()<=-QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetY = QUARTERTILEHEIGHT;
						m_MovingOffSetX = -QUARTERTILEWIDTH;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
			case iSW:
			{
				if (m_Halfway)
				{
					if( ! (getOffsetY() >= 0)  ) addOffsetY(HALFNPCSPEEDY);
					if( ! (getOffsetX() <= 0) ) addOffsetX(-NPCSPEEDX);

					if( getOffsetX()<=0 && getOffsetY()<=0) {
						m_HasMoved = false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( ! (getOffsetY() >= QUARTERTILEHEIGHT)  ) addOffsetY(HALFNPCSPEEDY);
					if( ! (getOffsetX() <= -QUARTERTILEWIDTH) ) addOffsetX(-NPCSPEEDX);

					if( getOffsetX()<=-QUARTERTILEWIDTH && getOffsetY()<=QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetY = -QUARTERTILEHEIGHT;
						m_MovingOffSetX = QUARTERTILEWIDTH;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
			case iSE:
			{
				if (m_Halfway)
				{
					if( getOffsetY() <= 0 ) addOffsetY(HALFNPCSPEEDY);
					if( getOffsetX() <= 0 ) addOffsetX(NPCSPEEDX);

					if( getOffsetX()>=0 && getOffsetY()>=0 ) {
						m_HasMoved = false;
						if (ShouldStop())
                           Stop();
						else
							GetNextPath();
					}
				}
				else
				{
					if( getOffsetY() <= QUARTERTILEHEIGHT ) addOffsetY(HALFNPCSPEEDY);
					if( getOffsetX() <= QUARTERTILEWIDTH ) addOffsetX(NPCSPEEDX);

					if( getOffsetX()>=QUARTERTILEWIDTH && getOffsetY()>=QUARTERTILEHEIGHT ) {
						SetPosition(m_NextDestX,m_NextDestY);
						m_MovingOffSetY = -QUARTERTILEHEIGHT;
						m_MovingOffSetX = -QUARTERTILEWIDTH;
						m_HasMoved = true;
						m_Halfway =true;
					}
				}
				break;
			}
		}
		m_Time = 0;
	}
	m_MoveAgain = false;
}

void cCharacterObject::MouseOver(int x, int y)
{
	int halfwidth;
	m_MouseOver = false;
	if (y < m_ScreenY && y > (m_ScreenY-m_Height))
	{
		halfwidth = m_Width/2;
		if (x > (m_ScreenX-halfwidth) && x < (m_ScreenX+halfwidth))
			m_MouseOver = true;
	}
};

void cCharacterObject::FileName(const char *name)
{
	strcpy(m_FileName,name);
};

char *cCharacterObject::FileName()
{
	return m_FileName;
};

void cCharacterObject::FaceTarget(cCharacterObject* trgt)
{
	CMobileObject::FaceTarget(trgt->GetxPos(),trgt->GetyPos());
};

void cCharacterObject::LoadPainsfx(std::string filename)
{
	m_Painsfx = m_GameParent->GetSoundManager()->LoadSoundFX(filename,false);
};

void cCharacterObject::LoadDeathsfx(std::string filename)
{
	m_Deathsfx = m_GameParent->GetSoundManager()->LoadSoundFX(filename,false);
};

void cCharacterObject::PlayPainsfx()
{
	if (m_Painsfx)
		m_GameParent->GetSoundManager()->PlaySoundFX(m_Painsfx);
};

void cCharacterObject::PlayDeathsfx()
{
	if (m_Deathsfx)
		m_GameParent->GetSoundManager()->PlaySoundFX(m_Deathsfx);

};

int cCharacterObject::TakeDamage(int physical, int magical)
{
	int rtn;
	int tmpPhy = 0;
	int tmpMag = 0;
	int	tmpDamage = 0;

	hge->System_Log("Damage1: %i Hits: %i", m_Damage,m_HitPoints);

	if(physical > 0)
		tmpPhy = physical - (m_PhyResist * physical);

	if(magical > 0)
		tmpMag = magical - (m_MagResist * magical);

	tmpDamage += tmpPhy + tmpMag;
	rtn = tmpDamage; //return the actual amount of damage taken

	m_Damage += tmpDamage;
	m_LastDamage = tmpDamage;
	if (m_Damage >= m_HitPoints)
	{
		m_Dead = true;
		DoAction(DEATH);
	}else
	{
		DoAction(PAIN);
	}
	if (m_Dead) rtn = -1; //-1 if damage killed me

	hge->System_Log("Damage2: %i Hits: %i", m_Damage,m_HitPoints);

	return rtn;
};

void cCharacterObject::SetPhyResist(float resist)
{
	if (resist < 1.0f)
		m_PhyResist = resist;
	else
		m_PhyResist = 0.99f;

};
void cCharacterObject::SetMagResist(float resist)
{
	if (resist < 1.0f)
		m_MagResist = resist;
	else
		m_MagResist = 0.99f;
};

float cCharacterObject::GetPhyResist() { return m_PhyResist; };
float cCharacterObject::GetMagResist() { return m_MagResist; };
int cCharacterObject::GetAttackMin() { return m_DamageMin; };
int cCharacterObject::GetAttackMax() { return m_DamageMax; };

void cCharacterObject::SetAttackMin(int imin){ m_DamageMin = imin; };
void cCharacterObject::SetAttackMax(int imax){ m_DamageMax = imax; };
int cCharacterObject::GetWidth() { return 128; }
int cCharacterObject::GetHeight() { return 128; }
int cCharacterObject::GetSide() { return m_Side; };
void cCharacterObject::SetHitPoints(int HitPoints) { m_HitPoints = HitPoints; };
void cCharacterObject::SetSide(int side) {m_Side = side; };

cCharacterObject* cCharacterObject::GetLastAttacker(){ return m_LastAttacker; };
cCharacterObject* cCharacterObject::GetCurrentTarget(){ return m_CurrentTarget; };
void cCharacterObject::SetLastAttacker(cCharacterObject* npc){ m_LastAttacker = npc; };
void cCharacterObject::SetCurrentTarget(cCharacterObject* npc){ m_CurrentTarget = npc; };
CHARSTATUS cCharacterObject::GetStatus(){ return m_Status; };
void cCharacterObject::SetStatus(CHARSTATUS stat){ m_Status = stat; };
void cCharacterObject::CreatePortrait(int frame)
{
	hge->System_Log("CreatePortrait");
	m_PortraitFrame = frame;
	m_target=hge->Target_Create(128,128,false);
	hge->Gfx_BeginScene(m_target);
	hge->Gfx_Clear(ARGB(100,100,100,100));
	//render
	m_Stand->SetDirection(iSW);
	m_Stand->SetFrame(m_PortraitFrame);
	m_Stand->Render(0.0f,0,0);
	hge->Gfx_EndScene();
    m_Portrait=new hgeSprite(hge->Target_GetTexture(m_target),40,20,50,50);
//	m_Portrait->SetTexture(hge->Target_GetTexture(m_target));

};

void cCharacterObject::RenderPortrait(int x, int y, int w, int h)
{
	m_Portrait->RenderStretch(x, y, w, h);
};

void cCharacterObject::RestorPortrait()
{
		ISODIR dir = m_Stand->GetDir();

		m_Stand->SetDirection(iSW);
		m_Stand->SetFrame(m_PortraitFrame);

		hge->Gfx_BeginScene(m_target);
		hge->Gfx_Clear(0);

		m_Stand->Render(0.0f,0,0);

		hge->Gfx_EndScene();
		m_Portrait->SetTexture(hge->Target_GetTexture(m_target));


		m_Stand->SetDirection(dir);

};

//int cCharacterObject::CameraFocus() { return cameraFocus; }
//int CSprite::curHP() { return curHP; }
//int CSprite::maxHP() { return

