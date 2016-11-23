#include "gui_support_CharSelectItem.h"

cCharSelectItem_gui_support::cCharSelectItem_gui_support(float phy, float mag, int defrate, int hitrate, int hps, int attrate, int min, int max, int moverate, int dir, int x, int y, int type, int spellID, int movement, std::string guid, std::string file, std::string name, std::string texpath)
{
	m_PhyResist = phy;
	m_MagResist = mag;
	m_AttackMin = min;
	m_AttackMax = max;
	m_HitPoints = hps;
	m_AttackRating = attrate;
	m_DefRating = defrate;
	m_HitRating = hitrate;
	m_MoveRating = moverate;
	m_Movement = movement;
	m_Dir = dir;
	m_X = x;
	m_Y = y;
	m_Type = type;
	m_GUID = guid;
	m_SpellID = spellID;
	m_FileName = file;
	m_Name = name;
	m_TexPath = texpath;
	m_Loaded = false;
	m_Debug = false;
	m_Stand = NULL;
	m_Selected = false;
};

cCharSelectItem_gui_support::~cCharSelectItem_gui_support()
{
	if (m_Stand)
		delete m_Stand;
};

void cCharSelectItem_gui_support::LoadStand()
{
	if (m_Loaded) return;

	ifstream inFile;
	char BaseName[150];
	char tmpBaseName[150];
	int iFrames;
	float iFPS;
	float iWidth, iHeight;
	int iPort;
	inFile.open(m_FileName.c_str(), ::ios::in);
	if(inFile.bad()) exit(-1);
	//Image File
	inFile >> BaseName;
	//Load Stand
	strcpy(tmpBaseName, m_TexPath.c_str());
	strcat(tmpBaseName, BaseName);
	inFile >> iPort >> iFrames >> iFPS >> iWidth >> iHeight;
	iFPS = hge->Random_Float(0.01f, 0.50f);
	m_Stand = new AniStorage();
	m_Stand->Init(tmpBaseName, iFrames, iFPS, iWidth, iHeight, false, false);
	m_Stand->SetMode(HGEANIM_FWD);
	m_Stand->SetHotSpot(iWidth/2,iHeight - 12);
	m_Stand->SetShadows(false);
	inFile.close();
	m_Loaded = true;
};

void cCharSelectItem_gui_support::Select(bool sel){ m_Selected = sel; };
bool cCharSelectItem_gui_support::IsSelected() { return m_Selected; };
void cCharSelectItem_gui_support::SetDirection(ISODIR dir)
{
	m_Stand->SetDirection(dir);
	m_Dir = dir;
};

void cCharSelectItem_gui_support::SetColor(DWORD color)
{
	m_Stand->SetColor(color);
};

void cCharSelectItem_gui_support::RenderInWindow(float fDeltaTime, float x, float y)
{
	m_Stand->SetDirection(iSW);
	m_Stand->Render(fDeltaTime, x, y);
	m_Stand->SetDirection((ISODIR)m_Dir);
}

void cCharSelectItem_gui_support::RenderInGame(float fDeltaTime, float camX, float camY)
{
	//Figure out the exact world cord
	float ScreenX;
	float ScreenY;

	//Figure out the exact world cord
	int WorldX = m_X * TILEWIDTH;
	int WorldY = m_Y * TILEHALFHEIGHT;

	//translate world X,Y to Screen X,Y
	if (m_Y % 2 == 0)
		ScreenX =  ((WorldX) - camX)-TILEHALFWIDTH;
	else
		ScreenX =  ((WorldX) - camX);

	ScreenY = ((WorldY) - camY)+15;

	m_Stand->Render(fDeltaTime, ScreenX, ScreenY);
	//m_Stand->Render(fDeltaTime, 100, 100);
}


void cCharSelectItem_gui_support::X(int x){ m_X= x; };
void cCharSelectItem_gui_support::Y(int y){ m_Y = y; };
float cCharSelectItem_gui_support::Physical(){ return m_PhyResist; };
float cCharSelectItem_gui_support::Magical(){ return m_MagResist; };
int cCharSelectItem_gui_support::AttackMin(){ return m_AttackMin; };
int cCharSelectItem_gui_support::AttackMax(){ return m_AttackMax; };
int cCharSelectItem_gui_support::HitPoints(){ return m_HitPoints; };
int cCharSelectItem_gui_support::AttackRating(){ return m_AttackRating; };
int cCharSelectItem_gui_support::HitRating(){ return m_HitRating; };
int cCharSelectItem_gui_support::DefRating(){ return m_DefRating; };
int cCharSelectItem_gui_support::Movement(){ return m_Movement; };
int cCharSelectItem_gui_support::MoveRating(){ return m_MoveRating; };

int cCharSelectItem_gui_support::Dir(){ return m_Dir; };
int cCharSelectItem_gui_support::X(){ return m_X; };
int cCharSelectItem_gui_support::Y(){ return m_Y; };
int cCharSelectItem_gui_support::Type(){ return m_Type; };
std::string cCharSelectItem_gui_support::GUID(){ return m_GUID; };
std::string cCharSelectItem_gui_support::FileName(){ return m_FileName; };
std::string cCharSelectItem_gui_support::Name(){ return m_Name; };
int cCharSelectItem_gui_support::SpellID() { return m_SpellID; };
bool cCharSelectItem_gui_support::IsLoaded(){ return m_Loaded; };
void cCharSelectItem_gui_support::SetDebug(){ m_Debug = true; };
