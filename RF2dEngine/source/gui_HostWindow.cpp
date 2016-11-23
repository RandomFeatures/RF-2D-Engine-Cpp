#include "RF3d_Global.h"
#include "gui_HostWindow.h"
#include "RF2d_Application.h"

cHostWindow_gui::cHostWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd)
{
	m_GameParent = GameParent;
	SetBaseFont(Font);
	ControlBox.Image = CtrlBtn;
	CloseButton.Image = CloseBtn;
	BackGround.Image = bckgrnd;
	BackGround.Color = ARGB(100,0,0,0);
	SetSize(170,320);
	AlignCenter();
	//SetMaxSize(200,320);
	//SetMinSize(200,320);
	SetBorderColor(150,200,100);
	Caption.Text = "Host a Game";
	CenterCaption();
	Grip.Visible = false;
	CloseButton.Enabled = false;
	CloseButton.Visible = false;
	ControlBox.Visible = false;
	ControlBox.Enabled = false;
	ControlTemp.BorderDColor = ARGB(255,150,150,150);
	ControlTemp.BorderLColor = ARGB(255,250,250,250);
	ControlTemp.Font = Font;
	ControlTemp.FontColor = ARGB(255,255,255,255);
	CreateControls();
	SetFocus();
};

cHostWindow_gui::~cHostWindow_gui()
{
};

void cHostWindow_gui::Init()
{
	m_NameTextBox->SetText(hge->Ini_GetString("host","name",""));
};

void cHostWindow_gui::CreateControls()
{
	m_NameLabel = new cLabel(m_BFont,16.0f,24.0f,20.0f,60.0f);
	m_NameLabel->SetText("Player Name (Optional)");
	AddControl(m_NameLabel);

	m_PassLabel = new cLabel(m_BFont,16.0f,78.0f,20.0f,100.0f);
	m_PassLabel->SetText("Password (Optional)");
	AddControl(m_PassLabel);

	m_NameTextBox = new cTextBox(m_BFont,16.0f,40.0f,20.0f,280.0f);
	m_NameTextBox->ID(2);
	m_NameTextBox->SetMaxLen(20);
	AddControl(m_NameTextBox);

	m_PassTextBox = new cTextBox(m_BFont,16.0f,95.0f,20.0f,280.0f);
	m_PassTextBox->ID(3);
	m_PassTextBox->SetMaxLen(20);
	AddControl(m_PassTextBox);

	m_BtnStart = new cButton(m_BFont,176.0f,140.0f,20.0f,75.0f);
	m_BtnStart->SetText("Next >>");
	m_BtnStart->ID(4);
	m_BtnStart->CenterCaption();
	AddControl(m_BtnStart);

	m_BtnBack = new cButton(m_BFont,55.0f,140.0f,20.0f,75.0f);
	m_BtnBack->SetText("<< Back");
	m_BtnBack->ID(5);
	m_BtnBack->CenterCaption();
	AddControl(m_BtnBack);
	m_NameTextBox->SetFocus();

};

void cHostWindow_gui::ResizeWindow()
{
};

void cHostWindow_gui::DrawWindow(float dt)
{
//	BackGround.Image->SetColor(ARGB(255,0,0,0));
	cBaseWindow::DrawWindow(dt);

};

bool cHostWindow_gui::KeyPress(int key)
{
	bool rtn = false;
	if (m_Visible)
	{
	//if (m_Focus && m_TextBox->GetFocus())
	//{
	//   if (key==HGEK_ENTER)
	//   {
	//	 SendText();
	//	 //m_TextBox->LostFocus();
	//	 rtn = true;
	//   }
	//   else
	//   if (key==HGEK_UP || key==HGEK_DOWN || key==HGEK_PGUP || key==HGEK_PGDN)
	//   {
	//	rtn = m_ScrollBox->KeyPress(key);
	//   }
	//   else
		rtn = m_PassTextBox->KeyPress(key);
		if (!rtn)
			rtn = m_NameTextBox->KeyPress(key);
	//}
	//else
	//   if (key==HGEK_ENTER)
	//	m_TextBox->SetFocus();
	}
	return rtn;
};

void cHostWindow_gui::ControlClick(int id)
{
	char map[50];
	strcpy(map,m_GameParent->GetMPlayerControl()->GetMapName());
	if (m_Visible)
	{
		switch(id)
		{
		case 4:
			{
				HideWindow();
				hge->Ini_SetString("host","name",m_NameTextBox->GetText().c_str());
				m_GameParent->GetMPlayerControl()->HostGame();
				m_GameParent->GetMPlayerControl()->SetPlayerData(m_NameTextBox->GetText(),m_PassTextBox->GetText());
				if (m_GameParent->GetMPlayerControl()->UseDefaultArmy())
				{
					m_GameParent->ChangeMap(map, m_GameParent->GetMPlayerControl()->GetSide());
					m_GameParent->GetGUIManager()->ShowMPlayGameWindows();
					m_GameParent->ChangeGameState(MPGAME, CURRENT);
				}
				else
				{
					m_GameParent->ChangeMap(map,CUSTTEAM);
					//goto character select
					m_GameParent->GetGUIManager()->ShowCharSelectWindow();
					m_GameParent->GetGUIManager()->ShowChatWindow();
					m_GameParent->ChangeGameState(TEAM, CURRENT);
				}
				break;
			}
		case 5:
			{
				HideWindow();
				m_GameParent->GetGUIManager()->ShowMapSelectWindow();
				break;
			}
		}
	}

};




