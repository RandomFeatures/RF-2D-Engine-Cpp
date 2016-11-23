#include "RF3d_Global.h"
#include "resource.h"

/*
render mouse cursor and trailing particle effects
track mouse world position
track mouse screen position
*/

#pragma warning(disable:4244)
using namespace std;

#ifndef RF2D_WEATHERCONTROL_H
#define RF2D_WEATHERCONTROL_H

class cApp;
class cCloudShadows;
class cFogControl;
class cSnowControl;
class cWaterControl;

class cWeatherControl
{
	public:
		cWeatherControl(cApp* pParent);
		~cWeatherControl();
		void RenderGroundFog();
		void RenderFog();
		void RenderClouds();
		void RenderSnow();
		void RenderWater();
		void Update(float dt);
		void CreateFog(int alpha, int red, int green, int blue, int fogtype);
		void CreateGroundFog(int alpha, int red, int green, int blue, int fogtype);
		void CreateSnow(int flakes);
		void CreateClouds(int alpha);
		void CreateWater(int alpha, int red, int green, int blue);
		void SetWindDir(DIR dir);
		void NoWeather();

	private:
		cApp*				m_pParent;
		bool				m_bGroundFog;
		bool				m_bFog;
		bool				m_bClouds;
		bool				m_bSnow;
		bool				m_bWater;
		DIR					m_WindDir;
		cCloudShadows*		m_CloudShadows;
		cFogControl*		m_FogControl;
		cSnowControl*		m_SnowControl;
		cWaterControl*		m_WaterControl;

};




#endif