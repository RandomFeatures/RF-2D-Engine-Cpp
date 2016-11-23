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
		float		m_xVel;

#endif