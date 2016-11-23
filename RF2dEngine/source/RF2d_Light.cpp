#include "RF2d_Application.h"
#include "RF3d_Global.h"
#include "RF2d_StaticTextures.h"

cLight::cLight(cApp *pParent)
{
	m_Enabled = true;
	m_pParent = pParent;
	m_GlowSprite=new hgeSprite(m_pParent->GetStaticTexture()->GetGlowTexture(),0, 0, 256, 256);
	m_GlowSprite->SetColor(m_GlowColor);
	m_GlowSprite->SetBlendMode(BLEND_COLORMUL);
	m_Flicker = 1;
	//m_GlowColor = 0;
};

cLight::~cLight()
{
	if (m_GlowSprite) delete m_GlowSprite;
};

bool cLight::isOn() { return m_Enabled; };
void cLight::SetGUID(char *guid) { strcpy(m_GUID, guid); };
void cLight::SetId(int idNo) { m_ID = idNo; };
void cLight::SetEnabled(bool enabled) { m_Enabled = enabled; };
void cLight::SetPosition(int x, int y){ m_PosX = x; m_PosY = y; };
void cLight::SetOffset(int x, int y) { m_xOffset = x; m_yOffset = y; };
void cLight::SetSize(float width, float height) { m_Width = width; m_Height = height; };

void cLight::SetFlickerCount(int count)
{
	if (count > 1) 	m_Flicker = count;
};
void cLight::SetColor(int alpha, int red, int green, int blue)
{
	SetColor(ARGB(alpha,red,green,blue));
};

void cLight::SetColor(DWORD color)
{
	m_GlowColor = color;
	m_GlowSprite->SetColor(m_GlowColor);
};

void cLight::Render(float fDeltaTime, float camX, float camY)
{
	int alpha, red, green, blue;
	DWORD tmpColor;
	//Figure out the exact world cord
	float ScreenX;
	float ScreenY;

	//Figure out the exact world cord
	int WorldX = m_PosX * TILEWIDTH;
	int WorldY = m_PosY * TILEHALFHEIGHT;
	if (m_Flicker > 1)
	{
		m_FlickTimer += fDeltaTime;
		if (m_FlickTimer > 0.11f)
		{
			m_FlickTimer = 0;
			red = GETR(m_GlowColor);
			green = GETG(m_GlowColor);
			blue = GETB(m_GlowColor);
			alpha = GETA(m_GlowColor);

			red += hge->Random_Int(-20,20);
			green += hge->Random_Int(-20,20);
			blue +=  hge->Random_Int(-20,20);

			if (red < 0) red = 0;
			if (green < 0) green = 0;
			if (blue < 0) blue = 0;
			if (red > 255) red = 255;
			if (green > 255) green = 255;
			if (blue > 255) blue = 255;

			tmpColor = ARGB(alpha,red,green,blue);


			m_GlowSprite->SetColor(tmpColor);
		}
	}
	m_OnScreen = true;
	//See if the object is onscreen before trying to draw
	if (((WorldX + TILEWIDTH) < camX) || (WorldX > (camX + SCREENWIDTH + TILEWIDTH)) ||
		((WorldY + TILEHEIGHT) < camY) || (WorldY  > (camY + SCREENHEIGHT + 128)))
	{
		m_OnScreen = false;
		return; //Not onscreen... get out
	}
	//translate world X,Y to Screen X,Y
	if (m_PosY % 2 == 0)
		ScreenX =  ((WorldX) - camX)-TILEHALFWIDTH;
	else
		ScreenX =  ((WorldX) - camX);

	ScreenY = ((WorldY) - camY);

	//ScreenX += TILEHALFWIDTH;
	ScreenY -= TILEHEIGHT+ TILEHALFHEIGHT;

    ScreenX += m_xOffset;
	ScreenY += m_yOffset;
	//Light source
	m_ScreenX = ScreenX;
	m_ScreenY = ScreenY;
	//draw rect
	ScreenX = ScreenX - (m_Width/2);
	ScreenY = ScreenY - (m_Height/2);


	m_GlowSprite->RenderStretch(ScreenX,ScreenY,ScreenX+m_Width,ScreenY+m_Height);

};




