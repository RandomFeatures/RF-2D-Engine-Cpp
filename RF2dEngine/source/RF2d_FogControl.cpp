#include "RF2d_Application.h"
#include "RF2d_FogControl.h"
#include "RF2d_StaticTextures.h"
/*
renders fog moving across the screen
supports different wind directions for moving fog
*/

cFogControl::cFogControl(cApp *pParent)
{
	m_Enabled = false;
	m_pParent = pParent;
	m_WindDir = EE;
	m_FogType = 1;

};

cFogControl::~cFogControl()
{

};

void cFogControl::SetTexture(int txt)
{
	// Load sound and textures
	m_GroundFog.tex=m_pParent->GetStaticTexture()->GetFogTexture(txt);
	// Set up the quad we will use for background animation
	m_GroundFog.blend= BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

	for(int i=0;i<4;i++)
	{
		// Set up z-coordinate of vertices
		m_GroundFog.v[i].z=0.5f;
		// Set up color. The format of DWORD col is 0xAARRGGBB
		m_GroundFog.v[i].col=0xFFFFFFFF;;//0xFFFFFFFF;
	}

	m_GroundFog.v[0].x=0; m_GroundFog.v[0].y=0;
	m_GroundFog.v[1].x=800; m_GroundFog.v[1].y=0;
	m_GroundFog.v[2].x=800; m_GroundFog.v[2].y=600;
	m_GroundFog.v[3].x=0; m_GroundFog.v[3].y=600;

};

void cFogControl::SetEnabled(bool enabled){ m_Enabled = enabled; };
void cFogControl::SetType(int fog)
{
	if (fog == 0)
		m_FogType = fog;
	else
		m_FogType = 1;

};

void cFogControl::SetColor(int alpha, int red, int green, int blue)
{
	SetColor(ARGB(alpha,red,green,blue));
};

void cFogControl::SetColor(DWORD color)
{
	m_FogColor = color;
	//m_FogHeavy->SetColor(m_FogColor);
	//m_FogLight->SetColor(m_FogColor);

	for(int i=0;i<4;i++)
	{
		// Set up color. The format of DWORD col is 0xAARRGGBB
		m_GroundFog.v[i].col=color;
	}
};

void cFogControl::SetWindDir(DIR dir){ m_WindDir = dir; };

void cFogControl::Update(float fDeltaTime)
{
    static float t=0.0f;
	float tx,ty;

	if (m_Enabled)
	{
		t+=fDeltaTime;

		switch (m_WindDir)
		{
		case NN:
		case NW://north
			tx=5*cosf(t/480);
			ty=5*sinf(t/480);
			m_GroundFog.v[0].tx=tx;         m_GroundFog.v[0].ty=ty;
			m_GroundFog.v[1].tx=tx-800/800; m_GroundFog.v[1].ty=ty;
			m_GroundFog.v[2].tx=tx-800/800; m_GroundFog.v[2].ty=ty+600/600;
			m_GroundFog.v[3].tx=tx;         m_GroundFog.v[3].ty=ty+600/600;
			break;
		case EE:
		case NE:	//east
			ty=3*cosf(t/480);
			tx=3*sinf(t/480);
			m_GroundFog.v[0].tx=tx;         m_GroundFog.v[0].ty=ty;
			m_GroundFog.v[1].tx=tx-800/800; m_GroundFog.v[1].ty=ty;
			m_GroundFog.v[2].tx=tx-800/800; m_GroundFog.v[2].ty=ty-600/600;
			m_GroundFog.v[3].tx=tx;         m_GroundFog.v[3].ty=ty-600/600;
			break;
		case SE: //southeast
			tx=5*sinf(t/480);
			ty=5*sinf(t/480);
			m_GroundFog.v[0].tx=tx;         m_GroundFog.v[0].ty=ty;
			m_GroundFog.v[1].tx=tx-800/800; m_GroundFog.v[1].ty=ty;
			m_GroundFog.v[2].tx=tx-800/800; m_GroundFog.v[2].ty=ty-600/600;
			m_GroundFog.v[3].tx=tx;         m_GroundFog.v[3].ty=ty-600/600;
			break;
		case SS://south
			tx=5*cosf(t/480);
			ty=5*sinf(t/480);
			m_GroundFog.v[0].tx=tx;         m_GroundFog.v[0].ty=ty;
			m_GroundFog.v[1].tx=tx-800/800; m_GroundFog.v[1].ty=ty;
			m_GroundFog.v[2].tx=tx-800/800; m_GroundFog.v[2].ty=ty-600/600;
			m_GroundFog.v[3].tx=tx;         m_GroundFog.v[3].ty=ty-600/600;
			break;
		case WW:
		case SW://west
			ty=5*cosf(t/480);
			tx=5*sinf(t/480);
			m_GroundFog.v[0].tx=tx;         m_GroundFog.v[0].ty=ty;
			m_GroundFog.v[1].tx=tx+800/800; m_GroundFog.v[1].ty=ty;
			m_GroundFog.v[2].tx=tx+800/800; m_GroundFog.v[2].ty=ty-600/600;
			m_GroundFog.v[3].tx=tx;         m_GroundFog.v[3].ty=ty-600/600;
			break;
		}
	}
}
void cFogControl::Render(int mode)
{
	if (m_Enabled && m_FogType == mode)
		hge->Gfx_RenderQuad(&m_GroundFog);


};
