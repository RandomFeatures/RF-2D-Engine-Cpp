#pragma warning(disable:4244)
#include "resource.h"

using namespace std;

#ifndef RF2D_LIGHT_H
#define RF2D_LIGHT_H

//class cCharacterObject;
class cApp;

class cLight 
{
	public:
		cLight(cApp *pParent);
		~cLight();

		bool isOn();
		void SetPosition(int x, int y);
		void SetOffset(int x, int y);
		void SetGUID(char *guid);
		void SetId(int idNo);
		void SetEnabled(bool enabled);
		void SetSize(float width, float height);
		void SetColor(int alpha, int red, int green, int blue);
		void SetColor(DWORD color);
		void SetFlickerCount(int count);
		void Render(float fDeltaTime, float camX, float camY);
	private:
		cApp		*m_pParent;
		DWORD		m_GlowColor;
		hgeSprite*	m_GlowSprite;

		bool		m_OnScreen;
		bool		m_Enabled;
		int			m_Flicker;
		int			m_PosX;
		int			m_PosY;
		int			m_xOffset; 
		int			m_yOffset;
		int			m_ID;
		float		m_ScreenX;
		float		m_ScreenY;
		float		m_Width;
		float		m_Height;
		float		m_FlickTimer;
		char		m_GUID[20];

};

#endif