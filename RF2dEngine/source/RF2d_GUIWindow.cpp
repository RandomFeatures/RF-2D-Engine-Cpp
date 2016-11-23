/*
This is the base class for all GUI windows and must be inherited from for
the GUI Manager to be able to... well... manage them.
*/
#include "RF2d_GUIWindow.h"
#include "RF2d_Application.h"
#include "RF2d_StaticTextures.h"
#include  <stdlib.h>


//Just create the window with all the default values
cBaseWindow::cBaseWindow()
{
	m_GameParent = NULL;
	m_BFont = NULL;
	m_CaptStart = 0.0f;
	m_FontColor = ARGB(255,255,255,255);
	m_Visible = true;
	m_MouseDown = false;
	m_Blend = 255;
	m_XPos = 0.0f;
	m_YPos = 0.0f;
	m_Width = 32.0f;
	m_Height = 32.0f;
	m_MinWidth = 32.0f;
	m_MinHeight = 32.0f;
	m_CaptClickX = m_XPos;
	m_CaptClickY = m_YPos;

	m_MaxWidth = SCREENWIDTH;
	m_MaxHeight = SCREENHEIGHT;
	m_Focus = false;
	m_GraphicGUI = false;
	ControlBox.Visible = true;
	ControlBox.Enabled = true;
	ControlBox.Click = false;
	ControlBox.Image = NULL;

	Caption.Visible =true;
	Caption.Enabled = true;
	Caption.Click = false;
	Caption.Image = NULL;
	Caption.Text = "";

	Grip.Visible =true;
	Grip.Enabled = true;
	Grip.Click = false;
	Grip.Image = NULL;

	CloseButton.Visible =true;
	CloseButton.Enabled = true;
	CloseButton.Click = false;
	CloseButton.Image = NULL;

	Border.Visible =true;
	Border.Enabled = true;
	Border.Click = false;
	Border.Image = NULL;
	Border.Color = ARGB(255,0,0,0);
	Border.ShadowColor = ARGB(255,0,0,0);
	Border.BColor = ARGB(255,0,0,0);

	BackGround.Visible =true;
	BackGround.Enabled = true;
	BackGround.Click = false;
	BackGround.Image = NULL;
	BackGround.Color = ARGB(255,0,0,0);
};

//Create the window speficing location and size
cBaseWindow::cBaseWindow(cApp* GameParent, float x, float y, float w, float h, int blend)
{
	m_GameParent = GameParent;
	m_BFont = NULL;
	m_CaptStart = 0.0f;
	m_FontColor = ARGB(255,255,255,255);
	m_XPos = x;
	m_YPos = y;
	m_CaptClickX = m_XPos;
	m_CaptClickY = m_YPos;
	m_Width = w;
	m_Height = h;
	m_MinWidth = w;
	m_MinHeight = h;
	m_MaxWidth = SCREENWIDTH;
	m_MaxHeight = SCREENHEIGHT;
	m_Blend = blend;
	m_Visible = true;
	m_Focus = false;
	m_MouseDown = false;

	ControlBox.Visible = true;
	ControlBox.Enabled = true;
	ControlBox.Click = false;
	ControlBox.Image = NULL;

	Caption.Visible = true;
	Caption.Enabled = true;
	Caption.Click = false;
	Caption.Image = NULL;

	Grip.Visible =true;
	Grip.Enabled = true;
	Grip.Click = false;
	Grip.Image = NULL;

	CloseButton.Visible =true;
	CloseButton.Enabled = true;
	CloseButton.Click = false;
	CloseButton.Image = NULL;

	Border.Visible =true;
	Border.Enabled = true;
	Border.Click = false;
	Border.Image = NULL;
	Border.Color = ARGB(255,0,200,0);
	Border.ShadowColor = ARGB(255,0,200,0);
	Border.BColor = ARGB(255,0,200,0);

	BackGround.Visible =true;
	BackGround.Enabled = true;
	BackGround.Click = false;
	BackGround.Image = NULL;
	BackGround.Color = ARGB(blend,0,0,0);
};
//Clean up
cBaseWindow::~cBaseWindow()
{
	DeleteControls();

	if (m_TopLeft) delete m_TopLeft;
	if (m_TopRight) delete m_TopRight;
	if (m_BottomLeft) delete m_BottomLeft;
	if (m_BottomRight) delete m_BottomRight;
	if (m_CenterLeft) delete m_CenterLeft;
	if (m_CenterRight) delete m_CenterRight;
	if (m_CenterTop) delete m_CenterTop;
	if (m_CenterBottom) delete m_CenterBottom;
	if (m_Close1) delete m_Close1;
	if (m_Close2) delete m_Close2;
	if (m_Close3) delete m_Close3;
};
/*
I know there is a lot of redundancy here but I really hate searching an someone elses object
looking for a property or method for a long time finally discovering it was just named
something that I would never have thought of.
I am also starting to use void cBaseWindow::Width(float w){m_Width = w;};
to set the property and float cBaseWindow::Width(){return m_Width;};
to read the property
*/
//Get and Set Properties
void cBaseWindow::Width(float w){m_Width = w;} ;
void cBaseWindow::Height(float h){m_Height = h;};
void cBaseWindow::Show(){m_Visible = true;};
void cBaseWindow::Hide(){m_Visible = false;};
void cBaseWindow::SetBaseFont(hgeFont* Font){m_BFont = Font;};
void cBaseWindow::ToggleVisible(){m_Visible= !m_Visible;};
float cBaseWindow::Width(){return m_Width;};
float cBaseWindow::Height(){return m_Height;};
float cBaseWindow::Left(){return m_XPos;};
float cBaseWindow::Top(){return m_YPos;};
int cBaseWindow::GetZPos(){return m_ZPos;}
bool cBaseWindow::Visible(){return m_Visible;};
bool cBaseWindow::GotFocus(){return m_Focus;};
void cBaseWindow::BringFront(){m_ZPos=0;};
void cBaseWindow::HideWindow(){LostFocus();m_Visible = false;};
void cBaseWindow::ShowWindow(){m_Visible = true; SetFocus();};
void cBaseWindow::SetZPos(int z){m_ZPos = z;};
void cBaseWindow::AlignTop(){m_YPos = 5.0f;};
void cBaseWindow::AlignBottom(){m_YPos = SCREENHEIGHT - m_Height-1.0f;};
void cBaseWindow::AlignLeft(){m_XPos = 1.0f;};
void cBaseWindow::AlignRight(){m_XPos = SCREENWIDTH - m_Width-1.0f;};
void cBaseWindow::AlignCenter()
{
	m_XPos = (SCREENWIDTH/2) - (m_Width/2);
	m_YPos = (SCREENHEIGHT/2) - (m_Height/2);
};
void cBaseWindow::CenterCaption()
{
	m_CaptStart = 15.0f;//m_Width/2.0f - m_BFont->GetStringWidth(Caption.Text)/2.0f;
}

void cBaseWindow::SetFocus()
{
	if (m_Visible)
	{
		m_Focus = true;
		Border.Color  = Border.BColor;
	}
};
void cBaseWindow::LostFocus()
{
	if (m_Visible)
	{
		m_Focus = false;
		Border.Color = Border.ShadowColor;
	}
};

void cBaseWindow::SetupCloseButton(bool enabled, bool visible)
{
    CloseButton.Enabled = enabled;
	CloseButton.Visible = visible;
}

void cBaseWindow::SetupControlBox(bool enabled, bool visible)
{
    ControlBox.Visible = visible;
	ControlBox.Enabled = enabled;
}

void cBaseWindow::SetCaption(std::string strText)
{
    Caption.Text = strText;
}

void cBaseWindow::SetBGColor(int blend, int red, int green, int blue)
{
	BackGround.Color = ARGB(blend,red,green,blue);
	if (BackGround.Image) BackGround.Image->SetColor(BackGround.Color);
}

void cBaseWindow::SetBorderColor(int red, int green, int blue)
{
	int tmpR,tmpB,tmpG;
	if (red - 100 <0)tmpR = 0;else tmpR = red - 100;
	if (green - 100 <0)tmpG = 0;else tmpG = green - 100;
	if (blue - 100 <0)tmpB = 0;else tmpB = blue - 100;
	Border.ShadowColor= ARGB(255,tmpR,tmpG,tmpB);
	Border.Color = ARGB(255,red,green,blue);
	Border.BColor = ARGB(255,red,green,blue);
}

void cBaseWindow::SetFontColor(int red, int green, int blue)
{
	m_FontColor = ARGB(255,red,green,blue);
};

void cBaseWindow::SetSize(float h, float w)
{
	m_Width = w;
	m_Height = h;
};

void cBaseWindow::SetMinSize(float h, float w)
{
	m_MinWidth = w;
	m_MinHeight = h;
	if (m_Width < m_MinWidth) m_Width = m_MinWidth;
	if (m_Height < m_MinHeight) m_Height = m_MinHeight;
};
void cBaseWindow::SetMaxSize(float h, float w)
{
	m_MaxWidth = w;
	m_MaxHeight = h;
	if (m_Width > m_MaxWidth) m_Width = m_MaxWidth;
	if (m_Height > m_MaxHeight) m_Height = m_MaxHeight;
};

void cBaseWindow::SetPos(float x, float y)
{
	m_XPos = x;
	m_YPos = y;
};


void cBaseWindow::UseGraphicGUI()
{
	m_GraphicGUI = true;
	LoadGUISprites();
};
/*
The GUI Manager will use this method to see
if the mouse is currently over this window before
passing in a mouse event
*/
bool cBaseWindow::MouseTest(float x, float y)
{
	if (x<m_XPos || x>m_XPos+m_Width) return false;
	if (y<m_YPos || y>m_YPos+m_Height) return false;
	return true;
}

/*
This will only be called by the GUI Manager if the Mouse is over
this window and it is the Top Most window. The default function
hanles the Control Box, Close Button, Size Grip etc. If the mouse is not
over any of the window controls then it tries to pass mouse ever on to the other
controls. You can override this when you inherite and do whatever you like
*/
void cBaseWindow::MouseMove(float x, float y)
{
	//Control button
	if(ControlBox.Click && m_Focus)
	{
		float tmpXPos = x-4.0f;
		float tmpYPos = y-4.0f;

		if (tmpXPos + m_Width < SCREENWIDTH)
			m_XPos = tmpXPos;
		else
			m_XPos = SCREENWIDTH - m_Width -1.0f;

		if (tmpYPos + m_Height < SCREENHEIGHT)
			m_YPos = tmpYPos;
		else
			m_YPos = SCREENHEIGHT - m_Height -1.0f;
	}
	//Caption Click
	if (Caption.Click && m_Focus)
	{
		float tmpXPos = x-m_CaptClickX;
		float tmpYPos = y-m_CaptClickY;

		if (tmpXPos + m_Width < SCREENWIDTH)
			m_XPos = tmpXPos;
		else
			m_XPos = SCREENWIDTH - m_Width -1.0f;

		if (tmpYPos + m_Height < SCREENHEIGHT)
			m_YPos = tmpYPos;
		else
			m_YPos = SCREENHEIGHT - m_Height -1.0f;
	}
	//Resize Grip
	if(Grip.Click && m_Focus)
	{
		float tmpWidth = x - m_XPos;
		float tmpHeight = y - m_YPos;

		if (tmpWidth > m_MinWidth && tmpWidth < m_MaxWidth)
			m_Width = tmpWidth;

		if (tmpHeight > m_MinHeight && tmpHeight < m_MaxHeight)
			m_Height = tmpHeight;
		ResizeWindow();
	}

	if (CloseButton.Visible)
	{
		CloseButton.MouseOver = false;
		if ((x>(m_XPos+m_Width)-36) && (x<(m_XPos+m_Width)-8))
			if ((y<m_YPos+18) && (y>m_YPos))
				CloseButton.MouseOver = true;
	}


	if (x<m_XPos || x>m_XPos+m_Width) return;
	if (y<m_YPos || y>m_YPos+m_Height) return;


	if (!CloseButton.Click && !ControlBox.Click && !Grip.Click && !Caption.Click)
	{//None of the window controls were clicked so see if this event is for a user control
		for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
		{ //Loop through all the user controls
			if (m_Controls[iloop]->MouseTest(x,y))
			{//Only pass the event to the control the mouse is over
				m_Controls[iloop]->MouseMove(x,y);
				break;
			}
		}
	}

};

/*
This will only be called by the GUI Manager if the Mouse is over
this window and it is the Top Most window. First it checks the window
controls(Control Box, Close Button, Size Grip etc). If the mouse is not
over one of them then it attempts to pass the event along to the user controls
You can override this when you inherite and do whatever you like
*/
void cBaseWindow::MouseDown(float x, float y)
{
	if (x<m_XPos || x>m_XPos+m_Width) return;
	if (y<m_YPos || y>m_YPos+m_Height) return;

	m_MouseDown = true;

	if (m_GraphicGUI)
	{
		if (CloseButton.Visible  && CloseButton.MouseOver) CloseButton.Click  = true;
		if (Caption.Visible && y<m_YPos+19)
		{
			Caption.Click = true;
			m_CaptClickX = x-m_XPos;
			m_CaptClickY = y-m_YPos;
		}
	}else
	{
		if (ControlBox.Visible && x<m_XPos+9 && y<m_YPos+9) ControlBox.Click  = true;
		if (Grip.Visible && x>m_XPos+m_Width-12 && y>m_YPos+m_Height-12) Grip.Click = true;
		if (CloseButton.Visible  && x>m_XPos+m_Width-9 && y<m_YPos+9) CloseButton.Click  = true;
		if (Caption.Visible  && !CloseButton.Click && !ControlBox.Click &&  y<m_YPos+19) Caption.Click = true;
	}

	if (CloseButton.Click && !ControlBox.Click && !Grip.Click && !Caption.Click)
	{//Only the close button was clicked. make sure this window can close
		if (CanClose()) HideWindow();
	}

	if (!CloseButton.Click && !ControlBox.Click && !Grip.Click && !Caption.Click)
	{//None of the window controls were clicked so see if this event is for a user control
		for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
		{ //Loop through all the user controls
			m_Controls[iloop]->LostFocus();//clear focus from all controls
			if (m_Controls[iloop]->MouseTest(x,y))
			{//Only pass the event to the control the mouse is over
				m_Controls[iloop]->MouseDown(x,y);
				//this will cause this control to need focus
				m_Controls[iloop]->SetFocus();
			}
		}
	}
};

/*
This will only be called by the GUI Manager for every window
no matter which window the mouse is over. This is to deal with the user
pressing the mouse button over one window then releasing it over another.
This way all controls will be reset to their default state
*/
void cBaseWindow::MouseUp(float x, float y)
{
	m_MouseDown = false;
	ControlBox.Click = false;
	Grip.Click = false;
	Caption.Click = false;
	CloseButton.Click = false;

	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{//pass this event to all controls reguardless just to be sure
		m_Controls[iloop]->MouseUp(x,y);
	}
	//TODO - 1/14/05
	//I dont think these two lines are needed. I will come back and test
	//if (x<m_XPos || x>m_XPos+m_Width) return;
	//if (y<m_YPos || y>m_YPos+m_Height) return;
};


/*
This will only be called by the GUI Manager if the Mouse is over
this window and it is the Top Most window. It will loop through all
of the user controls and see if any of them are expcting to get this
key stroke. If so the contol will process the key and let the GUI manager
know that it was handled. If not then the GUI Manager will pass the key on
to the game. You can override this when you inherite and do whatever you like
*/
bool cBaseWindow::KeyPress(int key)
{
	bool rtn = false;

	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{//Loop through all controls and see if one is expecting this key
		rtn = m_Controls[iloop]->KeyPress(key);
	//Note that by continuing the loop more than one control may recieve the key
	//My Text boxes handle this with a "Focus" property. If they dont have focus then
	//they ignore it
	}
	//Let the GUI Manager know if this key was handled or not
	return rtn;
};

bool cBaseWindow::CharPress(int key)
{
	bool rtn = false;

	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{//Loop through all controls and see if one is expecting this key
		rtn = m_Controls[iloop]->CharPress(key);
	//Note that by continuing the loop more than one control may recieve the key
	//My Text boxes handle this with a "Focus" property. If they dont have focus then
	//they ignore it
	}
	//Let the GUI Manager know if this key was handled or not
	return rtn;
};

//Add controls to the list so that the window can pass events and delete them
//when the window is deleted
void cBaseWindow::AddControl(cBaseControl* control)
{
	control->SetParentWindow(this);
	control->Setup(ControlTemp);
	m_Controls.push_back(control);
};
//Loop through all controls in the list and delete them
//then clear the list
void cBaseWindow::DeleteControls()
{
	cBaseControl *control;
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		control = m_Controls[iloop];
		delete control;
	}
	m_Controls.clear();
};
//Quick way to change some fo the defaults for the window
void cBaseWindow::ShowAllParts()
{
	Border.Visible  = true;
	Caption.Visible  = true;
	ControlBox.Visible = true;
	CloseButton.Visible  = true;
	Grip.Visible  = true;
};
void cBaseWindow::HideAllParts()
{
	Border.Visible  = false;
	Caption.Visible  = false;
	ControlBox.Visible = true;
	CloseButton.Visible  = false;
	Grip.Visible  = false;
};
//Reorder windows when one in the background is clicked
//and brought to the forground
void cBaseWindow::Reorder(int maxVal)
{
	if (m_ZPos < maxVal)
	{
		m_ZPos++;
		return;
	}
	if (m_ZPos = maxVal)
	{
		m_ZPos = 0;
		return;
	}
};

/*
Draw the window itsself including Background, boarders,
control button, close button, caption, size grip, etc.
*/
void cBaseWindow::DrawBaseWindow()
{
	float left = m_XPos;
	float right = m_XPos + m_Width ;
	float top = m_YPos;
	float bottom = m_YPos + m_Height;

	//Size Grip
	if (Grip.Visible)
	{
		hge->Gfx_RenderLine(right-4,bottom,right,bottom-4,ARGB(255,200,200,200));
		hge->Gfx_RenderLine(right-8,bottom,right,bottom-8,ARGB(255,200,200,200));
		hge->Gfx_RenderLine(right-12,bottom,right,bottom-12,ARGB(255,200,200,200));

		hge->Gfx_RenderLine(right-3,bottom,right,bottom-3,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-5,bottom,right,bottom-5,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-7,bottom,right,bottom-7,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-9,bottom,right,bottom-9,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-11,bottom,right,bottom-11,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-13,bottom,right,bottom-13,ARGB(255,100,100,100));

		//hge->Gfx_RenderLine(left+4,bottom,left,bottom-4,ARGB(255,200,200,200));
		//hge->Gfx_RenderLine(left+8,bottom,left,bottom-8,ARGB(255,200,200,200));
		//hge->Gfx_RenderLine(left+12,bottom,left,bottom-12,ARGB(255,200,200,200));

		//hge->Gfx_RenderLine(left+2,bottom,left,bottom-2,ARGB(255,100,100,100));
		//hge->Gfx_RenderLine(left+6,bottom,left,bottom-6,ARGB(255,100,100,100));
		//hge->Gfx_RenderLine(left+10,bottom,left,bottom-10,ARGB(255,100,100,100));
	}

	//Border
	if (Border.Visible)
	{
		//Top
		hge->Gfx_RenderLine(left,top,right,top,Border.Color);
		//Left
		hge->Gfx_RenderLine(left,top,left,bottom,Border.Color);
		//Right
		hge->Gfx_RenderLine(right,top,right,bottom+1,Border.Color);
		//Bottom
		hge->Gfx_RenderLine(left,bottom,right+1,bottom,Border.Color);

		//Top
		hge->Gfx_RenderLine(left,top+1,right,top+1,Border.ShadowColor);
		//Left
		hge->Gfx_RenderLine(left+1,top,left+1,bottom,Border.ShadowColor);
		//Right
		hge->Gfx_RenderLine(right+1,top,right+1,bottom+1,Border.ShadowColor);
		//Bottom
		hge->Gfx_RenderLine(left,bottom+1,right+1,bottom+1,Border.ShadowColor);

	}
	//Ctrlbox
	if (ControlBox.Visible)
	{
		if (ControlBox.Image) ControlBox.Image->Render(left,top);
		hge->Gfx_RenderLine(left,top+8,left+9,top+8,Border.Color);
		hge->Gfx_RenderLine(left+8,top,left+8,top+9,Border.Color);
	}
	//closeBox
	if (CloseButton.Visible)
	{
		if (CloseButton.Image) CloseButton.Image->Render(right-7,top);
		hge->Gfx_RenderLine(right-8,top+8,right,top+8,Border.Color);
		hge->Gfx_RenderLine(right-8,top,right-8,top+8,Border.Color);
	}
	//Caption box
	if (Caption.Visible)
	{
		m_BFont->SetColor(m_FontColor);
		if (m_BFont && !Caption.Text.empty()) m_BFont->printf(abs(left+ m_CaptStart), top + 3, HGETEXT_LEFT, "%s", Caption.Text.c_str());
		hge->Gfx_RenderLine(left+8,top+18,right-8,top+18,Border.Color);
		hge->Gfx_RenderLine(left+8,top,left+8,top+18,Border.Color);
		hge->Gfx_RenderLine(right-8,top,right-8,top+19,Border.Color);

		hge->Gfx_RenderLine(left+8,top+19,right-7,top+19,Border.ShadowColor);
		hge->Gfx_RenderLine(left+9,top,left+9,top+18,Border.ShadowColor);
		hge->Gfx_RenderLine(right-7,top,right-7,top+19,Border.ShadowColor);
	}
};


void cBaseWindow::DrawGraphicWindow()
{
	float left = m_XPos;
	float right = m_XPos + m_Width ;
	float top = m_YPos-4;
	float bottom = m_YPos + m_Height;
	float iloop;
	//Size Grip
	if (Grip.Visible)
	{
		hge->Gfx_RenderLine(right-4,bottom,right,bottom-4,ARGB(255,200,200,200));
		hge->Gfx_RenderLine(right-8,bottom,right,bottom-8,ARGB(255,200,200,200));
		hge->Gfx_RenderLine(right-12,bottom,right,bottom-12,ARGB(255,200,200,200));

		hge->Gfx_RenderLine(right-3,bottom,right,bottom-3,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-5,bottom,right,bottom-5,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-7,bottom,right,bottom-7,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-9,bottom,right,bottom-9,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-11,bottom,right,bottom-11,ARGB(255,100,100,100));
		hge->Gfx_RenderLine(right-13,bottom,right,bottom-13,ARGB(255,100,100,100));

		//hge->Gfx_RenderLine(left+4,bottom,left,bottom-4,ARGB(255,200,200,200));
		//hge->Gfx_RenderLine(left+8,bottom,left,bottom-8,ARGB(255,200,200,200));
		//hge->Gfx_RenderLine(left+12,bottom,left,bottom-12,ARGB(255,200,200,200));

		//hge->Gfx_RenderLine(left+2,bottom,left,bottom-2,ARGB(255,100,100,100));
		//hge->Gfx_RenderLine(left+6,bottom,left,bottom-6,ARGB(255,100,100,100));
		//hge->Gfx_RenderLine(left+10,bottom,left,bottom-10,ARGB(255,100,100,100));
	}

	//Border
	if (Border.Visible)
	{
		//Top
		m_TopLeft->Render(left,top);
		for (iloop=left+m_TopLeft->GetWidth();iloop<right-m_TopRight->GetWidth();iloop+=m_CenterTop->GetWidth())
		{
			m_CenterTop->Render(iloop,top);
		}
		m_TopRight->Render(right-m_TopRight->GetWidth(),top);

		//Left
		for (iloop=top+m_TopLeft->GetHeight();iloop<bottom;iloop+=m_CenterLeft->GetHeight())
		{
			m_CenterLeft->Render(left,iloop);
		}
		//hge->Gfx_RenderLine(left,top,left,bottom,Border.Color);


		//Right
		for (iloop=top+m_TopRight->GetHeight();iloop<bottom;iloop+=m_CenterRight->GetHeight())
		{
			m_CenterRight->Render(right-m_CenterRight->GetWidth(),iloop);
		}

		//Bottom
		m_BottomLeft->Render(left,bottom);
		for (iloop=left+m_BottomLeft->GetWidth();iloop<right-m_BottomRight->GetWidth();iloop+=m_CenterBottom->GetWidth())
		{
			m_CenterBottom->Render(iloop,bottom);
		}
		m_BottomRight->Render(right-m_BottomRight->GetWidth(),bottom);


	}
	//closeBox
	if (CloseButton.Visible)
	{
		if (CloseButton.MouseOver && CloseButton.Enabled)
		{
			m_Close2->Render(right-36,top);
		}else
		{
			m_Close1->Render(right-36,top);
		}


	}
	//Caption box
	if (Caption.Visible)
	{
		m_BFont->SetColor(m_FontColor);
		if (m_BFont && !Caption.Text.empty()) m_BFont->printf(abs(left+ m_CaptStart), top + 6, HGETEXT_LEFT, "%s", Caption.Text.c_str());
		//hge->Gfx_RenderLine(left+8,top+18,right-8,top+18,Border.Color);
		//hge->Gfx_RenderLine(left+8,top,left+8,top+18,Border.Color);
		//hge->Gfx_RenderLine(right-8,top,right-8,top+19,Border.Color);

		//hge->Gfx_RenderLine(left+8,top+19,right-7,top+19,Border.ShadowColor);
		//hge->Gfx_RenderLine(left+9,top,left+9,top+18,Border.ShadowColor);
		//hge->Gfx_RenderLine(right-7,top,right-7,top+19,Border.ShadowColor);
	}
};


/*
	Tell all user controls in the control list
	to draw themselves. This uses the window x,y as
	a starting refernece
*/
void cBaseWindow::DrawWindow(float dt)
{
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		m_Controls[iloop]->Render(m_XPos,m_YPos,dt);
	}
};



void cBaseWindow::Render(float dt)
{
	if (m_Visible)
	{
		BackGround.Image->SetColor(BackGround.Color);
		BackGround.Image->RenderStretch(m_XPos,m_YPos,m_XPos+m_Width,m_YPos+m_Height);
		if (m_GraphicGUI)
		{
			DrawGraphicWindow();
		}else
			DrawBaseWindow();
		DrawWindow(dt);
	}
};

void cBaseWindow::LoadGUISprites()
{

	m_BottomLeft=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 0, 32, 5, 4);
	m_BottomRight=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 17, 32, 5, 4);

	m_CenterLeft=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 0, 26, 5, 1);
	m_CenterRight=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 12, 26, 5, 1);

	m_CenterBottom=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 9, 32, 4, 4);

	m_TopLeft=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 0, 0, 8, 25);
	m_TopRight=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 18, 0, 8, 25);
	//m_BottomLeft=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 0, 28, 8, 8);
	//m_BottomRight=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 14, 28, 8, 8);
	//m_CenterLeft=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 0, 26, 8, 1);
	//m_CenterRight=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 9, 26, 8, 1);
	m_CenterTop=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 9, 0, 8, 25);
	//m_CenterBottom=new hgeSprite(m_GameParent->GetWindowTexture(), 9, 28, 4, 8);
	m_Close1=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 0, 93, 27, 18);
	m_Close2=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 29, 93, 27, 18);
	m_Close3=new hgeSprite(m_GameParent->GetStaticTexture()->GetWindowTexture(), 58, 93, 27, 18);

};
