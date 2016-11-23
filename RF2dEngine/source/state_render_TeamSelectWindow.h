#include "StateManager.h"
#include "GameState.h"



#ifndef STATE_RENDER_TEAMSELECTWINDOW_H_INCLUDED
#define STATE_RENDER_TEAMSELECTWINDOW_H_INCLUDED

class cApp;

class stateTeamSelectWindow : public GameState
{
   public:
      stateTeamSelectWindow(cApp*  pParent);
      ~stateTeamSelectWindow(){};

      void initialize();
      void shutdown();
      void captureinput(float dt);
      void frame();

   private:
       cApp*  m_pGame;
       float movetimer;
};

#endif // STATE_RENDER_TEAMSELECTWINDOW_H_INCLUDED
