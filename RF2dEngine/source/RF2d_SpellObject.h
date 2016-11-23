#pragma warning(disable:4244)
#include "RF2d_Anistorage.h"

using namespace std;

#ifndef RF2D_SPELLOBJECT_H
#define RF2D_SPELLOBJECT_H

class cCharacterObject;
class cEffectObject;
class cApp;
class cProjectileObject;

typedef vector<cCharacterObject*> TargetList;

class cSpellObject{
	public:
		cSpellObject(cApp *pParent, std::string filename);
		cSpellObject(){};//do not call
		cSpellObject(const cSpellObject& p);
		cSpellObject&	operator= (const cSpellObject &p);
		~cSpellObject();

		//Script Functions
		void LoadTargetFile(std::string fileName, std::string ttype, std::string tstart);
		void LoadTarget(std::string fileName, int trgtype, int trgstrt);
		void LoadSpellCast(std::string fileName);
		void CreateEffect();
		void CreateProjectile();
		void SetSpellRange(int rng);
		void SimpleCast();
		void SimpleHit();
		void SimpleDamage(cCharacterObject *target, int physical, int magical);
		void AutoPrepareLaunch();
		void LoadAttacksfx(std::string filename);
		void LoadHitsfx(std::string filename);
		void PlayAttacksfx();
		void PlayHitsfx();


		//Script Cast Setup
		void SwitchDirection(ISODIR direction);
		void SetCastMode(int mode);
		void PlayCast();

		//Script After Cast
		void AssignEffect();
		void LaunchProjectile();

		void Render(float fDeltaTime, float camX, float camY);
		int xPos();
		int yPos();
		int Width();
		int Height();
		int Id();
		int CastMode();

		ISODIR Direction();

		void SetPosition(int x, int y);
		void SetDirection(ISODIR dir);
		void AssignId(int idNo);
		void AssignCaster(cCharacterObject *caster);
		void AssignTarget(cCharacterObject *target);
		void Hit();
		void Cast();
		void Cast(cCharacterObject *caster, cCharacterObject *target);
		void Cast(cCharacterObject *caster, TargetList targetlist);
		bool Finished();
		WorldPoint GetTargetTile(bool even, int group, unsigned int it);
		int GetTargetGroup(bool even, int x, int y);
		int GetTileCount(int group);
		bool GetGroupActive(int group);
		int GetGroupCount();
		int GetRange();
		DWORD GetGlowColor();
		std::string GetScriptName();
		AniStorage* GetCastAni();
		cCharacterObject* GetCaster();
		cCharacterObject* GetTarget();
		TRGTTYPE GetTargetType();
		TRGTSTART GetTargetStart();
		bool TargetHitTest(bool even, int x, int y);

	private:

		HEFFECT		m_Attacksfx;
		HEFFECT		m_Hitsfx;
		ACTION		m_CurAction;
		ACTION		m_LastAction;
		ISODIR			m_CurDirection;
		ISODIR			m_LastDirection;
		int			m_GUID;
		int			m_ActionFrame;
		int			m_PosX;
		int			m_PosY;
		int			m_CastMode;
		int			m_Range;
		bool		m_Casting;
		bool		m_Loading;
		bool		m_ActionDone;
		TRGTTYPE	m_TrgtType;
		TRGTSTART	m_TrgtStart;
		std::string m_ScriptName;
		std::string m_TrgtFileName;
		AniStorage *m_Cast;

		vector<WorldPoint>	m_g1EvenList;
		vector<WorldPoint>	m_g1OddList;
		vector<WorldPoint>	m_g2EvenList;
		vector<WorldPoint>	m_g2OddList;
		vector<WorldPoint>	m_g3EvenList;
		vector<WorldPoint>	m_g3OddList;
		vector<WorldPoint>	m_g4EvenList;
		vector<WorldPoint>	m_g4OddList;

		cCharacterObject *m_Caster;
		cCharacterObject *m_Target;
		cApp			 *m_pParent;
		cEffectObject	 *m_Effect;
		cProjectileObject *m_Projectile;
		TargetList		m_TargetList;
		void SwitchAction(ACTION action);
		void AddEvenList(int group, int x, int y);
		void AddOddList(int group, int x, int y);

};

#endif
