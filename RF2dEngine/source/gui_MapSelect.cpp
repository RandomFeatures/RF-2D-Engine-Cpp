#include "RF3d_Global.h"
#include "gui_MapSelect.h"
#include "RF2d_Application.h"
#include "StateManager.h"
#include "RF2d_StaticTextures.h"
#include <string.h>
#include <stdio.h>


cMapSelect_gui::cMapSelect_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd)
{
	m_GameParent = GameParent;
	SetBaseFont(Font);
	ControlBox.Image = CtrlBtn;
	CloseButton.Image = CloseBtn;
	BackGround.Image = bckgrnd;
	BackGround.Color = ARGB(200,0,0,0);
	SetSize(190,320);
	AlignCenter();
	//SetMaxSize(200,320);
	//SetMinSize(200,320);
	SetBorderColor(150,200,100);
	Caption.Text = "Map Selection";
	CenterCaption();
	Grip.Visible = false;
	CloseButton.Enabled = true;
	CloseButton.Visible = true;
	ControlBox.Visible = false;
	ControlBox.Enabled = false;
	ControlTemp.BorderDColor = ARGB(255,150,150,150);
	ControlTemp.BorderLColor = ARGB(255,250,250,250);
	ControlTemp.Font = Font;
	ControlTemp.FontColor = ARGB(255,255,255,255);
	CreateControls();
	SetFocus();
};

cMapSelect_gui::~cMapSelect_gui()
{
};

void cMapSelect_gui::CreateControls()
{
	m_ScrollMaps = new cScrollListBox(m_BFont,16.0f,26.0f,130.0f,285.0f);;
	m_ScrollMaps->ID(3);
	m_ScrollMaps->UseGraphicGUI(m_GameParent->GetStaticTexture()->GetWindowTexture());
	m_ScrollMaps->SelectedIndx(0);
	AddControl(m_ScrollMaps);

	//m_DefaultChk = new cChkBox(m_BFont,16.0f,132.0f, 20.0f,285.0f);
	//m_DefaultChk->SetText("Load Default Army");
	//m_DefaultChk->Checked(false);
	//m_DefaultChk->UseGraphicGUI(m_GameParent->GetWindowTexture());
	//m_DefaultChk->ID(2);
	//AddControl(m_DefaultChk);

	m_BtnNext = new cButton(m_BFont,126.0f,162.0f,20.0f,75.0f);
	m_BtnNext->SetText("Next >>");
	m_BtnNext->UseGraphicGUI(m_GameParent->GetStaticTexture()->GetWindowTexture());
	m_BtnNext->ID(4);
	m_BtnNext->CenterCaption();
	AddControl(m_BtnNext);

	//m_BtnBack = new cButton(m_BFont,55.0f,162.0f,20.0f,75.0f);
	//m_BtnBack->SetText("<< Back");
	//m_BtnBack->UseGraphicGUI(m_GameParent->GetWindowTexture());
	//m_BtnBack->ID(5);
	//m_BtnBack->CenterCaption();
	//AddControl(m_BtnBack);
	m_ScrollMaps->SetFocus();

};

void cMapSelect_gui::Init()
{
	m_ScrollMaps->SelectedIndx(0);
};


void cMapSelect_gui::ResizeWindow()
{
};

void cMapSelect_gui::DrawWindow(float dt)
{
//	BackGround.Image->SetColor(ARGB(255,0,0,0));
	cBaseWindow::DrawWindow(dt);

};

bool cMapSelect_gui::KeyPress(int key)
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
		rtn = m_ScrollMaps->KeyPress(key);

	//}
	//else
	//   if (key==HGEK_ENTER)
	//	m_TextBox->SetFocus();
	}
	return rtn;
};

void cMapSelect_gui::ControlClick(int id)
{
	char map[50];
	if (m_Visible)
	{
		switch(id)
		{
		case 4://Next
			if (m_ScrollMaps->SelectedIndx() > -1)
			{
				HideWindow();
				strcpy(map,m_ScrollMaps->GetSelectedItem().c_str());
				if (m_GameParent->GetGameType() == MPLAY)
				{//multiplayer game
					m_GameParent->GetMPlayerControl()->UseDefaultArmy(false);
					m_GameParent->GetMPlayerControl()->SetMapName(map);
					m_GameParent->GetGUIManager()->ShowHostWindow();
				}else
				{//single player
					//if (m_DefaultChk->Checked())
					//{//user default army
					//	m_GameParent->GetMPlayerControl()->SetSide(HOMETEAM);
					//	m_GameParent->ChangeMap(map,DEFAULTTEAM);
					//	m_GameParent->GetGUIManager()->ShowGameWindows();
					//	m_GameParent->ChangeGameState(GAME, ALL);
					//}else
					//{//Custom Army
						m_GameParent->GetMPlayerControl()->SetSide(HOMETEAM);
						m_GameParent->ChangeMap(map,CUSTTEAM);
						//goto character select
						m_GameParent->GetGUIManager()->ShowCharSelectWindow();
						//Current here will remove the load screeen
						m_GameParent->ChangeGameState(TEAM, CURRENT);
					//}
				}
 				break;
			}
		case 5://Back
			{
				HideWindow();
				StateManager::getInstance()->popState();//Back Button
				break;
			}
		}
	}

};


int cMapSelect_gui::GetFileList(const char *dir)
{
     WIN32_FIND_DATA FindFileData;
     HANDLE hFind = INVALID_HANDLE_VALUE;
     char DirSpec[MAX_PATH];  // directory specification
     DWORD dwError;

	 m_ScrollMaps->Clear();
	 strncpy (DirSpec, dir, strlen(dir)+1);
     strncat (DirSpec, "\\*.map", 7);

     hFind = FindFirstFile(DirSpec, &FindFileData);

     if (hFind == INVALID_HANDLE_VALUE)
     {
          return (-1);
     }
     else
     {
		 //add the first map
		 if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		            m_ScrollMaps->AddItem(FindFileData.cFileName);

         //keep going and get the rest of the maps
		 while (FindNextFile(hFind, &FindFileData) != 0)
          {
               if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		            m_ScrollMaps->AddItem(FindFileData.cFileName);
          }

          dwError = GetLastError();
          FindClose(hFind);
          if (dwError != ERROR_NO_MORE_FILES)
          {
               return (-1);
          }
     }
	 m_ScrollMaps->SelectedIndx(0);
     return 0;
}
