#include "RF2d_Application.h"
#include "RF3d_Global.h"

cSoundManager::cSoundManager(cApp* pParent)
{
	m_pParent = pParent;
	m_bShouldFadeIn = false;
	m_bShouldFadeOut = false;
	m_WalkSFX = NULL;
};

cSoundManager::~cSoundManager()
{
	if (m_MusicChannel) hge->Channel_Stop(m_MusicChannel);
	if (m_Music) hge->Stream_Free(m_Music);
	if (m_WalkSFX) hge->Effect_Free(m_WalkSFX);
	ClearAmbientSounds();
	ClearSoundFXs();
};

void cSoundManager::PlayMusic(std::string filename)
{
	std::string  tmpMusicFileName;
	if (m_Music) hge->Stream_Free(m_Music);
	tmpMusicFileName = m_pParent->GetGamePath(THEME);
	tmpMusicFileName += filename;
	m_Music = hge->Stream_Load(tmpMusicFileName.c_str(),0);
	m_MusicChannel = hge->Stream_Play(m_Music,true);
	hge->Channel_SetVolume(m_MusicChannel,0);
	FadeSoundIn();
};

HEFFECT cSoundManager::LoadSoundFX(std::string filename, bool protect)
{
	std::string  tmpMusicFileName;
	HEFFECT sound;

	bool bFound = false;
	//Seach the Master SFX list and find a pointer to the sfx
	vector<SFXListItem*>::iterator iter;
	for (iter = SoundList.begin();iter < SoundList.end();iter++)
	{
		if ((*iter)->FileName.compare(filename) == 0)
		{
			sound = (*iter)->Sound;
			(*iter)->Used = true;
			bFound = true;
 			//hge->System_Log("Used Existing %s", filename);
			break;
		}
	}

	if (!bFound)
	{
		tmpMusicFileName = m_pParent->GetGamePath(SFX);
		tmpMusicFileName += filename;
		sound = hge->Effect_Load(tmpMusicFileName.c_str(),0);
		SFXListItem *listItem = new SFXListItem;
		listItem->FileName = filename;
		listItem->Sound = sound;
		listItem->Used = true;
		listItem->Protected = protect;
		SoundList.push_back(listItem);
		//hge->System_Log("Loaded New %s", filename);
	}

	return sound;
};
void cSoundManager::PlaySoundFX(HEFFECT effct)
{
	hge->Effect_PlayEx(effct,m_SFXVolume,0,1.0,false);
};

void ClearSoundFX(HEFFECT effect)
{
	hge->Effect_Free(effect);
};

void cSoundManager::PlayAmbientSound(std::string filename)
{
	std::string tmpMusicFileName;
	HCHANNEL channel;
	HEFFECT sound;
	tmpMusicFileName = m_pParent->GetGamePath(AMB);
	tmpMusicFileName += filename;
	sound = hge->Effect_Load(tmpMusicFileName.c_str(),0);
	m_AmbientSounds.push_back(sound);
	channel = hge->Effect_PlayEx(sound,0,0,1.0,true);
	hge->Channel_SetVolume(channel,0);
	FadeSoundIn();
	m_AmbientChannels.push_back(channel);
};

void cSoundManager::LoadWalkSFX(std::string filename)
{
	std::string tmpMusicFileName;
	tmpMusicFileName = m_pParent->GetGamePath(SFX);
	tmpMusicFileName += filename;
	m_WalkSFX = hge->Effect_Load(tmpMusicFileName.c_str());
};

void cSoundManager::PlayWalkSFX()
{
	m_WalkChannel = hge->Effect_PlayEx(m_WalkSFX,m_SFXVolume,0,1.0,true);

};
void cSoundManager::StopWalkSFX()
{
	hge->Channel_Stop(m_WalkChannel);
};


void cSoundManager::ClearAmbientSounds()
{
	HCHANNEL myChannel;
	for (int iloop=0;iloop<m_AmbientChannels.size();iloop++)
	{
		myChannel = m_AmbientChannels[iloop];
		if (myChannel)	hge->Channel_Stop(myChannel);
	}
	m_AmbientChannels.clear();
	HEFFECT myEffect;
	for (int iloop=0;iloop<m_AmbientSounds.size();iloop++)
	{
		myEffect = m_AmbientSounds[iloop];
		if (myEffect) hge->Effect_Free(myEffect);
	}
	m_AmbientSounds.clear();

};

void cSoundManager::ResetUsage()
{
	for (unsigned int iloop=0;iloop<SoundList.size();iloop++)
	{
		SoundList[iloop]->Used = false;
	}
};

void cSoundManager::ClearUnusedSFX()
{
	SFXListItem *sfx;
	//Loop through the ActiveSpellList looking for spells that are finshed
	//In a turn based game this list should never have more than 1 or 2 items to compare
	vector<SFXListItem*>::iterator iter;
	for (iter = SoundList.begin();iter< SoundList.end();)
	{
		if ((*iter)->Used == false && (*iter)->Protected == false)
		{
			sfx = *iter;
			hge->Effect_Free(sfx->Sound);
			delete sfx;
			iter = SoundList.erase(iter);
		}else
            iter++;
	}
};

void cSoundManager::ClearSoundFXs()
{

	SFXListItem *sfx;
	vector<SFXListItem*>::iterator iter;
	for (iter = SoundList.begin();iter< SoundList.end();iter++)
	{
		sfx = *iter;
		hge->Effect_Free(sfx->Sound);
		delete sfx;
	};
	SoundList.clear();

};

void cSoundManager::FadeSoundIn()
{
	m_bShouldFadeIn = true;
	m_bShouldFadeOut = false;
	m_FadeTimer = 0.0f;
	m_FadeVolume = 0;
};

void cSoundManager::FadeSoundOut()
{
	m_bShouldFadeIn = false;
	m_bShouldFadeOut = true;
	m_FadeTimer = 0.0f;
	if (m_MusicVolume <= m_AmbientVolume)
		m_FadeVolume = m_AmbientVolume;
	else
		m_FadeVolume = m_MusicVolume;
};

void cSoundManager::FadeSound(float fDeltaTime)
{
	static int vol;
	m_FadeTimer += fDeltaTime;
	bool bSetVol = false;

	if (m_bShouldFadeIn)
	{
		if (m_FadeTimer >= 0.15)
		{
			if (m_FadeVolume <= m_MusicVolume)
			{
				bSetVol = true;
				hge->Channel_SetVolume(m_MusicChannel,m_FadeVolume);
			}

			if (m_FadeVolume <= m_AmbientVolume)
			{
				bSetVol = true;
				HCHANNEL myChannel;
				for (int iloop=0;iloop<m_AmbientChannels.size();iloop++)
				{
					myChannel = m_AmbientChannels[iloop];
					if (myChannel)	hge->Channel_SetVolume(myChannel,m_FadeVolume);
				}
			}

			if (bSetVol)
			{
				m_FadeVolume++;
				m_FadeTimer = 0.0f;
				bSetVol = false;
			}else
			 m_bShouldFadeIn = false;
		}
	}

	if (m_bShouldFadeOut)
	{
		if (m_FadeTimer >= 0.15)
		{
			if (m_FadeVolume > 0)
			{
				bSetVol = true;
				if (m_FadeVolume == m_MusicVolume-1)
				 hge->Channel_SetVolume(m_MusicChannel,m_FadeVolume);

				HCHANNEL myChannel;
				if (m_FadeVolume == m_AmbientVolume-1)
				for (int iloop=0;iloop<m_AmbientChannels.size();iloop++)
				{
					myChannel = m_AmbientChannels[iloop];
					if (myChannel)	hge->Channel_SetVolume(myChannel,m_FadeVolume);
				}

			}
			if (bSetVol)
			{
				m_FadeVolume--;
				m_FadeTimer = 0.0f;
			}else
			 m_bShouldFadeOut = false;
		}
	}
};

void cSoundManager::MusicVolume(int vol)
{
	if (m_MusicVolume > 100) m_MusicVolume = 100;
	if (m_MusicVolume < 0) m_MusicVolume = 0;
	m_MusicVolume = vol;
	if (m_MusicChannel)
		hge->Channel_SetVolume(m_MusicChannel,vol);
	hge->Ini_SetInt("sound","musicvol",vol);
};
int	cSoundManager::MusicVolume()
{
	return m_MusicVolume;
};

void cSoundManager::SFXVolume(int vol)
{
	if (m_SFXVolume > 100) m_SFXVolume = 100;
	if (m_SFXVolume < 0) m_SFXVolume = 0;
	m_SFXVolume = vol;
	hge->Ini_SetInt("sound","sfxvol",vol);
};
int cSoundManager::SFXVolume()
{
	return m_SFXVolume;
};

void cSoundManager::AbmVolume(int vol)
{
	if (m_AmbientVolume > 100) m_AmbientVolume = 100;
	if (m_AmbientVolume < 0) m_AmbientVolume = 0;
	m_AmbientVolume = vol;
	hge->Ini_SetInt("sound","abmvol",vol);
	HCHANNEL myChannel;
	for (int iloop=0;iloop<m_AmbientChannels.size();iloop++)
	{
		myChannel = m_AmbientChannels[iloop];
		if (myChannel)	hge->Channel_SetVolume(myChannel,m_AmbientVolume);
	}

};
int cSoundManager::AbmVolume()
{
	return m_AmbientVolume;
};
