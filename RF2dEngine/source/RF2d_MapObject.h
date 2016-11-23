#pragma warning(disable:4244)

#include "hgeanim.h"

using namespace std;

#ifndef RF2D_MAPOBJECT_H
#define RF2D_MAPOBJECT_H

#define TRANSTILE 0
#define	TILEWIDTH 64
#define	TILEHEIGHT 32
#define	TILEHALFWIDTH 32
#define	TILEHALFHEIGHT 16

typedef struct
{
public:
  int xOffset;
  int yOffset;
  int Width;
  int Height;
}sScreenInfo;



class cMapObject
{
	public:
		cMapObject(cApp* pParent);
		~cMapObject();
		//static cMapObject *Instance();

		void RenderMap(int layer);
		void RenderBaseLayer();
		void Render();
		void RenderTeamSides();
		void UpdateTarget();
		void InitializeMap(int dimensionx, int dimensiony);
		void MoveCamx(int value);
		void MoveCamy(int value);
		void ClearMap();
		void Load(::ifstream &inFile);
		void CreateMap(int dimensionx, int dimensiony);
		void MoveCamera(int change, int x, int y);
		void SetTile(int layer, int x, int y, int tile);
		void SetAmbientColor(int red, int green, int blue);
		void SetAmbientColor(DWORD ambientcolor);
		void SetCharacterCollision(int x, int y);
		bool CheckCharacterCollision(int x, int y);
		void ClearCharacterCollision();
		void GainFocus();
		void MapName(char *name);
		char *MapName();		
		bool CheckCollision(int x, int y);
		bool CheckTeamSide(int x, int y);
		int GetEvent(int which, int x, int y);
		int GetTile(int layer, int x, int y);
		int SizeX();
		int SizeY();
		int MapWidth();
		int MapHeight();
		int ScreenX();
		int ScreenY();
		int ScreenWidth();
		int ScreenHeight();
		
		void SetScreenDim(int height, int width);

	private:
		cApp* m_pParent;
		//static cMapObject *_instance;
		bool m_Loaded;
		int mapXTiles, mapYTiles; //int version of the map dim
		//int ***mapdata;
		int m_MapData[7][64][32];
		int tilesX, tilesY; //How many tiles per screen
		int scrollx, scrolly;
		int mapWidth, mapHeight; //float version of the map dim
		char m_MapName[20];
		sScreenInfo Screen;
		hgeAnimation* BaseTileSet;
		hgeAnimation* TileSet;
		hgeSprite* Ground;
		hgeSprite* m_MyTiles;
		hgeSprite* m_TheirTiles;
		HTEXTURE m_TrgtTex;

		HTEXTURE m_BaseTex;
		HTEXTURE m_Tex;
		HTARGET	 m_target;
		DWORD	m_AmbientColor;
};




#endif