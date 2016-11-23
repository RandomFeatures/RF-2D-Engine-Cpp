class cApp;
class cCharacterObject;
class cSpellObject;
class CGameObject;
class cProjectileObject;
class cEffectObject;


struct func_list {
  const char *name;
  lua_CFunction func;
};

void l_bindClasses(lua_State *luaVM);
void l_LoadScriptFile(bool Autoexec, lua_State *luaVM, char *fname);
void l_Autoexec(lua_State *luaVM);
void l_Postexec(lua_State *luaVM);
void l_LoadSpell(cSpellObject *spell);
void l_LoadProjectile(cSpellObject *spell, cProjectileObject *proj);
void l_LoadEffect(cSpellObject *spell, cEffectObject *effect);
void l_BeginCast(cSpellObject *spell);
void l_SpellAction(cSpellObject *spell);
void l_EndCast(cSpellObject *spell);
void l_SpellHit(cSpellObject *spell, cCharacterObject *target);
void l_SpellStruck(cSpellObject *spell);
void l_SpellHit(cSpellObject *spell, cCharacterObject *target);

void l_StartTurn(cCharacterObject *source);
void l_AttackDone(cCharacterObject *source, cCharacterObject *target);
void l_AttackTarget(cCharacterObject *source, cCharacterObject *target);
void l_MoveIn(cCharacterObject *source, cCharacterObject *target);
void l_Flee(cCharacterObject *source, cCharacterObject *target);
void l_MoveDone(cCharacterObject *source, cCharacterObject *target);




//void check_zones(int zone);
//void check_entities(int entity);
//void AssignEffect(int effectID, int charID);

//int l_turnRandomTextOff(lua_State *luaVM);
//int l_turnRandomTextOn(lua_State *luaVM);
//int l_setRandomText(lua_State *luaVM);
//int l_setRandTextSpeed(lua_State *luaVM);
//int l_setRandTextDisp(lua_State *luaVM);
//int l_renderFrame(lua_State *luaVM);
//int l_changeMap(lua_State *luaVM);
//int l_loadMap(lua_State *luaVM);
//int l_setDir(lua_State *luaVM);
//int l_setState(lua_State *luaVM);
//int l_updateSprites(lua_State *luaVM);
//int l_waitEntities(lua_State *luaVM);
//int l_playSound(lua_State *luaVM);
//int l_moveCamera(lua_State *luaVM);
//int l_setCameraFocus(lua_State *luaVM);
//int l_renderMode(lua_State *luaVM);
//int l_fadeOut(lua_State *luaVM);
//int l_fadeIn(lua_State *luaVM);
//int l_setMoveScript(lua_State *luaVM);
//int l_setMoveMode(lua_State *luaVM);
//int l_setTile(lua_State *luaVM);
//int l_makeSnow(lua_State *luaVM);
//int l_makeRain(lua_State *luaVM);
//int l_killWeather(lua_State *luaVM);
//int l_setWeather(lua_State *luaVM);
//int l_setIntensity(lua_State *luaVM);
//int l_playMusic(lua_State *luaVM);
//int l_killMusic(lua_State *luaVM);
//int l_drawMap(lua_State *luaVM);
//int l_changeMap(lua_State *luaVM);
//int l_loadMap(lua_State *luaVM);
//int l_addNPC(lua_State *luaVM);
//int l_deleteNPC(lua_State *luaVM);
//int l_moveNPC(lua_State *luaVM);
//int l_textBox(lua_State *luaVM);
//int l_getFlag(lua_State *luaVM);
//int l_setFlag(lua_State *luaVM);
//int l_setEntityId(lua_State *luaVM);
//int l_destroyEntityId(lua_State *luaVM);
//int l_promptBox(lua_State *luaVM);
//int l_stockMap(lua_State *luaVM);
//int l_getFromMap(lua_State *luaVM);
//int l_getNumNpc(lua_State *luaVM);
//int l_addSpell(lua_State *luaVM);
//int l_addEffect(lua_State *luaVM);
//int l_setCurrentSpell(lua_State *luaVM);
//int l_castEffect(lua_State *luaVM);

