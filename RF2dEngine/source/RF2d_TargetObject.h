#include "RF2d_GameObject.h"
#include "RF2d_SpellObject.h"
#include "RF2d_EffectObject.h"

/*
render and control attact targeting system. 
*/

#pragma warning(disable:4244)


using namespace std;

#ifndef RF2D_TARGETOBJECT_H
#define RF2D_TARGETOBJECT_H

class cApp;

class cTargetObject{
	public:
		cTargetObject(cApp* pParent);
		~cTargetObject();
		void TargetType(TRGTTYPE tp);
		TRGTTYPE TargetType();
		void SetSource(int x, int y);
		void SetSource(cCharacterObject* source);
		void SetTarget(int x, int y);
		void SetTarget(cCharacterObject* target);
		void SetMode(TRGTMODE mode);
		bool isShown();
		void Cancel();
		void UserClick();
		void Reset();
		void Update();
		void Render(float fDeltaTime, float camX, float camY);
	private:
		void AddTargetToList(WorldPoint wldtrgt, WorldPoint offset);
		void AddTargetToList(int x, int y);
		ScreenPoint GetMouseTileScrnPos();
		ScreenPoint GetScrnPos(int wrldx, int wrldy);
		ScreenPoint GetScrnPos(WorldPoint wrldx);
		WorldPoint GetOffSetTileWrldPos(int scrnx, int scrny);
		float GetOffSetTilePosScrnX(float start, int count);
		float GetOffSetTileNegScrnX(float start, int count);
		float GetOffSetTilePosScrnY(float start, int count);
		float GetOffSetTileNegScrnY(float start, int count);
		void GetTargets();
		void CastSpell();
		void MeleeAttack();
		void FindTargetsInSpellArea();
		bool MouseInHighLightList(TRGTMODE mode);
		void BuildHighlightList(int count, WorldPoint center, TRGTMODE mode);
		void BuildGroupHighlightList(WorldPoint center, TRGTMODE mode);
		bool CheckCollision(WorldPoint wrld, TRGTMODE mode);
		void RenderHighLightWalk(hgeSprite* sprt);
		void RenderHighLightAttack(hgeSprite* sprt);
		void RenderSpellArea(hgeSprite* sprt);
		void RenderGroupSpellArea(hgeSprite* sprt);
		hgeSprite* GetRedSprite();
		hgeSprite* GetYellowSprite();
		hgeSprite* GetBlueSprite();
		hgeSprite* GetGreySprite();


	private:
		cApp*						m_pParent;
		bool						m_ValidTarget;
		HTEXTURE					m_TrgtTex;
		hgeSprite*					m_TrgtSprite;
		hgeSprite*					m_RedSprite;
		hgeSprite*					m_YellowSprite;
		hgeSprite*					m_BlueSprite;
		hgeSprite*					m_GreySprite;
		TRGTTYPE					m_TrgtType;
		TRGTSTART					m_TrgtStart;
		TRGTMODE					m_TrgtMode;
		ATTACKTYPE					m_AttackType;
		cCharacterObject*			m_SourceNPC;
		cCharacterObject*			m_TargetNPC;
		cSpellObject*				m_SourceSpell;
		WorldPoint					m_TargetWldPos;
		WorldPoint					m_SourceWldPos;
		ScreenPoint					m_TargetScrnPos;
		ScreenPoint					m_SourceScrnPos;
		ScreenPoint					m_MouseScrnPos;
		float						m_ScreenX;
		float						m_ScreenY;
		vector<WorldPoint>			m_HighlightWlkList;
		vector<WorldPoint>			m_HighlightAttkList;
		TargetList					m_TargetList;
		
};

#endif