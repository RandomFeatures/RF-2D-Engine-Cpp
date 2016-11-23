#include "RF2d_Application.h"
#include "RF2d_StaticTextures.h"
#include "RF2d_CloudShadows.h"

/*
Render shadows from clouds moving across the ground
supports clouds moving in various wind directions
*/


cCloudShadows::cCloudShadows(cApp *pParent)
{
	m_Enabled = false;
	m_pParent = pParent;
	m_WindDir = EE;
	m_FadeDir = 1;
	m_Blend = 100;
	m_FadeBlend = 100;
	// Load sound and textures

	m_Shadows.tex=m_pParent->GetStaticTexture()->GetCloudsTexture(1);
	// Set up the quad we will use for background animation
	m_Shadows.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

	for(int i=0;i<4;i++)
	{
		// Set up z-coordinate of vertices
		m_Shadows.v[i].z=0.5f;
		// Set up color. The format of DWORD col is 0xAARRGGBB
		//m_Shadows.v[i].col=ARGB(35,0,0,0);
		m_Shadows.v[i].col=0x23000000;
	}

	m_Shadows.v[0].x=0; m_Shadows.v[0].y=0;
	m_Shadows.v[1].x=800; m_Shadows.v[1].y=0;
	m_Shadows.v[2].x=800; m_Shadows.v[2].y=600;
	m_Shadows.v[3].x=0; m_Shadows.v[3].y=600;

};

cCloudShadows::~cCloudShadows()
{
};

void cCloudShadows::SetEnabled(bool enabled){ m_Enabled = enabled; };
void cCloudShadows::SetBlend(int alpha)
{
	m_Blend = alpha;
	m_FadeBlend = alpha;
	for(int i=0;i<4;i++)
	{
		// Set up color. The format of DWORD col is 0xAARRGGBB
		m_Shadows.v[i].col=ARGB(alpha,0,0,0);
	}

};


void cCloudShadows::SetWindDir(DIR dir){ m_WindDir = dir; };

void cCloudShadows::Update(float fDeltaTime)
{
    static float t=0.0f;
	static float TexSwitch=0.0f;
	static float FadeTimer=0.0f;
	float tx,ty;
	static int i = 1;

	if (m_Enabled)
	{

		if (TexSwitch>20.5f)
		{
			if(FadeTimer>0.5f)
			{
				switch(m_FadeDir)
				{
				case 1://down
						m_FadeBlend--;
						if (m_FadeBlend<1)
						{
							m_FadeBlend=0;
							m_FadeDir = -1;
							m_Shadows.tex=m_pParent->GetStaticTexture()->GetCloudsTexture(i);
							if (i==3){i=0;}else i++;
						};

					break;
				case -1://up
						m_FadeBlend++;
						if (m_FadeBlend>m_Blend)
						{
							m_FadeBlend = m_Blend;
							m_FadeDir = 1;
							TexSwitch=0.0f;
						};
					break;
				}

				//hge->System_Log("blend: %i", m_FadeBlend);
				FadeTimer=0.0f;
				for(int i=0;i<4;i++)
				{
					m_Shadows.v[i].col=ARGB(m_FadeBlend,0,0,0);
				}
			}

			FadeTimer+=fDeltaTime;

		}else TexSwitch+=fDeltaTime;

		t+=fDeltaTime;
		switch (m_WindDir)
		{
		case NN:
		case NW://north
			tx=3*cosf(t/480);
			ty=3*sinf(t/480);
			m_Shadows.v[0].tx=tx;         m_Shadows.v[0].ty=ty;
			m_Shadows.v[1].tx=tx-800/800; m_Shadows.v[1].ty=ty;
			m_Shadows.v[2].tx=tx-800/800; m_Shadows.v[2].ty=ty+600/600;
			m_Shadows.v[3].tx=tx;         m_Shadows.v[3].ty=ty+600/600;
			break;
		case EE:
		case NE:	//east
			ty=3*cosf(t/480);
			tx=3*sinf(t/480);
			m_Shadows.v[0].tx=tx;         m_Shadows.v[0].ty=ty;
			m_Shadows.v[1].tx=tx-800/800; m_Shadows.v[1].ty=ty;
			m_Shadows.v[2].tx=tx-800/800; m_Shadows.v[2].ty=ty-600/600;
			m_Shadows.v[3].tx=tx;         m_Shadows.v[3].ty=ty-600/600;
			break;
		case SE: //southeast
			tx=3*sinf(t/480);
			ty=3*sinf(t/480);
			m_Shadows.v[0].tx=tx;         m_Shadows.v[0].ty=ty;
			m_Shadows.v[1].tx=tx-800/800; m_Shadows.v[1].ty=ty;
			m_Shadows.v[2].tx=tx-800/800; m_Shadows.v[2].ty=ty-600/600;
			m_Shadows.v[3].tx=tx;         m_Shadows.v[3].ty=ty-600/600;
			break;
		case SS://south
			tx=3*cosf(t/480);
			ty=3*sinf(t/480);
			m_Shadows.v[0].tx=tx;         m_Shadows.v[0].ty=ty;
			m_Shadows.v[1].tx=tx-800/800; m_Shadows.v[1].ty=ty;
			m_Shadows.v[2].tx=tx-800/800; m_Shadows.v[2].ty=ty-600/600;
			m_Shadows.v[3].tx=tx;         m_Shadows.v[3].ty=ty-600/600;
			break;
		case WW:
		case SW://west
			ty=3*cosf(t/480);
			tx=3*sinf(t/480);
			m_Shadows.v[0].tx=tx;         m_Shadows.v[0].ty=ty;
			m_Shadows.v[1].tx=tx+800/800; m_Shadows.v[1].ty=ty;
			m_Shadows.v[2].tx=tx+800/800; m_Shadows.v[2].ty=ty-600/600;
			m_Shadows.v[3].tx=tx;         m_Shadows.v[3].ty=ty-600/600;
			break;
		}
	}
}

void cCloudShadows::Render()
{
	if (m_Enabled)
	hge->Gfx_RenderQuad(&m_Shadows);
};
