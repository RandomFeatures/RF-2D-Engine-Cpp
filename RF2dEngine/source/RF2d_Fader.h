//Created by Scott Conner
//May 2005
//INCLUDES/////////////////
#include <hge.h>
///////////////////////////
#pragma once

/* 
Fade the screen to black
fade the scren from black
*/


//CLASS/////////////////////
class cFader
{
private:
   hgeQuad      fader;
   float      alpha;
   int         direction;
protected:
public:
   cFader();
   ~cFader();
   void update( float timeDelta );
   void render();
   void fadeIn();
   void fadeOut();

   bool doneFading();
};
////////////////////////////
