#include "StateManager.h"
#include "GameState.h"


#ifndef STATE_MENU_MPLAYER_H_INCLUDED
#define STATE_MENU_MPLAYER_H_INCLUDED


class cApp;

class stateMenuMPlayer : public GameState
{
   public:
      stateMenuMPlayer(cApp*  pParent);
      ~stateMenuMPlayer(){};

      void initialize();
      void shutdown();
      void captureinput(float dt);
      void frame();
      void MenuHost();
      void MenuConnect();

   private:
       cApp*  m_pGame;
       hgeFont*		m_MenuFont1;
	   hgeGUI*		gui1;
       HTEXTURE		LogoTex;
       hgeSprite*	LogoSprite;
       HEFFECT		snd;
       int			lastid;
};

#endif // STATE_MENU_MPLAYER_H_INCLUDED
