#include "RF2d_Application.h"
#include "RF3d_Global.h"
#include "RF2d_StaticTextures.h"
#include "Sorts.h"
//My Files
#include "MPlayerStructures.h"

//use quick sort to keep windows drawn in the proper order
// compare the y values only.
int CompareZ( cBaseWindow* l, cBaseWindow* r )
{
	if (l->GetZPos() == r->GetZPos() )
	{
		if( l->GetZPos() < r->GetZPos() )
			return 1;
		if( l->GetZPos() > r->GetZPos() )
			return -1;
	}
	if( l->GetZPos() < r->GetZPos() )
        return -1;
    if( l->GetZPos() > r->GetZPos() )
        return 1;
    return 0;
}

cGUIManager::cGUIManager(cApp* pParent)
{
	m_pParent = pParent;
	ChatWindow = NULL;
//	StatWindow = NULL;
//	LobbyWindow = NULL;
	ConnectWindow = NULL;
	HostWindow = NULL;
	MapSelectWindow = NULL;
	m_CurrentWindow = NULL;
	CharSelectWindow = NULL;
	m_Clicked = false;
}

cGUIManager::~cGUIManager()
{
	//Free all the GUIWindows that were used
	m_WindowList.clear();
	delete ChatWindow;
	delete CharSelectWindow;
	delete HostWindow;
	delete MapSelectWindow;
	delete ConnectWindow;
	//delete StatWindow;
	//delete LobbyWindow;

	ChatWindow = NULL;
//	StatWindow = NULL;
//	LobbyWindow = NULL;
	ConnectWindow = NULL;
	HostWindow = NULL;
	MapSelectWindow = NULL;
	CharSelectWindow = NULL;

	m_CurrentWindow = NULL;
	//delete hge sprites and fonts
	if (m_BaseFont) delete m_BaseFont;
	if (m_CloseBtn) delete m_CloseBtn;
	if (m_CtrlBtn) delete m_CtrlBtn;
	if (m_BckGrnd) delete m_BckGrnd;
};

void cGUIManager::Setup()
{
	char tmpFilePath[64];

	//Create sprites
	m_CloseBtn=new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 8, 32, 8, 8);
	m_CtrlBtn=new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 0, 32, 8, 8);
	m_BckGrnd=new hgeSprite(m_pParent->GetStaticTexture()->GetGUITexture(), 33, 1, 16, 16);
	//setup fonts
	strcpy(tmpFilePath, m_pParent->GetGamePath(FONT));
	strcat(tmpFilePath, "font2.fnt");
	//hge->System_Log(tmpFilePath);
	m_BaseFont=new hgeFont(tmpFilePath);
	m_ClickSFX = m_pParent->GetSoundManager()->LoadSoundFX("click3.wav",true);
};

void cGUIManager::ClearFocus()
{
	//Take focus away from everyone just to be sure
	for (unsigned int iloop=0;iloop<m_WindowList.size();iloop++)
	{
		if (m_WindowList[iloop]->Visible())
			m_WindowList[iloop]->LostFocus();
	}
	m_CurrentWindow = NULL;
};

bool cGUIManager::GUIMouseDown(float x, float y)
{
	int tmpZ = 0;
	bool rtn = false;
	if (m_Clicked) return  rtn;
	m_CurrentWindow = NULL;
	for (unsigned int iloop=0;iloop<m_WindowList.size();iloop++)
	{
		if (m_WindowList[iloop]->Visible() && m_WindowList[iloop]->MouseTest(x,y))
		{//Find the top most window under the mouse click
			m_Clicked = true;
			m_CurrentWindow = m_WindowList[iloop];
			tmpZ =  m_CurrentWindow->GetZPos();
			if (tmpZ > 0)
			{
				ClearFocus();//take focus away from all other windows just to be sure
				//m_WindowList[iloop]->SetFocus();//set focus to the one clicked
				m_CurrentWindow = m_WindowList[iloop];
				m_CurrentWindow->SetFocus();
			}
			m_CurrentWindow->MouseDown(x,y);
			m_pParent->GetSoundManager()->PlaySoundFX(m_ClickSFX);
			rtn = true;//Clicked in the window
			break; //Found the window so get out of the loop
		}
	}

	if (tmpZ > 0)
	{ //Brought a window to the front so reorder the others down one
		for (unsigned int iloop=0;iloop<m_WindowList.size();iloop++)
		{
			m_WindowList[iloop]->Reorder(tmpZ);
		}
		//Resort the List for the New Order
		QuickSort(m_WindowList,0,m_WindowList.size(),CompareZ);
	}

	return rtn;

};

void cGUIManager::Render(float dt)
{
	//Render all of the windows
	for (int iloop=m_WindowList.size()-1;iloop>-1;iloop--)
	{
		m_WindowList[iloop]->Render(dt);
	}
};

void cGUIManager::GUIMouseMove(float x, float y)
{ //Only the top window needs these events
	if (m_CurrentWindow)
		m_CurrentWindow->MouseMove(x,y);
};
void cGUIManager::GUIMouseUp(float x, float y)
{ //Only the top window needs these events
	m_Clicked = false;
	if (m_CurrentWindow)
		m_CurrentWindow->MouseUp(x,y);
};
bool cGUIManager::GUIKeyPress(int key)
{
	bool rtn = false;
	if (m_CurrentWindow)
		rtn = m_CurrentWindow->KeyPress(key);

	return rtn;
};

bool cGUIManager::GUICharPress(int key)
{
	bool rtn = false;
	if (m_CurrentWindow)
		rtn =  m_CurrentWindow->CharPress(key);
	return rtn;
};


void cGUIManager::CreateWindows()
{
	//hge->System_Log("CreateWindows");
	ChatWindow=new cChatWindow_gui(m_pParent,m_BaseFont,m_CtrlBtn,m_CloseBtn,m_BckGrnd);
	ChatWindow->SetZPos(m_WindowList.size());
	ChatWindow->Init();
	ChatWindow->HideWindow();
	m_WindowList.push_back(ChatWindow);

	//LobbyWindow=new cLobbyWindow_gui((m_pParent,m_BaseFont,m_CtrlBtn,m_CloseBtn,m_BckGrnd);
	//LobbyWindow->SetZPos(m_WindowList.size());
	//LobbyWindow->Init();
	//LobbyWindow->HideWindow();
	//m_WindowList.push_back(LobbyWindow);

	ConnectWindow=new cConnectWindow_gui(m_pParent,m_BaseFont,m_CtrlBtn,m_CloseBtn,m_BckGrnd);
	ConnectWindow->SetZPos(m_WindowList.size());
	ConnectWindow->Init();
	ConnectWindow->HideWindow();
	m_WindowList.push_back(ConnectWindow);

	HostWindow=new cHostWindow_gui(m_pParent,m_BaseFont,m_CtrlBtn,m_CloseBtn,m_BckGrnd);
	HostWindow->SetZPos(m_WindowList.size());
	HostWindow->Init();
	HostWindow->HideWindow();
	m_WindowList.push_back(HostWindow);

	MapSelectWindow=new cMapSelect_gui(m_pParent,m_BaseFont,m_CtrlBtn,m_CloseBtn,m_BckGrnd);
	MapSelectWindow->SetZPos(m_WindowList.size());
	MapSelectWindow->UseGraphicGUI();
	MapSelectWindow->Init();
	MapSelectWindow->HideWindow();
	m_WindowList.push_back(MapSelectWindow);


	CharSelectWindow=new cCharSelect_gui(m_pParent,m_BaseFont,m_CtrlBtn,m_CloseBtn,m_BckGrnd);
	CharSelectWindow->SetZPos(m_WindowList.size());
	CharSelectWindow->UseGraphicGUI();
	CharSelectWindow->Init();
	CharSelectWindow->HideWindow();
	m_WindowList.push_back(CharSelectWindow);

	/*
	m_StatWindow=new cBaseWindow(m_pParent,100,300,400,128,255);
	m_StatWindow->SetMaxSize(200,500);
	m_StatWindow->SetMinSize(50,200);
	m_StatWindow->SetBorderColor(200,100,100);
	m_StatWindow->SetBaseFont(m_BaseFont);
	m_StatWindow->SetCtrlBtn(m_CtrlBtn);
	m_StatWindow->SetCloseBtn(m_CloseBtn);
	m_StatWindow->SetCaption("Stats");
	m_StatWindow->SetBackGround(m_BckGrndTex);
	m_StatWindow->SetZPos(m_WindowList.size());
	m_WindowList.push_back(m_StatWindow);
//	m_ChatWindow->SetColor(100,100,100,100);
*/
};

void cGUIManager::ShowChatWindow()
{
	if (ChatWindow)
		ChatWindow->ShowWindow();
};

void cGUIManager::HideChatWindow()
{
	if (ChatWindow)
		ChatWindow->HideWindow();
};

void cGUIManager::ShowMPlayGameWindows()
{
	ShowGameWindows();
	if (ChatWindow)
	{
		ChatWindow->AlignBottom();
		ChatWindow->AlignLeft();
		ChatWindow->ShowWindow();
	}
};

void cGUIManager::HideMPlayGameWindows()
{
	HideGameWindows();
	if (ChatWindow)
		ChatWindow->HideWindow();
};

void cGUIManager::ShowGameWindows()
{

};
void cGUIManager::HideGameWindows()
{
	m_CurrentWindow = NULL;

};
void cGUIManager::ShowLobbyWindow()
{
//	if (LobbyWindow)
//		LobbyWindow->ShowWindow();
};
void cGUIManager::HideLobbyWindow()
{
//	if (LobbyWindow)
//		LobbyWindow->HideWindow();
//	m_CurrentWindow = NULL;
};

void  cGUIManager::ShowConnectWindow()
{
	if (ConnectWindow)
	{
		ConnectWindow->ShowWindow();
		m_CurrentWindow = ConnectWindow;
		//m_CurrentWindow->SetFocus();
	}
};

void  cGUIManager::HideConnectWindow()
{
	if (ConnectWindow)
		ConnectWindow->HideWindow();
	m_CurrentWindow = NULL;
};


void  cGUIManager::ShowHostWindow()
{
	if (HostWindow)
	{
		HostWindow->ShowWindow();
		m_CurrentWindow = HostWindow;
		//m_CurrentWindow->SetFocus();
	}
};

void  cGUIManager::HideHostWindow()
{
	if (HostWindow)
		HostWindow->HideWindow();
	m_CurrentWindow = NULL;
};


void  cGUIManager::ShowMapSelectWindow()
{
	if (MapSelectWindow)
	{
		 hge->System_Log("MapSelectWindow");
		switch(m_pParent->GetGameType())
		{
		case SINGMAP:
			MapSelectWindow->GetFileList(m_pParent->GetGamePath(SINGLEMAP));
			 hge->System_Log("SINGLEMAP");
			break;
		case MPLAY:
			MapSelectWindow->GetFileList(m_pParent->GetGamePath(MULTIMAP));
			break;
		default:
			MapSelectWindow->GetFileList(m_pParent->GetGamePath(SINGLEMAP));
			break;
		}
        hge->System_Log("switch");
		MapSelectWindow->ShowWindow();
		hge->System_Log("ShowWindow");
		m_CurrentWindow = MapSelectWindow;
		//m_CurrentWindow->SetFocus();
		hge->System_Log("SetFocus");
	}
};

void  cGUIManager::HideMapSelectWindow()
{
	if (MapSelectWindow)
		MapSelectWindow->HideWindow();
	m_CurrentWindow = NULL;
};

void  cGUIManager::ShowCharSelectWindow()
{
	if (CharSelectWindow)
	{
		if (m_pParent->GetGameType() == MPLAY)
		{
			if (m_pParent->GetMPlayerControl()->GetSide() == HOMETEAM)
			{
				if (ChatWindow)
				{
					ChatWindow->AlignTop();
					ChatWindow->AlignRight();
				}
				CharSelectWindow->AlignBottom();
				CharSelectWindow->AlignRight();
			}else
			{
				if (ChatWindow)
				{
					ChatWindow->AlignBottom();
					ChatWindow->AlignLeft();
				}
				CharSelectWindow->AlignTop();
				CharSelectWindow->AlignLeft();
			}
		}else
		{
			if (m_pParent->GetGameObjMangr()->GetFacing()==2)
			{
				CharSelectWindow->AlignBottom();
				CharSelectWindow->AlignRight();
			}else
			{
				CharSelectWindow->AlignTop();
				CharSelectWindow->AlignLeft();
			}
		}
		CharSelectWindow->LoadCharData();
		CharSelectWindow->ShowWindow();
		m_CurrentWindow = CharSelectWindow;
		//m_CurrentWindow->SetFocus();
	}
};

void  cGUIManager::HideCharSelectWindow()
{
	if (CharSelectWindow)
	{
		CharSelectWindow->HideWindow();
		CharSelectWindow->Clear();
	}
	m_CurrentWindow = NULL;
};

void cGUIManager::HideAllWindows()
{
	HideCharSelectWindow();
	HideMapSelectWindow();
	HideHostWindow();
	HideMPlayGameWindows();
};

//Process Packets that have been identified for the GUI Manager
void cGUIManager::ProcessPacket(Packet *p, unsigned char packetIdentifier)
{
    switch (packetIdentifier)
	{
		//The first 4 should mainly be used to notify the user
		//RakNet specific
		case ID_DISCONNECTION_NOTIFICATION:
			break;
		case ID_MODIFIED_PACKET:
			break;
		case ID_CONNECTION_LOST:
			break;
		//The rest pertain to this demo
		case ID_RECEIVED_STATIC_DATA:
			//Very like will be the MOTD from the server
			//sprintf(message,"System: %s", ((EnumerationDataStruct*)(m_client->GetStaticServerData())->GetData())->MOTD);
			ReceiveChatMessage(p);
			break;
		case ID_CHAT_MESSAGE:
			//Standard chat message
			ReceiveChatMessage(p);
			break;
		default:
			hge->System_Log("Unknown packet type in GUIManager::ProcessPacket.  Type %i with length %i", packetIdentifier, p->length);
			//assert(1);
			break;
	}

};

void cGUIManager::ReceiveChatMessage(Packet *p)
{
	ChatMessageStruct s = *((ChatMessageStruct *) p->data);
	//Notify the chat window about the incoming message
	if (ChatWindow)
		ChatWindow->AddText(p->data+sizeof(ChatMessageStruct));
};









