#include "StateManager.h"
#include "GameState.h"


#ifndef STATE_RENDER_SINGLEGUIWINDOW_H_INCLUDED
#define STATE_RENDER_SINGLEGUIWINDOW_H_INCLUDED

class cApp;

class stateSingleGUIWindow : public GameState
{
   public:
      stateSingleGUIWindow(cApp*  pParent);
      ~stateSingleGUIWindow(){};

      void initialize();
      void shutdown();
      void captureinput(float dt);
      void frame();

   private:
       cApp*  m_pGame;
       HTEXTURE LogoTex;
       hgeSprite* LogoSprite;
};


#endif // STATE_RENDER_SINGLEGUIWINDOW_H_INCLUDED
