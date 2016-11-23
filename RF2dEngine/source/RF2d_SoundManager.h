#pragma warning(disable:4244)

#include <vector>

using namespace std;

#ifndef RF2D_SOUNDMANAGER_H
#define RF2D_SOUNDMANAGER_H

struct SFXListItem 
{
	bool Used;
	bool Protected;
	std::string FileName;
	HEFFECT Sound;
};


class cSoundManager
{
	public:
		cSoundManager(cApp* pParent);
		~cSoundManager();
		void PlayMusic(std::string filename);
		void PlayAmbientSound(std::string filename);
		HEFFECT LoadSoundFX(std::string filename, bool protect = false);
		void LoadWalkSFX(std::string filename);
		void PlayWalkSFX();
		void StopWalkSFX();
		void PlaySoundFX(HEFFECT effct);
		void ClearSoundFX(HEFFECT effect);
		void ClearAmbientSounds();
		void ClearSoundFXs();
		void FadeSoundIn();
		void FadeSoundOut();
		void FadeSound(float fDeltaTime);
		void MusicVolume(int vol);
		int	MusicVolume();
		void SFXVolume(int vol);
		int	SFXVolume();
		void AbmVolume(int vol);
		int	AbmVolume();
		void ResetUsage();
		void ClearUnusedSFX();
	private:
		cApp*				m_pParent;
		int					m_MusicVolume;
		int					m_SFXVolume;
		int					m_AmbientVolume;
		bool				m_bShouldFadeIn;
		bool				m_bShouldFadeOut;
		float				m_FadeTimer;
		int					m_FadeVolume;
		HSTREAM				m_Music;
		HCHANNEL			m_MusicChannel;
		HEFFECT				m_WalkSFX;
		HCHANNEL			m_WalkChannel;
		vector<HEFFECT>		m_AmbientSounds;
		vector<HCHANNEL>	m_AmbientChannels;
		vector<SFXListItem*> SoundList;

};


#endif