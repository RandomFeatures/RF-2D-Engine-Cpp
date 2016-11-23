#pragma warning(disable:4244)
#include "RF2d_Anistorage.h"
#include "AStar.h"

using namespace std;

#ifndef RF2D_PROJECTILEOBJECT_H
#define RF2D_PROJECTILEOBJECT_H

class cSpellObject;
class cCharacterObject;
class cApp;

class cProjectileObject :public CGameObject
{
	public:
		cProjectileObject(){};  //DO NOT CALL
		cProjectileObject(cApp *pParent);
		cProjectileObject(const cProjectileObject& p);
		cProjectileObject&	operator= (const cProjectileObject &p);
		~cProjectileObject();

		//Script Functions
		void LoadHit(std::string fileName);
		void LoadTravel(std::string fileName);
		void LoadTravel2(std::string firstFile, std::string secondFile);
		void LoadPSI(std::string fileName, float fps);
		void AddGlowEffect(int red, int green, int blue);
		void AssignCaster(cCharacterObject *caster);
		void AssignTarget(cCharacterObject *target);
		void AssignSpell(cSpellObject *spell);
		void SetPosition(int x, int y);
		void GetStartPosFromCaster();


		void Render(float fDeltaTime, float camX, float camY);
		int addOffsetX(int x);
		int addOffsetY(int y);
		int getOffsetX();
		int getOffsetY();
		void Launch();
		bool ShouldStop();
		void PrepareLaunch(cSpellObject *p);
		void UpdateMove(float fDeltaTime);
		void GlowEffect(DWORD glowcolor);
		void GetNextPath();
		bool PathFindTo(int x, int y);
		void SetDirection(DIR dir);
		void Stop();
		void Hit();
		bool Finished();
		void ResetMoved();
		bool GetMoving();
		bool HasMoved();
		int Id();

	private:
		cApp		*m_pParent;

		ACTION		m_CurAction;
		ACTION		m_LastAction;
		DIR			m_CurDirection;
		DIR			m_LastDirection;
		int			m_GUID;
		int			m_ActionFrame;
		bool		m_Traveling;
		bool		m_Hitting;
		bool		m_Loading;
		bool		m_ActionDone;
		bool		m_Moving;
		bool		m_MoveAgain;
		bool		m_HasMoved;
		bool		m_Halfway;
		bool		m_Lighted;
		int			m_MovingOffSetX;
		int			m_MovingOffSetY;
		int			m_DestX;
		int			m_DestY;
		int			m_NextDestX;
		int			m_NextDestY;
		int			m_LastPosX;
		int			m_LastPosY;
		float		m_Time;
		float		m_FlickTimer;
		float		m_Particlefps;
		DWORD		m_GlowColor;
		cAStar		*m_AStar;
		//HTEXTURE			m_PartTex;
		hgeParticleSystem*	m_psi;
		hgeSprite*			m_PartSprite;
		hgeSprite*			m_GlowSprite;
		hgeVector*			m_MyVector;
		hgeVector*			m_TargetVector;
		AniStorage*			m_Travel;
		AniStorage*			m_Hit;
		AniStorage*			m_TmpTravel1;
		AniStorage*			m_TmpTravel2;
		cCharacterObject*	m_Caster;
		cCharacterObject*	m_Target;
		cSpellObject*		m_SourceSpell;
		std::string			m_psiFileName;
		void SwitchAction(ACTION action);
};

#endif
