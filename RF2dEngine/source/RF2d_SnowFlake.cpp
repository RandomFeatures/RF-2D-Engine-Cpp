#include "RF3d_Global.h"
#include "RF2d_SnowFlake.h"


cSnowFlake::cSnowFlake(int w, int h)
{	//screen size defaults
	m_ScreenWidth = w;
	m_ScreenHeight = h;

	
	//random z... 
	m_zPos =  hge->Random_Int(50,600);

	//base size on distance
	m_Size = DeterminSize(m_zPos);

	m_ScreenHeight = (m_ScreenHeight * m_Size)*2;
	
	//random x somewhere on the screen
	m_xPos = hge->Random_Float(0,m_ScreenWidth);
	m_yPos = hge->Random_Float(0,m_ScreenHeight);

	//set the gravity
	m_yVel = 2.0f;
	m_xVel = hge->Random_Float(-1,1);
	//multiply velocity by scale to make those up front move faster
	
	m_xVel= m_xVel * m_Size;
	m_yVel= m_yVel * m_Size;
	//hge->System_Log("int x:%f y:%f size %f", m_xVel,m_yVel,m_Size );
};
float cSnowFlake::GetXPos() {return m_xPos;};
float cSnowFlake::GetYPos() {return m_yPos;};
float cSnowFlake::GetSize() {return m_Size;};void cSnowFlake::Update(int wind)
{

	m_xPos = m_xPos +m_xVel;
	m_yPos = m_yPos + m_yVel;
	m_xPos = m_xPos+ (wind*m_Size);

	if (m_yPos>m_ScreenHeight) 
		m_yPos = 0;
	if (m_xPos>m_ScreenWidth) 
	{
		m_xPos = 0;
	}else
	{ 
		if (m_xPos<0) m_xPos = m_ScreenWidth;
	}
};float cSnowFlake::DeterminSize(float z){	int fov;	int bns;	fov = hge->Random_Int(100,300);	bns = hge->Random_Int(300,500);	return (fov/(z+bns));}