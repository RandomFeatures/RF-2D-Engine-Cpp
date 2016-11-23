#pragma warning(disable:4244)
#include "resource.h"
#include "RF3d_Global.h"
/*
renders fog moving across the screen
supports different wind directions for moving fog
*/
using namespace std;

#ifndef RF2D_FOGCONTROL_H
#define RF2D_FOGCONTROL_H

//class cCharacterObject;
class cApp;

class cFogControl 
{
	public:
		cFogControl(cApp *pParent);
		~cFogControl();
		void SetTexture(int txt);
		void SetEnabled(bool enabled);
		void SetColor(int alpha, int red, int green, int blue);
		void SetColor(DWORD color);
		void SetType(int fog);
		void SetWindDir(DIR dir);
		void Update(float fDeltaTime);
		void Render(int mode = 0);
	private:
		cApp		*m_pParent;
		DWORD		m_FogColor;
		hgeQuad		m_GroundFog;
		bool		m_Enabled;
		int			m_TopX;
		int			m_TopY;
		int			m_BottomX;
		int			m_BottomY;
		DIR			m_WindDir;
		int			m_FogType;


};

#endif