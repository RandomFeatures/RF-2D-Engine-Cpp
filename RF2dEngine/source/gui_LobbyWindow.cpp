#include "RF3d_Global.h"
#include "gui_LobbyWindow.h"

cLobbyWindow_gui::cLobbyWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd)
{
	m_GameParent = GameParent;
//	m_MPlayerControl = NULL;
	SetBaseFont(Font);
	ControlBox.Image = CtrlBtn;
	CloseButton.Image = CloseBtn;
	BackGround.Image = bckgrnd;
	BackGround.Color = ARGB(255,100,100,100);
	SetSize(500,700);
	AlignCenter();
	//SetPos(200,250);
	SetMaxSize(500,700);
	SetMinSize(500,700);
	SetBorderColor(200,200,200);
	Caption.Text = "Lobby";
	CenterCaption();
	Grip.Visible = false;
	CloseButton.Enabled = false;
	CloseButton.Visible = false;
	ControlTemp.BorderDColor = ARGB(255,180,180,180);
	ControlTemp.BorderLColor = ARGB(255,250,250,250);
	ControlTemp.Font = Font;
	ControlTemp.FontColor = ARGB(255,255,255,255);
	SetFocus();
	CreateControls();
};

cLobbyWindow_gui::~cLobbyWindow_gui()
{
	cBaseWindow::~cBaseWindow();
};

void cLobbyWindow_gui::CreateControls()
{

	cGroupBox* tmpGroupBox;

	tmpGroupBox = new cGroupBox(m_BFont,10.0f,30.0f,200.0f,m_Width -20.0f);
	tmpGroupBox->SetText("Select your army");
	AddControl(tmpGroupBox);
	tmpGroupBox = NULL;



	tmpGroupBox = new cGroupBox(m_BFont,10.0f,240.0f,250.0f,m_Width -20.0f);
	tmpGroupBox->SetText("Game Lobby");
	AddControl(tmpGroupBox);
	m_ChatGroup = new cTabControl(m_BFont,tmpGroupBox->X() + tmpGroupBox->Width() -166.0f,tmpGroupBox->Y() + 10.0f,tmpGroupBox->Height() - 40.0f,156.0f,75.0f,20.0f);
	tmpGroupBox->AddControl(m_ChatGroup);
	m_ChatGroup->AddTab("Room");
	m_ChatGroup->AddTab("Friends");

	m_ScrollRoom = new cScrollListBox(m_BFont,m_ChatGroup->X()+5,m_ChatGroup->Y()+25,m_ChatGroup->Height()-30,m_ChatGroup->Width()-10);
	m_ScrollFriends = new cScrollListBox(m_BFont,m_ChatGroup->X()+5,m_ChatGroup->Y()+25, m_ChatGroup->Height()-30,m_ChatGroup->Width()-10);
	m_ChatGroup->AddControl(m_ScrollRoom,0);
	m_ChatGroup->AddControl(m_ScrollFriends,1);

	m_CheckBox = new cChkBox(m_BFont,tmpGroupBox->X()+tmpGroupBox->Width()-166.0f,tmpGroupBox->Y()+tmpGroupBox->Height()-25.0f, 20.0f,156.0f);
	m_CheckBox->SetText("Accept Challenges");
	tmpGroupBox->AddControl(m_CheckBox);


	m_ScrollChatBox = new cScrollTextBox(m_BFont,tmpGroupBox->X()+10.0f,tmpGroupBox->Y()+10.0f,tmpGroupBox->Height()-40.0f,tmpGroupBox->Width() -186.0f);
	tmpGroupBox->AddControl(m_ScrollChatBox);
	m_BtnSend = new cButton(m_BFont,tmpGroupBox->X()+tmpGroupBox->Width()-226.0f,tmpGroupBox->Y()+tmpGroupBox->Height() - 25.0f,20.0f,50.0f);
	m_BtnSend->SetText("Send");
	m_BtnSend->ID(1);
	m_BtnSend->CenterCaption();
	tmpGroupBox->AddControl(m_BtnSend);
	m_TextBox = new cTextBox(m_BFont,tmpGroupBox->X()+10.0f,tmpGroupBox->Y()+tmpGroupBox->Height() - 25.0f,20.0f,tmpGroupBox->Width() -246.0f);
	m_TextBox->ID(2);
	m_TextBox->SetMaxLen(100);
	tmpGroupBox->AddControl(m_TextBox);
	tmpGroupBox = NULL;


	//m_BtnSend = new cButton(m_BFont,100.0f,100.0f,20.0f,50.0f);
	//m_BtnSend->SetText("Send");
	//m_BtnSend->ID(1);
	//m_Panel->AddControl(m_BtnSend,1);
	//m_BtnSend->CenterCaption();
};

void cLobbyWindow_gui::ResizeWindow()
{
//	m_ScrollBox->SetSize(m_Height-60.0f,m_Width-10.0f);
	CenterCaption();
};

void cLobbyWindow_gui::DrawWindow(float dt)
{
	cBaseWindow::DrawWindow(dt);
};

bool cLobbyWindow_gui::KeyPress(int key)
{
	bool rtn = false;

//	if (m_Focus && m_TextBox->GetFocus())
//	{
//	   if (key==HGEK_ENTER)
//	   {
////		 SendText();
//		 m_TextBox->LostFocus();
//	   }
//	   else
//		rtn = m_TextBox->KeyPress(key);
//	}
//	else
//	   if (key==HGEK_ENTER)
//		m_TextBox->SetFocus();

	return rtn;
};

void cLobbyWindow_gui::ControlClick(int id)
{
//	switch(id)
//	{
//	case 1:SendText();//Button click
//		break;
//	case 2:SendText();//Text box keypress(enter)
//		break;
//	}

};




