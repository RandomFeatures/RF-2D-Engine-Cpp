#include "RF2d_Anistorage.h"

AniStorage::AniStorage()
{
	NESprite= NULL;
	NWSprite= NULL;
	SESprite= NULL;
	SWSprite= NULL;
	m_Tex = NULL;
	m_Loaded = false;
	m_Debug = false;
	m_Color = ARGB(255,255,255,255);
	m_Blended = false;
	m_Shadows = false;
	m_NewStyle = false;
	m_log = false;
	m_DividePoint=0;
};

AniStorage::~AniStorage()
{
	if (m_Debug) hge->System_Log("~AniStorage");
	if (NESprite) delete NESprite;
	if (NWSprite) delete NWSprite;
	if (SESprite) delete SESprite;
	if (SWSprite) delete SWSprite;
};

AniStorage::AniStorage(const AniStorage& p)
{
	m_OneFacing = p.m_OneFacing;
	m_Dir = p.m_Dir;
	m_HotX = p.m_HotX;
	m_HotY = p.m_HotY;
	m_Mode = p.m_Mode;
	m_BlendMode = p.m_BlendMode;
	m_Loaded = false;
	m_Debug = false;
	m_Blended = p.m_Blended;
	m_Color = ARGB(255,255,255,255);
	m_Shadows = false;
	m_DividePoint = 0;
	if (m_Debug) hge->System_Log("CopyInit m_Text Assigned");
	m_Loaded = true;
	if (m_OneFacing)
	{
		if (m_Debug) hge->System_Log("CopyInit Load One Facing");
		NESprite = NULL;
		NWSprite = NULL;
		SESprite = NULL;
		SWSprite = p.SWSprite;
	}
	else
	{
		if (m_Debug) hge->System_Log("CopyInit Load All Facing");
		NESprite = p.NESprite;
		NWSprite = p.NWSprite;
		SESprite = p.SESprite;
		SWSprite = p.SWSprite;
	}

	SetHotSpot(m_HotX, m_HotY);
	SetMode(m_Mode);
	if (m_Blended)
		SetBlendMode(m_BlendMode);

}


void AniStorage::Init(char* ImageFile, int nFrames, float FPS, float iWidth, float iHeight, bool SingleFacing, bool KeepTexLive)
{
//	m_Tex=hge->Texture_Load(ImageFile);

	LoadTexture(ImageFile, KeepTexLive);
	if (m_Tex)
	{
		m_Loaded = true;
		if (SingleFacing)
		{
			NESprite= NULL;
			NWSprite= NULL;
			SESprite= NULL;
			SWSprite= new hgeAnimation(m_Tex, nFrames, FPS, 0, 0, iWidth, iHeight);
		}
		else
		{
			NESprite= new hgeAnimation(m_Tex,nFrames,FPS, 0, 0, iWidth, iHeight);
			NWSprite= new hgeAnimation(m_Tex,nFrames,FPS, 0, iHeight, iWidth, iHeight);
			SESprite= new hgeAnimation(m_Tex,nFrames,FPS, 0, iHeight*2, iWidth, iHeight);
			SWSprite= new hgeAnimation(m_Tex,nFrames,FPS, 0, iHeight*3, iWidth, iHeight);
		}
	}
	m_OneFacing = SingleFacing;
	m_nFrames = nFrames;
	m_FPS = FPS;
	m_FrameWidth = iWidth;
	m_FrameHeight = iHeight;
	m_Dir = iSW;
	//hge->System_Log("%s  FPS=%f", ImageFile, FPS);
}

void AniStorage::Init()
{
	m_NewStyle = true;
	m_TexSE = NULL;
	m_TexNE = NULL;
	m_TexSW = NULL;
	m_TexNW = NULL;
	m_OneFacing = false;
	m_Dir = iSW;
}

void AniStorage::AddFacing(char* ImageFile, int nFrames, float FPS, float iWidth, float iHeight, DIR dir, bool KeepTexLive)
{

	switch(dir)
	{
	case SE:
		LoadTexture(ImageFile, KeepTexLive, SE);
		if (m_TexSE && !SESprite)
			SESprite = new hgeAnimation(m_TexSE,nFrames,FPS, 0, 0, iWidth, iHeight);
		break;
	case SW:
		LoadTexture(ImageFile, KeepTexLive, SW);
		if (m_TexSW && !SWSprite)
			SWSprite= new hgeAnimation(m_TexSW,nFrames,FPS, 0, 0, iWidth, iHeight);
		break;
	case NE:
		LoadTexture(ImageFile, KeepTexLive, NE);
		if (m_TexNE && !NESprite)
			NESprite = new hgeAnimation(m_TexNE,nFrames,FPS, 0, 0, iWidth, iHeight);
		break;
	case NW:
		LoadTexture(ImageFile, KeepTexLive, NW);
		if (m_TexNW && !NWSprite)
			NWSprite = new hgeAnimation(m_TexNW,nFrames,FPS, 0, 0, iWidth, iHeight);
		break;
	}
}


void AniStorage::SetDividePoint(int divide)
{
	m_DividePoint = divide;
};

void AniStorage::Play()
{
	if (NESprite) NESprite->Play();
	if (NWSprite) NWSprite->Play();
	if (SESprite) SESprite->Play();
	if (SWSprite) SWSprite->Play();

};

void AniStorage::SetMode(int mode)
{
    m_Mode = mode;
	//Stop();
	if (NESprite) NESprite->SetMode(mode);
	if (NWSprite) NWSprite->SetMode(mode);
	if (SESprite) SESprite->SetMode(mode);
	if (SWSprite) SWSprite->SetMode(mode);
};

void AniStorage::SetColor(DWORD col)
{

	if (NESprite) NESprite->SetColor(col);
	if (NWSprite) NWSprite->SetColor(col);
	if (SESprite) SESprite->SetColor(col);
	if (SWSprite) SWSprite->SetColor(col);
	m_Color = col;
}

void AniStorage::Stop()
{
	if (NESprite) NESprite->Stop();
	if (NWSprite) NWSprite->Stop();
	if (SESprite) SESprite->Stop();
	if (SWSprite) SWSprite->Stop();
};
void AniStorage::Update(float fDeltaTime)
{
	if (NESprite) NESprite->Update(fDeltaTime);
	if (NWSprite) NWSprite->Update(fDeltaTime);
	if (SESprite) SESprite->Update(fDeltaTime);
	if (SWSprite) SWSprite->Update(fDeltaTime);
};
bool AniStorage::IsPlaying()
{
	bool rtn = false;
	switch ( m_Dir )
      {
         case iNE:
            if (NESprite) rtn = NESprite->IsPlaying();
            break;
         case iNW:
            if (NWSprite) rtn = NWSprite->IsPlaying();
            break;
         case iSE:
            if (SESprite) rtn = SESprite->IsPlaying();
            break;
         case iSW:
            if (SWSprite) rtn = SWSprite->IsPlaying();
            break;
      }
   return rtn;
};

void AniStorage::SetSpeed(float fps)
{
	if (NESprite) NESprite->SetSpeed(fps);
	if (NWSprite) NWSprite->SetSpeed(fps);
	if (SESprite) SESprite->SetSpeed(fps);
	if (SWSprite) SWSprite->SetSpeed(fps);
};

void AniStorage::SetFrame(int n)
{
	if (NESprite) NESprite->SetFrame(n);
	if (NWSprite) NWSprite->SetFrame(n);
	if (SESprite) SESprite->SetFrame(n);
	if (SWSprite) SWSprite->SetFrame(n);
};

void AniStorage::SetBlendMode(int blend)
{
	m_BlendMode = blend;
	m_Blended = true;
	if (NESprite) NESprite->SetBlendMode(blend);
	if (NWSprite) NWSprite->SetBlendMode(blend);
	if (SESprite) SESprite->SetBlendMode(blend);
	if (SWSprite) SWSprite->SetBlendMode(blend);
};

int AniStorage::GetFrame()
{
	int rtn = 0;
	switch ( m_Dir )
      {
         case iNE:
            if (NESprite) rtn = NESprite->GetFrame();
            break;
         case iNW:
            if (NWSprite) rtn =NWSprite->GetFrame();
            break;
         case iSE:
            if (SESprite) rtn =SESprite->GetFrame();
            break;
         case iSW:
            if (SWSprite) rtn =SWSprite->GetFrame();
            break;
      }
   return rtn;

};
void AniStorage::SetDirection(ISODIR dir)
{
	if (!m_OneFacing)
		m_Dir = dir;
};

void AniStorage::LogMe()
{
	m_log = true;
};
void AniStorage::RenderBottom(hgeAnimation* Ani, float x, float y)
{

   float origX,origY,origW,origH;

   if(Ani->GetFrame() < 4 )
   {
		Ani->GetTextureRect(&origX,&origY,&origW,&origH);
		//Ani->SetTextureRect(origX,origY+(origH-m_DividePoint),origW,m_DividePoint);
		Ani->SetTextureRect(origX,origY+64.0f,origW,64.0f);

		Ani->Render(x,y+64.0f);
		Ani->SetTextureRect(origX,origY,origW,origH);
		if (m_log) m_log = false;
   }
   else
   {
	   	Ani->GetTextureRect(&origX,&origY,&origW,&origH);
		//Ani->SetTextureRect(origX,64.0f+origY+(origH-m_DividePoint),origW,m_DividePoint);
		Ani->SetTextureRect(origX,origY+128.0f,origW,64.0f);

		Ani->Render(x,y+64.0f);
		Ani->SetTextureRect(origX,origY,origW,origH);
   }

}

void AniStorage::RenderTop(hgeAnimation* Ani, float x, float y)
{

   float origX,origY,origW,origH;
   if(Ani->GetFrame() < 4 )
   {
		Ani->GetTextureRect(&origX,&origY,&origW,&origH);
		//Ani->SetTextureRect(origX,origY,origW,origH-m_DividePoint);
		Ani->SetTextureRect(origX,origY,origW,64.0f);

		Ani->Render(x,y);
		Ani->SetTextureRect(origX,origY,origW,origH);
   }
    else
   {
	   	Ani->GetTextureRect(&origX,&origY,&origW,&origH);
		//Ani->SetTextureRect(origX,64.0f+origY2+(origH-m_DividePoint),origW,m_DividePoint);
		Ani->SetTextureRect(origX,64.0f,origW,64.0f);

		Ani->Render(x,y);
		Ani->SetTextureRect(origX,origY,origW,origH);
   }


}

void AniStorage::Render(float fDeltaTime, float x, float y, int mode)
{
    //Update all the Facings keep them in sync
	if (mode!=2)
	{
		if (NESprite) NESprite->Update(fDeltaTime);
		if (NWSprite) NWSprite->Update(fDeltaTime);
		if (SESprite) SESprite->Update(fDeltaTime);
		if (SWSprite) SWSprite->Update(fDeltaTime);
	}
//hge->System_Log("AniStorage::Render");
    //Render the one for the current Direction
	//if (m_OneFacing)  hge->System_Log("Here");

	switch ( m_Dir )
	{
		case iNE:
			if (m_Debug) hge->System_Log("Render NE");
			if (NESprite)
			{
				if (!m_Blended && m_Shadows && (mode!=1))
				{
					NESprite->SetColor(ARGB(150,10,10,10));
					NESprite->Render4V(x+8,y-64,x+72,y-64,x+40,y+12,x-44,y+12);
					NESprite->SetColor(m_Color);
				}

				switch (mode)
				{
				case 0:
					NESprite->Render(x, y);
					break;
				case 1:
					if (m_DividePoint > 0)
						RenderBottom(NESprite,x,y);
					else
						NESprite->Render(x, y);
					break;
				case 2:
					if (m_DividePoint > 0)
						RenderTop(NESprite,x,y);
					break;
				}
			}
			break;
		case iNW:
			if (m_Debug) hge->System_Log("Render NW");
			if (NWSprite)
			{
				if (!m_Blended && m_Shadows && (mode!=1))
				{
					NWSprite->SetColor(ARGB(150,10,10,10));
					NWSprite->Render4V(x+8,y-64,x+72,y-64,x+40,y+12,x-44,y+12);
					NWSprite->SetColor(m_Color);
				}

				switch (mode)
				{
				case 0:
					NWSprite->Render(x, y);
					break;
				case 1:
					if (m_DividePoint > 0)
						RenderBottom(NWSprite,x,y);
					else
						NWSprite->Render(x, y);
					break;
				case 2:
					if (m_DividePoint > 0)
						RenderTop(NWSprite,x,y);
					break;
				}
			}
			break;
		case iSE:
			if (m_Debug) hge->System_Log("Render SE");
			if (SESprite)
			{

				if (!m_Blended && m_Shadows && (mode!=1))
				{
					SESprite->SetColor(ARGB(150,10,10,10));
					SESprite->Render4V(x+8,y-64,x+72,y-64,x+40,y+12,x-44,y+12);
					SESprite->SetColor(m_Color);
				}

				switch (mode)
				{
				case 0:
					SESprite->Render(x, y);
					break;
				case 1:
					if (m_DividePoint > 0)
						RenderBottom(SESprite,x,y);
					else
						SESprite->Render(x, y);
					break;
				case 2:
					if (m_DividePoint > 0)
						RenderTop(SESprite,x,y);
					break;
				}
			}
			break;
		case iSW:

			if (m_Debug) hge->System_Log("Render SW");
			if (SWSprite) {
				if (m_Debug) hge->System_Log("SWSprite Assigned x: %f  y: %f  frame: %i",x, y, SWSprite->GetFrame());
				if (!m_Blended && m_Shadows && (mode!=1))
				{
					SWSprite->SetColor(ARGB(150,10,10,10));
					SWSprite->Render4V(x+8,y-64,x+72,y-64,x+40,y+12,x-44,y+12);
					SWSprite->SetColor(m_Color);
				}

				switch (mode)
				{
				case 0:
					SWSprite->Render(x, y);
					break;
				case 1:
					if (m_DividePoint > 0)
						RenderBottom(SWSprite,x,y);
					else
						SWSprite->Render(x, y);
					break;
				case 2:
					if (m_DividePoint > 0)
						RenderTop(SWSprite,x,y);
					break;
				}
			}
			break;
	}

};
void AniStorage::SetHotSpot(float x, float y)
{
	m_HotX = x;
	m_HotY = y;
	if (NESprite) NESprite->SetHotSpot(x, y);
	if (NWSprite) NWSprite->SetHotSpot(x, y);
	if (SESprite) SESprite->SetHotSpot(x, y);
	if (SWSprite) SWSprite->SetHotSpot(x, y);

};

bool AniStorage::IsLoaded()
{
	return m_Loaded;
};

void AniStorage::LoadTexture(char* FileName, bool keeplive)
{
	resourcemanager->GetTexture(FileName, m_Tex, keeplive);
};

void AniStorage::LoadTexture(char* FileName, bool keeplive, DIR dir)
{

	switch(dir)
	{
	case SE:
		resourcemanager->GetTexture(FileName, m_TexSE, keeplive);
		break;
	case NE:
		resourcemanager->GetTexture(FileName, m_TexNE, keeplive);
		break;
	case SW:
		resourcemanager->GetTexture(FileName, m_TexSW, keeplive);
		break;
	case NW:
		resourcemanager->GetTexture(FileName, m_TexNW, keeplive);
		break;
	}
};

void AniStorage::SetDebug(){m_Debug = true;};
void AniStorage::SetShadows(bool shadow) { m_Shadows=shadow;};
