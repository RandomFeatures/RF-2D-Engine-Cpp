#include "RF2d_Anistorage.h"
using namespace std;

#ifndef RF2D_EFFECTOBJECT_H
#define RF2D_EFFECTOBJECT_H

class cSpellObject;
class cApp;

class cEffectObject{
	public:
		cEffectObject(cApp *pParent);
		cEffectObject(){};//DO NOT CALL
		cEffectObject(const cEffectObject& p);
		cEffectObject(const cEffectObject* p);
		cEffectObject&	operator= (const cEffectObject &p);

		~cEffectObject();

		void LoadPSI(std::string fileName, float fps);
		void LoadEffect(std::string fileName);
		void AssignTarget(cCharacterObject *target);

		void Render(float fDeltaTime, float xPos, float yPos);
		void Update(float fDeltaTime);
		void Activate();
		void DeActivate();
		void AssignId(int idNo);
		void AssignDuration(float dur);
		void ResetDuration();
		float TimeRemaining();
		bool Finished();
		int Id();
		void SetColor(int red, int green, int blue);
		DWORD GetColor();
	private:
		cApp		*m_pParent;
		bool		m_Copy;
		bool		m_Finished;
		int			m_GUID;
		int			m_ActionFrame;
		float		m_Duration;
		float		m_Elapsed;
		float		m_Particlefps;
		DWORD		m_EffectColor;
		AniStorage *m_Effect;
		hgeParticleSystem*	m_psi;
		hgeSprite*			m_PartSprite;;
		cCharacterObject *m_Target;
		std::string m_psiFileName;

};

#endif
