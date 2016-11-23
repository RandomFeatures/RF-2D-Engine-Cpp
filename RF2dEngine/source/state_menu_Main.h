#include "StateManager.h"
#include "GameState.h"



#ifndef STATE_MENU_MAIN_H_INCLUDED
#define STATE_MENU_MAIN_H_INCLUDED


class cApp;

class stateMenuMain : public GameState
{
   public:
      stateMenuMain(cApp*  pParent);
      ~stateMenuMain(){};

      void initialize();
      void shutdown();
      void captureinput(float dt);
      void frame();
      void MenuSingleMap();
   private:
       cApp*  m_pGame;
       hgeFont*	m_MenuFont;
       hgeGUI		*gui;
       HTEXTURE	LogoTex;
       hgeSprite* LogoSprite;
       int		lastid;
       HEFFECT	snd;

};


#endif // STATE_MENU_MAIN_H_INCLUDED
