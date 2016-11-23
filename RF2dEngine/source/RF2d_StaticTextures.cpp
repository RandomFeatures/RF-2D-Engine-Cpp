#include "RF2d_Application.h"
#include "RF2d_StaticTextures.h"
#include "RF3d_Global.h"

cStaticTextures::cStaticTextures(cApp*  pParent)
{
    char tmpFilePath[128];

    m_TexturePath = pParent->GetGamePath(TEXTURE);

	//Load Cursor Texture
	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\gui.png");
	m_GUITex=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\windowcontrols.png");
	m_WindowTex=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\particles.png");
	m_ParTex=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\light.png");
	m_GlowTex=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\fog0.png");
	m_FogTex0=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\fog1.png");
	m_FogTex1=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\fog2.png");
	m_FogTex2=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\fog3.png");
	m_FogTex3=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\snow.png");
	m_SnowTex=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\clouds0.png");
	m_CloudTex0=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\clouds1.png");
	m_CloudTex1=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\clouds2.png");
	m_CloudTex2=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\clouds3.png");
	m_CloudTex3=hge->Texture_Load(tmpFilePath);

	strcpy(tmpFilePath, m_TexturePath.c_str());
	strcat(tmpFilePath, "\\gui\\water.png");
	m_WaterTex=hge->Texture_Load(tmpFilePath);

}

cStaticTextures::~cStaticTextures()
{
 	if (m_GUITex) hge->Texture_Free(m_GUITex);
	if (m_WindowTex) hge->Texture_Free(m_WindowTex);
	if (m_ParTex) hge->Texture_Free(m_ParTex);
	if (m_GlowTex) hge->Texture_Free(m_GlowTex) ;
	if (m_FogTex0) hge->Texture_Free(m_FogTex0);
	if (m_FogTex1) hge->Texture_Free(m_FogTex1);
	if (m_FogTex2) hge->Texture_Free(m_FogTex2);
	if (m_FogTex3) hge->Texture_Free(m_FogTex3);
	if (m_CloudTex0) hge->Texture_Free(m_CloudTex0);
	if (m_CloudTex1) hge->Texture_Free(m_CloudTex1);
	if (m_CloudTex2) hge->Texture_Free(m_CloudTex2);
	if (m_CloudTex3) hge->Texture_Free(m_CloudTex3);
	if (m_WaterTex) hge->Texture_Free(m_WaterTex);
	if (m_SnowTex) hge->Texture_Free(m_SnowTex);
}

HTEXTURE cStaticTextures::GetGUITexture() { return m_GUITex; };
HTEXTURE cStaticTextures::GetWindowTexture() { return m_WindowTex; };
HTEXTURE cStaticTextures::GetGlowTexture() { return m_GlowTex; };
HTEXTURE cStaticTextures::GetParticleTexture() { return m_ParTex; };
HTEXTURE cStaticTextures::GetSnowTexture() { return m_SnowTex; };
HTEXTURE cStaticTextures::GetWaterTexture() { return m_WaterTex; };
HTEXTURE cStaticTextures::GetCloudsTexture(int txt)
{
	HTEXTURE myTxt = NULL;
	switch(txt)
	{
	case 0:
		myTxt = m_CloudTex0;
		break;
	case 1:
		myTxt = m_CloudTex1;
		break;
	case 2:
		myTxt = m_CloudTex2;
		break;
	case 3:
		myTxt = m_CloudTex3;
		break;
	}
	return myTxt;
};

HTEXTURE cStaticTextures::GetFogTexture(int txt)
{
	HTEXTURE myTxt = NULL;
	switch(txt)
	{
	case 0:
		myTxt = m_FogTex0;
		break;
	case 1:
		myTxt = m_FogTex1;
		break;
	case 2:
		myTxt = m_FogTex2;
		break;
	case 3:
		myTxt = m_FogTex3;
		break;
	}
	return myTxt;
};

