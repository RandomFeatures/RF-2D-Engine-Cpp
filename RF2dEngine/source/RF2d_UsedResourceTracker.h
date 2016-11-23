
using namespace std;

#define resourcemanager (cUsedResources::Instance())

#ifndef RF2D_USEDRESOURCES_H
#define RF2D_USEDRESOURCES_H

struct TextureListItem 
{
	bool Used;
	bool Protected;
	char FileName[150];
	HTEXTURE Texture;
};

class cUsedResources{
	public:
		cUsedResources(){};
		~cUsedResources();
		static cUsedResources *Instance();
		void GetTexture(char *fileName, HTEXTURE &rtnTex, bool protect = false);
		void ResetUsage();
		void ClearUnusedTex();
		int	MakeTextureGrayScale(HTEXTURE tex);
	private:
		vector<TextureListItem*> TextureList;
		static cUsedResources *_instance;
};

#endif