#include <fstream>
#include <vector>
#include "RF2d_Application.h"
#include "RF3d_Global.h"


cMapObject::cMapObject(cApp* pParent)
{
	char tmpFilePath[256];
	Screen.xOffset = 0;
	Screen.yOffset = 0;
	mapWidth = mapHeight = 0;
	mapXTiles = mapYTiles = 0;
	scrollx = 0;
	scrolly = 0;
	//define a 3d array
	//mapdata = new int**[7];
	TileSet= NULL;
	BaseTileSet = NULL;
	Ground = 0;
	m_pParent = pParent;
	m_Loaded = false;
	m_AmbientColor = ARGB(255,255,255,255);

	//Load Particle Texture
	strcpy(tmpFilePath, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpFilePath, "\\gui\\targettiles.png");
	m_TrgtTex=hge->Texture_Load(tmpFilePath);
	m_MyTiles=new hgeSprite(m_TrgtTex, 64, 0, 64, 32);//yellow
	m_MyTiles->SetHotSpot(0,0);

	m_TheirTiles=new hgeSprite(m_TrgtTex, 192, 0, 64, 32); //grey
	m_TheirTiles->SetHotSpot(0,0);
}

//object destructor
cMapObject::~cMapObject()
{
//	int a,b;
	ClearMap();
	////delete the array itself
	//for (a=0;a<7;a++)
	//	for (b=0;b<mapXTiles;b++)
	//			delete [] mapdata[a][b];
	//
	////delete the 3 layers
	//for (a=0;a<7;a++)
	//	delete[] mapdata[a];
	//
	//delete[] mapdata;

	if (m_MyTiles) delete m_MyTiles;
	if (m_TheirTiles) delete m_TheirTiles;
	if (m_TrgtTex) hge->Texture_Free(m_TrgtTex);
}

void cMapObject::MapName(char *name)
{
	strcpy(m_MapName,name);
};
char* cMapObject::MapName()
{
	return m_MapName;
};

void cMapObject::InitializeMap(int dimensionx, int dimensiony)
{
	mapXTiles = dimensionx;
	mapYTiles = dimensiony;

	mapWidth = mapXTiles * TILEWIDTH;
	mapHeight = mapYTiles * TILEHALFHEIGHT;
}

void cMapObject::ClearMap()
{
	int a,b,c;
	if (m_Loaded == true)
	{
		//delete all of the tiles in each
		//of the 3 layer
		//for (a=0;a<7;a++)
		//	for (b=0;b<mapXTiles;b++)
		//			//delete [] mapdata[a][b];
		//			mapdata[a][b] = 0;
		////delete the 3 layers
		//for (a=0;a<7;a++)
		//	//delete[] mapdata[a];
		//	mapdata[a] = 0;

		for (a=0;a<5;a++)
		{
			for (b=0;b<mapYTiles;b++)
			{
				for (c=0;c<mapXTiles;c++)
				{
					m_MapData[a][b][c] = 0;
				}
			}
		}

		if (m_BaseTex) hge->Texture_Free(m_BaseTex);
		if (BaseTileSet) delete BaseTileSet;
		if (m_Tex) hge->Texture_Free(m_Tex);
		if (TileSet) delete TileSet;
		if (m_target) hge->Target_Free(m_target);
		if (Ground) delete Ground;
		m_Loaded = false;
	}
}
void cMapObject::CreateMap(int dimensionx, int dimensiony)
{
	//based on the dimentions read in from the file
	//Seven Layers
//	int a,b,c;

	//for (a=0;a<7;a++)
	//	mapdata[a] = new int*[dimensiony];
	////Width
	//for (a=0;a<7;a++)
	//	for (b=0;b<dimensiony;b++)
	//		mapdata[a][b] = new int[dimensionx];

	////height
	//for (a=0;a<7;a++)
	//	for (b=0;b<dimensiony;b++)
	//		for (c=0;c<dimensionx;c++)
	//			mapdata[a][b][c] = 0;
}

void cMapObject::MoveCamx(int value)
{
	int oldx;

	oldx = Screen.xOffset;
	Screen.xOffset += value;
	scrollx -= value;


	if (Screen.xOffset < 64)
	{
		Screen.xOffset = 64;
		scrollx = 0;
//		if (oldx != Screen.xOffset)
		 UpdateTarget();
	}

	if((Screen.xOffset + Screen.Width + 128) > mapWidth)
	{
		Screen.xOffset = mapWidth - Screen.Width - 128;
		scrollx = 0;
//		if (oldx != Screen.xOffset)
		 UpdateTarget();
	}

	if (scrollx <= -64)
	{
		scrollx = 0;
		UpdateTarget();
	}
	if (scrollx >= 64)
	{
		scrollx = 0;
		UpdateTarget();
	}

}

void cMapObject::MoveCamy(int value)
{
	int oldy;

	oldy = Screen.yOffset;
	Screen.yOffset += value;
	scrolly -= value;

	if (Screen.yOffset < 32)
	{
		Screen.yOffset = 32;
		scrolly = 0;
	//	if (oldy != Screen.yOffset)
			UpdateTarget();
	}
	if ((Screen.yOffset + Screen.Height + 64) > mapHeight)
	{
		Screen.yOffset = mapHeight - Screen.Height - 64;
		scrolly = 0;
	//	if (oldy != Screen.yOffset)
			UpdateTarget();
	}

	if (scrolly <= -32)
	{
		scrolly = 0;
		UpdateTarget();
	}
	if (scrolly >= 32)
	{
		scrolly = 0;
		UpdateTarget();
	}


}

void cMapObject::GainFocus()
{
	if (m_Loaded)
	{
		Ground->SetTexture(hge->Target_GetTexture(m_target));
		UpdateTarget();
	}

};

void  cMapObject::UpdateTarget()
{
	hge->Gfx_BeginScene(m_target);
	hge->Gfx_Clear(0);
	RenderBaseLayer();
	RenderMap(1);
	hge->Gfx_EndScene();
	scrollx = 0;
	scrolly = 0;
};


void cMapObject::Render()
{
	if (Ground)
	{
        Ground->SetColor(m_AmbientColor);
        Ground->Render(scrollx-64,scrolly-32);
	}
};


void cMapObject::RenderBaseLayer()
{


   int StartTileX = (Screen.xOffset-64)  / TILEWIDTH;
   int StartTileY = (Screen.yOffset-32)  / TILEHEIGHT;
   int StartX=0;
   int StartY=0;
   float TileLeft=0;
   float TileTop=0;
   int a = -1;

//   hge->System_Log("StartTileX: %i\n", StartTileX);
//   hge->System_Log("StartTileY: %i\n", StartTileY);

   for (int y=StartTileY; y < StartTileY + 23; y++)
   {
       for (int x=StartTileX; x < StartTileX + 17; x++)
       {

			TileLeft = float((StartX + (x * TILEWIDTH) - Screen.xOffset) -TILEWIDTH);
			TileTop = float((StartY + (y * TILEHEIGHT) - Screen.yOffset) -TILEHEIGHT);

			if (BaseTileSet->GetFrame() != m_MapData[0][y][x]) //if a tiles is assigend to this layer
				{// dont move the frame till the index chagnes
					BaseTileSet->SetFrame(m_MapData[0][y][x]);
				}
				//this should be the main drawing
				if(BaseTileSet->GetFrame() != TRANSTILE) //draw non transparent tile
				{
					BaseTileSet->Render(TileLeft,TileTop);
				}
       }
   }

};
void cMapObject::RenderMap(int layer)
{
   int StartTileX = (Screen.xOffset-64)  / TILEWIDTH;
   int StartTileY = (Screen.yOffset-32)  / TILEHALFHEIGHT;
   int StartX=0;
   int StartY=0;
   float TileLeft=0;
   float TileTop=0;
   //int a = -1;

   for (int y=StartTileY; y < StartTileY + 42; y++)
   {
       if (y % 2 == 0) StartX = 0; // Even Line //
       else   StartX =  TILEHALFWIDTH; // Odd Line  //

       for (int x=StartTileX; x < StartTileX + 17; x++)
       {

			TileLeft = float((StartX + (x * TILEWIDTH) - Screen.xOffset) -TILEWIDTH);
			TileTop = float((StartY + (y * TILEHALFHEIGHT) - Screen.yOffset) -TILEHALFHEIGHT);

			if (TileSet->GetFrame() != m_MapData[layer][y][x]) //if a tiles is assigend to this layer
			{// dont move the frame till the index chagnes
				TileSet->SetFrame(m_MapData[layer][y][x]);
				//hge->System_Log("Tile Index: %i", mapdata[layer][int(xval)][int(yval)]);
			}
			//this should be the main drawing
			if(TileSet->GetFrame() != TRANSTILE) //draw non transparent tile
			{
				TileSet->Render(TileLeft,TileTop);
			}
			//hge->System_Log("x: %i  y:%i \n", TileLeft, TileTop);
       }
   }
};

bool cMapObject::CheckTeamSide(int x, int y)
{

	return (m_MapData[3][y][x] == m_pParent->GetMPlayerControl()->GetSide());
	//bool rtn = false;
	//switch (m_MapData[3][y][x])
	//		{
	//		case 0:
	//			break;
	//		case HOMETEAM:
	//			{
	//				if (m_pParent->GetMPlayerControl()->GetSide() == HOMETEAM)
	//					rtn = true;
	//				else
	//					rtn = false;
	//			}
	//			break;
	//		case CHALLENGER:
	//			{
	//				if (m_pParent->GetMPlayerControl()->GetSide() == CHALLENGER)
	//					rtn = true;
	//				else
	//					rtn = false;
	//			}
	//			break;
	//		}
	//return rtn;

};

void cMapObject::RenderTeamSides()
{
   int StartTileX = (Screen.xOffset-64)  / TILEWIDTH;
   int StartTileY = (Screen.yOffset-32)  / TILEHALFHEIGHT;
   int StartX=0;
   int StartY=0;
   float TileLeft=0;
   float TileTop=0;
   int a = -1;

   for (int y=StartTileY; y < StartTileY + 42; y++)
   {
       if (y % 2 == 0) StartX = 0; // Even Line //
       else   StartX =  TILEHALFWIDTH; // Odd Line  //

       for (int x=StartTileX; x < StartTileX + 17; x++)
       {

			TileLeft = float((StartX + (x * TILEWIDTH) - Screen.xOffset) -((TILEWIDTH + TILEWIDTH)));
			TileTop = float((StartY + (y * TILEHALFHEIGHT) - Screen.yOffset) -(TILEHEIGHT + TILEHALFHEIGHT));

			switch (m_MapData[3][y][x])
			{
			case HOMETEAM:
				{
					if (m_pParent->GetMPlayerControl()->GetSide() == HOMETEAM)
						m_MyTiles->Render(TileLeft,TileTop);
					else
						m_TheirTiles->Render(TileLeft,TileTop);
				}
				break;
			case CHALLENGER:
				{
					if (m_pParent->GetMPlayerControl()->GetSide() == CHALLENGER)
						m_MyTiles->Render(TileLeft,TileTop);
					else
						m_TheirTiles->Render(TileLeft,TileTop);
				}
				break;
			}
       }
   }

};

void cMapObject::Load(::ifstream &inFile)
{
	int a,b,c;
	int BaseFrames;
	int OverFrames;
    int tempx,tempy,temp;
	char ImageFile[150];
	char tmpBaseName[150];
	//::ifstream inFile;

	Screen.xOffset = 0;
	Screen.yOffset = 0;
//	inFile->open(filename);

	//Load the Texture File
	inFile >> ImageFile;
	strcpy(tmpBaseName, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpBaseName, ImageFile);
	//hge->System_Log(tmpBaseName);
	m_BaseTex=hge->Texture_Load(tmpBaseName);
	//m_pParent->MakeTextureGrayScale(m_BaseTex);

	inFile >> ImageFile;
	strcpy(tmpBaseName, m_pParent->GetGamePath(TEXTURE));
	strcat(tmpBaseName, ImageFile);
	//hge->System_Log(tmpBaseName);
	m_Tex=hge->Texture_Load(tmpBaseName);
	//m_pParent->MakeTextureGrayScale(m_Tex);

	inFile >> BaseFrames;
	inFile >> OverFrames;

	BaseTileSet= new hgeAnimation(m_BaseTex,BaseFrames,0, 0, 0, TILEWIDTH, TILEHEIGHT);
	BaseTileSet->SetHotSpot(0,0);

	TileSet= new hgeAnimation(m_Tex,OverFrames,0, 0, 0, TILEWIDTH, TILEHEIGHT);
	TileSet->SetHotSpot(0,0);


	//get the length and width of the map
	inFile >> tempx;
	inFile >> tempy;

	InitializeMap(tempx,tempy);
	//mapXTiles = tempx;
	//mapYTiles = tempy;

	//mapWidth = mapXTiles * TILEWIDTH;
	//mapHeight = mapYTiles * TILEHALFHEIGHT;

//	hge->System_Log("mapXTiles: %i",mapXTiles);
//	hge->System_Log("mapYTiles: %i",mapYTiles);
//	hge->System_Log("mapwidth: %i",mapWidth);
//	hge->System_Log("mapheight: %i",mapHeight);

	CreateMap(mapXTiles,mapYTiles);

	//load the tile info in
	for (a=0;a<4;a++)
	{
		for (b=0;b<mapYTiles;b++)
		{
			for (c=0;c<mapXTiles;c++)
			{
				//int = tile image index
				inFile >> temp;
				//if(a==0&&temp==-1) //-1 means to pick a random tile
				//	mapdata[a][b][c] = abs(hge->System_Rand(1, BaseFrames));
				//else
				//if(temp==-1) //-1 means to pick a random tile
				//	mapdata[a][b][c] = abs(hge->System_Rand(1, OverFrames));
				//else
					//mapdata[a][b][c] = temp;
					m_MapData[a][b][c] = temp;
			}
		}
	}


	//inFile.close();
	m_target=hge->Target_Create(1024,1024,false);
    Ground=new hgeSprite(hge->Target_GetTexture(m_target),0,0,1024,1024);

//    Ground=new hgeSprite(hge->Target_GetTexture(m_target),0,0,928,664);
//    Ground=new hgeSprite(hge->Target_GetTexture(m_target),0,0,1000,700);

	Screen.xOffset = 64;
	Screen.yOffset = 32;
	UpdateTarget();

	m_Loaded = true;
}

void cMapObject::SetCharacterCollision(int x, int y)
{
	m_MapData[4][y][x] = 1;
};

bool cMapObject::CheckCharacterCollision(int x, int y)
{
	if (m_MapData[4][y][x] != 0)
		return true;
	else
		return false;
};

void cMapObject::ClearCharacterCollision()
{
	int b,c;
	for (b=0;b<mapYTiles;b++)
	{
		for (c=0;c<mapXTiles;c++)
		{
			m_MapData[4][b][c] = 0;
		}
	}
};


bool cMapObject::CheckCollision(int x, int y)
{
	//put a collision boundry around the map
	if (x < 1 || y < 1)
		return true;
	else
	if (x > mapXTiles || y > mapYTiles)
		return true;
	else
//	if (mapdata[2][y][x] != 0)
	if (m_MapData[2][y][x] != 0)
		return true;
	else
		return false;
}

//which zone did the player step on?
int cMapObject::GetEvent(int which, int x, int y)
{
	switch (which)
	{
	case 0:
		 return m_MapData[6][x][y]; break;
	case 1:
		 return m_MapData[5][x][y]; break;
	}
	return 0;
}

void cMapObject::MoveCamera(int change, int x, int y)
{
////smoothly scroll the screen to a new location
//	float diffx = -x - camX;
//	float diffy = -y - camY;
//	int xtest = diffx < 0.0;
//	int ytest = diffy < 0.0;
//	int done = 0;
//	while (!done)
//	{
//	/*	if (change_map)
//		{
//			change_map = 0;
//			mainmap->Load(cur_map);
//		}*/
//		//new sprite may have come onto the screen
////		spritemanager->Update(0);
//		//main render wont be called now so call it
//	//	dx->Render();
//		if (diffx < 0.0)
//		{
//			if (xtest)
//			{
//				MoveCamx(-change);
//				diffx += change;
//			}
//			else
//				diffx = 0.0;
//		}
//		else if (diffx > 0.0)
//		{
//			if (!xtest)
//			{
//				MoveCamx(change);
//				diffx -= change;
//			}
//			else
//				diffx = 0.0;
//		}
//		if (diffy < 0.0)
//		{
//			if (ytest)
//			{
//				MoveCamy(-change);
//				diffy += change;
//			}
//			else
//				diffy = 0.0;
//		}
//		else if (diffy > 0.0)
//		{
//			if (!ytest)
//			{
//				MoveCamy(change);
//				diffy -= change;
//			}
//			else
//				diffy = 0.0;
//		}
//		if ((diffx == 0.0) && (diffy == 0.0)) done = 1;
//	}
}

void cMapObject::SetScreenDim(int height, int width)
{
	Screen.Height = height;
	Screen.Width = width;
};

void cMapObject::SetAmbientColor(int red, int green, int blue)
{
	m_AmbientColor = ARGB(255,red,green,blue);
};
void cMapObject::SetAmbientColor(DWORD ambientcolor)
{
	m_AmbientColor = ambientcolor;
};


int cMapObject::SizeX() { return mapWidth; };
int cMapObject::SizeY() { return mapHeight; };
int cMapObject::ScreenX()  { return Screen.xOffset+64; };
int cMapObject::ScreenY()  { return Screen.yOffset+32; };
int cMapObject::MapWidth() { return mapXTiles; };
int cMapObject::MapHeight() { return mapYTiles; };
int cMapObject::ScreenWidth()  { return Screen.Width; }
int cMapObject::ScreenHeight()  { return Screen.Height; }
int cMapObject::GetTile(int layer, int x, int y) { return m_MapData[layer][x][y]; }
void cMapObject::SetTile(int layer, int x, int y, int tile) { m_MapData[layer][x][y] = tile; }

