#include "RF2d_Cursor.h"
#include "RF2d_Application.h"
#include "RF2d_StaticTextures.h"
/*
render mouse cursor and trailing particle effects
track mouse world position
track mouse screen position
*/

cCursor::cCursor(cApp* pParent)
{
	char tmpFilePath[256];
	m_pParent = pParent;

	//Load Particle Texture
	m_Cursor=new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 0, 0, 32, 32);
	m_Cursor->SetHotSpot(0,0);

	m_PartSprite=new hgeSprite(m_pParent->GetStaticTexture()->GetParticleTexture(), 32, 0, 32, 32);
	m_PartSprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
	m_PartSprite->SetHotSpot(16,16);

	strcpy(tmpFilePath, m_pParent->GetGamePath(PARTICLE));
	strcat(tmpFilePath, "cursor.psi");

	m_CursorFX=new hgeParticleSystem(tmpFilePath,m_PartSprite);
	m_CursorFX->info.nEmission=80;
	m_CursorFX->Fire();

};

cCursor::~cCursor()
{
	if (m_CursorFX) delete m_CursorFX;
	if (m_Cursor) delete m_Cursor;
	if (m_PartSprite) delete m_PartSprite;
};

void cCursor::Render()
{
	m_Cursor->Render(m_MouseX, m_MouseY);
	m_CursorFX->Render();
};
void cCursor::Update(float dt, float x, float y)
{
	float ScreenX = m_pParent->GetMainMap()->ScreenX() + x;
	float ScreenY = m_pParent->GetMainMap()->ScreenY() + y;

	m_WorldY = ScreenY/TILEHALFHEIGHT;

	if (m_WorldY % 2 == 0)
		// Even Line //
		m_WorldX = ((ScreenX+TILEHALFWIDTH)/TILEWIDTH);
	else
		// Odd Line  //
		m_WorldX = ((ScreenX)/TILEWIDTH);

	m_MouseX = x;
	m_MouseY = y;
	m_CursorFX->MoveTo(x+17,y+18);
	m_CursorFX->Update(dt);

};

void cCursor::SetWorldPos(int x, int y)
{
	m_WorldX = x;
	m_WorldY = y;
};

WorldPoint cCursor::GetWorldPoint()
{
    WorldPoint mouse;
    mouse.x = m_WorldX;
    mouse.y = m_WorldY;
    return mouse;
}

float cCursor::x(){ return m_MouseX; };
float cCursor::y(){ return m_MouseY; };
int cCursor::WorldX(){ return m_WorldX; };
int cCursor::WorldY(){ return m_WorldY; };

