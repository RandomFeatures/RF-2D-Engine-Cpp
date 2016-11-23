#pragma warning(disable:4244)
#include "resource.h"
#include "RF3d_Global.h"
/*
Render shadows from clouds moving across the ground 
supports clouds moving in various wind directions
*/

using namespace std;

#ifndef RF2D_CLOUDSHADOWS_H
#define RF2D_CLOUDSHADOWS_H

//class cCharacterObject;
class cApp;

class cCloudShadows
{
	public:
		cCloudShadows(cApp *pParent);
		~cCloudShadows();

		void SetEnabled(bool enabled);
		void SetBlend(int alpha);
		void SetWindDir(DIR dir);
		void Update(float fDeltaTime);
		void Render();
	private:
		cApp		*m_pParent;
		int			m_Blend;
		int			m_FadeBlend;
		hgeQuad		m_Shadows;
		bool		m_Enabled;
		int			m_FadeDir;
		DIR			m_WindDir;
		

};

#endif