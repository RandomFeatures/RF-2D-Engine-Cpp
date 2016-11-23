#include "RF3d_Global.h"
#include "gui_ConnectWindow.h"
#include "RF2d_Application.h"

cConnectWindow_gui::cConnectWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd)
{
	m_GameParent = GameParent;
	SetBaseFont(Font);
	ControlBox.Image = CtrlBtn;
	CloseButton.Image = CloseBtn;
	BackGround.Image = bckgrnd;
	BackGround.Color = ARGB(100,0,0,0);
	SetSize(240,320);
	AlignCenter();
	//SetMaxSize(200,320);
	//SetMinSize(200,320);
	SetBorderColor(150,200,100);
	Caption.Text = "Join a Game";
	CenterCaption();
	Grip.Visible = false;
	CloseButton.Enabled = false;
	CloseButton.Visible = false;
	ControlTemp.BorderDColor = ARGB(255,150,150,150);
	ControlTemp.BorderLColor = ARGB(255,250,250,250);
	ControlTemp.Font = Font;
	ControlTemp.FontColor = ARGB(255,255,255,255);
	CreateControls();
	SetFocus();
	m_Visible = false;
};

cConnectWindow_gui::~cConnectWindow_gui()
{
};

void cConnectWindow_gui::Init()
{
	m_NameTextBox->SetText(hge->Ini_GetString("join","name",""));
	m_IPTextBox->SetText(hge->Ini_GetString("join","ip",""));
	//hge->System_Log(hge->Ini_GetString("join","name",""));
	//hge->System_Log(hge->Ini_GetString("join","ip",""));
};
void cConnectWindow_gui::CreateControls()
{

	m_IPLabel = new cLabel(m_BFont,16.0f,24.0f,20.0f,60.0f);
	m_IPLabel->SetText("IP Address");
	AddControl(m_IPLabel);

	m_NameLabel = new cLabel(m_BFont,16.0f,78.0f,20.0f,100.0f);;
	m_NameLabel->SetText("Player Name (Optional)");
	AddControl(m_NameLabel);

	m_PassLabel = new cLabel(m_BFont,16.0f,132.0f,20.0f,100.0f);;
	m_PassLabel->SetText("Password (Optional)");
	AddControl(m_PassLabel);

	m_IPTextBox = new cTextBox(m_BFont,16.0f,40.0f,20.0f,280.0f);
	m_IPTextBox->ID(1);
	m_IPTextBox->SetMaxLen(15);
	AddControl(m_IPTextBox);

	m_NameTextBox = new cTextBox(m_BFont,16.0f,95.0f,20.0f,280.0f);
	m_NameTextBox->ID(2);
	m_NameTextBox->SetMaxLen(20);
	AddControl(m_NameTextBox);

	m_PassTextBox = new cTextBox(m_BFont,16.0f,150.0f,20.0f,280.0f);
	m_PassTextBox->ID(3);
	m_PassTextBox->SetMaxLen(20);
	AddControl(m_PassTextBox);

	m_DefaultChk = new cChkBox(m_BFont,16.0f,180.0f, 20.0f,285.0f);
	m_DefaultChk->SetText("Load Default Army");
	m_DefaultChk->Checked(false);
	m_DefaultChk->ID(6);
	AddControl(m_DefaultChk);

	m_BtnConnect = new cButton(m_BFont,176.0f,210.0f,20.0f,75.0f);
	m_BtnConnect->SetText("Connect");
	m_BtnConnect->ID(4);
	m_BtnConnect->CenterCaption();
	AddControl(m_BtnConnect);

	m_BtnBack = new cButton(m_BFont,55.0f,210.0f,20.0f,75.0f);
	m_BtnBack->SetText("<< Back");
	m_BtnBack->ID(5);
	m_BtnBack->CenterCaption();
	AddControl(m_BtnBack);
	m_IPTextBox->SetFocus();

};

void cConnectWindow_gui::ResizeWindow()
{
};

void cConnectWindow_gui::DrawWindow(float dt)
{
//	BackGround.Image->SetColor(ARGB(255,0,0,0));
	cBaseWindow::DrawWindow(dt);

};

bool cConnectWindow_gui::KeyPress(int key)
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
		rtn = m_IPTextBox->KeyPress(key);
		if (!rtn)
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

void cConnectWindow_gui::ControlClick(int id)
{
	if (m_Visible)
	{
		switch(id)
		{
		case 4:
			{
			if (m_IPTextBox->GetText().length() > 0)
			{
				HideWindow();
				hge->Ini_SetString("join","name",m_NameTextBox->GetText().c_str());
				hge->Ini_SetString("join","ip",m_IPTextBox->GetText().c_str());
				m_GameParent->GetMPlayerControl()->SetPlayerData(m_NameTextBox->GetText(),m_PassTextBox->GetText());
				m_GameParent->GetMPlayerControl()->UseDefaultArmy(m_DefaultChk->Checked());
//				m_GameParent->GetGUIManager()->ShowCharSelectWindow();
				m_GameParent->ConnectGame(m_IPTextBox->GetText().c_str());//ConnectButton
			}
			break;
			}
		case 5:
			{
				HideWindow();
				m_GameParent->ChangeGameState(MPMENU, CURRENT);//Back Button
			break;
			}
		}
	}

};




