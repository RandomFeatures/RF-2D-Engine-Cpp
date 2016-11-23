#include "RF2d_Application.h"
#include "RF2d_WaterControl.h"
#include "RF2d_StaticTextures.h"
/*
Render shadows from clouds moving across the ground
supports clouds moving in various wind directions
*/

// Some "gameplay" variables
const int nRows=64;
const int nCols=64;
const float cellw=1024.0f/(nCols-1);
const float cellh=1024.0f/(nRows-1);

float laplas[nRows][nCols], vel[nRows][nCols], arr[nRows][nCols];
float blur_coefficient=0.015f;
float pressure=-30;

float mx, my;

const float meshx=0;
const float meshy=0;



cWaterControl::cWaterControl(cApp *pParent)
{
	m_Enabled = false;
	m_pParent = pParent;
	m_WindDir = EE;
	m_FadeDir = 1;
	m_Blend = 100;
	m_FadeBlend = 100;

	memset(laplas,0,sizeof(laplas));
	memset(vel,0,sizeof(laplas));
	memset(arr,0,sizeof(laplas));

	m_Mesh=new hgeDistortionMesh(nCols, nRows);
	m_Mesh->SetTexture(m_pParent->GetStaticTexture()->GetWaterTexture());
	m_Mesh->SetTextureRect(0,0,1024,1024);
	//m_Mesh->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_ZWRITE);
	m_Mesh->SetBlendMode(BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE);
	m_Mesh->Clear(0xFF000000);

};

cWaterControl::~cWaterControl()
{
	delete m_Mesh;
};

void cWaterControl::SetEnabled(bool enabled){ m_Enabled = enabled; };
void cWaterControl::SetBlend(int alpha)
{
	m_Blend = alpha;
	m_FadeBlend = alpha;

};

void cWaterControl::SetColor(int alpha, int red, int green, int blue)
{
	SetColor(ARGB(alpha,red,green,blue));
};

void cWaterControl::SetColor(DWORD color)
{
	m_WaterColor = color;
};


void cWaterControl::SetWindDir(DIR dir){ m_WindDir = dir; };

void cWaterControl::Update(float fDeltaTime)
{
    static float t=0.0f;
	static float im=1.0f;
	int ax, ay;
	int i, j;


	if (m_Enabled)
	{
		t+=fDeltaTime;
		im-=fDeltaTime;
		// Process keys
		hge->Input_GetMousePos(&mx, &my);
		if(hge->Input_GetKeyState(HGEK_LBUTTON))
		{
			ax=int((mx-meshx)/cellw);
			ay=int((my-meshy)/cellh);
			if(ax>1 && ax<nCols-1 && ay>1 && ay<nRows-1)
			{
				arr[ay][ax]=pressure;
				if(ay+1<nRows-1) arr[ay+1][ax]=pressure;
				if(ax+1<nCols-1) arr[ay][ax+1]=pressure;
				if(ay+1<nRows-1 && ax+1<nCols-1) arr[ay+1][ax+1]=pressure;
			}
		}


		for(i=1;i<nRows-1;i++)
			for(j=1;j<nCols-1;j++)
			{
				laplas[i][j]=-arr[i][j]*4+arr[i+1][j]+arr[i-1][j]+arr[i][j+1]+arr[i][j-1];
				vel[i][j]+=laplas[i][j];
			}

		for(i=1;i<nRows-1;i++)
			for(j=1;j<nCols-1;j++)
				arr[i][j]+=fDeltaTime*vel[i][j];

		for(i=1;i<nRows-1;i++)
			for(j=1;j<nCols-1;j++)
				arr[i][j]+=fDeltaTime*vel[i][j]+blur_coefficient*laplas[i][j];

		for(i=0;i<nRows;i++)
		for(j=0;j<nCols;j++)
		{
			m_Mesh->SetDisplacement(j,i,0,arr[i][j],HGEDISP_NODE);
			m_Mesh->SetColor(j,i,m_WaterColor);
			//m_Mesh->SetColor(j,i,0xFF<<24 | nCols<<16 | (nCols/2)<<8);
		}

	}
}

void cWaterControl::Render()
{


	if (m_Enabled)
	{
		// Render graphics
		m_Mesh->Render(0, 0);
	}

};
