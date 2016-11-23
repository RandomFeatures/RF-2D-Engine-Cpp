/**************************************************
RF3d_Global.h
Random Features 3D RPG Component

by Allen Halsted (10 June 2002)
**************************************************/

#ifndef _RF3D_GLOBAL_H_
#define _RF3D_GLOBAL_H_

// Windows includes
#include <windows.h>

// Standard ANSI-C includes
#include <stdio.h>
#include <vector>
#include <libsqlitewrapped.h>

//MyStuff
#include "strfunctions.h"

//HGE
#include "hge.h"
#include "hgesprite.h"
#include "hgefont.h"
#include "hgeparticle.h"
#include "hgeanim.h"
#include "hgegui.h"
//#include "hgecolor.h"
//#include "hgevector.h"

// Core includes
#include "resource.h"
#include "System.h"

#include "RF2d_GameObjectManager.h"
#include "RF2d_MapObject.h"
#include "RF2d_UsedResourceTracker.h"
#include "RF2d_GUIWindow.h"
#include "RF2d_GUIControls.h"
#include "RF2d_GUIManager.h"
#include "RF2d_MPlayer.h"
#include "RF2d_Cursor.h"
#include "RF2d_TargetObject.h"
#include "RF2d_CharGUI.h"
#include "RF2d_Light.h"
#include "RF2d_Sound.h"
#include "RF2d_Projectile.h"
#include "RF2d_SoundManager.h"
#include "RF2d_CloudShadows.h"
#include "RF2d_FogControl.h"
#include "RF2d_SnowControl.h"
#include "RF2d_WeatherControl.h"
#include "RF2d_BattleControl.h"
///////////////////////////////////////////////////////////
// Global declarations
///////////////////////////////////////////////////////////
extern HGE *hge;

#endif

