#include "StateManager.h"
#include "GameState.h"



#ifndef STATE_RENDER_GAMEWINDOW_H_INCLUDED
#define STATE_RENDER_GAMEWINDOW_H_INCLUDED

class cApp;

class stateGameWindow : public GameState
{
   public:
      stateGameWindow(cApp*  pParent);
      ~stateGameWindow(){};

      void initialize();
      void shutdown();
      void captureinput(float dt);
      void frame();

   private:
       cApp*  m_pGame;
       float movetimer;
};


#endif // STATE_RENDER_GAMEWINDOW_H_INCLUDED
