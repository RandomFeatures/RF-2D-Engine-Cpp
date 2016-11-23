#pragma warning(disable:4244)
#include "resource.h"
#include "RF3d_Global.h"

#include "hgedistort.h"

#include <math.h>
/*
Render shadows from clouds moving across the ground 
supports clouds moving in various wind directions
*/

using namespace std;

#ifndef RF2D_WATERCONTROL_H
#define RF2D_WATERCONTROL_H

//class cCharacterObject;
class cApp;

class cWaterControl
{
	public:
		cWaterControl(cApp *pParent);
		~cWaterControl();

		void SetEnabled(bool enabled);
		void SetBlend(int alpha);
		void SetWindDir(DIR dir);
		void SetColor(int alpha, int red, int green, int blue);
		void SetColor(DWORD color);
		void Update(float fDeltaTime);
		void Render();
	private:
		cApp		*m_pParent;
		int			m_Blend;
		int			m_FadeBlend;
		DWORD		m_WaterColor;
		bool		m_Enabled;
		int			m_FadeDir;
		DIR			m_WindDir;
		hgeDistortionMesh*	m_Mesh;
		

};

#endif