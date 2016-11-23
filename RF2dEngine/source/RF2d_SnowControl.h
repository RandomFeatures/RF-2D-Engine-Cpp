#include <vector>
#include "RF3d_Global.h"
/*
renders fog moving across the screen
supports different wind directions for moving fog
*/
using namespace std;

#ifndef RF2D_SNOWCONTROL_H
#define RF2D_SNOWCONTROL_H

class cApp;
class cSnowFlake;

class cSnowControl 
{
	public:
		cSnowControl(cApp *pParent);
		~cSnowControl();

		void SetEnabled(bool enabled);
		void Update(float fDeltaTime);
		void Render();
		void SetWindSpeed(int wind);
		void SetFlakeCount(int flakes);
	private:
		cApp		*m_pParent;
		bool		m_Enabled;
		hgeSprite*	m_SnowFlake;		int			m_Wind;
		int			m_Flakes;
		int			m_ScreenHeight;
		int			m_ScreenWidth;
		vector<cSnowFlake*> SnowList;
};

#endif