#include "RF2d_CloudShadows.h"
#include "RF2d_FogControl.h"
#include "RF2d_SnowControl.h"
#include "RF2d_WeatherControl.h"
#include "RF2d_WaterControl.h"


cWeatherControl::cWeatherControl(cApp* pParent)
{
	m_pParent = pParent;
	m_bGroundFog = false;
	m_bFog = false;
	m_bClouds = false;
	m_bSnow = false;
	m_bWater = false;
	m_CloudShadows = new cCloudShadows(m_pParent);
	m_FogControl = new cFogControl(m_pParent);
	m_SnowControl = new cSnowControl(m_pParent);
	m_WaterControl = new cWaterControl(m_pParent);
};

cWeatherControl::~cWeatherControl()
{
	delete m_CloudShadows;
	delete m_FogControl;
	delete m_SnowControl;
	delete m_WaterControl;

};

void cWeatherControl::SetWindDir(DIR dir)
{ 
	m_WindDir = dir; 
	m_FogControl->SetWindDir(dir);
	m_CloudShadows->SetWindDir(dir);
	m_WaterControl->SetWindDir(dir);
};

void cWeatherControl::CreateFog(int alpha, int red, int green, int blue, int fogtype)
{
	m_FogControl->SetTexture(fogtype); 
	m_FogControl->SetColor(alpha,red,green,blue);
	m_FogControl->SetType(1);
	m_FogControl->SetEnabled(true);
	m_bFog = true;
};
void cWeatherControl::CreateGroundFog(int alpha, int red, int green, int blue, int fogtype)
{
	m_FogControl->SetTexture(fogtype); 
	m_FogControl->SetColor(alpha,red,green,blue);
	m_FogControl->SetType(0);
	m_FogControl->SetEnabled(true); 
	m_bGroundFog = true;

};

void cWeatherControl::CreateWater(int alpha, int red, int green, int blue)
{
	m_WaterControl->SetColor(alpha,red,green,blue);
	m_WaterControl->SetEnabled(true); 
	m_bWater = true;

};

void cWeatherControl::CreateSnow(int flakes)
{
	m_SnowControl->SetEnabled(true);
	m_bSnow = true;
	if (flakes > 0)
		m_SnowControl->SetFlakeCount(flakes); 
};
void cWeatherControl::CreateClouds(int alpha)
{
	if (alpha>0) 
		m_CloudShadows->SetBlend(alpha);
	m_CloudShadows->SetEnabled(true);
	m_bClouds = true;
};

void cWeatherControl::NoWeather()
{
	m_CloudShadows->SetEnabled(false);
	m_bClouds = false;
	m_SnowControl->SetEnabled(false);
	m_bSnow = false;
	m_FogControl->SetEnabled(false);
	m_bGroundFog = false;
	m_bFog = false;
	m_WaterControl->SetEnabled(false);
	m_bWater = false;
};
void cWeatherControl::Update(float dt)
{
	if (m_bClouds) m_CloudShadows->Update(dt); 
	if (m_bFog || m_bGroundFog) m_FogControl->Update(dt);
	if (m_bSnow) m_SnowControl->Update(dt);
	if (m_bWater) m_WaterControl->Update(dt);
};

void cWeatherControl::RenderGroundFog()
{
	if (m_bGroundFog) m_FogControl->Render(0);
};

void cWeatherControl::RenderFog()
{
	if (m_bFog) m_FogControl->Render(1);
};

void cWeatherControl::RenderClouds()
{
	if (m_bClouds) m_CloudShadows->Render();
};

void cWeatherControl::RenderSnow()
{
	if (m_bSnow) m_SnowControl->Render();
};

void cWeatherControl::RenderWater()
{
	if (m_bWater) m_WaterControl->Render();
};

