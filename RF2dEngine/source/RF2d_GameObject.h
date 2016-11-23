#pragma warning(disable:4244)
#include "RF2d_Anistorage.h"
#include "AStar.h"

/*
game character and ansister objects
*/
using namespace std;

#ifndef RF2D_GAMEOBJECT_H
#define RF2D_GAMEOBJECT_H

#define WAIT_TIME 100


struct ObjectPos
{
	ISODIR direction;
	int amountToMove;
	int moveMode;
	int posx, posy;
	int id;
	spriteStatus status;
};

class cSpellObject;
class cEffectObject;


class CGameObject
{
public:
	CGameObject();
	CGameObject(bool blend, bool locked, char* TexPath);
	virtual ~CGameObject();


 	char *GetTexPath();		//Path to the texture
	char *GetDisplayName();	//Sprites Display Name
	char *GetGUID();		//This sprites GUID
	int GetxPos();			//World X
	int GetyPos();			//World Y
	int GetxOffset();		//Sprite offset from wrold x
	int GetyOffset();		//Sprite offset from world y
	int GetScreenX();		//Actual Screen X
	int GetScreenY();		//Actual Screen Y
	int GetTileLength();	//How many tiles long
	int	GetTileWidth();		//How many tiles wide
	int	GetFrame();			//Get the current animation frame
	int GetId();			//Get this sprites ID
	bool IsOnScreen();		//see if this sprite is on the screen
	bool IsMouseOver();		//See if the mouse is over this sprite
	bool GetEnabled();		//See if this sprite is visible
	bool GetBlended();		//See if this sprite is blended
	bool GetLocked();		//Specific action lock
	bool GetSelected();		//See if this sprite is selected

	OBJTYPE GetType(); //find out what type of object this is
	virtual void SetShadows(bool shadow);
	void SetDisplayName(const char *displayname);
	void SetTextPath(char *textpath);
	void SetEnabled(bool enabled);
	void SetBlended(bool blend);
	void SetLocked(bool lock);
	void SetPosition(int x, int y);
	void SetTileSpace(int length, int width);
	void SetOffset(int x, int y);
	void SetGUID(const char *GUID);
	void SetId(int idNo);
	void SetSelected(bool val);
	void SetType(OBJTYPE mtype);
	void SetAmbientColor(int red, int green, int blue);
	void SetAmbientColor(DWORD ambientcolor);
	void LoadStatic(char* frame, char* fileName);
	void LoadSprite(char* fileName, char* spriteName);
	void LoadDefaultTexture(char * fileName);
	virtual	void Render(float fDeltaTime, float camX, float camY, int mode = 0);
	void RenderBottom(hgeSprite* Spri, float x, float y);
	void RenderTop(hgeSprite* Spri, float x, float y);

	virtual void RenderDynamicShadow(int degree);
	virtual void MouseOver(int x, int y);
	void SetGameParent(cApp* GameParent);

	//After the artwork and mesh is loaded
	//virtual bool OnInitalize();
	//virtual void DoAction(int action);
private:
protected:
	bool	m_MouseOver;
	bool	m_Enabled;  //Show the sprite;
	bool	m_Blended; //Blend the Sprite
	bool	m_Locked;  // Specific action lock
	bool	m_OnScreen;
	bool	m_Selected;
	bool	m_UseShadows;

	int		m_PosX;
	int		m_PosY;
	int		m_xOffset;
	int		m_yOffset;
	int     m_TileLength;
	int		m_TileWidth;
	int		m_Frame;
	int		m_ID;
	int     m_ScreenX;
	int     m_ScreenY;
	int		m_Width;
	int		m_Height;
	int		m_DividePoint;
	float	m_HotX;
	float	m_HotY;
	char	m_GUID[20];
	char	m_TexPath[150];
	char	m_DisplayName[20];
	OBJTYPE m_Type; //Object Type
	hgeAnimation*	m_DefaultAni;
	hgeSprite*		m_DefaultSprite;
	HTEXTURE		m_DefaultTex;
	DWORD			m_AmbientColor;
	cApp*			m_GameParent;
};


class CLiveObject :public CGameObject
{
protected:
	int	m_HitPoints;
	int	m_Damage;
	bool m_Dead;
	HEFFECT m_Painsfx;
	HEFFECT m_Deathsfx;

public:
	CLiveObject();
	~CLiveObject();

	int GetHitPoints();
	int GetDamage();
	bool IsDead();
	virtual void SetHitPoints(int HitPoints);
	virtual void SetDamage(int damage);
	virtual int TakeDamage(int physical, int magical);
	virtual bool OnDeath(){return true;};
};


class CAIObject :public CLiveObject
{
public:
	CAIObject(){};
	~CAIObject(){};
	void InitAI(){};
	void ExecuteAI(){};
};

class CMobileObject :public CAIObject
{
protected:
	bool		m_Moving;
	bool		m_MoveAgain;
	bool		m_HasMoved;
	bool		m_Halfway;

	ISODIR			m_CurDirection;
	ISODIR			m_LastDirection;

	int			m_MovingOffSetX;
	int			m_MovingOffSetY;
	int			m_DestX;
	int			m_DestY;
	int			m_NextDestX;
	int			m_NextDestY;

	float		m_Speed;
	float		m_Distance; // Follow/Evade distance
	float		m_Time;
	cIsoAStar	*m_AStar;
public:
	CMobileObject();
	~CMobileObject();

	ISODIR	GetDirection();
	bool GetMoving();
	bool HasMoved();
	bool ShouldStop();
	float GetSpeed();
	float GetDistance();
	void ResetMoved();

	void SetSpeed(float speed);
	void SetDistance(float distance);
	void FaceTarget(int x, int y);
	void SetMoving(bool move);
	void GetNextPath();

	int addOffsetX(int x);
	int addOffsetY(int y);
	int getOffsetX();
	int getOffsetY();
	virtual void UpdateMove(float fDeltaTime);
	virtual void MoveTo(int x, int y, ISODIR dir);
	virtual bool PathFindTo(int x, int y);
	virtual void SetDirection(ISODIR dir);
	virtual void Stop();
	virtual void SwitchDirection(ISODIR direction);

private:
	int pathLength[2];     //stores length of the found path for critter
	int pathLocation[2];   //stores current position along the chosen path for critter
	int* pathBank [2];

	//Path reading variables
	int pathStatus[2];
	int xPath[2];
	int yPath[2];


//	virtual bool Update(long Elapsed){};
//	void Meander(long Elapsed, float *XMove, float *YMove){};
//	bool CheckMove(float *XMove, float *YMove){};
//	bool ValidateMove(float *XMove, float *YMove){};
	//void Update(int keyPress, int entityId);
	//void RandomMove();
	//void MoveNPC();
	//void Displacement();
	//void DistanceToMove();
	//int CheckSpriteCollisions();

};


class cCharacterObject:public CMobileObject
{
	public:
		cCharacterObject(cApp* GameParent, int dir, int action, const char* fileName, char* TexPath);
		~cCharacterObject();

		void Render(float fDeltaTime, float camX, float camY, int mode = 0);
		void RenderPortrait(int x, int y, int w, int h);


		int GetWidth();
		int GetHeight();

		//void ParsePass();
		//int SimulateKeyPress(char direction);
		//int Move();
		//int Type();
		//int MoveMode();
		//int CameraFocus();
		//int curHP();
		//int maxHP();
		//spriteStatus CurStatus();
		//TEXTURE Portrait();

		void SetAction(ACTION action);
		void DoAction(ACTION action);
		void PlayAction(std::string action);
		void StopAction();
		void Stop();
		void ClearTarget();
		void AddTarget(cCharacterObject *npc);
		void AssignCurrentSpellID(int spell);
		int GetCurrentSpellID();
		int GetWalkDistance();
		int GetSide();
		void SetSide(int side);
		void AddSpellList(int spell);
		void ClearSpellList();
		void AddEffect(cEffectObject *effect);
		void ClearEffectList(bool forced);
		void SetAttackAniMode(int mode);
		void UpdateMove(float fDeltaTime);
		void MoveTo(int x, int y, ISODIR dir);
		ACTION GetCurrentAction();
		void SetDirection(ISODIR dir);
		void SwitchDirection(ISODIR direction);
		void MouseOver(int x, int y);
		bool PathFindTo(int x, int y);
		void FileName(const char *name);
		char *FileName();
		ATTACKTYPE GetAttackType();
		CHARSTATUS GetStatus();
		void SetStatus(CHARSTATUS stat);
		void SetAttackType(ATTACKTYPE at);
		void SetAttackType(int at);
		void FaceTarget(cCharacterObject* trgt);
		void SetShadows(bool shadow);
		void LoadPainsfx(std::string filname);
		void LoadDeathsfx(std::string filname);
		void PlayPainsfx();
		void PlayDeathsfx();
		int TakeDamage(int physical, int magical);
		void SetPhyResist(float resist);
		void SetMagResist(float resist);
		void SetAttackMin(int imin);
		void SetAttackMax(int imax);
		void SetHitPoints(int HitPoints);
		void SetWalkDistance(int walk);
		float GetPhyResist();
		float GetMagResist();
		int GetAttackMin();
		int GetAttackMax();
		cCharacterObject* GetLastAttacker();
		cCharacterObject* GetCurrentTarget();
		void SetLastAttacker(cCharacterObject* npc);
		void SetCurrentTarget(cCharacterObject* npc);
		DIR	GetDirFromFileName(std::string filename);
		void RestorPortrait();

	private:

		ACTION		m_CurAction;
		ACTION		m_LastAction;
		ATTACKTYPE  m_AttackType;
		CHARSTATUS	m_Status;
		AniStorage *m_Stand;
		AniStorage *m_Walk;
		AniStorage *m_Attack;
		AniStorage *m_Death;
		AniStorage *m_Pain;

		hgeSprite* m_Portrait;
		HTARGET	 m_target;

		cCharacterObject* m_CurrentTarget;
		cCharacterObject* m_LastAttacker;
		char m_FileName[100];
		int m_PortraitFrame;
		int m_CurrentSpellID;
		int m_WalkDistance;
		int m_Side;
		int m_DamageMin;
		int m_DamageMax;
		int	m_LastDamage;
		float m_PhyResist;
		float m_MagResist;
		DWORD m_EffectColor;
		vector<cCharacterObject*> m_TargetList;
		vector<int> m_SpellList;
		vector<cEffectObject*> m_EffectList;

		void SwitchAction(ACTION action);
		bool UpdateCurrentEffect(int effect);
		void CreatePortrait(int frame);
};


#endif
