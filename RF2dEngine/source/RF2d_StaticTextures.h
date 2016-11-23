#include <string>
#include "hge.h"

#ifndef RF2D_STATICTEXTURES_H_INCLUDED
#define RF2D_STATICTEXTURES_H_INCLUDED

class cApp;

class cStaticTextures
{
   public:
    cStaticTextures(cApp*  pParent);
    ~cStaticTextures();
   	HTEXTURE			GetGUITexture();
	HTEXTURE			GetWindowTexture();
	HTEXTURE			GetGlowTexture();
	HTEXTURE			GetFogTexture(int txt);
	HTEXTURE			GetSnowTexture();
	HTEXTURE			GetWaterTexture();
	HTEXTURE			GetCloudsTexture(int txt);
	HTEXTURE			GetParticleTexture();

   private:
    cApp*               m_pGame;
    std::string	        m_TexturePath;
	HTEXTURE			m_GUITex;
	HTEXTURE			m_WindowTex;
	HTEXTURE			m_ParTex;
	HTEXTURE			m_GlowTex;
	HTEXTURE			m_FogTex0;
	HTEXTURE			m_FogTex1;
	HTEXTURE			m_FogTex2;
	HTEXTURE			m_FogTex3;
	HTEXTURE			m_CloudTex0;
	HTEXTURE			m_CloudTex1;
	HTEXTURE			m_CloudTex2;
	HTEXTURE			m_CloudTex3;
	HTEXTURE			m_WaterTex;
	HTEXTURE			m_SnowTex;

};

#endif // RF2D_STATICTEXTURES_H_INCLUDED
