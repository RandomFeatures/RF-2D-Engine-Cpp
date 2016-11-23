#pragma warning(disable:4244)
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
}

// luabind
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>


using namespace luabind; // this using namespace command is of course optional

#include <stdio.h>
#include <string.h>
#include "RF2d_Scripting.h"
#include "RF3d_Global.h"
#include "RF2d_SpellObject.h"
#include "RF2d_EffectObject.h"
#include "RF2d_Projectile.h"
#include "RF2d_GameObject.h"
#include "RF2d_BattleControl.h"
#include "RF2d_Application.h"
#define NUM_FUNCS 37

void l_bindClasses(lua_State *luaVM)
{
	module(luaVM)
	[
		class_<cApp>("Game")
		.def("ChangeMap",&cApp::ChangeMap),
		class_<cBattleControl>("ObjBattle")
		.def("SetTargetNPC",&cBattleControl::SetTargetNPC)
		.def("TurnComplete",&cBattleControl::TurnComplete),
		class_<cGameObjectManager>("ObjManager")
		.def("IdleWander",&cGameObjectManager::IdleWander)
		.def("addSpell",&cGameObjectManager::AddSpell)
		.def("ScriptCastSpell",&cGameObjectManager::ScriptCastSpell)
		.def("GetNPCAt",&cGameObjectManager::GetNPCAt)
		.def("GetWeakestNPC",&cGameObjectManager::GetWeakestNPC)
		.def("GetStrongestNPC",&cGameObjectManager::GetStrongestNPC)
		.def("GetHighPhyNPC",&cGameObjectManager::GetHighPhyNPC)
		.def("GetHighMagNPC",&cGameObjectManager::GetHighMagNPC)
		.def("GetLowPhyNPC",&cGameObjectManager::GetLowPhyNPC)
		.def("GetLowMagNPC",&cGameObjectManager::GetLowMagNPC)
		.def("GetNearestNPC",&cGameObjectManager::GetNearestNPC)
		.def("GetSelectNPC",&cGameObjectManager::GetSelectNPC)
		.def("GetSpell",&cGameObjectManager::GetSpell)
		.def("MoveToAttackRange",&cGameObjectManager::MoveToAttackRange),
		class_<cSpellObject>("Spell")
		.def("LoadTargetFile", &cSpellObject::LoadTargetFile)
		.def("LoadSpellCast", &cSpellObject::LoadSpellCast)
		.def("CreateEffect", &cSpellObject::CreateEffect)
		.def("AutoPrepareLaunch", &cSpellObject::AutoPrepareLaunch)
		.def("CreateProjectile", &cSpellObject::CreateProjectile)
		.def("SetSpellRange", &cSpellObject::SetSpellRange)
		.def("SetCastMode", &cSpellObject::SetCastMode)
		.def("PlayCast", &cSpellObject::PlayCast)
		.def("AssignEffect", &cSpellObject::AssignEffect)
		.def("LaunchProjectile", &cSpellObject::LaunchProjectile)
		.def("SimpleCast", &cSpellObject::SimpleCast)
		.def("SimpleHit", &cSpellObject::SimpleHit)
		.def("SimpleDamage", &cSpellObject::SimpleDamage)
		.def("LoadAttacksfx", &cSpellObject::LoadAttacksfx)
		.def("LoadHitsfx", &cSpellObject::LoadHitsfx)
		.def("PlayAttacksfx", &cSpellObject::PlayAttacksfx)
		.def("PlayHitsfx", &cSpellObject::PlayHitsfx),
		class_<cEffectObject>("Effect")
		.def("SetColor", &cEffectObject::SetColor)
		.def("LoadEffect", &cEffectObject::LoadEffect)
		.def("LoadPSI", &cEffectObject::LoadPSI),
		class_<CGameObject>("objGame"),
		class_<cProjectileObject, CGameObject>("Projectile")
		.def("LoadHit", &cProjectileObject::LoadHit)
		.def("LoadTravel", &cProjectileObject::LoadTravel)
		.def("LoadTravel2", &cProjectileObject::LoadTravel2)
		.def("LoadPSI", &cProjectileObject::LoadPSI)
		.def("AddGlowEffect", &cProjectileObject::AddGlowEffect)
		.def("AssignCaster", &cProjectileObject::AssignCaster)
		.def("AssignTarget", &cProjectileObject::AssignTarget)
		.def("SetPosition", &cProjectileObject::SetPosition)
		.def("GetStartPosFromCaster", &cProjectileObject::GetStartPosFromCaster),
		class_<CLiveObject, CGameObject>("objLive"),
		class_<CAIObject, CLiveObject>("objAI"),
		class_<CMobileObject, CAIObject>("objMobile"),
		class_<cCharacterObject, CMobileObject>("objCharacter")
		.def("GetCurrentSpellID", &cCharacterObject::GetCurrentSpellID)
		.def("GetGUID", &cCharacterObject::GetGUID)
		.def("FaceCharacter", &cCharacterObject::FaceTarget)
		.def("PathFindTo", &cCharacterObject::PathFindTo)
		.def("AddEffect", &cCharacterObject::AddEffect)
		.def("PlayAction", &cCharacterObject::AddEffect)
		.def("TakeDamage", &cCharacterObject::TakeDamage)
	];
}

void l_LoadScriptFile(bool Autoexec, lua_State *luaVM, char *fname)
{
	luaL_dofile(luaVM, fname);
	if (Autoexec)
		l_Autoexec(luaVM);
};

void l_Autoexec(lua_State *luaVM)
{
	call_function<void>(luaVM,"autoexec",g_Game,g_Game->GetGameObjMangr());
};

void l_Postexec(lua_State *luaVM)
{
	call_function<void>(luaVM,"postexec",g_Game,g_Game->GetGameObjMangr());
};

void l_StartTurn(cCharacterObject *source)
{
	hge->System_Log("Test: StartTurn");
	call_function<void>(g_Game->GetLuaVM(),"StartTurn",g_Game,g_Game->GetGameObjMangr(),g_Game->GetBattleControl(), source);
};

void l_AttackDone(cCharacterObject *source, cCharacterObject *target)
{
	hge->System_Log("Test: AttackDone");
	call_function<void>(g_Game->GetLuaVM(),"AttackDone",g_Game,g_Game->GetGameObjMangr(), source, target);
};

void l_AttackTarget(cCharacterObject *source, cCharacterObject *target)
{
	hge->System_Log("Test: AttackTarget");
	call_function<void>(g_Game->GetLuaVM(),"AttackTarget",g_Game,g_Game->GetGameObjMangr(), source, target);
};

void l_MoveIn(cCharacterObject *source, cCharacterObject *target)
{
	hge->System_Log("Test: MoveIn");
	call_function<void>(g_Game->GetLuaVM(),"MoveIn",g_Game,g_Game->GetGameObjMangr(), source, target);
};

void l_Flee(cCharacterObject *source, cCharacterObject *target)
{
	hge->System_Log("Test: Flee");
	call_function<void>(g_Game->GetLuaVM(),"Flee",g_Game,g_Game->GetGameObjMangr(), source, target);
};

void l_MoveDone(cCharacterObject *source, cCharacterObject *target)
{
	hge->System_Log("Test: MoveDone");
	call_function<void>(g_Game->GetLuaVM(),"MoveDone",g_Game->GetGameObjMangr(),source,target);
};


void l_LoadSpell(cSpellObject *spell)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//LoadSpell( pGame, pObjMan, pSpell )
	call_function<void>(g_Game->GetLuaVM(),"LoadSpell",g_Game,g_Game->GetGameObjMangr(),spell);
};

void l_LoadProjectile(cSpellObject *spell, cProjectileObject *proj)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//LoadProjectile( pObjMan, pSpell, pProj)
	call_function<void>(g_Game->GetLuaVM(),"OnCreateProjectile",g_Game->GetGameObjMangr(), spell, proj);

};
void l_LoadEffect(cSpellObject *spell, cEffectObject *effect)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//LoadEffect( pObjMan, pSpell, pEffect)
	call_function<void>(g_Game->GetLuaVM(),"OnCreateEffect",g_Game->GetGameObjMangr(), spell, effect);
};

void l_BeginCast( cSpellObject *spell)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//OnBeginSpellCast( pGame, pObjMan, pSpell, pCaster, pTarget )
	call_function<void>(g_Game->GetLuaVM(),"OnBeginSpellCast",g_Game, g_Game->GetGameObjMangr(), spell, spell->GetCaster(), spell->GetTarget());
};

void l_SpellAction(cSpellObject *spell)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//OnSpellAction( pGame, pObjMan, pSpell, pCaster, pTarget )
	call_function<void>(g_Game->GetLuaVM(),"OnSpellAction",g_Game, g_Game->GetGameObjMangr(), spell, spell->GetCaster(), spell->GetTarget());
};

void l_EndCast( cSpellObject *spell)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//OnEndSpellCast( pGame, pObjMan, pSpell, pCaster )
	call_function<void>(g_Game->GetLuaVM(),"OnEndSpellCast",g_Game, g_Game->GetGameObjMangr(), spell, spell->GetCaster());
};

void l_SpellHit(cSpellObject *spell, cCharacterObject *target)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//OnSpellHit( pGame, pObjMan, pSpell, pTarget )
	call_function<void>(g_Game->GetLuaVM(),"OnSpellHit",g_Game, g_Game->GetGameObjMangr(), spell, target);
};

void l_SpellStruck(cSpellObject *spell)
{
	luaL_dofile(g_Game->GetLuaVM(), spell->GetScriptName().c_str());
	//OnSpellStruck( pGame, pObjMan, pSpell, pTarget )
	call_function<void>(g_Game->GetLuaVM(),"OnSpellStruck",g_Game, g_Game->GetGameObjMangr(), spell, spell->GetTarget());
};


//extern int render, flags[3000];
//extern int debug, fps, change_map;
//extern char cur_map[40];
//extern CWeather *weather;

//static const struct func_list f_list[NUM_FUNCS] = {
//	{ "addNPC", l_addNPC },
//	{ "deleteNPC", l_deleteNPC },
//	{ "textBox", l_textBox },
//	{ "moveNPC", l_moveNPC },
//	{ "getFlag", l_getFlag },
//	{ "setFlag", l_setFlag },
//	{ "playMusic", l_playMusic },
//	{ "killMusic", l_killMusic },
//	{ "makeSnow", l_makeSnow },
//	{ "makeRain", l_makeRain },
//	{ "killWeather", l_killWeather },
//	{ "setWeather", l_setWeather },
//	{ "setIntensity", l_setIntensity },
//	{ "setEntityId", l_setEntityId },
//	{ "destroyEntityId", l_destroyEntityId},
//	{ "promptBox", l_promptBox},
//	{ "setTile", l_setTile},
//	{ "setMoveMode", l_setMoveMode },
//	{ "setMoveScript", l_setMoveScript },
//	{ "fadeOut", l_fadeOut },
//	{ "fadeIn", l_fadeIn },
//	{ "renderMode", l_renderMode },
//	{ "moveCamera", l_moveCamera },
//	{ "setCameraFocus", l_setCameraFocus },
//	{ "playSound", l_playSound },
//	{ "waitEntities", l_waitEntities },
//	{ "changeMap", l_changeMap },
//	{ "loadMap", l_loadMap },
//	{ "setRandomText", l_setRandomText },
//	{ "turnRandomTextOn", l_turnRandomTextOn },
//	{ "turnRandomTextOff", l_turnRandomTextOff },
//	{ "setRandTextSpeed", l_setRandTextSpeed },
//	{ "setRandTextDisp", l_setRandTextDisp },
//	{ "addSpell", l_addSpell },
//	{ "addEffect", l_addEffect },
//	{ "setCurrentSpell", l_setCurrentSpell },
//	{ "castEffect", l_castEffect },
//
//};
//
//int l_setRandomText(lua_State *luaVM)
//{
//	//int id = lua_tonumber(luaVM, 1);
//	//char *msg[3];
//	//for (int a=0;a<3;a++)
//	//{
//	//	msg[a] = (char*)lua_tostring(luaVM, a+2);
//	//}
////	spritemanager->SetRandomText(id, msg[0], msg[1], msg[2]);
//	return 0;
//}
//
//int l_turnRandomTextOn(lua_State *luaVM)
//{
////	int id = lua_tonumber(luaVM, 1);
////	spritemanager->TurnRandomTextOn(id);
//	return 0;
//}
//
//int l_turnRandomTextOff(lua_State *luaVM)
//{
////	int id = lua_tonumber(luaVM, 1);
////	spritemanager->TurnRandomTextOff(id);
//	return 0;
//}
//
//int l_setRandTextSpeed(lua_State *luaVM)
//{
//	//int id, speed;
//	//id = lua_tonumber(luaVM, 1);
//	//speed = lua_tonumber(luaVM, 2);
////	spritemanager->SetRandTextSpeed(id, speed);
//	return 0;
//}
//
//int l_setRandTextDisp(lua_State *luaVM)
//{
////	int id, speed;
////	id = lua_tonumber(luaVM, 1);
////	speed = lua_tonumber(luaVM, 2);
////	spritemanager->SetRandTextDisp(id, speed);
//	return 0;
//}
//
//int l_changeMap(lua_State *luaVM)
//{
//	char *name = (char *)lua_tostring(luaVM, 1);
//	int a = lua_tonumber(luaVM,2);
//	g_Game->ChangeMap(name, a);
//	return 0;
//}
//
//int l_loadMap(lua_State *luaVM)
//{
//	char *name = (char *)lua_tostring(luaVM, 1);
//	hge->System_Log(name);
//	g_Game->GetGameObjMangr()->LoadMap(name,false);
//	return 0;
//}
//
//int l_setDir(lua_State *luaVM)
//{
//	//int a,b;
//	//a = lua_tonumber(luaVM,1);
//	//b = lua_tonumber(luaVM,2);
////	spritemanager->SetDir(a, (ISODIR)b);
//	return 0;
//}
//
//int l_setState(lua_State *luaVM)
//{
//	//int a,b;
//	//a = lua_tonumber(luaVM,1);
//	//b = lua_tonumber(luaVM,2);
////	spritemanager->SetDir(a, (ISODIR)b);
//	return 0;
//}
//
//int l_updateSprites(lua_State *luaVM)
//{
////	spritemanager->Update(0);
////	lua_pushnumber(luaVM, 0);
//	return 1;
//}
//
//int l_waitEntities(lua_State *luaVM)
//{
////	int a,b;
////	a = lua_tonumber(luaVM, 1);
////	b = lua_tonumber(luaVM, 2);
////	spritemanager->Wait(a,b);
//	return 0;
//}
//
//int l_playSound(lua_State *luaVM)
//{
////	char *msg = (char *)lua_tostring(luaVM, 1);
////	sound->Play(msg);
//	return 0;
//}
//
//int l_moveCamera(lua_State *luaVM)
//{
//	//float a,b,c;
//	//a = lua_tonumber(luaVM,1);
//	//b = lua_tonumber(luaVM,2);
//	//c = lua_tonumber(luaVM,3);
////	mainmap->MoveCamera(a,b,c);
//	return 0;
//}
//
//int l_setCameraFocus(lua_State *luaVM)
//{
//	//int a,b;
//	//a = lua_tonumber(luaVM, 1);
//	//b = lua_tonumber(luaVM, 2);
////	spritemanager->SetCameraFocus(a,b);
//////	if (b)
//////		mainmap->SnapToPlayer(a);
//	return 0;
//}
//
//// 0 - Rendering Off
//// 1 - Rendering On
//int l_renderMode(lua_State *luaVM)
//{
////	int a = lua_tonumber(luaVM,1);
////	render = a;
//	return 0;
//}
//int l_fadeOut(lua_State *luaVM)
//{
////	int a = lua_tonumber(luaVM,1);
////	highcolor_fade_out(a);
//	return 0;
//}
//
//int l_fadeIn(lua_State *luaVM)
//{
////	lua_pushnumber(luaVM, 0);
//	return 1;
//}
//
//int l_setMoveScript(lua_State *luaVM)
//{
//	//int a,b;
//	//char *msg;
//	//a = lua_tonumber(luaVM, 1);
//	//msg = (char *)lua_tostring(luaVM, 2);
//	//b = lua_tonumber(luaVM, 3);
////	spritemanager->SetMoveScript(a, msg, b);
//	return 0;
//}
//
//int l_setMoveMode(lua_State *luaVM)
//{
//	//int a,b, c;
//	//a = lua_tonumber(luaVM, 1);
//	//b = lua_tonumber(luaVM, 2);
//	//c = lua_tonumber(luaVM, 3);
////	spritemanager->SetMoveMode(a, b, c);
//	return 0;
//}
//
//int l_setTile(lua_State *luaVM)
//{
//	//int a, b, c, d;
//	//a = lua_tonumber(luaVM, 1);
//	//b = lua_tonumber(luaVM, 2);
//	//c = lua_tonumber(luaVM, 3);
//	//d = lua_tonumber(luaVM, 4);
////	mainmap->SetTile(a,b,c,d);
//	return 0;
//}
//
//int l_promptBox(lua_State *luaVM)
//{
//	//int a, posx, posy, ent_no;
//	//int lineCount = 0;
//	//char *msg[4];
//
//	//posx = lua_tonumber(luaVM, 1);
//	//posy = lua_tonumber(luaVM, 2);
//	//ent_no = lua_tonumber(luaVM, 3);
//
//	//for (a=0;a<4;a++){
//	//	msg[a] = (char *)lua_tostring(luaVM,a+4);
//	//	if(strlen(msg[a]) >= 2){
//	//		lineCount++;
//	//	}
//	//}
//	//
//	//lua_pushnumber(luaVM, a);
//	return 1;
//}
//
//int l_setEntityId(lua_State *luaVM){
////	int a = lua_tonumber(luaVM, 1);
////	spritemanager->SetEntId(a);
//	return 0;
//}
//
//int l_destroyEntityId(lua_State *luaVM){
////	spritemanager->DestroyEntId();
//	return 0;
//}
//
//
//int l_makeSnow(lua_State *luaVM)
//{
////	int a = lua_tonumber(luaVM, 1);
////	CSnow *snow;
////	snow = new CSnow();
////	weather = snow;
////	weather->MakeWeather();
////	weather->WeatherSwitch(1);
//	return 0;
//}
//
//int l_makeRain(lua_State *luaVM)
//{
////	CRain *rain;
////	rain = new CRain();
////	weather = rain;
////	weather->MakeWeather();
////	lua_pushnumber(luaVM, 0);
////	weather->WeatherSwitch(1);
//	return 1;
//}
//
//int l_killWeather(lua_State *luaVM)
//{
////	weather->WeatherSwitch(0);
//	return 0;
//}
//
//
//int l_setWeather(lua_State *luaVM)
//{
////	char *script = (char *)lua_tostring(luaVM, 1);
////	weather->SetWeatherScript(script);
//	return 0;
//}
//
//
//int l_setIntensity(lua_State *luaVM)
//{
////	int a = lua_tonumber(luaVM, 1);
////	weather->ChangeIntensity(a);
//	return 0;
//}
//
//
//int l_playMusic(lua_State *luaVM)
//{
////	char *msg = (char *)lua_tostring(luaVM, 1);
////	music->Load(msg);
//	return 0;
//}
//
//int l_killMusic(lua_State *luaVM)
//{
////	music->Kill();
////	lua_pushnumber(luaVM, 0);
//	return 1;
//}
//
//int l_addNPC(lua_State *luaVM)
//{
//	int dir = lua_tonumber(luaVM, 1);
//	int side = lua_tonumber(luaVM, 2);
//	int action = lua_tonumber(luaVM, 3);
//	int PosX = lua_tonumber(luaVM, 4);
//	int PosY = lua_tonumber(luaVM, 5);
//
//	char *msg[3];
//	msg[0] = (char *)lua_tostring(luaVM, 6);
//	msg[1] = (char *)lua_tostring(luaVM, 7);
//	msg[2] = (char *)lua_tostring(luaVM, 8);
//	msg[3] = (char *)lua_tostring(luaVM, 9);
//
//	g_Game->GetGameObjMangr()->AddNpc(dir, side, action, PosX, PosY, msg[0], msg[1], msg[2]);
//
//	return 0;
//}
//
//int l_deleteNPC(lua_State *luaVM)
//{
////	int id = lua_tonumber(luaVM, 1);
////	spritemanager->RemoveNpc(id);
//	return 0;
//}
//
//int l_addSpell(lua_State *luaVM)
//{
//	char *msg[2];
//	int id = lua_tonumber(luaVM, 1);
//	msg[0] = (char *)lua_tostring(luaVM, 2);
//	msg[1] = (char *)lua_tostring(luaVM, 3);
//
//	g_Game->GetGameObjMangr()->AddSpell(id, msg[0], msg[1]);
//
//	return 0;
//}
//
//int l_addEffect(lua_State *luaVM)
//{
//	char *msg[1];
//	msg[0] = (char *)lua_tostring(luaVM, 1);
//
//	g_Game->GetGameObjMangr()->AddEffect(msg[0]);
//
//	return 0;
//}
//
//int l_setCurrentSpell(lua_State *luaVM)
//{
////	int charID = lua_tonumber(luaVM, 1);
////	int spellID = lua_tonumber(luaVM, 2);
//
////	gameobjectmanager->SetCurrentSpell(charID, spellID);
//
//	return 0;
//}
//
//int l_castEffect(lua_State *luaVM)
//{
//	int effectID = lua_tonumber(luaVM, 1);
//	int charID = lua_tonumber(luaVM, 2);
//
//	//gameobjectmanager->CastEffect(effectID, charID);
//
//	return 0;
//}
//
//
//int l_textBox(lua_State *luaVM)
//{
//	//int input = 0, textinput = 0;
//	//int noLines = lua_tonumber(luaVM, 1);
//	//char *txt[16];
//	//for (int a=0;a<noLines;a++)
//	//	txt[a] = (char *)lua_tostring(luaVM, a+2);
//
////	textbox->setDimensions(textbox->longestString(txt, noLines), textbox->Height(), 310, 240);
////	textbox->textBox(txt, noLines, D3DCOLOR_ARGB(255,255,255,255));
//
////	while(textbox->activated){
////		input = 0;
////		textinput = 0;
////		dx->InputHandler(input, textinput);
////		spritemanager->Update(0);
////		textbox->getInput(textinput);
////		dx->Render();
////	}
//
//	return 0;
//}
//
//int l_moveNPC(lua_State *luaVM)
//{
//	//int a,b,c, d;
//	//a = lua_tonumber(luaVM, 1);
//	//b = lua_tonumber(luaVM, 2);
//	//c = lua_tonumber(luaVM, 3);
//	//d = lua_tonumber(luaVM, 4);
////	if (a == 0 && spritemanager->CameraFocus(0)){
//////		mainmap->SetCamera(b - 304,c - 207);
////		mainmap->ResetScroll(0);
////	}
////	spritemanager->MoveNpc(a,b,c, (ISODIR)d);
//	return 0;
//}
//
//int l_getFlag(lua_State *luaVM)
//{
////	int num = lua_tonumber(luaVM, 1);
////	if ((num >= 0) && (num <= 2999))
////		lua_pushnumber(luaVM, flags[num]);
//	return 1;
//}
//
//int l_setFlag(lua_State *luaVM)
//{
////	int num = lua_tonumber(luaVM, 1);
////	if ((num >= 0) && (num <= 2999))
////		flags[num] = lua_tonumber(luaVM, 2);
//	return 0;
//}

//void check_zones(lua_State *luaVM, int zone)
//{
//	int top = lua_gettop(luaVM);
//	//Call Zone Check from the script file... cool
//	lua_getglobal(luaVM, "zone_check");
//	//pass the script a pram
//	lua_pushnumber(luaVM, zone);
//	lua_call(luaVM, 1, 0);
//	lua_settop(luaVM, top);
//}
//
//void check_entities(lua_State *luaVM, int entity)
//{
//	int top = lua_gettop(luaVM);
//
//	lua_getglobal(luaVM, "entity_check");
//	lua_pushnumber(luaVM, entity);
//	lua_call(luaVM, 1, 0);
//	lua_settop(luaVM, top);
//}
//
//void cast_effect(lua_State *luaVM, int effectID, int charID)
//{
//	int top = lua_gettop(luaVM);
//	//Call Zone Check from the script file... cool
//	lua_getglobal(luaVM, "cast_effect");
//	//pass the script a pram
////	lua_pushnumber(luaVM, effectID);
//	lua_pushnumber(luaVM, charID);
//	lua_call(luaVM, 1, 0);
//	lua_settop(luaVM, top);
//}
