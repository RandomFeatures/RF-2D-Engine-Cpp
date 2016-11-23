#include "RF2d_Application.h"
#include "RF3d_Global.h"
#include "RF2d_CallBackHeader.h"





bool ptCheckCollision(int x, int y)
{
   return g_Game->GetMainMap()->CheckCollision(x,y);
};


bool ptCheckCharCollision(int x, int y)
{
   return g_Game->GetMainMap()->CheckCharacterCollision(x,y);
};


// compare the y values only.
int CompareY( CGameObject* l, CGameObject* r )
{

	if (l->GetyPos() == r->GetyPos() )
	{
		if( l->GetxPos() < r->GetxPos() )
			return 1;
		if( l->GetxPos() > r->GetxPos() )
			return -1;
	}
	if( l->GetyPos() < r->GetyPos() )
        return -1;
    if( l->GetyPos() > r->GetyPos() )
        return 1;
    return 0;
};

