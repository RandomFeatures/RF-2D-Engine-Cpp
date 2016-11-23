#include "RF2d_Application.h"
#include "RF3d_Global.h"


cSound::cSound(cApp *pParent)
{
	m_Enabled = true;
	m_pParent = pParent;
	m_Pan = 0;
	m_Track = 0.0f;
	m_Vol = m_pParent->GetSoundManager()->SFXVolume();
	m_Loop = false;
};

cSound::~cSound()
{
	Stop();
};

bool cSound::isOn() { return m_Enabled; };
void cSound::SetPan(int pan) { m_Pan = pan; };
void cSound::SetGUID(std::string guid) { m_GUID = guid; };
void cSound::SetEnabled(bool enabled){ m_Enabled = enabled; };
void cSound::LoopSFX() { m_Loop = true; };
std::string cSound::GetGUID(){ return m_GUID; };
void cSound::LoadSFX(std::string filename)
{
	m_SFX = m_pParent->GetSoundManager()->LoadSoundFX(filename.c_str(),false);
};

void cSound::SetInterval(bool random, int inter)
{
	m_Random = random;
	m_Interval = inter;
	m_MaxInterval = inter;
};

void cSound::PlayNow()
{
	m_Channel = hge->Effect_PlayEx(m_SFX,m_Vol,m_Pan,1.0,m_Loop);
	m_Enabled = true;
};

void cSound::Stop()
{
	if (m_Channel)
		hge->Channel_Stop(m_Channel);
	m_Enabled = false;
};
void cSound::Update(float fDeltaTime)
{
	int tmpVol = m_pParent->GetSoundManager()->SFXVolume();
	if (!m_Enabled) return;

	if (m_Loop)
	{
		if (m_Vol != tmpVol)
		{
			m_Vol = tmpVol;
			hge->Channel_SetVolume(m_Channel, m_Vol);
		}
	}else
	{
		if (m_Vol != tmpVol)
			m_Vol = tmpVol;

		m_Track += fDeltaTime;

		if (m_Track >= m_Interval)
		{
			PlayNow();
			m_Track = 0.0f;
			if (m_Random)
				m_Interval = hge->Random_Int(0,m_MaxInterval);
		}
	}
};
