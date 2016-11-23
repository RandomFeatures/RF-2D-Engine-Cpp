#include "RF3d_Global.h"

using namespace std;

#ifndef ANISTORAGE_H
#define ANISTORAGE_H


class AniStorage
{
public:
	AniStorage();
	AniStorage(const AniStorage& p);
	AniStorage&	operator= (const AniStorage &p);
	~AniStorage();
	void Init(char* ImageFile, int nFrames, float FPS, float iWidth, float iHeight, bool SingleFacing, bool KeepTexLive = false);
	void Init();
	void AddFacing(char* ImageFile, int nFrames, float FPS, float iWidth, float iHeight, DIR dir, bool KeepTexLive = false);

	void Play();
	void SetMode(int mode);
	void Stop();
	void Update(float fDeltaTime);
	bool IsPlaying();
	void SetSpeed(float fps);
	void SetFrame(int n);
	void SetBlendMode(int blend);
	void SetColor(DWORD col);
	int GetFrame();
	void SetDirection(ISODIR dir);
	void RenderTop(hgeAnimation* Ani, float x, float y);
	void RenderBottom(hgeAnimation* Ani, float x, float y);
	void Render(float fDeltaTime, float x, float y, int mode = 0);
	void SetHotSpot(float x, float y);
	void LoadTexture(char* FileName, bool keeplive);
	void LoadTexture(char* FileName, bool keeplive, DIR dir);

	bool IsLoaded();
	void SetDebug();
	void SetShadows(bool shadow);
	void SetDividePoint(int divide);
	ISODIR  GetDir() { return m_Dir;};
	bool GetOneFacing() { return m_OneFacing;};
	bool GetLoaded() { return m_Loaded;};
	bool GetDebug() { return m_Debug;};
	bool GetBlended() {return m_Blended;};
	int GetMode() { return m_Mode;};
	int GetnFrames() { return m_nFrames;};
	int GetBlendMode() { return m_BlendMode;};
	float GetHotX() { return m_HotX;};
	float GetHotY() { return m_HotY;};
	float GetFPS() { return m_FPS;};
	float GetFrameWidth() { return m_FrameWidth;};
	float GetFrameHeight() { return m_FrameHeight;};
	void LogMe();
	hgeAnimation*       NESprite;
	hgeAnimation*       NWSprite;
	hgeAnimation*       SESprite;
	hgeAnimation*       SWSprite;

private:
	ISODIR				m_Dir;
	HTEXTURE			m_Tex;
	HTEXTURE			m_TexSE;
	HTEXTURE			m_TexNE;
	HTEXTURE			m_TexSW;
	HTEXTURE			m_TexNW;

	DWORD				m_Color;
	bool				m_OneFacing;
	bool				m_Loaded;
	bool				m_Debug;
	bool				m_Blended;
	bool				m_Shadows;
	bool				m_NewStyle;
	int					m_Mode;
	int					m_nFrames;
	int					m_BlendMode;
	int					m_DividePoint;
	float				m_HotX;
	float				m_HotY;
	float				m_FPS;
	float				m_FrameWidth;
	float				m_FrameHeight;
	bool				m_log;
};


#endif