#include "RF3d_Global.h"
#include "RF2d_Application.h"

/*
GUI controls. recieve events from the window and react
*/


cBaseControl::cBaseControl()
{
	m_Clicked = false;
	m_BorderLColor = ARGB(255,100,100,100);
	m_BorderDColor = ARGB(255,100,100,100);
	m_FontColor = ARGB(255,255,255,255);
	m_MouseOver = false;
	m_Enabled = true;
	//m_Text = new char;
};

cBaseControl::~cBaseControl()
{
	//if ((m_Text.length()>0))
	//{
		//hge->System_Log("Delete %s",m_Text);
		//delete [] m_Text;
		//hge->System_Log("Delete");
	//}
};
void cBaseControl::SetPos(float x, float y){m_XPos = x; m_YPos = y;};
void cBaseControl::SetSize(float h, float w){m_Width = w;m_Height = h;};
void cBaseControl::SetParentWindow(cBaseWindow* parent){m_Parent = parent;};
void cBaseControl::SetFocus(){m_Focus = true;};
void cBaseControl::LostFocus(){m_Focus = false;};
void cBaseControl::SetFont(hgeFont* Font){m_Font = Font;};
void cBaseControl::SetText(std::string txt){ m_Text=txt; };
std::string cBaseControl::GetText(){return m_Text;};
void cBaseControl::SetLBorderColor(int red,int green, int blue){m_BorderLColor = ARGB(255,red,green,blue);};
void cBaseControl::SetDBorderColor(int red,int green, int blue){m_BorderDColor = ARGB(255,red,green,blue);};
void cBaseControl::SetFontColor(int red,int green, int blue){m_FontColor = ARGB(255,red,green,blue);};
int cBaseControl::ID(){return m_ID;};
void cBaseControl::ID(int i) {m_ID = i;};
float cBaseControl::Width(){return m_Width;};
void cBaseControl::Width(float w){m_Width = w;};
float cBaseControl::Height(){return m_Height;};
void cBaseControl::Height(float h){m_Height = h;};
float cBaseControl::X(){return m_XPos;};
void cBaseControl::X(float x){m_XPos = x;};
float cBaseControl::Y(){return m_YPos;};
void cBaseControl::Y(float y){m_YPos = y;};
void cBaseControl::SetEnabled(bool state){ m_Enabled = state; };

void cBaseControl::Setup(sControlTemplate ct)
{
	m_Font = ct.Font;
	m_FontColor = ct.FontColor;
	m_BorderDColor = ct.BorderDColor;
	m_FontColor = ct.FontColor;
	m_BorderLColor = ct.BorderLColor;
	m_BackColor = ct.BackColor;
};


bool cBaseControl::MouseTest(float x, float y)
{
	m_MouseOver = false;
	if (x<m_Parent->Left() + m_XPos || x>m_Parent->Left() + m_XPos+m_Width) return false;
	if (y<m_Parent->Top() + m_YPos || y>m_Parent->Top() + m_YPos+m_Height) return false;
	m_MouseOver = true;
	return true;
}

/*******************  BASE CONTAINER  ********************/

cBaseContainer::cBaseContainer(float x, float y, float h, float w)
{
	SetPos(x,y);
	SetSize(h,w);
	m_ShowBorder = true;
};
cBaseContainer::~cBaseContainer()
{
	DeleteControls();
	cBaseControl::~cBaseControl();
};
void cBaseContainer::DeleteControls()
{
	cBaseControl *control;
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		control = m_Controls[iloop];
		delete control;
	}
	m_Controls.clear();
};

void cBaseContainer::ShowBorder(bool show){m_ShowBorder=show;};
bool cBaseContainer::ShowBorder(){return m_ShowBorder;};

void cBaseContainer::AddControl(cBaseControl* control)
{
	control->SetParentWindow(m_Parent);
	control->Setup(CntrlTmp);
	m_Controls.push_back(control);
};

void cBaseContainer::Setup(sControlTemplate ct)
{
	m_Font = ct.Font;
	m_FontColor = ct.FontColor;
	m_BorderDColor = ct.BorderDColor;
	m_FontColor = ct.FontColor;
	m_BorderLColor = ct.BorderLColor;
	m_BackColor = ct.BackColor;
	//CntrlTmp = ct;
	CntrlTmp.BackColor = ct.BackColor;
	CntrlTmp.BorderDColor = ct.BorderDColor;
	CntrlTmp.BorderLColor = ct.BorderLColor;
	CntrlTmp.Font = ct.Font;
	CntrlTmp.FontColor = ct.FontColor;
	if (!CntrlTmp.Font) hge->System_Log("No Font2");
//	hge->System_Log("cBaseContainer Setup");

};

bool cBaseContainer::CharPress(int key)
{
	if (m_Controls.size() > 0)
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		if (m_Controls[iloop]->CharPress(key))
		{
			return true;
			break;
		}
	}
	return false;
};
bool cBaseContainer::KeyPress(int key)
{
	if (m_Controls.size() > 0)
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		if (m_Controls[iloop]->KeyPress(key))
		{
			return true;
			break;
		};
	}
	return false;
}

void cBaseContainer::MouseMove(float x, float y)
{
	if (m_Controls.size() > 0)
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		if (m_Controls[iloop]->MouseTest(x,y))
		{//Only pass the event to the control the mouse is over
			m_Controls[iloop]->MouseMove(x, y);
			break;
		}
	}
};
void cBaseContainer::MouseDown(float x, float y)
{
	if (m_Controls.size() > 0)
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		if (m_Controls[iloop]->MouseTest(x,y))
		{//Only pass the event to the control the mouse is over
			m_Controls[iloop]->MouseDown(x, y);
			break;
		}
	}
};
void cBaseContainer::MouseUp(float x, float y)
{
	if (m_Controls.size() > 0)
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		m_Controls[iloop]->MouseUp(x, y);
	}
};

void cBaseContainer::Render(float x, float y, float dt)
{
	DrawBaseContainer(x, y);
	DrawContainer(x, y, dt);
};

void cBaseContainer::DrawContainer(float x, float y,float dt)
{
	if (m_Controls.size() > 0)
	for (unsigned int iloop=0;iloop<m_Controls.size();iloop++)
	{
		m_Controls[iloop]->Render(x, y, dt);
	}
};

void cBaseContainer::DrawBaseContainer(float x, float y)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;

	//OuterFrame
	if (m_ShowBorder)
	{
		//Top
		hge->Gfx_RenderLine(left,top,right,top,m_BorderLColor);
		//Left
		hge->Gfx_RenderLine(left,top,left,bottom,m_BorderLColor);
		//Right
		hge->Gfx_RenderLine(right,top,right,bottom+1.0f,m_BorderLColor);
		//Bottom
		hge->Gfx_RenderLine(left,bottom,right+1.0f,bottom,m_BorderLColor);
	}
};


/*******************  LABEL  ********************/

cLabel::cLabel(hgeFont* Font, float x, float y, float h, float w)
{
	SetPos(x,y);
	SetSize(h,w);
	SetFont(Font);
};

void cLabel::ShowBorder(bool show){m_ShowBorder=show;};
bool cLabel::ShowBorder(){return m_ShowBorder;};

void cLabel::Render(float x, float y, float dt)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;

	//OuterFrame
	if (m_ShowBorder)
	{
		//Top
		hge->Gfx_RenderLine(left,top,right,top,m_BorderLColor);
		//Left
		hge->Gfx_RenderLine(left,top,left,bottom,m_BorderLColor);
		//Right
		hge->Gfx_RenderLine(right,top,right,bottom+1.0f,m_BorderLColor);
		//Bottom
		hge->Gfx_RenderLine(left,bottom,right+1.0f,bottom,m_BorderLColor);
	}
	if (m_Font && (m_Text.length()>0)) m_Font->printf(left+2.0f, top+2.0f, HGETEXT_LEFT, "%s", m_Text.c_str());
};

/******************* GROUP BOX ********************/

cGroupBox::cGroupBox(hgeFont* Font, float x, float y, float h, float w)
{
	SetPos(x,y);
	SetSize(h,w);
	SetFont(Font);
	m_ShowBorder = true;
	m_GraphicBack = false;
};
cGroupBox::~cGroupBox()
{
	if (m_BckGrnd) delete m_BckGrnd;
};

void cGroupBox::UseGraphicBack(HTEXTURE guiTex)
{
	m_GraphicBack = true;
	m_BckGrnd=new hgeSprite(guiTex, 10, 3, 5, 20);
};

void cGroupBox::DrawBaseContainer(float x, float y)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;
	int CaptionWidth = 0;
	if (m_Font && (m_Text.length()>0)) CaptionWidth = 6.0f + m_Font->GetStringWidth(m_Text.c_str());

	if (m_GraphicBack)
	{
		m_BckGrnd->RenderStretch(left,top,right,bottom-1.0f);
	};

	//Top Left
	hge->Gfx_RenderLine(left,top,left + 4.0f,top,m_BorderLColor);
	//Top Right
	hge->Gfx_RenderLine(left + CaptionWidth,top,right,top,m_BorderLColor);

	//Left
	hge->Gfx_RenderLine(left,top,left,bottom,m_BorderLColor);
	//Right
	hge->Gfx_RenderLine(right,top,right,bottom+1.0f,m_BorderLColor);
	//Bottom
	hge->Gfx_RenderLine(left,bottom,right+1.0f,bottom,m_BorderLColor);

	if (m_Font && (m_Text.length()>0)) m_Font->printf(left+5.0f, top-8.0f, HGETEXT_LEFT, "%s", m_Text.c_str());
};

/******************* SCROLL BOX ********************/

cScrollBox::cScrollBox(hgeFont* Font, float x, float y, float h, float w)
{
	SetFont(Font);
	Init(x,y,h,w);
};

cScrollBox::~cScrollBox()
{
	if(m_imgNormal) delete m_imgNormal;
	if(m_imgDown) delete m_imgDown;
	if(m_TopArrow) delete m_TopArrow;
	if(m_BotArrow) delete m_BotArrow;
	if(m_BckGrnd) delete m_BckGrnd;
};
void cScrollBox::Init(float x, float y, float h, float w)
{
	m_Font = NULL;
	m_BorderDColor = ARGB(255,80,80,80);
	m_BorderLColor = ARGB(255,120,120,120);
	m_ItemHeight = 16;
	m_DisplayCount = 6;
	m_MaxLength=100;
	m_DisplayStart = 0;
	SetPos(x,y);
	SetSize(h,w);
	m_TBtnDown = false;
	m_BBtnDown = false;
	m_RowCount = 0;
	m_GraphicGUI = false;
};


void cScrollBox::MouseDown(float x, float y)
{
	float left = m_Parent->Left()+m_XPos;
	float right = m_Parent->Left()+m_XPos + m_Width ;
	float top = m_Parent->Top()+m_YPos;
	float bottom = m_Parent->Top()+m_YPos + m_Height;

	if (!m_Clicked)
	{
		if (x> right-12.0f && y<top + 12.0f)
		{
			m_TBtnDown = true;
			ScrollUp();
		}
		if (x> right-12.0f && y>bottom - 12.0f)
		{
			m_BBtnDown = true;
			ScrollDown();
		}
	}
	m_Clicked = true;

};
void cScrollBox::MouseUp(float x, float y)
{
	m_Clicked = false;
	m_TBtnDown = false;
	m_BBtnDown = false;
};

void cScrollBox::ScrollUp()
{
	if (m_DisplayStart > 0)
		m_DisplayStart--;
};
void cScrollBox::ScrollDown()
{

	if (m_RowCount > m_DisplayCount && m_DisplayStart < m_RowCount-m_DisplayCount)
		m_DisplayStart++;

};

void cScrollBox::PageUp()
{
	for (int iLoop=0;iLoop<m_DisplayCount;iLoop++)
	{
		ScrollUp();
	}

};
void cScrollBox::PageDown()
{
	for (int iLoop=0;iLoop<m_DisplayCount;iLoop++)
	{
		ScrollDown();
	}

};

bool cScrollBox::KeyPress(int key)
{
	bool rtn = true;

	switch (key)
	{
		case HGEK_UP:
			ScrollUp();
			break;
		case HGEK_DOWN:
			ScrollDown();
			break;
		case HGEK_PGUP:
			PageUp();
			break;
		case HGEK_PGDN:
			PageDown();
			break;
		default:
			rtn = false;
	}
	return rtn;
};

void cScrollBox::SetItemHeight(int h){m_ItemHeight = h;};
void cScrollBox::SetSize(float h, float w)
{
	m_Width = w;
	m_Height = h;
	m_DisplayCount = h / m_ItemHeight;
	//hge->System_Log("DisplayCount %i", m_DisplayCount);
	//hge->System_Log("m_Width %f", m_Width);
};

void cScrollBox::UseGraphicGUI(HTEXTURE guiTex)
{
	m_GraphicGUI = true;
	m_imgNormal=new hgeSprite(guiTex, 26, 65, 27, 27);
//	m_imgOver=new hgeSprite(guiTex, 0, 65, 27, 27);
	m_imgDown=new hgeSprite(guiTex, 56, 65, 27, 27);
	m_TopArrow=new hgeSprite(guiTex, 34, 28, 7, 7);
	m_BotArrow=new hgeSprite(guiTex, 26, 28, 7, 7);
//	m_BckGrnd=new hgeSprite(guiTex, 54, 30, 2, 2);
//	m_BckGrnd->SetColor( ARGB(255,100,100,100));
//	m_BckGrnd=new hgeSprite(guiTex, 91, 1, 8, 13);
	m_BckGrnd=new hgeSprite(guiTex, 10, 3, 5, 20);

};

void cScrollBox::Render(float x, float y, float dt)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;
	float sbleft = right - 15.0f;
	float sbright = right - 1.0f;
	float sbtop = top;
	float sbbottom = bottom - 1.0f;
	//OuterFrame
	//Top
	hge->Gfx_RenderLine(left,top,right,top,m_BorderLColor);
	//Left
	hge->Gfx_RenderLine(left,top,left,bottom,m_BorderLColor);
	//Right
	hge->Gfx_RenderLine(right,top,right,bottom+1,m_BorderLColor);
	//Bottom
	hge->Gfx_RenderLine(left,bottom,right+1,bottom,m_BorderLColor);

	if (m_GraphicGUI)
	{
		m_BckGrnd->RenderStretch(left,top,sbleft,bottom-1.0f);
	};
	//Scroll Bar
	hge->Gfx_RenderLine(sbleft,sbtop,sbleft,bottom,m_BorderLColor);
	if (m_TBtnDown)
	{
		if (m_GraphicGUI)
		{
			m_imgDown->RenderStretch(sbleft,sbtop,sbright,sbtop+14.0f);
			//arrow
			m_TopArrow->Render(sbleft+3.5f,sbtop+1.5f);
		}else
		{
			//Top Button Down
			hge->Gfx_RenderLine(right-12.0f,top,right,top,m_BorderDColor);
			hge->Gfx_RenderLine(right-12.0f,top,right-12.0f,top+12.0f,m_BorderDColor);
			hge->Gfx_RenderLine(right-12.0f,top+12.0f,right,top+12.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right,top,right,top+12.0f,m_BorderLColor);
			//arrow
			hge->Gfx_RenderLine(right-5.0f,top+2.0f,right-8.0f,top+8.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-5.0f,top+2.0f,right-2.0f,top+8.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-7.0f,top+8.0f,right-1.0f,top+8.0f,m_BorderLColor);
		}
	}else{

		if (m_GraphicGUI)
		{
			m_imgNormal->RenderStretch(sbleft,sbtop,sbright,sbtop+14.0f);
			//arrow
			m_TopArrow->Render(sbleft+3.5f,sbtop+2.0f);

		}else
		{
			//Top Button Up
			hge->Gfx_RenderLine(right-12.0f,top,right-12.0f,top+12.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-12.0f,top,right,top,m_BorderLColor);
			hge->Gfx_RenderLine(right-12.0f,top+12.0f,right,top+12.0f,m_BorderDColor);
			hge->Gfx_RenderLine(right,top,right,top+12.0f,m_BorderDColor);

			hge->Gfx_RenderLine(right-6.0f,top+3.0f,right-9.0f,top+9.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-6.0f,top+3.0f,right-3.0f,top+9.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-8.0f,top+9.0f,right-2.0f,top+9.0f,m_BorderLColor);
		}
	}

	if (m_BBtnDown)
	{

		if (m_GraphicGUI)
		{
			m_imgDown->RenderStretch(sbleft,sbbottom-14.0f,sbright,sbbottom);
			//todo arrow
			m_BotArrow->Render(sbleft+3.5f,sbbottom-9.5f);
		}else
		{
			//Bottom Button Down
			hge->Gfx_RenderLine(right-12.0f,bottom,right,bottom,m_BorderLColor);
			hge->Gfx_RenderLine(right-12.0f,bottom,right-12.0f,bottom-12.0f,m_BorderDColor);
			hge->Gfx_RenderLine(right-12.0f,bottom-12.0f,right,bottom-12.0f,m_BorderDColor);
			hge->Gfx_RenderLine(right,bottom-12.0f,right,bottom,m_BorderLColor);

			hge->Gfx_RenderLine(right-5.0f,bottom-2.0f,right-8.0f,bottom-8.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-5.0f,bottom-2.0f,right-2.0f,bottom-8.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-7.0f,bottom-8.0f,right-1.0f,bottom-8.0f,m_BorderLColor);
		}
	}else {

		if (m_GraphicGUI)
		{
			m_imgNormal->RenderStretch(sbleft,sbbottom-14.0f,sbright,sbbottom);
			//todo arrow
			m_BotArrow->Render(sbleft+3.5f,sbbottom-10.0f);
		}else
		{
			//Bottom Button Up
			hge->Gfx_RenderLine(right-12.0f,bottom,right,bottom,m_BorderDColor);
			hge->Gfx_RenderLine(right-12.0f,bottom,right-12.0f,bottom-12.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-12.0f,bottom-12.0f,right,bottom-12.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right,bottom,right,bottom-12.0f,m_BorderDColor);

			hge->Gfx_RenderLine(right-6.0f,bottom-3.0f,right-9.0f,bottom-9.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-6.0f,bottom-3.0f,right-3.0f,bottom-9.0f,m_BorderLColor);
			hge->Gfx_RenderLine(right-8.0f,bottom-9.0f,right-2.0f,bottom-9.0f,m_BorderLColor);
		}
	}
};


/******************* SCROLL TEXT BOX ********************/

cScrollTextBox::cScrollTextBox(hgeFont* Font, float x, float y, float h, float w)
{
	SetFont(Font);
	Init(x,y,h,w);
};

cScrollTextBox::~cScrollTextBox()
{
	Clear();
};

void cScrollTextBox::AddItem(std::string item)
{
	float txtLen = (m_Width-15.0f)/6.0f;
	unsigned int iLoop;
	unsigned int iLastSpace;
	int iStart;
	std::string newItem;

	if (item.length() > txtLen)
	{ //Line is too big
		iStart = 0;
		iLastSpace = item.length()-1;

		for (iLoop=0;iLoop<item.length();iLoop++)
		{
			//mark the last space found
			if (item.substr(iLoop,1) == " ")
				iLastSpace = iLoop;

			if (iLoop >= iStart + txtLen)
			{//have exceeded the len for a line
			 //wrap at the last space
				if (iLastSpace < item.length() && iLastSpace < iLoop)
				{
					newItem = item.substr(iStart,iLastSpace);
					iStart = iLastSpace + 1;
					iLastSpace = item.length();
				}else
				{
					newItem = item.substr(iStart,iLoop);
					iStart = iLoop + 1;
					iLastSpace = item.length();
				}
				m_Items.push_front(newItem);
				m_RowCount++;
			}

			if (iLoop == item.length()-1)
			{//reached the end of the text but not the end of the len for the line
			 //go ahead and add this line as it is
				newItem = item.substr(iStart,item.length());
				m_Items.push_front(newItem);
				m_RowCount++;

			}
		}
	}else
	{
		m_Items.push_front(item);
		m_RowCount++;
	}

	if (m_RowCount >= m_MaxLength)
	{
		//delete item;
		m_Items.pop_back();
		m_RowCount--;
	}
};

void cScrollTextBox::ScrollUp()
{
	if (m_RowCount > m_DisplayCount && m_DisplayStart < m_RowCount-m_DisplayCount)
		m_DisplayStart++;
};
void cScrollTextBox::ScrollDown()
{
	if (m_DisplayStart > 0)
		m_DisplayStart--;

};

void cScrollTextBox::Clear()
{
	std::string item;
	for (int iloop=0;iloop<m_RowCount;iloop++)
	{
		item = m_Items[iloop].c_str();
		item.clear();
	}
	m_Items.clear();
};

void cScrollTextBox::Render(float x, float y, float dt)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;
	float txtLen = (m_Width-15.0f)/6.0f;

	cScrollBox::Render(x,y,dt);
	if (m_Font && m_RowCount > 0)
	{
   	    //hge->System_Log("RowCount %i",m_RowCount);
		for (int iLoop=0;iLoop<m_DisplayCount;iLoop++)
		{
			//hge->System_Log("Draw  %s",m_Items[iLoop+m_DisplayStart].c_str());
			if (iLoop+m_DisplayStart < m_RowCount)
			  m_Font->printf(left+5.0f,bottom-(iLoop*m_ItemHeight)-m_ItemHeight,HGETEXT_LEFT, "%s",m_Items[iLoop+m_DisplayStart].c_str());
		}
	}

};


/******************* SCROLL LIST BOX ********************/

cScrollListBox::cScrollListBox(hgeFont* Font, float x, float y, float h, float w)
{
	SetFont(Font);
	Init(x,y,h,w);
	m_HighlightColor = ARGB(255,100,125,175);
	m_SelectedIndx=-1;
};

cScrollListBox::~cScrollListBox()
{
	Clear();
};

void cScrollListBox::MouseDown(float x, float y)
{
	float left = m_Parent->Left()+m_XPos;
	float right = m_Parent->Left()+m_XPos + m_Width ;
	float top = m_Parent->Top()+m_YPos;
	float bottom = m_Parent->Top()+m_YPos + m_Height;

	if (!m_Clicked)
	{
		//Scroll Bar Buttons
		if (x> right-12.0f && y<top + 12.0f)
		{//Top Button
			m_TBtnDown = true;
			ScrollUp();
		}
		if (x> right-12.0f && y>bottom - 12.0f)
		{ //Bottom Button
			m_BBtnDown = true;
			ScrollDown();
		}
		if (x < right-12.0f && x>left)
		{//	inside the control
			for (int iLoop=0;iLoop<m_DisplayCount+1;iLoop++)
			{//figure out which item was clicked
				if (top+(iLoop*m_ItemHeight) > y)
				{//select the clicked item
					m_SelectedIndx = (iLoop-1)+m_DisplayStart;
					break;
				}
			}
		}

	}
	m_Clicked = true;

};

void cScrollListBox::SetHighlightColor(int red,int green, int blue)
{
	m_HighlightColor = ARGB(255,red,green,blue);
};

int	cScrollListBox::SelectedIndx()
{
	return m_SelectedIndx;
};

void cScrollListBox::SelectedIndx(int indx)
{
	m_SelectedIndx = indx;
}

std::string cScrollListBox::GetItemAt(int indx)
{
	std::string tmpStr = "";
	if (indx > -1 && indx < m_RowCount)
	    tmpStr = m_Items[indx];

	return tmpStr;
};

std::string cScrollListBox::GetSelectedItem()
{
	std::string tmpStr = "";
	if (m_SelectedIndx > -1 && m_SelectedIndx < m_RowCount)
		tmpStr = m_Items[m_SelectedIndx];

	return tmpStr;
};

void cScrollListBox::AddItem(std::string item)
{
	m_Items.push_back(item);
	m_RowCount++;
};

void cScrollListBox::RemoveItem(int indx)
{
	VecItor itor;

	itor = std::find(m_Items.begin(),m_Items.end(),m_Items[indx]);
	m_Items.erase(itor);
	m_RowCount--;

};


void cScrollListBox::Clear()
{
	//std::string item;
	//for (int iloop=0;iloop<m_RowCount;iloop++)
	//{
	//	item = m_Items[iloop];
	//	item.clear();
	//}
	m_Items.clear();
	m_RowCount = 0;
	m_SelectedIndx = -1;
};

void cScrollListBox::Render(float x, float y, float dt)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;
	float txtLen = (m_Width-15.0f)/6.0f;

	//Debug Info
	//m_Font->printf(10.0f,200,"%i",SelectedIndx);
	cScrollBox::Render(x,y,dt);
	if (m_Font && m_RowCount > 0)
	{
   	    //hge->System_Log("RowCount %i",m_RowCount);
		for (int iLoop=0;iLoop<m_DisplayCount;iLoop++)
		{
			//hge->System_Log("Draw  %s",m_Items[iLoop+m_DisplayStart].c_str());
			if (iLoop+m_DisplayStart < m_RowCount)
			{
				if (iLoop+m_DisplayStart == m_SelectedIndx)
				{
					//m_Font->SetColor(m_HighlightColor);
					for (int jLoop=0;jLoop<m_ItemHeight;jLoop++)
						hge->Gfx_RenderLine(left+2.0f,(top+0.5f+(iLoop*m_ItemHeight))+jLoop,right-16.0f,(top+0.5f+(iLoop*m_ItemHeight))+jLoop,m_HighlightColor);
					//hge->Gfx_RenderLine(left+2.0f,(top+(iLoop*m_ItemHeight)),right-13.0f,(top+(iLoop*m_ItemHeight)),m_HighlightColor);
					//hge->Gfx_RenderLine(left+2.0f,(top+(iLoop*m_ItemHeight))+m_ItemHeight,right-13.0f,(top+(iLoop*m_ItemHeight))+m_ItemHeight,m_HighlightColor);

				}
				//else
				//	m_Font->SetColor(m_FontColor);

				 m_Font->printf(left+5.0f,top+1.0f+(iLoop*m_ItemHeight),HGETEXT_LEFT, "%s",m_Items[iLoop+m_DisplayStart].substr(0,txtLen).c_str());
			}
		}
	}


};

/******************* Button ********************/

cButton::cButton(hgeFont* Font, float x, float y, float h, float w)
{
	SetPos(x,y);
	SetSize(h,w);
	SetFont(Font);
	m_TextLeft = 0.0f;
	m_TextTop = 0.0f;
	m_BtnDown = false;
	m_GraphicGUI = false;
	m_GraphicBack = false;
	m_Enabled = true;
};
cButton::~cButton()
{
	if (m_imgNormal) delete m_imgNormal;
	if (m_imgDown) delete m_imgDown;
	if (m_imgOver) delete m_imgOver;
	if (m_BckGrnd) delete m_BckGrnd;
};

void cButton::UseGraphicGUI(HTEXTURE guiTex)
{
	m_GraphicGUI = true;
	m_GraphicBack = false;
	m_imgNormal=new hgeSprite(guiTex, 61, 37, 62, 27);
	m_imgOver=new hgeSprite(guiTex, 27, 0, 62, 27);
	m_imgDown=new hgeSprite(guiTex, 0, 37, 62, 27);
}

void cButton::UseGraphicBack(HTEXTURE guiTex)
{
	m_GraphicBack = true;
	m_GraphicGUI = false;
	m_BckGrnd=new hgeSprite(guiTex, 10, 3, 5, 20);
};



void cButton::Render(float x, float y, float dt)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;

	//m_Font->SetColor(m_FontColor);

	if (m_GraphicBack)
	{
		m_BckGrnd->RenderStretch(left,top,right,bottom-1.0f);
	};

	//OuterFrame
	if (m_BtnDown && m_Enabled)
	{
		if (m_GraphicGUI)
		{
			m_imgDown->RenderStretch(left,top,right,bottom);
		}else
		{
			//Top
			hge->Gfx_RenderLine(left,top,right,top,m_BorderDColor);
			//Left
			hge->Gfx_RenderLine(left,top,left,bottom,m_BorderDColor);
			//Right
			hge->Gfx_RenderLine(right,top,right,bottom+1.0f,m_BorderLColor);
			//Bottom
			hge->Gfx_RenderLine(left,bottom,right+1.0f,bottom,m_BorderLColor);
		}

		if (m_Font && (m_Text.length()>0) ) m_Font->printf(left+ m_TextLeft +2.0f, top + m_TextTop +2.0f,HGETEXT_LEFT,  "%s", m_Text.c_str());
	}else
	{

		if (m_GraphicGUI)
		{
			if (m_MouseOver && m_Enabled)
			{
				m_imgOver->RenderStretch(left,top,right,bottom);
			}else
				m_imgNormal->RenderStretch(left,top,right,bottom);
		}else
		{
			//Top
			hge->Gfx_RenderLine(left,top,right,top,m_BorderLColor);
			//Left
			hge->Gfx_RenderLine(left,top,left,bottom,m_BorderLColor);
			//Right
			hge->Gfx_RenderLine(right,top,right,bottom+1.0f,m_BorderDColor);
			//Bottom
			hge->Gfx_RenderLine(left,bottom,right+1.0f,bottom,m_BorderDColor);
		}


		if (m_Font && (m_Text.length()>0)) m_Font->printf(left+ m_TextLeft, top + m_TextTop,HGETEXT_LEFT,  "%s", m_Text.c_str());
	}


};
void cButton::MouseDown(float x, float y)
{
	if (m_Enabled)
	{
		if (!m_Clicked) m_Parent->ControlClick(m_ID);
		m_Clicked = true;
		m_BtnDown = true;
	}

};
void cButton::MouseUp(float x, float y)
{
	m_Clicked = false;
	m_BtnDown = false;
};

void cButton::CenterCaption()
{
	if(m_Font)
	{
		m_TextLeft = m_Width/2 - m_Font->GetStringWidth(m_Text.c_str())/2;
		m_TextTop = m_Height/2 - 7;
	}
}


/******************* TEXT BOX ********************/

cTextBox::cTextBox(hgeFont* Font, float x, float y, float h, float w)
{
	SetPos(x,y);
	SetSize(h,w);
	SetFont(Font);
	m_TextTop = m_Height/2.0f - 7.0f;
	m_Focus = false;
	m_BlinkTimer = 0;
	m_BlinkOn = false;
	m_MaxLength = 249;
	m_CursorPos = 0;
};

void cTextBox::SetFocus(){m_Focus = true;};
void cTextBox::LostFocus(){m_Focus = false;};
bool cTextBox::GetFocus(){return m_Focus;};
int	cTextBox::GetMaxLen(){return m_MaxLength;};
void cTextBox::SetMaxLen(int l)
{
	if (l<249) m_MaxLength = l;
};

void cTextBox::Render(float x, float y, float dt)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;
	char CursorDisplay[200];
	float CursorDrawPos = 0.0f;
	std::string tmpstr;
	m_BlinkTimer += dt;
	//Top
	hge->Gfx_RenderLine(left,top,right,top,m_BorderLColor);
	//Left
	hge->Gfx_RenderLine(left,top,left,bottom,m_BorderLColor);
	//Right
	hge->Gfx_RenderLine(right,top,right,bottom+1,m_BorderLColor);
	//Bottom
	hge->Gfx_RenderLine(left,bottom,right+1,bottom,m_BorderLColor);
	/*
		m_Width < m_Font->GetStringWidth

	*/
	tmpstr = m_strFullText.substr(m_DisplayStart, m_DisplayStop);

	if (m_CursorPos > m_DisplayStart)
	{
		strcpy(CursorDisplay,tmpstr.substr(0,m_CursorPos-m_DisplayStart).c_str());
		CursorDrawPos = m_Font->GetStringWidth(CursorDisplay);
	}


	if (m_Font)
	{
		//Debug stuff
  		//m_Font->printf(10.0f,200,"%i/%i/%i",m_DisplayStart,m_DisplayStop,tmpstr.length());

		m_Font->SetColor(ARGB(255,255,255,255));
  		m_Font->printf(left + 5.0f,top + m_TextTop,HGETEXT_LEFT, "%s",tmpstr.c_str());
		if (m_Focus && m_BlinkOn)
		{
			m_Font->SetColor(ARGB(255,255,255,0)); //change the color of the cursor
			m_Font->printf(left + 5.0f + CursorDrawPos,top + m_TextTop,HGETEXT_LEFT, "|");
			m_Font->SetColor(ARGB(255,255,255,255));//put it back when we are done
		}
	}

	if (m_BlinkTimer > 0.40f)
	{
		m_BlinkTimer = 0;
		m_BlinkOn = !m_BlinkOn;
	};
}

void cTextBox::MouseDown(float x, float y)
{
	m_Focus = true;
};

void cTextBox::SetText(char *txt)
{
	m_strFullText=txt;

	m_CursorPos = m_strFullText.length();
    m_DisplayStop = m_strFullText.length();
	m_DisplayStart = GetNewDisplayStart(m_DisplayStop);
};

bool cTextBox::CharPress(int key)
{//Get the letter key passed in
	std::string tmpChar;

	if (!m_Focus) return false;
	if (key==0) return false;
	if (key==HGEK_ENTER) return false;
	if (!m_Enabled) return false;
	if (key==HGEK_ESCAPE)
		m_Focus = false;
	else
	if (key==HGEK_BACKSPACE)
	{
		if (!m_strFullText.empty() && m_CursorPos > 0)
		{ //trim off the previous character
		  m_strFullText.erase(m_CursorPos-1,1);
		  m_CursorPos--;
		}
		if (m_DisplayStop>m_strFullText.length())
		{
			m_DisplayStop = m_strFullText.length();
			m_DisplayStart = GetNewDisplayStart(m_DisplayStop);
		}
	}else
	{
		if (m_strFullText.length() < m_MaxLength)
		{
			tmpChar = char(key);
			m_strFullText.insert(m_CursorPos,tmpChar);
			tmpChar.clear();
			m_CursorPos++;
		}
		if (m_CursorPos = m_strFullText.length())
		{
			m_DisplayStop = m_strFullText.length();
			m_DisplayStart = GetNewDisplayStart(m_DisplayStop);
		}
	}


	return true;
};

int cTextBox::GetNewDisplayStart(int displaystop)
{
	char tmpStr[250];
	int rtn;
	rtn = 0;
	//get the width of the string from display start to the end
	strcpy(tmpStr,m_strFullText.substr(rtn,displaystop).c_str());
	if (m_Font->GetStringWidth(tmpStr) >= m_Width -12.0f)
		for (int iLoop=m_strFullText.length();iLoop>0;iLoop--)
		{//loop through the string to find a stop that consides with the width
			strcpy(tmpStr,m_strFullText.substr(iLoop,displaystop).c_str());
			if (m_Font->GetStringWidth(tmpStr) >= m_Width -12.0f)
			{
				rtn = iLoop+1;
				break;
			}
		}
	return rtn;
};

int cTextBox::GetNewDisplayStop(int displaystart)
{
	char tmpStr[250];
	int rtn;

	rtn = m_strFullText.length();

	//get the width of the string from display start to the end
	strcpy(tmpStr,m_strFullText.substr(displaystart).c_str());
	if (m_Font->GetStringWidth(tmpStr) >= m_Width -12.0f)
		for (int iLoop=m_DisplayStart;iLoop<m_strFullText.length();iLoop++)
		{//loop through the string to find a stop that consides with the width
			strcpy(tmpStr,m_strFullText.substr(displaystart,iLoop).c_str());
			if (m_Font->GetStringWidth(tmpStr) >= m_Width -12.0f)
			{
				rtn = iLoop -1;
				break;
			}
		}
	return rtn;
};

bool cTextBox::KeyPress(int key)
{
	bool rtn = true;

	if (!m_Focus) return false;
	if (key==0) return false;
	if (!m_Enabled) return false;
	switch(key)
	{
		case HGEK_LEFT:
  			if (m_CursorPos > 0)
			{
				if (m_DisplayStart > 0 && m_DisplayStart == m_CursorPos)
				{
					m_DisplayStart--;
					m_DisplayStop =GetNewDisplayStop(m_DisplayStart);
				}
				m_CursorPos--;
			}
			break;

		case HGEK_RIGHT:
			if (m_CursorPos < m_strFullText.length())
			{
				if (m_CursorPos == m_DisplayStop)
				{
					m_DisplayStop++;
					m_DisplayStart = GetNewDisplayStart(m_DisplayStop);
				}
				m_CursorPos++;
			}
			break;

		case HGEK_HOME:
			{
				m_CursorPos = 0;
				m_DisplayStart = 0;
				m_DisplayStop =GetNewDisplayStop(m_DisplayStart);
			}
			break;

		case HGEK_END:
			{
				m_CursorPos = m_strFullText.length();
				m_DisplayStop = m_strFullText.length();
				m_DisplayStart = GetNewDisplayStart(m_DisplayStop);
			}
			break;

		case HGEK_DELETE:
			{
				if (!m_strFullText.empty() && m_CursorPos < m_strFullText.length())
				{ //trim off the next character
				m_strFullText.erase(m_CursorPos,1);
				}
			}
			break;
		default:
			rtn = false;
	}
	return rtn;
};


std::string cTextBox::GetText()
{
	return m_strFullText;
};

void cTextBox::Clear()
{
	m_DisplayStart = 0;
	m_CursorPos = 0;
	m_strFullText = "";
};

/******************* TAB CONTROL ********************/

cTabControl::cTabControl(hgeFont* Font, float x, float y, float h, float w, float tw, float th)
{
	SetFont(Font);
	m_TabWidth	= tw;
	m_TabHeight = th;
	m_TabCount = 0;
	m_CurrentIndx = 0;
	m_Clicked = false;
	SetPos(x,y);
	SetSize(h,w);
};

cTabControl::~cTabControl()
{
	DeleteControls();
};


void cTabControl::DeleteControls()
{
	cBaseContainer *control;
	for (unsigned int iloop=0;iloop<m_Tabs.size();iloop++)
	{
		control = m_Tabs[iloop];
		delete control;
	}
	m_Tabs.clear();
};

void cTabControl::Setup(sControlTemplate ct)
{
	m_Font = ct.Font;
	m_FontColor = ct.FontColor;
	m_BorderDColor = ct.BorderDColor;
	m_FontColor = ct.FontColor;
	m_BorderLColor = ct.BorderLColor;
	m_BackColor = ct.BackColor;
	//CntrlTmp = ct;
	TabTmp.BackColor = ct.BackColor;
	TabTmp.BorderDColor = ct.BorderDColor;
	TabTmp.BorderLColor = ct.BorderLColor;
	TabTmp.Font = ct.Font;
	TabTmp.FontColor = ct.FontColor;
};

void cTabControl::Render(float x, float y, float dt)
{

	DrawTabs(x,y);
	DrawTabPage(x,y,dt);
};

void cTabControl::DrawTabs(float x, float y)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float tabBottom = y+m_YPos+m_TabHeight;
	float bottom = y+m_YPos + m_Height;
	int txtLeft;

	for (unsigned int iLoop = 0;iLoop<m_TabCount;iLoop++)
	{
		txtLeft = m_TabWidth/2 - m_Font->GetStringWidth(m_Tabs[iLoop]->GetText().c_str())/2;
		if (iLoop == m_CurrentIndx)
		{
			hge->Gfx_RenderLine(left+(iLoop * m_TabWidth)+3,tabBottom,left+(iLoop * m_TabWidth)+3,top,m_BorderLColor);
			hge->Gfx_RenderLine(left+(iLoop * m_TabWidth)+3,top,left+(iLoop * m_TabWidth)+m_TabWidth+1,top,m_BorderLColor);
			hge->Gfx_RenderLine(left+(iLoop * m_TabWidth)+m_TabWidth,tabBottom,left+(iLoop * m_TabWidth)+m_TabWidth,top,m_BorderLColor);
			m_Font->printf(left+(iLoop * m_TabWidth)+txtLeft,top+3,HGETEXT_LEFT, "%s",m_Tabs[iLoop]->GetText());
		}
		else
		{
			hge->Gfx_RenderLine(left+(iLoop * m_TabWidth)+3,tabBottom-3,left+(iLoop * m_TabWidth)+3,top,m_BorderDColor);
			hge->Gfx_RenderLine(left+(iLoop * m_TabWidth)+3,top,left+(iLoop * m_TabWidth)+m_TabWidth+1,top,m_BorderDColor);
			hge->Gfx_RenderLine(left+(iLoop * m_TabWidth)+m_TabWidth,tabBottom-3,left+(iLoop * m_TabWidth)+m_TabWidth,top,m_BorderDColor);
			m_Font->printf(left+(iLoop * m_TabWidth)+txtLeft,top+3,HGETEXT_LEFT, "%s",m_Tabs[iLoop]->GetText());
		}
	}

	//OuterFrame
	//Top
	if (m_CurrentIndx > -1)
	{
		hge->Gfx_RenderLine(left,tabBottom,left+(m_CurrentIndx * m_TabWidth)+3,tabBottom,m_BorderLColor);
		hge->Gfx_RenderLine(left+(m_CurrentIndx * m_TabWidth)+m_TabWidth,tabBottom,right,tabBottom,m_BorderLColor);
	}
	else
		hge->Gfx_RenderLine(left,tabBottom,right,tabBottom,m_BorderLColor);

	//Left
	hge->Gfx_RenderLine(left,tabBottom,left,bottom,m_BorderLColor);
	//Right
	hge->Gfx_RenderLine(right,tabBottom,right,bottom+1.0f,m_BorderLColor);
	//Bottom
	hge->Gfx_RenderLine(left,bottom,right+1.0f,bottom,m_BorderLColor);

};

void cTabControl::DrawTabPage(float x, float y, float dt)
{
	if (m_CurrentIndx>-1)
	{
	   m_Tabs[m_CurrentIndx]->Render(x,y,dt);
	}
};

void cTabControl::AddControl(cBaseControl* control, int tabindx)
{
	m_Tabs[tabindx]->AddControl(control);
};

void cTabControl::AddTab(char *capt)
{
	cBaseContainer* newTabControl = new cBaseContainer(m_XPos, m_YPos+m_TabHeight, m_Height-m_TabHeight, m_Width);
	newTabControl->SetParentWindow(m_Parent);
	newTabControl->ShowBorder(false);
	newTabControl->Setup(TabTmp);
	newTabControl->SetText(capt);
	//newTabControl->SetFont(m_Font);
	m_Tabs.push_back(newTabControl);
	newTabControl = NULL;
	m_TabCount++;
};

bool cTabControl::CharPress(int key)
{
	if ((m_CurrentIndx > -1) && m_Enabled)
		return m_Tabs[m_CurrentIndx]->CharPress(key);
	else
		return false;
};
bool cTabControl::KeyPress(int key)
{
	if ((m_CurrentIndx > -1) && m_Enabled)
		return m_Tabs[m_CurrentIndx]->KeyPress(key);
	else
		return false;
}

void cTabControl::MouseMove(float x, float y)
{
	if ((m_CurrentIndx > -1) && m_Enabled)
		m_Tabs[m_CurrentIndx]->MouseMove(x, y);
};
void cTabControl::MouseDown(float x, float y)
{

	if (m_Enabled)
	{
		float left = m_Parent->Left()+m_XPos;
		float top = m_Parent->Top()+m_YPos;
		float tabBottom = top+m_TabHeight;

		if (y > top && y < tabBottom )
		{
			if(!m_Clicked)
			for (unsigned int iLoop=0;iLoop<m_TabCount;iLoop++)
			{
				if (x > (left+(iLoop * m_TabWidth)+3) && x < (left+(iLoop * m_TabWidth)+m_TabWidth))
				{
					if (iLoop != m_CurrentIndx)
						m_CurrentIndx = iLoop;
					m_Clicked = true;
					break;
				}
			}
		}
		else
		if (m_CurrentIndx > -1)
			m_Tabs[m_CurrentIndx]->MouseDown(x, y);
	}
};
void cTabControl::MouseUp(float x, float y)
{
	m_Clicked = false;
	if (m_CurrentIndx > -1)
		m_Tabs[m_CurrentIndx]->MouseUp(x, y);
};

/******************* Check Box ********************/

cChkBox::cChkBox(hgeFont* Font, float x, float y, float h, float w)
{
	SetPos(x,y);
	SetSize(h,w);
	SetFont(Font);
	m_TextLeft = 0.0f;
	m_TextTop = 0.0f;
	m_Checked = false;
	//strcpy(m_Text, "Check Box");
};
cChkBox::~cChkBox()
{
	if (m_imgNormal) delete m_imgNormal;
	if (m_imgChecked) delete m_imgChecked;
};
bool cChkBox::Checked(){return m_Checked;};
void cChkBox::Checked(bool chk){m_Checked=chk;};

void cChkBox::UseGraphicGUI(HTEXTURE guiTex)
{
	m_GraphicGUI = true;
	m_imgNormal=new hgeSprite(guiTex, 84, 79, 13, 13);
	m_imgChecked=new hgeSprite(guiTex, 84, 65, 13, 13);
};

void cChkBox::Render(float x, float y, float dt)
{
	float left = x+m_XPos;
	float right = x+m_XPos + m_Width ;
	float top = y+m_YPos;
	float bottom = y+m_YPos + m_Height;


	if (m_GraphicGUI)
	{
		if(m_Checked)
		{
			m_imgChecked->RenderStretch(left,top+4.0f,left+12.0f,bottom-4.0f);
		}else
		{
			m_imgNormal->RenderStretch(left,top+4.0f,left+12.0f,bottom-4.0f);
		}
	}else
	{
		//Left
		hge->Gfx_RenderLine(left,top+5.0f,left,bottom-5.0f,m_BorderDColor);
		//Top
		hge->Gfx_RenderLine(left,top+5.0f,left+10.0f,top+5.0f,m_BorderDColor);
		//Right
		hge->Gfx_RenderLine(left+10,top+5.0f,left+10.0f,bottom-4.5f,m_BorderDColor);
		//Bottom
		hge->Gfx_RenderLine(left,bottom-5.0f,left+10.0f,bottom-5.0f,m_BorderDColor);

		if(m_Checked)
		{
			hge->Gfx_RenderLine(left+1.0f,top+6.0f,left+10,bottom-5.0f,m_FontColor);
			hge->Gfx_RenderLine(left+10,top+5.0f,left,bottom-5.0f,m_FontColor);
		}
	}

	if (m_Font && (m_Text.length()>0)) m_Font->printf(left+15.0f, top+3.0f, HGETEXT_LEFT, "%s", m_Text.c_str());

};
void cChkBox::MouseDown(float x, float y)
{
	if (!m_Clicked && m_Enabled)
	{
		m_Clicked = true;
	}

};
void cChkBox::MouseUp(float x, float y)
{
   if (MouseTest(x,y) && m_Clicked)
   {
	   m_Checked = !m_Checked;
   }
   m_Clicked = false;
};







