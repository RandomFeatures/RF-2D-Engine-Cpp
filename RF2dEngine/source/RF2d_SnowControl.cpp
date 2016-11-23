#include "RF2d_SnowControl.h"
#include "RF2d_SnowFlake.h"
#include "RF2d_Application.h"
#include "RF2d_StaticTextures.h"

cSnowControl::cSnowControl(cApp *pParent)
{
	int iLoop;

	m_Enabled = false;
	m_pParent = pParent;
	m_Wind = 1;
	m_Flakes = 300;

	m_ScreenHeight = m_pParent->GetScreenHeight();
	m_ScreenWidth = m_pParent->GetScreenWidth();

	cSnowFlake* snowflake;
	for (iLoop=0;iLoop<m_Flakes;iLoop++)
	{
		snowflake = new cSnowFlake(m_ScreenWidth,m_ScreenHeight);
		SnowList.push_back(snowflake);
	}

	//show sprite
	m_SnowFlake=new hgeSprite(m_pParent->GetStaticTexture()->GetSnowTexture(), 0, 0, 32, 32);
	//m_SnowFlake->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
	m_SnowFlake->SetHotSpot(0,0);

};

cSnowControl::~cSnowControl()
{
	cSnowFlake* snowflake;
	for (unsigned int iloop=0;iloop<SnowList.size();iloop++)
	{
		snowflake = SnowList[iloop];
		delete (cSnowFlake*)snowflake;
	}
	SnowList.clear();
};

void cSnowControl::SetEnabled(bool enabled){ m_Enabled = enabled; };
void cSnowControl::SetWindSpeed(int wind) { m_Wind = wind; };
void cSnowControl::SetFlakeCount(int flakes)
{
	bool bEnabledState = m_Enabled;
	m_Enabled = false;
	m_Flakes = flakes;
	//Delete existing flakes
	cSnowFlake* snowflake;
	for (unsigned int iloop=0;iloop<SnowList.size();iloop++)
	{
		snowflake = SnowList[iloop];
		delete (cSnowFlake*)snowflake;
	}
	SnowList.clear();

	//make a new lis with the new count
	for (unsigned int iloop=0;iloop<m_Flakes;iloop++)
	{
		snowflake = new cSnowFlake(m_ScreenWidth,m_ScreenHeight);
		SnowList.push_back(snowflake);
	}
	//restore the enabled state
	m_Enabled = bEnabledState;

};
void cSnowControl::Update(float fDeltaTime)
{
	if (m_Enabled)
	{
		for (unsigned int iloop=0;iloop<SnowList.size();iloop++)
		{
			SnowList[iloop]->Update(m_Wind);
		}
	}
};
void cSnowControl::Render()
{
	if (m_Enabled)
	{
		for (unsigned int iloop=0;iloop<SnowList.size();iloop++)
		{
			m_SnowFlake->RenderEx(SnowList[iloop]->GetXPos(), SnowList[iloop]->GetYPos(), 0,SnowList[iloop]->GetSize(),SnowList[iloop]->GetSize());
		}
	}
};
