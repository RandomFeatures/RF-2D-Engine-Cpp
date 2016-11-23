//INCLUDES//////////
#include "RF2d_Fader.h"
int const SCREEN_WIDTH = 1024;
int const SCREEN_HEIGHT = 768;
////////////////////

/* 
Fade the screen to black
fade the scren from black
*/

//external
extern HGE*      hge;

//CONSTRUCTORS//////
cFader::cFader()
{
         //starts the alpha at full black, and is ready to fade in
   alpha = 255;
   direction = -1;

   //sets up the quad to the coords of the screen
   fader.v[0].x = 0;
   fader.v[0].y = 0;
   fader.v[1].x = SCREEN_WIDTH;
   fader.v[1].y = 0;
   fader.v[2].x = SCREEN_WIDTH;
   fader.v[2].y = SCREEN_HEIGHT;
   fader.v[3].x = 0;
   fader.v[3].y = SCREEN_HEIGHT;
   fader.v[0].z = 1.0f;
   fader.v[1].z = 1.0f;
   fader.v[2].z = 1.0f;
   fader.v[3].z = 1.0f;

   //sets the color of the quad
   for ( int x = 0; x < 4; x++ )
   {
      fader.v[x].col = ARGB( alpha, 0, 0, 0 );
   }

   fader.tex = 0;
   fader.blend = BLEND_DEFAULT;
}

cFader::~cFader()
{

}

void cFader::update( float timeDelta )
{
   if ( direction == 0 )
   {
      return;
   }

   //updates the fader/////////////
        //note: 300 can be replaced for any fader speed, could even
        //be a parameter if you so chose. 300 works well.
   alpha += timeDelta * 300 * direction;

   //caps//////////
   if ( alpha >= 255 )
   {
      alpha = 255;
      direction = 0;
   }
   else if ( alpha <= 0 )
   {
      direction = 0;
      alpha = 0;
   }
   ////////////////

   for ( int x = 0; x < 4; x++)
   {
      fader.v[x].col = ARGB( alpha,0,0,0 );
   }

   ////////////////////////////////
}

void cFader::render()
{
   if ( alpha != 0 )
   {
      hge->Gfx_RenderQuad( &fader );
   }
}

void cFader::fadeIn()
{
   direction = -1;
}
   
void cFader::fadeOut()
{
   direction = 1;
}

bool cFader::doneFading()
{
   return direction == 0;
}
//////////////////// 