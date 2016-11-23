#include <fstream>
#include <vector>
#include "RF3d_Global.h"

cUsedResources *cUsedResources::_instance = NULL;

cUsedResources::~cUsedResources()
{
	TextureListItem *tex;
	for (unsigned int iloop=0;iloop<TextureList.size();iloop++)
	{
		tex = TextureList[iloop];
		hge->Texture_Free(tex->Texture);
		delete tex;
	}
	TextureList.clear();

};

//Return a single instance of the gameobject manager
cUsedResources *cUsedResources::Instance()
{
	if (_instance == NULL)
		_instance = new cUsedResources();
	return _instance;
};

void cUsedResources::ResetUsage()
{
	for (unsigned int iloop=0;iloop<TextureList.size();iloop++)
	{
		TextureList[iloop]->Used = false;
	}

}
void cUsedResources::ClearUnusedTex()
{
	TextureListItem *txt;
	//Loop through the ActiveSpellList looking for spells that are finshed
	//In a turn based game this list should never have more than 1 or 2 items to compare
	vector<TextureListItem*>::iterator iter;
	for (iter = TextureList.begin();iter< TextureList.end();)
	{
		if ((*iter)->Used == false && (*iter)->Protected == false)
		{
			txt = *iter;
			hge->Texture_Free(txt->Texture);
			delete txt;
			iter = TextureList.erase(iter);
		}else
		iter++;
	}

}

void cUsedResources::GetTexture(char *fileName, HTEXTURE &rtnTex, bool protect)
{
	bool bFound = false;
	//Seach the Master NPC list and find a pointer to the caster
	vector<TextureListItem*>::iterator iter;
	for (iter = TextureList.begin();iter < TextureList.end();iter++)
	{
		if (strcmp((*iter)->FileName,fileName) == 0)
		{
			rtnTex = (*iter)->Texture;
			(*iter)->Used = true;
			bFound = true;
 			//hge->System_Log("Used Existing %s", fileName);
			break;
		}
	}

	if (!bFound)
	{
		rtnTex=hge->Texture_Load(fileName);
		//MakeTextureGrayScale(rtnTex);
		TextureListItem *listItem = new TextureListItem;
		strcpy(listItem->FileName, fileName);
		listItem->Texture = rtnTex;
		listItem->Used = true;
		listItem->Protected = protect;
		TextureList.push_back(listItem);
		//hge->System_Log("Loaded New %s", fileName);
	}
}

int cUsedResources::MakeTextureGrayScale(HTEXTURE tex)
{
   unsigned char *surface = (unsigned char *)hge->Texture_Lock(tex, false);

   if (surface == NULL) return -1;

   int w, h;

   w = hge->Texture_GetWidth(tex);
   h = hge->Texture_GetHeight(tex);

   // As were updating the whole texture we need no regard for X,Y
   // points - just the amount of data

   int pixels = w * h * 4;  // each pixel is 32 bits ARGB

   int i;
   int b1, b2, b3, res;

   for (i = 0; i < pixels; i += 4) // skip along 4 bytes at a time
   {
      // Since reading byte at a time...byte ordering is important
      // but because of what were doing only the alpha has to be left

      b1 = (int)surface[i];
      b2 = (int)surface[i + 1];
      b3 = (int)surface[i + 2];

      res = (b1 + b2 + b3) / 3;

      surface[i] = (unsigned char)res;
      surface[i + 1] = (unsigned char)res;
      surface[i + 2] = (unsigned char)res;
   }

   // Free surface so it gets put back into video memory.

   hge->Texture_Unlock(tex);

        return 0;
}
