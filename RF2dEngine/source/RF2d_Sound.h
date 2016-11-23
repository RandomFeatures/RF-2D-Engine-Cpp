#pragma warning(disable:4244)
#include "resource.h"

using namespace std;

#ifndef RF2D_SOUND_H
#define RF2D_SOUND_H

//class cCharacterObject;
class cApp;

class cSound 
{
	public:
		cSound(cApp *pParent);
		~cSound();

		bool isOn();
		void SetPan(int pan);
		void SetGUID(std::string guid);
		void SetEnabled(bool enabled);
		void SetInterval(bool random, int inter);
		void Update(float fDeltaTime);
		void LoopSFX();
		void PlayNow();
		void Stop();
		void LoadSFX(std::string filename); 
		std::string GetGUID();

	private:
		cApp		*m_pParent;

		bool		m_Enabled;
		bool		m_Loop;
		bool		m_Random;
		int			m_Pan;
		int			m_Vol;
		int			m_Interval;
		int			m_MaxInterval;
		float		m_Track;
		std::string	m_GUID;
		HEFFECT		m_SFX;
		HCHANNEL	m_Channel;
};

#endif