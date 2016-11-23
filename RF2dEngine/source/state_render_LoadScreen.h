#include "StateManager.h"
#include "GameState.h"


#ifndef STATE_RENDER_LOADSCREEN_H_INCLUDED
#define STATE_RENDER_LOADSCREEN_H_INCLUDED

class cApp;

class stateLoadScreen : public GameState
{
   public:
      stateLoadScreen(cApp*  pParent);
      ~stateLoadScreen(){};

      void initialize();
      void shutdown();
      void captureinput(float dt);
      void frame();

   private:
        cApp*  m_pGame;
        HTEXTURE LoadBGtex;
        HTEXTURE StatusBartex;
        hgeSprite* BGSprite;
        hgeSprite* StatusBarSprite;
        hgeAnimation* StatusBar;
};


#endif // STATE_RENDER_LOADSCREEN_H_INCLUDED
