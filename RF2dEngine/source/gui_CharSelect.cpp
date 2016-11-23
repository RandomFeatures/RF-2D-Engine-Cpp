#include "RF3d_Global.h"
#include "gui_CharSelect.h"
#include "RF2d_Application.h"
#include "RF2d_StaticTextures.h"

cCharSelect_gui::cCharSelect_gui(cApp* GameParent,hgeFont* Font,hgeSprite* CtrlBtn, hgeSprite* CloseBtn, hgeSprite* bckgrnd)
{
	m_GameParent = GameParent;
	SetBaseFont(Font);
	ControlBox.Image = CtrlBtn;
	CloseButton.Image = CloseBtn;
	BackGround.Image = bckgrnd;
	BackGround.Color = ARGB(255,0,0,0);
	SetSize(190,525);
	AlignCenter();
	//SetMaxSize(200,320);
	//SetMinSize(200,320);
	SetBorderColor(150,200,100);
	Caption.Text = "Character Selection";
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
	m_SelectedChar = NULL;
	m_CurrIndx = -1;
	m_ArmySize = "5";
};


cCharSelect_gui::~cCharSelect_gui()
{
	m_ScrollChar = NULL;
	Clear();
};

void cCharSelect_gui::CreateControls()
{
	m_Group = new cGroupBox(m_BFont,183.0f,40.0f,115.0f,327.0f);
	m_Group->UseGraphicBack(m_GameParent->GetStaticTexture()->GetWindowTexture());
	AddControl(m_Group);

	//m_ScrollLabel = new cLabel(m_BFont,183.0f,24.0f,20.0f,60.0f);
	//m_ScrollLabel->SetText("Click below to place the character");
	//AddControl(m_ScrollLabel);

	//m_ScrollLabel = new cLabel(m_BFont,16.0f,24.0f,20.0f,60.0f);
	//m_ScrollLabel->SetText("Available Characters");
	//AddControl(m_ScrollLabel);

	m_ScrollLabel = new cLabel(m_BFont,285.0f,50.0f,20.0f,60.0f);
	m_ScrollLabel->SetText("Class:");
	AddControl(m_ScrollLabel);

	m_ClassLabel = new cLabel(m_BFont,400.0f,50.0f,20.0f,60.0f);
	m_ClassLabel->SetText("");
	AddControl(m_ClassLabel);

	m_ScrollLabel = new cLabel(m_BFont,285.0f,70.0f,20.0f,60.0f);
	m_ScrollLabel->SetText("Hit Rating:");
	AddControl(m_ScrollLabel);

	m_HitLabel = new cLabel(m_BFont,400.0f,70.0f,20.0f,60.0f);
	m_HitLabel->SetText("");
	AddControl(m_HitLabel);

	m_ScrollLabel = new cLabel(m_BFont,284.0f,90.0f,20.0f,60.0f);
	m_ScrollLabel->SetText("Attack Rating:");
	AddControl(m_ScrollLabel);

	m_AttackLabel = new cLabel(m_BFont,400.0f,90.0f,20.0f,60.0f);
	m_AttackLabel->SetText("");
	AddControl(m_AttackLabel);

	m_ScrollLabel = new cLabel(m_BFont,285.0f,110.0f,20.0f,60.0f);
	m_ScrollLabel->SetText("Defense Rating:");
	AddControl(m_ScrollLabel);

	m_DefenseLabel = new cLabel(m_BFont,400.0f,110.0f,20.0f,60.0f);
	m_DefenseLabel->SetText("");
	AddControl(m_DefenseLabel);

	m_ScrollLabel = new cLabel(m_BFont,285.0f,130.0f,20.0f,60.0f);
	m_ScrollLabel->SetText("Movement Rating:");
	AddControl(m_ScrollLabel);

	m_MoveLabel = new cLabel(m_BFont,400.0f,130.0f,20.0f,60.0f);
	m_MoveLabel->SetText("");
	AddControl(m_MoveLabel);


	m_ScrollLabel = new cLabel(m_BFont,16.0f,165.0f,20.0f,60.0f);
	m_ScrollLabel->SetText("Group Size: ");
	AddControl(m_ScrollLabel);

	m_ArmySizeLabel = new cLabel(m_BFont,80.0f,165.0f,20.0f,60.0f);
	m_ArmySizeLabel->SetText("0/5");
	AddControl(m_ArmySizeLabel);


	m_ScrollChar = new cScrollListBox(m_BFont,16.0f,40.0f,115.0f,160.0f);
	m_ScrollChar->ID(3);
	m_ScrollChar->UseGraphicGUI(m_GameParent->GetStaticTexture()->GetWindowTexture());
	m_ScrollChar->SelectedIndx(0);
	AddControl(m_ScrollChar);


	m_BtnBack = new cButton(m_BFont,135.0f,165.0f,20.0f,90.0f);
	m_BtnBack->SetText("Cancel");
	m_BtnBack->UseGraphicGUI(m_GameParent->GetStaticTexture()->GetWindowTexture());
	m_BtnBack->ID(5);
	m_BtnBack->CenterCaption();
	AddControl(m_BtnBack);

	m_BtnStart = new cButton(m_BFont,230.0f,165.0f,20.0f,90.0f);
	m_BtnStart->SetText("Start Game");
	m_BtnStart->UseGraphicGUI(m_GameParent->GetStaticTexture()->GetWindowTexture());
	m_BtnStart->SetEnabled(false);
	m_BtnStart->ID(4);
	m_BtnStart->CenterCaption();
	AddControl(m_BtnStart);

	m_BtnRes = new cButton(m_BFont,325.0f,165.0f,20.0f,90.0f);
	m_BtnRes->SetText("Reset All");
	m_BtnRes->UseGraphicGUI(m_GameParent->GetStaticTexture()->GetWindowTexture());
	m_BtnRes->ID(2);
	m_BtnRes->CenterCaption();
	AddControl(m_BtnRes);

	m_BtnChar = new cButton(m_BFont,420.0f,165.0f,20.0f,90.0f);
	m_BtnChar->SetText("Add Character");
	m_BtnChar->UseGraphicGUI(m_GameParent->GetStaticTexture()->GetWindowTexture());
	m_BtnChar->ID(6);
	m_BtnChar->CenterCaption();
	AddControl(m_BtnChar);


};

void cCharSelect_gui::ResizeWindow()
{
};

void cCharSelect_gui::DrawWindow(float dt)
{
	cBaseWindow::DrawWindow(dt);
	float x = m_XPos + m_Group->X() + 50;
	float y = m_YPos + m_Group->Y() + m_Group->Height() - 5;

	if (m_CurrIndx != m_ScrollChar->SelectedIndx())
	{//index has changed so update
		m_CurrIndx = m_ScrollChar->SelectedIndx();
		m_SelectedChar = NULL;
		m_ClassLabel->SetText(LookupClass(MasterCharList[m_ScrollChar->SelectedIndx()]->Type()));
		m_HitLabel->SetText(LookupStat(MasterCharList[m_ScrollChar->SelectedIndx()]->HitRating()));
		m_AttackLabel->SetText(LookupStat(MasterCharList[m_ScrollChar->SelectedIndx()]->AttackRating()));
		m_DefenseLabel->SetText(LookupStat(MasterCharList[m_ScrollChar->SelectedIndx()]->DefRating()));
		m_MoveLabel->SetText(LookupStat(MasterCharList[m_ScrollChar->SelectedIndx()]->MoveRating()));
	}

	if ((m_ScrollChar->SelectedIndx() > -1) && (MasterCharList.size() > 0))
	{//draw character preview
		MasterCharList[m_ScrollChar->SelectedIndx()]->RenderInWindow(dt,x,y);
	}

	for (int iLoop=0;iLoop<SelectedCharList.size();iLoop++)
	{//draw selected characters on map
		SelectedCharList[iLoop]->RenderInGame(dt,m_GameParent->GetMainMap()->ScreenX(),m_GameParent->GetMainMap()->ScreenY());
	}

	if (m_SelectedChar)
	{
		//attached character to mouse cursor
		if (m_GameParent->GetMainMap()->CheckTeamSide(m_GameParent->GetCursor()->WorldX(),m_GameParent->GetCursor()->WorldY()))
			m_SelectedChar->SetColor(ARGB(255,255,255,255));
		else
			m_SelectedChar->SetColor(ARGB(100,200,200,200));

		m_SelectedChar->X(m_GameParent->GetCursor()->WorldX());
		m_SelectedChar->Y(m_GameParent->GetCursor()->WorldY());
		m_SelectedChar->RenderInGame(dt,m_GameParent->GetMainMap()->ScreenX(),m_GameParent->GetMainMap()->ScreenY());
		m_SelectedChar->SetColor(ARGB(255,255,255,255));
	}

};

void cCharSelect_gui::PlaceCharacter(int key)
{
	if (m_Visible)
	{
		if (key==HGEK_LBUTTON)
		{
			if (m_SelectedChar && m_GameParent->GetMainMap()->CheckTeamSide(m_GameParent->GetCursor()->WorldX(),m_GameParent->GetCursor()->WorldY()))
			{
				m_SelectedChar->Select(true);
				SelectedCharList.push_back(m_SelectedChar);
				m_ArmySizeLabel->SetText(IntToStr(SelectedCharList.size()) + "/" + m_ArmySize);
				m_SelectedChar = NULL;
				m_BtnStart->SetEnabled(true);
			}
		}
		if (key==HGEK_RBUTTON)
			m_SelectedChar = NULL;
	}
};

bool cCharSelect_gui::KeyPress(int key)
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
	//}
	//else
	//   if (key==HGEK_ENTER)
	//	m_TextBox->SetFocus();
	}
	return rtn;
};

void cCharSelect_gui::ControlClick(int id)
{
	if (m_Visible)
	{
		switch(id)
		{
		case 2://reset
			{
				for (unsigned int iloop=0;iloop<SelectedCharList.size();iloop++)
					SelectedCharList[iloop]->Select(false);
				SelectedCharList.clear();
				m_ArmySizeLabel->SetText("0/" + m_ArmySize);
				break;
			}
		case 4://start
			{
				HideWindow();
				AddCharsToGame();
				Clear();
				if (m_GameParent->GetGameType() == MPLAY)
				{
					m_GameParent->GetGameObjMangr()->xmit_AllNPCs();
					m_GameParent->GetGUIManager()->ShowMPlayGameWindows();
					m_GameParent->ChangeGameState(MPGAME, CURRENT);
				}else
				{
					m_GameParent->GetGUIManager()->ShowGameWindows();
					m_GameParent->ChangeGameState(GAME,CURRENT);
					m_GameParent->GetBattleControl()->StartRound();
				}
				break;
			}
		case 5://<< Back
			{
				m_GameParent->GetGUIManager()->HideAllWindows();
				//Clear();
				m_GameParent->ChangeGameState(MPMENU, CURRENT);//Back Button
				break;
			}
		case 6: //Select
			{
				if ((m_ScrollChar->SelectedIndx() > -1) && (MasterCharList.size() > 0))
				if (!MasterCharList[m_ScrollChar->SelectedIndx()]->IsSelected() && (SelectedCharList.size() < m_GameParent->GetGameObjMangr()->GetArmySize()))
				{
					m_SelectedChar = MasterCharList[m_ScrollChar->SelectedIndx()];

					if (m_GameParent->GetGameType() == MPLAY)
					{
						switch (m_GameParent->GetMPlayerControl()->GetSide())
						{
							case HOMETEAM:
								m_SelectedChar->SetDirection(iSE);
								break;
							case CHALLENGER:
								m_SelectedChar->SetDirection(iNW);
							break;
						}
					}else
					{
						if (m_GameParent->GetGameObjMangr()->GetFacing()==2)
						{
							m_SelectedChar->SetDirection(iSE);
						}else
						{
							m_SelectedChar->SetDirection(iNW);
						}
					}
				}
				break;
			}
		}
	}
};

void cCharSelect_gui::Clear()
{
	cCharSelectItem_gui_support *CharItem;
	//Free all the CharSelectItems that were used
	for (unsigned int iloop=0;iloop<MasterCharList.size();iloop++)
	{
		CharItem = MasterCharList[iloop];
 		delete CharItem;
	}

	MasterCharList.clear();
	SelectedCharList.clear();

	//if(m_ScrollChar) m_ScrollChar->Clear();

};

void cCharSelect_gui::LoadCharData()
{
	std::string CharList;
	char tmpGUID[20];
	char tmpFileName[100];
	char tmpName[20];
	ifstream inFile;
	int iClass;
	int iDefRate;
	int iHitRate;
	int iMoveRate;
	int iAttRate;
	int iCount = 0;
	int iSpellID;
	float fPhy;
	float fMag;
	int iHps;
	int iMin;
	int iMax;
	int iMove;

	CharList = m_GameParent->GetGamePath(BASE);
	CharList += "data\\characters\\masterlist.lst";

	inFile.open(CharList.c_str(), ::ios::in);
	if(inFile.bad()) exit(-1);

	inFile >> iCount;

	for (int iLoop=1;iLoop<iCount;iLoop++)
	{
		//guid name class defense hit points attack movement file
		inFile >> tmpGUID >> tmpName >> iClass >> iDefRate >> iHitRate >> iAttRate >> iMoveRate >> tmpFileName >> iSpellID >> fPhy >> fMag >> iHps >> iMin >> iMax >> iMove;
		AddCharItem(tmpGUID, tmpName, iClass, iDefRate, iHitRate, iAttRate, iMoveRate, tmpFileName, iSpellID, fPhy, fMag, iHps, iMin, iMax, iMove);
	}

	inFile.close();

	m_ArmySize = IntToStr(m_GameParent->GetGameObjMangr()->GetArmySize());
	m_ArmySizeLabel->SetText("0/" + m_ArmySize);
};

void cCharSelect_gui::AddCharItem(std::string guid, std::string name, int type, int defrate, int hitrate, int attrate, int moverate, std::string file, int spellID, float phy, float mag, int hps, int imin, int imax, int movement)
{
	std::string Name = name;
	std::string tmpstr;
	std::string filename = m_GameParent->GetGamePath(CHARACTER) + file;

	cCharSelectItem_gui_support* chr = new cCharSelectItem_gui_support(phy,mag,defrate,hitrate,hps,attrate,imin,imax,moverate,1,0,0,type,spellID,movement,guid,filename,name,m_GameParent->GetGamePath(TEXTURE));
	MasterCharList.push_back(chr);
	chr->LoadStand();

	do
		Name += " ";
	while (Name.size() < 10);

	tmpstr = Name.substr(0,10);// + "  " + LookupClass(type) + "  " + LookupStat(defrate) + "  " + LookupStat(hitrate) + "  " +  LookupStat(attrate) + "  " + LookupStat(moverate);
	m_ScrollChar->AddItem(tmpstr);

};
void cCharSelect_gui::AddCharsToGame()
{
	for (int iLoop=0;iLoop<SelectedCharList.size();iLoop++)
		m_GameParent->GetGameObjMangr()->AddNpc(SelectedCharList[iLoop]->Dir(),m_GameParent->GetMPlayerControl()->GetSide(),0,SelectedCharList[iLoop]->X(),SelectedCharList[iLoop]->Y(),SelectedCharList[iLoop]->HitPoints(),SelectedCharList[iLoop]->AttackMin(),SelectedCharList[iLoop]->AttackMax(),SelectedCharList[iLoop]->Movement(), SelectedCharList[iLoop]->SpellID(), SelectedCharList[iLoop]->Physical(),SelectedCharList[iLoop]->Magical(), SelectedCharList[iLoop]->GUID().c_str(),SelectedCharList[iLoop]->FileName().c_str(),SelectedCharList[iLoop]->Name().c_str());
};

std::string cCharSelect_gui::LookupClass(int type)
{
	std::string rtn;
	switch (type)
	{
	case 1:rtn = "Warrior";
		break;
	case 2:rtn = "Archer ";
		break;
	case 3:rtn = "Caster ";
		break;
	case 4:rtn = "Thief  ";
		break;
	}
	return rtn;
};

std::string cCharSelect_gui::LookupStat(int stat)
{
	std::string rtn;
	switch (stat)
	{
	case 1:rtn = "Low-  ";
		break;
	case 2:rtn = "Low   ";
		break;
	case 3:rtn = "Medium";
		break;
	case 4:rtn = "High  ";
		break;
	case 5:rtn = "High+ ";
		break;
	}
	return rtn;
};
