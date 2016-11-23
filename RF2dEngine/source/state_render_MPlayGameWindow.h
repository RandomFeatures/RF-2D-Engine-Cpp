#include "StateManager.h"
#include "GameState.h"


#ifndef STATE_RENDER_MPLAYGAMEWINDOW_H_INCLUDED
#define STATE_RENDER_MPLAYGAMEWINDOW_H_INCLUDED

class cApp;

class stateMPlayGameWindow : public GameState
{
   public:
      stateMPlayGameWindow(cApp*  pParent);
      ~stateMPlayGameWindow(){};

      void initialize();
      void shutdown();
      void captureinput(float dt);
      void frame();

   private:
       cApp*  m_pGame;
       float movetimer;
};


#endif // STATE_RENDER_MPLAYGAMEWINDOW_H_INCLUDED
