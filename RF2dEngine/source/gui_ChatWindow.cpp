/*
This is the code that defines the look and functionality of the chat window that the
user see floating inside the main the game window. It is used for entering and
displaying chat messages between the user and other clients

This GUI Window Object inherits from cBaseWindow in RF2d_GUIWindow.h/.cpp which handles
the basic functionality and drawing of the window.

cChatWindow_gui's main job is to create and manage the user controls it contains
much like cGUIManager creates and manages cChatWindow_gui. The diagram below shows
the parent/child relationship for this object.

			   +-------------+
			   | Game Engine |
			   +-------------+
					  |
			   +-------------+
			   | cGUIManager |
			   +-------------+
					  |
			   +-------------+
			   | cChatWindow_gui |
			   +-------------+
			   /      |      \
	          /       |       \
             /        |        \
  +---------+    +----------+   +----------------+
  | cButton | -  | cTextBox | - | cScrollTextBox |
  +---------+    +----------+   +----------------+


*/

#include "RF3d_Global.h"
#include "gui_ChatWindow.h"
#include "RF2d_Application.h"

cChatWindow_gui::cChatWindow_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd)
{
	m_GameParent = GameParent;
	SetBaseFont(Font);
	ControlBox.Image = CtrlBtn;
	CloseButton.Image = CloseBtn;
	BackGround.Image = bckgrnd;
	BackGround.Color = ARGB(100,0,0,0);
	SetSize(120,400);
	AlignBottom();
	AlignLeft();
	SetMaxSize(250,500);
	SetMinSize(100,150);
	SetBorderColor(150,200,100);
	Caption.Text = "Main Chat";
	CenterCaption();
	Grip.Visible = false;
	CloseButton.Enabled = false;
	CloseButton.Visible = false;
	ControlBox.Visible = true;
	ControlBox.Enabled = true;
	ControlTemp.BorderDColor = ARGB(255,150,150,150);
	ControlTemp.BorderLColor = ARGB(255,250,250,250);
	ControlTemp.Font = Font;
	ControlTemp.FontColor = ARGB(255,255,255,255);
	CreateControls();
	SetFocus();
};

cChatWindow_gui::~cChatWindow_gui()
{
};

void cChatWindow_gui::CreateControls()
{
	m_ScrollBox = new cScrollListBox(m_BFont,5,22,m_Height-50,m_Width-10);
/*
	//For Testing
	m_ScrollBox->AddItem("Manual Goldstien is the evil anti-Christ.");
	m_ScrollBox->AddItem("Oceania is at war with East Asia.");
	m_ScrollBox->AddItem("Manual Goldstien is a traitor to Oceania.");
	m_ScrollBox->AddItem("Oceania has always been at war with East Asia.");
	m_ScrollBox->AddItem("Eur-Asia is our ally. Love Big Brother.");
	m_ScrollBox->AddItem("Eur-Asia has always been our ally.");
	m_ScrollBox->AddItem("Love Big Brother.");
	m_ScrollBox->AddItem("two plus two equals five.");
	m_ScrollBox->AddItem("War is Peace   Freedom is Slavery   Ignorance is Strength");
	m_ScrollBox->AddItem("If there's anything more important than my ego around,");
	m_ScrollBox->AddItem("I want it hunted and killed now!");
    m_ScrollBox->AddItem("Only the good die young. That is why immortality is");
	m_ScrollBox->AddItem("reserved for the greatest of evils.");
	m_ScrollBox->AddItem("The two rules for success in life are:");
	m_ScrollBox->AddItem("1) Never tell them everything you know");
	m_ScrollBox->AddItem("What if the Hokey Pokey is really what it's all about?");
*/
	AddControl(m_ScrollBox);

	m_BtnSend = new cButton(m_BFont,345.0f,97.0f,20.0f,50.0f);
	m_BtnSend->SetText("Send");
	m_BtnSend->ID(1);
	m_BtnSend->CenterCaption();
	AddControl(m_BtnSend);

	m_TextBox = new cTextBox(m_BFont,5.0f,97.0f,20.0f,335.0f);
	m_TextBox->ID(2);
	m_TextBox->SetMaxLen(100);
	AddControl(m_TextBox);

};

void cChatWindow_gui::ResizeWindow()
{
	//m_ScrollBox->SetSize(m_Height-60.0f,m_Width-10.0f);
	CenterCaption();
};

void cChatWindow_gui::DrawWindow(float dt)
{
	//BackGround.Image->SetColor(ARGB(255,0,0,0));
	cBaseWindow::DrawWindow(dt);

};

bool cChatWindow_gui::KeyPress(int key)
{
	bool rtn = false;

	if (m_Focus && m_TextBox->GetFocus())
	{
	   if (key==HGEK_ENTER)
	   {
		 SendText();
		 //m_TextBox->LostFocus();
		 rtn = true;
	   }
	   else
	   if (key==HGEK_UP || key==HGEK_DOWN || key==HGEK_PGUP || key==HGEK_PGDN)
	   {
		rtn = m_ScrollBox->KeyPress(key);
	   }
	   else
		rtn = m_TextBox->KeyPress(key);
	}
	else
	   if (key==HGEK_ENTER)
		m_TextBox->SetFocus();

	return rtn;
};

void cChatWindow_gui::ControlClick(int id)
{
	switch(id)
	{
	case 1:SendText();//Button click
		break;
	case 2:SendText();//Text box keypress(enter)
		break;
	}

};

void cChatWindow_gui::SendText()
{
	std::string tmpStr;
	char msg[2048];
	tmpStr = m_GameParent->GetMPlayerControl()->GetPlayerName();
	tmpStr += ": ";
	tmpStr += m_TextBox->GetText();
	strcpy(msg,tmpStr.c_str());
	//hge->System_Log(msg);
	if (!tmpStr.empty())
	{
		m_GameParent->GetMPlayerControl()->TransmitChatMessage(msg);
		m_ScrollBox->AddItem(tmpStr.c_str());
	}
	m_TextBox->Clear();

};
void cChatWindow_gui::AddText(unsigned char *msg)
{
	if (msg==0)
		return;
	std::string tmpStr;
	tmpStr = (char*)msg;
	if (!tmpStr.empty())
	m_ScrollBox->AddItem(tmpStr.c_str());
};




