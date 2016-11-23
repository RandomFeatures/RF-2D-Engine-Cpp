#include "RF3d_Global.h"

/*
render mouse cursor and trailing particle effects
track mouse world position
track mouse screen position
*/

#pragma warning(disable:4244)
using namespace std;

#ifndef RF2D_CURSOR_H
#define RF2D_CURSOR_H

class cApp;

class cCursor
{
	public:
		cCursor(cApp* pParent);
		~cCursor();
		void Render();
		void Update(float dt, float x, float y);
		void SetWorldPos(int x, int y);
		float x();
		float y();
		int WorldX();
		int WorldY();
		WorldPoint GetWorldPoint();
	private:
		cApp*				m_pParent;
		hgeSprite*			m_PartSprite;
		hgeSprite*			m_Cursor;
		hgeParticleSystem*	m_CursorFX;
		int					m_WorldX, m_WorldY;
		float				m_MouseX, m_MouseY;
};




#endif
