#include <fstream>
#include <vector>
#include "RF2d_EffectObject.h"
#include "RF2d_Application.h"
#include "RF2d_StaticTextures.h"

cEffectObject::cEffectObject(cApp *pParent)
{
	//Character data file
	m_Effect = NULL;
	m_psi = NULL;
	m_PartSprite = NULL;
	m_pParent = pParent;
	m_Finished = false;
	m_Elapsed = 0;
	m_Duration = 0;
	m_Particlefps = 50.0f;
	m_EffectColor= 0;
	m_Copy = false;
};

cEffectObject::cEffectObject(const cEffectObject& p)
{
	hge->System_Log("effect copy");
	m_Effect = NULL;
	m_psi = NULL;
	m_PartSprite = NULL;
	m_Finished = false;
	m_Copy = true;
	m_Elapsed = 0;
	m_Duration = 0;
	m_EffectColor= 0;
	m_pParent = p.m_pParent;
	if (p.m_Effect) m_Effect = p.m_Effect;
	if (p.m_psi) LoadPSI(p.m_psiFileName, p.m_Particlefps);
	m_EffectColor = p.m_EffectColor;
	m_GUID = p.m_GUID;
	m_Target = p.m_Target;
	m_Duration = p.m_Duration;
	m_ActionFrame = p.m_ActionFrame;
};


cEffectObject::cEffectObject(const cEffectObject* p)
{
	hge->System_Log("effect copy");
	m_Effect = NULL;
	m_psi = NULL;
	m_PartSprite = NULL;
	m_Finished = false;
	m_Elapsed = 0;
	m_Duration = 0;
	m_EffectColor= 0;
	m_Copy = true;
	m_pParent = p->m_pParent;
	if (p->m_Effect) m_Effect = p->m_Effect;
	if (p->m_psi) LoadPSI(p->m_psiFileName, p->m_Particlefps);
	m_EffectColor = p->m_EffectColor;
	m_GUID = p->m_GUID;
	m_Target = p->m_Target;
	m_Duration = p->m_Duration;
	m_ActionFrame = p->m_ActionFrame;
};


cEffectObject::~cEffectObject()
{
	//dont delete resources for copies
	if (!m_Copy)
	{
		if(m_Effect) delete m_Effect;
	}

	if (m_PartSprite) delete m_PartSprite;
	if (m_psi) delete m_psi;


};

void cEffectObject::Render(float fDeltaTime, float xPos, float yPos)
{
	if ((m_Effect) && m_Effect->IsLoaded() && !m_Finished)
	{
		m_Effect->Render(fDeltaTime, xPos, yPos);
		if (m_psi)
		{
			//m_psi->MoveTo(xPos,yPos);
			m_psi->Update(fDeltaTime);
			m_psi->Render();
		}
	}
};

void cEffectObject::Update(float fDeltaTime)
{
	if (m_Finished) return;

	//No need to track for instant and indefant
	if (m_Duration > 0) m_Elapsed += fDeltaTime;


	if (m_Duration == 0 && !m_Effect->IsPlaying())
	{//0=instant so only last the duration of the animation
		m_Finished = true;
	}else
	{//if m_Duration is -1 just keep going
		if (m_Duration > 0) //otherwise stop when time expires
			if (m_Elapsed > m_Duration)
			{
				if(m_Effect)
                    m_Effect->Stop();
				m_Finished = true;
			}
	}

};

void cEffectObject::SetColor(int red, int green, int blue)
{
	m_EffectColor = ARGB(255,red,green,blue);
};

DWORD cEffectObject::GetColor()
{
	return m_EffectColor;
};

void cEffectObject::Activate()
{
	if(m_Effect)
        m_Effect->Play();
	if (m_psi)
		m_psi->Fire();
};
void cEffectObject::DeActivate()
{
	if(m_Effect)
        m_Effect->Stop();
	m_Finished = true;
};

void cEffectObject::AssignTarget(cCharacterObject *target)
{
	m_Target = target;

	if (m_psi)
		m_psi->MoveTo(m_Target->GetScreenX() ,m_Target->GetScreenY());
};

void cEffectObject::ResetDuration()
{ //Start the duration timer over;
	m_Elapsed = 0;
};

void cEffectObject::LoadEffect(std::string fileName)
{
	hge->System_Log("EffectLoadEffect");
	char tmpFilePath[256];
	char BaseName[100];
	char tmpFileName[150];
	ifstream inFile;
	int iFrames;
	int iMode;
	float iFPS;
	float iWidth, iHeight;
	bool iOneFace;

		//Open the Script File
	strcpy(tmpFileName,m_pParent->GetGamePath(DATA));
	strcat(tmpFileName,fileName.c_str());

	inFile.open(tmpFileName, ::ios::in);
	if(inFile.bad()) exit(-1);

	/********Load Effect**********/
	//Construct the texture file name
	inFile >> BaseName;
	strcpy(tmpFilePath, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpFilePath, BaseName);
	//Load the animation data
	inFile >> iFrames >> iFPS >> iWidth >> iHeight >> iOneFace >> iMode >> m_ActionFrame >> m_Duration;
	if (iFrames > 0)// Frames are assigned
	{
		//Create the HGEAnimation object
		m_Effect = new AniStorage();
		m_Effect->Init(tmpFilePath, iFrames, iFPS, iWidth, iHeight, iOneFace, true);
		m_Effect->SetBlendMode(BLEND_ALPHAADD);
		m_Effect->SetMode(iMode);
		//ToDO -- Add hotspot to dat file
		m_Effect->SetHotSpot(64,96);
	}

	//Close the file handle
	inFile.close();
};

void cEffectObject::LoadPSI(std::string fileName, float fps)
{
	hge->System_Log("EffectLoadPSI %s", fileName.c_str());
	char tmpFilePath[256];
	//Load Particle Texture
	if (!m_PartSprite)
	m_PartSprite=new hgeSprite(m_pParent->GetStaticTexture()->GetParticleTexture(), 0, 0, 32, 32);
	m_PartSprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD);
	m_PartSprite->SetHotSpot(16,16);
	m_psiFileName = fileName;
	strcpy(tmpFilePath, m_pParent->GetGamePath(PARTICLE));
	strcat(tmpFilePath, fileName.c_str());
	m_Particlefps = fps;
	if (!m_psi)
	m_psi=new hgeParticleSystem(tmpFilePath,m_PartSprite);
	//m_psi->info.nEmission=120;
};

bool cEffectObject::Finished() { return m_Finished; };
void cEffectObject::AssignDuration(float dur){ m_Duration = dur; };
void cEffectObject::AssignId(int idNo) { m_GUID = idNo; };
float cEffectObject::TimeRemaining(){ return m_Duration; };
int cEffectObject::Id() { return m_GUID; };











