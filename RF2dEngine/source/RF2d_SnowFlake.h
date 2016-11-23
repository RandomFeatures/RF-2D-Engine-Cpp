#ifndef RF2D_SNOWFLAKE_H
#define RF2D_SNOWFLAKE_H


class cSnowFlake
{
	public:
		cSnowFlake() {};
		cSnowFlake(int w, int h);
		~cSnowFlake(){};
		void Update(int wind);
		float DeterminSize(float z);
		float GetXPos();
		float GetYPos();
		float GetSize();
	private:
		float		m_xVel;		float		m_yVel;		float		m_xPos;		float		m_yPos;		int		m_zPos;		float   m_Size;		int m_ScreenWidth;		int m_ScreenHeight;};

#endif