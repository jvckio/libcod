#include "gsc.hpp"
#include "libcod.hpp"

#include <stdint.h>
#include <sys/time.h>

const char *getParamTypeAsString(int type)
{
	switch ( type )
	{
		case 0: return "UNDEFINED";
		case 1: return "OBJECT";
		case 2: return "STRING";
		case 3: return "LOCALIZED_STRING";
		case 4: return "VECTOR";
		case 5: return "FLOAT";
		case 6: return "INT";
		case 7: return "CODEPOS";
		case 8: return "PRECODEPOS";
		case 9: return "FUNCTION";
		case 10: return "STACK";
		case 11: return "ANIMATION";
		case 12: return "DEVELOPER_CODEPOS";
		case 13: return "INCLUDE_CODEPOS";
		case 14: return "THREAD_LIST";
		case 15: return "THREAD_1";
		case 16: return "THREAD_2";
		case 17: return "THREAD_3";
		case 18: return "THREAD_4";
		case 19: return "STRUCT";
		case 20: return "REMOVED_ENTITY";
		case 21: return "ENTITY";
		case 22: return "ARRAY";
		case 23: return "REMOVED_THREAD";
		default: return "UNKNOWN TYPE";
	}
}

const char *stackGetParamTypeAsString(int param)
{
	if ( param >= Scr_GetNumParam() )
		return "UNDEFINED";

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type == 1 ) // pointer to object
		return getParamTypeAsString(Scr_GetPointerType(param));
	else
		return getParamTypeAsString(var->type);

}
void Scr_AddFunc(const char *value)
{
	IncInParam();
	scrVmPub.top->type = STACK_FUNCTION;
	scrVmPub.top->u.codePosValue = value;
}
void NULL_FUNC_ENTITY(scr_entref_t id) {}
void NULL_FUNC(void) {}

scr_function_t scriptFunctions[] =
{
	#if COD_VERSION == COD2_1_0
	{"endParty", NULL_FUNC, 0}, // cod2 1.2
	#endif

	#if COMPILE_BOTS == 1
	{"setNextTestClientName", gsc_bots_setnexttestclientname, 0},
	{"resetTestClientNaming", gsc_bots_resettestclientnaming, 0},
	#endif
		
	#if COMPILE_MYSQL_DEFAULT == 1
	{"mysql_init", gsc_mysql_init, 0},
	{"mysql_real_connect", gsc_mysql_real_connect, 0},
	{"mysql_close", gsc_mysql_close, 0},
	{"mysql_query", gsc_mysql_query, 0},
	{"mysql_errno", gsc_mysql_errno, 0},
	{"mysql_error", gsc_mysql_error, 0},
	{"mysql_affected_rows", gsc_mysql_affected_rows, 0},
	{"mysql_store_result", gsc_mysql_store_result, 0},
	{"mysql_num_rows", gsc_mysql_num_rows, 0},
	{"mysql_num_fields", gsc_mysql_num_fields, 0},
	{"mysql_field_seek", gsc_mysql_field_seek, 0},
	{"mysql_fetch_field", gsc_mysql_fetch_field, 0},
	{"mysql_fetch_row", gsc_mysql_fetch_row, 0},
	{"mysql_free_result", gsc_mysql_free_result, 0},
	{"mysql_real_escape_string", gsc_mysql_real_escape_string, 0},
	{"mysql_async_create_query", gsc_mysql_async_create_query, 0},
	{"mysql_async_create_query_nosave", gsc_mysql_async_create_query_nosave, 0},
	{"mysql_async_getdone_list", gsc_mysql_async_getdone_list, 0},
	{"mysql_async_getresult_and_free", gsc_mysql_async_getresult_and_free, 0},
	{"mysql_async_initializer", gsc_mysql_async_initializer, 0},
	{"mysql_reuse_connection", gsc_mysql_reuse_connection, 0},
	#endif

	#if COMPILE_MYSQL_VORON == 1
	{"mysql_initialize", gsc_mysql_initialize, 0},
	{"mysql_close", gsc_mysql_close, 0},
	{"mysql_query", gsc_mysql_query, 0},
	{"mysql_errno", gsc_mysql_errno, 0},
	{"mysql_error", gsc_mysql_error, 0},
	{"mysql_affected_rows", gsc_mysql_affected_rows, 0},
	{"mysql_store_result", gsc_mysql_store_result, 0},
	{"mysql_num_rows", gsc_mysql_num_rows, 0},
	{"mysql_num_fields", gsc_mysql_num_fields, 0},
	{"mysql_field_seek", gsc_mysql_field_seek, 0},
	{"mysql_fetch_field", gsc_mysql_fetch_field, 0},
	{"mysql_fetch_row", gsc_mysql_fetch_row, 0},
	{"mysql_free_result", gsc_mysql_free_result, 0},
	{"mysql_real_escape_string", gsc_mysql_real_escape_string, 0},

	{"async_mysql_initialize", gsc_async_mysql_initialize, 0},
	{"async_mysql_close", gsc_async_mysql_close, 0},
	{"async_mysql_create_query", gsc_async_mysql_create_query, 0},
	{"async_mysql_create_query_nosave", gsc_async_mysql_create_query_nosave, 0},
	{"async_mysql_checkdone", gsc_async_mysql_checkdone, 0},
	{"async_mysql_errno", gsc_async_mysql_errno, 0},
	{"async_mysql_error", gsc_async_mysql_error, 0},
	{"async_mysql_affected_rows", gsc_async_mysql_affected_rows, 0},
	{"async_mysql_num_rows", gsc_async_mysql_num_rows, 0},
	{"async_mysql_num_fields", gsc_async_mysql_num_fields, 0},
	{"async_mysql_field_seek", gsc_async_mysql_field_seek, 0},
	{"async_mysql_fetch_field", gsc_async_mysql_fetch_field, 0},
	{"async_mysql_fetch_row", gsc_async_mysql_fetch_row, 0},
	{"async_mysql_free_task", gsc_async_mysql_free_task, 0},
	{"async_mysql_real_escape_string", gsc_async_mysql_real_escape_string, 0},
	#endif

	#if COMPILE_EXEC == 1
	{"execute", gsc_exec, 0},
	{"execute_async_create", gsc_exec_async_create, 0},
	{"execute_async_create_nosave", gsc_exec_async_create_nosave, 0},
	{"execute_async_checkdone", gsc_exec_async_checkdone, 0},
	#endif

	#if COMPILE_LEVEL == 1
	{"getNumberOfStaticModels", gsc_level_getnumberofstaticmodels, 0},
	{"getStaticModelName", gsc_level_getstaticmodelname, 0},
	{"getStaticModelOrigin", gsc_level_getstaticmodelorigin, 0},
	{"getEntityCount", gsc_level_getentitycount, 0},
	#endif

	#if COMPILE_MEMORY == 1
	{"memory_malloc", gsc_memory_malloc, 0},
	{"memory_free", gsc_memory_free, 0},
	{"memory_int_get", gsc_memory_int_get, 0},
	{"memory_int_set", gsc_memory_int_set, 0},
	{"memory_memset", gsc_memory_memset, 0},
	{"binarybuffer_new", gsc_binarybuffer_new, 0},
	{"binarybuffer_free", gsc_binarybuffer_free, 0},
	{"binarybuffer_seek", gsc_binarybuffer_seek, 0},
	{"binarybuffer_write", gsc_binarybuffer_write, 0},
	{"binarybuffer_read", gsc_binarybuffer_read, 0},
	#endif

	#if COMPILE_SOCKET == 1
	{"socketInit", gsc_socket_init, 0},
	{"socketBind", gsc_socket_bind, 0},
	{"socketConnect", gsc_socket_connect, 0},
	{"socketSend", gsc_socket_send, 0},
	{"socketReceive", gsc_socket_receive, 0},
	{"socketClose", gsc_socket_close, 0},
	#endif

	#if COMPILE_UTILS == 1
	{"getSoundaliasesFromFile", gsc_utils_getsoundaliasesfromfile, 0},
	{"getSoundDuration", gsc_utils_getsoundduration, 0},
	{"getSoundInfo", gsc_utils_getsoundinfo, 0},
	{"kick", gsc_utils_kick, 0},
	{"processRemoteCommand", gsc_utils_processremotecommand, 0},
	{"executeCommand", gsc_utils_executecommand, 0},
	{"sendCommandToClient", gsc_utils_sendcommandtoclient, 0},
	{"sendPacket", gsc_utils_sendpacket, 0},
	#if ENABLE_UNSAFE == 1
	{"system", gsc_utils_system, 0},
	#endif

	{"printf", gsc_utils_printf, 0},
	{"sprintf", gsc_utils_sprintf, 0},
	{"putChar", gsc_utils_putchar, 0},
	{"logPrintConsole", gsc_utils_logprintconsole, 0},
	{"getArrayKeys", gsc_utils_getarraykeys, 0},
	{"getAscii", gsc_utils_getascii, 0},
	{"toUpper", gsc_utils_toupper, 0},

	#if ENABLE_UNSAFE == 1
	{"file_link", gsc_utils_file_link, 0},
	{"file_unlink", gsc_utils_file_unlink, 0},
	{"file_exists", gsc_utils_file_exists, 0},
	{"loaddir", gsc_utils_loaddir, 0},
	{"scandir", gsc_utils_scandir, 0},
	{"fopen", gsc_utils_fopen, 0},
	{"fread", gsc_utils_fread, 0},
	{"fwrite", gsc_utils_fwrite, 0},
	{"fclose", gsc_utils_fclose, 0},
	{"fsize", gsc_utils_fsize, 0},
	{"fremove", gsc_utils_fremove, 0},
	#endif

	{"getSystemTime", gsc_utils_getsystemtime, 0},
	{"getServerStartTime", gsc_utils_getserverstarttime, 0},
	{"getLocalTime", gsc_utils_getlocaltime, 0},

	{"findConfigStringIndexOriginal", gsc_utils_findconfigstringindexoriginal, 0},
	{"findConfigStringIndex", gsc_utils_findconfigstringindex, 0},
	{"getConfigString", gsc_utils_getconfigstring, 0},
	{"setConfigString", gsc_utils_setconfigstring, 0},
	{"makeLocalizedString", gsc_utils_makelocalizedstring, 0},
	{"makeClientLocalizedString", gsc_utils_makeclientlocalizedstring, 0},
	{"makeString", gsc_utils_makestring, 0},

	{"float", gsc_utils_float, 0},
	{"exponent", gsc_utils_exponent, 0},
	{"round", gsc_utils_round, 0},
	{"sqrt", gsc_utils_sqrt, 0},
	{"sqrtInv", gsc_utils_sqrtinv, 0},
	{"vectorScale", gsc_utils_vectorscale, 0},

	{"chr", gsc_utils_chr, 0},
	{"ord", gsc_utils_ord, 0},
	{"fromHex", gsc_utils_fromhex, 0},
	{"toHex", gsc_utils_tohex, 0},

	{"getLastTestClientNumber", gsc_utils_getlasttestclientnumber, 0},
	{"bulletHitEffect", gsc_utils_bullethiteffect, 0},
	{"getType", gsc_utils_gettype, 0},

	{"error", gsc_utils_error, 0},
	#endif

	#if COMPILE_CUSTOM_VOICE == 1
	{"getSoundFileDuration", gsc_utils_getsoundfileduration, 0},
	{"loadSoundFile", gsc_utils_loadsoundfile, 0},
	{"loadSpeexFile", gsc_utils_loadspeexfile, 0},
	{"saveSpeexFile", gsc_utils_savespeexfile, 0},
	#endif

	#if COMPILE_WEAPONS == 1
	{"getWeaponMaxAmmo", gsc_weapons_getweaponmaxammo, 0},
	{"setWeaponMaxAmmo", gsc_weapons_setweaponmaxammo, 0},
	{"getWeaponClipSize", gsc_weapons_getweaponclipsize, 0},
	{"setWeaponClipSize", gsc_weapons_setweaponclipsize, 0},
	{"getWeaponDamage", gsc_weapons_getweapondamage, 0},
	{"setWeaponDamage", gsc_weapons_setweapondamage, 0},
	{"getWeaponMeleeDamage", gsc_weapons_getweaponmeleedamage, 0},
	{"setWeaponMeleeDamage", gsc_weapons_setweaponmeleedamage, 0},
	{"getWeaponFireTime", gsc_weapons_getweaponfiretime, 0},
	{"setWeaponFireTime", gsc_weapons_setweaponfiretime, 0},
	{"getWeaponMeleeTime", gsc_weapons_getweaponmeleetime, 0},
	{"setWeaponMeleeTime", gsc_weapons_setweaponmeleetime, 0},
	{"getWeaponReloadTime", gsc_weapons_getweaponreloadtime, 0},
	{"setWeaponReloadTime", gsc_weapons_setweaponreloadtime, 0},
	{"getWeaponReloadEmptyTime", gsc_weapons_getweaponreloademptytime, 0},
	{"setWeaponReloadEmptyTime", gsc_weapons_setweaponreloademptytime, 0},
	{"getWeaponCookable", gsc_weapons_getweaponcookable, 0},
	{"setWeaponCookable", gsc_weapons_setweaponcookable, 0},
	{"getWeaponFuseTime", gsc_weapons_getweaponfusetime, 0},
	{"setWeaponFuseTime", gsc_weapons_setweaponfusetime, 0},
	{"getWeaponHitLocMultiplier", gsc_weapons_getweaponhitlocmultiplier, 0},
	{"setWeaponHitLocMultiplier", gsc_weapons_setweaponhitlocmultiplier, 0},
	{"getWeaponMoveSpeedScale", gsc_weapons_getmovespeedscale, 0},
	{"setWeaponMoveSpeedScale", gsc_weapons_setmovespeedscale, 0},
	{"getLoadedweapons", gsc_weapons_getloadedweapons, 0},
	{"getWeaponDisplayName", gsc_weapons_getweapondisplayname, 0},
	{"resetIgnoredWeapons", gsc_weapons_resetignoredweapons, 0},
	{"ignoreWeapon", gsc_weapons_ignoreweapon, 0},
	{"setDefaultWeapon", gsc_weapons_setdefaultweapon, 0},
	#endif

	#ifdef EXTRA_FUNCTIONS_INC
	#include "extra/functions.hpp"
	#endif

	{NULL, NULL, 0} // Terminator
};

xfunction_t Scr_GetCustomFunction(const char **fname, int *fdev)
{
	xfunction_t m = Scr_GetFunction(fname, fdev);
	if ( m )
		return m;

	for ( int i = 0; scriptFunctions[i].name; i++ )
	{
		if ( strcasecmp(*fname, scriptFunctions[i].name) )
			continue;

		scr_function_t func = scriptFunctions[i];
		*fname = func.name;
		*fdev = func.developer;
		return func.call;
	}

	return NULL;
}

scr_method_t scriptMethods[] =
{
	#if COMPILE_BOTS == 1
	{"setWalkValues", gsc_bots_setwalkvalues, 0},
	{"setWalkDir", gsc_bots_setwalkdir, 0},
	{"setLean", gsc_bots_setlean, 0},
	{"setBotStance", gsc_bots_setbotstance, 0},
	{"throwGrenade", gsc_bots_throwgrenade, 0},
	{"fireWeapon", gsc_bots_fireweapon, 0},
	{"meleeWeapon", gsc_bots_meleeweapon, 0},
	{"reloadWeapon", gsc_bots_reloadweapon, 0},
	{"setAim", gsc_bots_setaim, 0},
	{"switchToWeaponId", gsc_bots_switchtoweaponid, 0},
	#endif

	#if COMPILE_ENTITY == 1
	{"addEntityVelocity", gsc_entity_addentityvelocity, 0},
	{"disableBounce", gsc_entity_disablebounce, 0},
	{"disableGravity", gsc_entity_disablegravity, 0},
	{"enableBounce", gsc_entity_enablebounce, 0},
	{"enableGravity", gsc_entity_enablegravity, 0},
	{"getClipmask", gsc_entity_getclipmask, 0},
	{"getContents", gsc_entity_getcontents, 0},
	{"getEntityVelocity", gsc_entity_getentityvelocity, 0},
	{"getTagAngles", gsc_entity_gettagangles, 0},
	{"getTagOrigin", gsc_entity_gettagorigin, 0},
	{"getTurretOwner", gsc_entity_getturretowner, 0},
	{"getVmax", gsc_entity_getvmax, 0},
	{"getVmin", gsc_entity_getvmin, 0},
	{"hasTag", gsc_entity_hastag, 0},
	{"isLinkedTo", gsc_entity_islinkedto, 0},
	{"setAlive", gsc_entity_setalive, 0},
	{"setBounds", gsc_entity_setbounds, 0},
	{"setClipmask", gsc_entity_setclipmask, 0},
	{"setEntityVelocity", gsc_entity_setentityvelocity, 0},
	{"setLight", gsc_entity_setlight, 0},
	#endif

	#if COMPILE_MYSQL_VORON == 1
	{"async_mysql_create_entity_query", gsc_async_mysql_create_entity_query, 0},
	{"async_mysql_create_entity_query_nosave", gsc_async_mysql_create_entity_query_nosave, 0},
	#endif

	#if COMPILE_PLAYER == 1
	{"getStance", gsc_player_getstance, 0},
	{"setStance", gsc_player_setstance, 0},
	{"setVelocity", gsc_player_setvelocity, 0},
	{"addVelocity", gsc_player_addvelocity, 0},
	{"getVelocity", gsc_player_getvelocity, 0},
	{"aimButtonPressed", gsc_player_button_ads, 0},
	{"leftButtonPressed", gsc_player_button_left, 0},
	{"rightButtonPressed", gsc_player_button_right, 0},
	{"forwardButtonPressed", gsc_player_button_forward, 0},
	{"backButtonPressed", gsc_player_button_back, 0},
	{"leanleftButtonPressed", gsc_player_button_leanleft, 0},
	{"leanrightButtonPressed", gsc_player_button_leanright, 0},
	{"jumpButtonPressed", gsc_player_button_jump, 0},
	{"reloadButtonPressed", gsc_player_button_reload, 0},
	{"fragButtonPressed", gsc_player_button_frag, 0},
	{"smokeButtonPressed", gsc_player_button_smoke, 0},
	{"getIP", gsc_player_getip, 0},
	{"getPing", gsc_player_getping, 0},
	{"setPing", gsc_player_setping, 0},
	{"getSpectatorClient", gsc_player_getspectatorclient, 0},
	{"getLastConnectTime", gsc_player_getlastconnecttime, 0},
	{"getLastMSG", gsc_player_getlastmsg, 0},
	{"getAddressType", gsc_player_addresstype, 0},
	{"getClientConnectState", gsc_player_getclientconnectstate, 0},
	{"renameClient", gsc_player_renameclient, 0},
	{"getUserinfo", gsc_player_getuserinfo, 0},
	{"setUserinfo", gsc_player_setuserinfo, 0},
	{"disableItemPickup", gsc_player_disableitempickup, 0},
	{"enableItemPickup", gsc_player_enableitempickup, 0},
	{"connectionlessPacketToClient", gsc_player_connectionlesspackettoclient, 0},
	{"connectionlessPacketToServer", gsc_player_connectionlesspackettoserver, 0},
	{"processClientCommand", gsc_player_processclientcommand, 0},
	{"processClientUserinfoChange", gsc_player_processclientuserinfochange, 0},
	{"resetNextReliableTime", gsc_player_resetnextreliabletime, 0},
	{"setSpeed", gsc_player_setspeed, 0},
	{"setGravity", gsc_player_setgravity, 0},
	{"getSpeed", gsc_player_getspeed, 0},
	{"getGravity", gsc_player_getgravity, 0},
	{"setBulletMask", gsc_player_setbulletmask, 0},
	{"setFireThroughWalls", gsc_player_setfirethroughwalls, 0},
	{"setFireRangeScale", gsc_player_setfirerangescale, 0},
	{"setWeaponFireMeleeDelay", gsc_player_setweaponfiremeleedelay, 0},
	{"setMeleeHeightScale", gsc_player_setmeleeheightscale, 0},
	{"setMeleeRangeScale", gsc_player_setmeleerangescale, 0},
	{"setTurretSpreadScale", gsc_player_setturretspreadscale, 0},
	{"setWeaponSpreadScale", gsc_player_setweaponspreadscale, 0},
	{"setMeleeWidthScale", gsc_player_setmeleewidthscale, 0},
	{"setAnimation", gsc_player_setanimation, 0},
	{"playScriptAnimation", gsc_player_playscriptanimation, 0},
	{"getJumpSlowdownTimer", gsc_player_getjumpslowdowntimer, 0},
	{"clearJumpState", gsc_player_clearjumpstate, 0},
	{"getCookTime", gsc_player_getcooktime, 0},
	{"setGuid", gsc_player_setguid, 0},
	{"clientHasClientMuted", gsc_player_clienthasclientmuted, 0},
	{"muteClient", gsc_player_muteclient, 0},
	{"unmuteClient", gsc_player_unmuteclient, 0},
	{"getLastGamestate", gsc_player_getlastgamestatesize, 0},
	{"getFPS", gsc_player_getfps, 0},
	{"isMantling", gsc_player_ismantling, 0},
	{"isOnLadder", gsc_player_isonladder, 0},
	{"isUsingTurret", gsc_player_isusingturret, 0},
	{"lookAtKiller", gsc_player_lookatkiller, 0},
	{"isBot", gsc_player_isbot, 0},
	{"getCurrentOffhandSlotAmmo", gsc_player_getcurrentoffhandslotammo, 0},
	{"getWeaponIndexOffhand", gsc_player_getweaponindexoffhand, 0},
	{"noClip", gsc_player_noclip, 0},
	{"getInactivityTime", gsc_player_getinactivitytime, 0},
	{"disableEarthquakes", gsc_player_disableearthquakes, 0},
	{"enableEarthquakes", gsc_player_enableearthquakes, 0},
	{"earthquakeForPlayer", gsc_player_earthquakeforplayer, 0},
	{"playFxForPlayer", gsc_player_playfxforplayer, 0},
	{"playFxOnTagForPlayer", gsc_player_playfxontagforplayer, 0},
	{"getClientHudElemCount", gsc_player_getclienthudelemcount, 0},
	{"disableSilent", gsc_player_disablesilent, 0},
	{"enableSilent", gsc_player_enablesilent, 0},
	{"getGroundEntity", gsc_player_getgroundentity, 0},
	{"getEnterTime", gsc_player_getentertime, 0},
	{"stopUseTurret", gsc_player_stopuseturret, 0},
	{"getPlayerstateFlags", gsc_player_getplayerstateflags, 0},
	{"objective_player_add", gsc_player_objective_player_add, 0},
	{"objective_player_delete", gsc_player_objective_player_delete, 0},
	{"objective_player_icon", gsc_player_objective_player_icon, 0},
	{"objective_player_position", gsc_player_objective_player_position, 0},
	{"objective_player_state", gsc_player_objective_player_state, 0},
	{"getViewOrigin", gsc_player_getvieworigin, 0},
	{"getServerCommandQueueSize", gsc_player_getservercommandqueuesize, 0},
	{"setConfigstring", gsc_player_setconfigstring, 0},
	{"getCollisionTeam", gsc_player_getcollisionteam, 0},
	{"setCollisionTeam", gsc_player_setcollisionteam, 0},
	{"disableBulletImpacts", gsc_player_disablebulletimpacts, 0},
	{"enableBulletImpacts", gsc_player_enablebulletimpacts, 0},
	{"disableTalkerIcon", gsc_player_disabletalkericon, 0},
	{"enableTalkerIcon", gsc_player_enabletalkericon, 0},
	#if COMPILE_CUSTOM_VOICE == 1
	{"playSoundFile", gsc_player_playsoundfile, 0},
	{"stopSoundFile", gsc_player_stopsoundfile, 0},
	{"getRemainingSoundFileDuration", gsc_player_getremainingsoundfileduration, 0},
	{"isPlayingSoundFile", gsc_player_isplayingsoundfile, 0},
	#endif
	#if COMPILE_JUMP == 1
	{"setJumpHeight", gsc_player_setjumpheight, 0},
	{"setJumpSlowdownEnable", gsc_player_setjumpslowdownenable, 0},
	#endif
	#endif

	#if COMPILE_WEAPONS == 1
	{"addGrenadeFuseTime", gsc_weapons_addgrenadefusetime, 0},
	{"getWeaponItemAmmo", gsc_weapons_getweaponitemammo, 0},
	{"setWeaponItemAmmo", gsc_weapons_setweaponitemammo, 0},
	{"spawnGrenade", gsc_weapons_spawngrenade, 0},
	#endif

	#ifdef EXTRA_METHODS_INC
	#include "extra/methods.hpp"
	#endif

	{NULL, NULL, 0} // Terminator
};

xmethod_t Scr_GetCustomMethod(const char **fname, qboolean *fdev)
{
	xmethod_t m = Scr_GetMethod(fname, fdev);

	if ( m )
		return m;

	for ( int i = 0; scriptMethods[i].name; i++ )
	{
		if ( strcasecmp(*fname, scriptMethods[i].name) )
			continue;

		scr_method_t func = scriptMethods[i];

		*fname = func.name;
		*fdev = func.developer;

		return func.call;
	}

	return NULL;
}

int stackGetParamType(int param)
{
	if ( param >= Scr_GetNumParam() )
		return STACK_UNDEFINED;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	return var->type;
}

void stackError(const char *format, ...)
{
	char s[MAX_STRINGLENGTH];
	int len = 0;
	va_list va;

	va_start(va, format);
	Q_vsnprintf(s, sizeof(s) - 1, format, va);
	va_end(va);

	len = strlen(s);
	s[len] = '\n';
	s[len + 1] = '\0';
	Com_PrintMessage(0, s);
	Scr_CodeCallback_Error(qfalse, qfalse, "stackError", s);
}

int stackGetParams(const char *params, ...)
{
	va_list args;
	va_start(args, params);

	int errors = 0;

	for ( size_t i = 0; i < strlen(params); i++ )
	{
		switch ( params[i] )
		{
		case ' ': // Ignore param
			break;

		case 'i':
		{
			int *tmp = va_arg(args, int *);
			if ( !stackGetParamInt(i, tmp) )
			{
				Com_DPrintf("\nstackGetParams() Param %i is not an int\n", i);
				errors++;
			}
			break;
		}

		case 'v':
		{
			float *tmp = va_arg(args, float *);
			if ( !stackGetParamVector(i, tmp) )
			{
				Com_DPrintf("\nstackGetParams() Param %i is not a vector\n", i);
				errors++;
			}
			break;
		}

		case 'f':
		{
			float *tmp = va_arg(args, float *);
			if ( ! stackGetParamFloat(i, tmp) )
			{
				Com_DPrintf("\nstackGetParams() Param %i is not a float\n", i);
				errors++;
			}
			break;
		}

		case 's':
		{
			char **tmp = va_arg(args, char **);
			if ( !stackGetParamString(i, tmp) )
			{
				Com_DPrintf("\nstackGetParams() Param %i is not a string\n", i);
				errors++;
			}
			break;
		}

		case 'c':
		{
			unsigned int *tmp = va_arg(args, unsigned int *);
			if ( !stackGetParamConstString(i, tmp) )
			{
				Com_DPrintf("\nstackGetParams() Param %i is not a const string\n", i);
				errors++;
			}
			break;
		}

		case 'l':
		{
			char **tmp = va_arg(args, char **);
			if ( !stackGetParamLocalizedString(i, tmp) )
			{
				Com_DPrintf("\nstackGetParams() Param %i is not a localized string\n", i);
				errors++;
			}
			break;
		}

		default:
			errors++;
			Com_DPrintf("\nUnknown identifier [%c] passed to stackGetParams()\n", params[i]);
			break;
		}
	}

	va_end(args);
	return errors == 0; // success if no errors
}

int stackGetParamInt(int param, int *value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type == STACK_FLOAT )
	{
		*value = var->u.floatValue;
		return 1;
	}

	if ( var->type != STACK_INT )
		return 0;

	*value = var->u.intValue;

	return 1;
}

int stackGetParamFunction(int param, int *value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type != STACK_FUNCTION )
		return 0;

	*value = var->u.codePosValue - scrVarPub.programBuffer;

	return 1;
}

int stackGetParamString(int param, char **value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type != STACK_STRING )
		return 0;

	*value = SL_ConvertToString(var->u.stringValue);

	return 1;
}

int stackGetParamConstString(int param, unsigned int *value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type != STACK_STRING )
		return 0;

	*value = var->u.stringValue;

	return 1;
}

int stackGetParamLocalizedString(int param, char **value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type != STACK_LOCALIZED_STRING )
		return 0;

	*value = SL_ConvertToString(var->u.stringValue);

	return 1;
}

int stackGetParamVector(int param, vec3_t value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type != STACK_VECTOR )
		return 0;

	VectorCopy(var->u.vectorValue, value);

	return 1;
}

int stackGetParamFloat(int param, float *value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type == STACK_INT )
	{
		*value = var->u.intValue;
		return 1;
	}

	if ( var->type != STACK_FLOAT )
		return 0;

	*value = var->u.floatValue;

	return 1;
}

int stackGetParamObject(int param, unsigned int *value)
{
	if ( param >= Scr_GetNumParam() )
		return 0;

	VariableValue *var;
	var = &scrVmPub.top[-param];

	if ( var->type != STACK_OBJECT )
		return 0;

	*value = var->u.pointerValue;

	return 1;
}

qboolean IsNullVector(float *vec)
{
	return vec[0] == 0.0 && vec[1] == 0.0 && vec[2] == 0.0;
}

/**
 * @brief Base time in seconds
 */
time_t sys_timeBase = 0;

/**
 * @brief Current time in ms, using sys_timeBase as origin
 */
uint64_t Sys_MilliSeconds64(void)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);

	if ( !sys_timeBase )
	{
		sys_timeBase = tp.tv_sec;
		return tp.tv_usec / 1000;
	}

	return (tp.tv_sec - sys_timeBase) * 1000 + tp.tv_usec / 1000;
}