#include "gsc_player.hpp"

#if COMPILE_CUSTOM_VOICE == 1
#include <speex/speex.h>
#endif

#if COMPILE_PLAYER == 1

extern customPlayerState_t customPlayerState[MAX_CLIENTS];
extern customStringIndex_t custom_scr_const;

void gsc_player_getweaponindexoffhand(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getweaponindexoffhand() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}
	
	playerState_t *ps = SV_GameClientNum(id);

	stackPushInt(ps->weapon);
}

void gsc_player_getcurrentoffhandslotammo(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getcurrentoffhandslotammo() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);

	stackPushInt(ps->ammoclip[ps->offHandIndex - 1]);
}

void gsc_player_disableitempickup(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_disableitempickup() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int old_setting = !customPlayerState[id].noPickup;
	customPlayerState[id].noPickup = qtrue;

	stackPushInt(old_setting);
}

void gsc_player_enableitempickup(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_enableitempickup() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int old_setting = !customPlayerState[id].noPickup;
	customPlayerState[id].noPickup = qfalse;

	stackPushInt(old_setting);
}

void gsc_player_isbot(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_isbot() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->bot);
}

void gsc_player_lookatkiller(scr_entref_t ref)
{
	int id = ref.entnum;
	int inflictor, attacker;

	if ( !stackGetParams("ii", &inflictor, &attacker) )
	{
		stackError("gsc_player_lookatkiller() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *self_entity = G_GetEntity(id);
	if ( !self_entity )
	{
		stackError("gsc_player_lookatkiller() self_entity state is invalid");
		stackPushUndefined();
		return;
	}

	gentity_t *inflictor_entity = G_GetEntity(inflictor);
	if ( !inflictor_entity )
	{
		stackError("gsc_player_lookatkiller() inflictor_entity state is invalid");
		stackPushUndefined();
		return;
	}

	gentity_t *attacker_entity = G_GetEntity(attacker);
	if ( !attacker_entity )
	{
		stackError("gsc_player_lookatkiller() attacker_entity state is invalid");
		stackPushUndefined();
		return;
	}

	LookAtKiller(self_entity, inflictor_entity, attacker_entity);

	stackPushBool(qtrue);
}

void gsc_player_setvelocity(scr_entref_t ref)
{
	int id = ref.entnum;
	vec3_t velocity;

	if ( !stackGetParams("v", &velocity) )
	{
		stackError("gsc_player_setvelocity() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setvelocity() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);
	VectorCopy(velocity, ps->velocity);

	stackPushBool(qtrue);
}

void gsc_player_addvelocity(scr_entref_t ref)
{
	int id = ref.entnum;
	vec3_t velocity;

	if ( !stackGetParams("v", &velocity) )
	{
		stackError("gsc_player_addvelocity() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_addvelocity() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);
	VectorAdd(ps->velocity, velocity, ps->velocity);

	stackPushBool(qtrue);
}

void gsc_player_getvelocity(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getvelocity() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);

	stackPushVector(ps->velocity);
}

void gsc_player_processclientuserinfochange(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_processclientuserinfochange() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}
	
	ClientUserinfoChanged(id);

	stackPushBool(qtrue);
}

void gsc_player_getuserinfo(scr_entref_t ref)
{
	int id = ref.entnum;
	char *key;

	if ( !stackGetParams("s", &key) )
	{
		stackError("gsc_player_getuserinfo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getuserinfo() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	char *val = Info_ValueForKey(client->userinfo, key);

	if ( strlen(val) )
		stackPushString(val);
	else
		stackPushString("");
}

void gsc_player_setuserinfo(scr_entref_t ref)
{
	int id = ref.entnum;
	char *key, *value;

	if ( !stackGetParams("ss", &key, &value) )
	{
		stackError("gsc_player_setuserinfo() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setuserinfo() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	Info_SetValueForKey(client->userinfo, key, value);

	stackPushBool(qtrue);
}

void gsc_player_button_ads(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_ads() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.buttons & KEY_MASK_ADS_MODE ? qtrue : qfalse);
}

void gsc_player_button_left(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_left() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.rightmove == KEY_MASK_MOVELEFT ? qtrue : qfalse);
}

void gsc_player_button_right(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_right() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.rightmove == KEY_MASK_MOVERIGHT ? qtrue : qfalse);
}

void gsc_player_button_forward(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_forward() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.forwardmove == KEY_MASK_FORWARD ? qtrue : qfalse);
}

void gsc_player_button_back(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_back() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.forwardmove == KEY_MASK_BACK ? qtrue : qfalse);
}

void gsc_player_button_leanleft(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_leanleft() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.buttons & KEY_MASK_LEANLEFT ? qtrue : qfalse);
}

void gsc_player_button_leanright(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_leanright() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.buttons & KEY_MASK_LEANRIGHT ? qtrue : qfalse);
}

void gsc_player_button_reload(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_reload() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.buttons & KEY_MASK_RELOAD ? qtrue : qfalse);
}

void gsc_player_button_jump(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_jump() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.buttons & KEY_MASK_JUMP ? qtrue : qfalse);
}

void gsc_player_button_frag(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_frag() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.buttons & KEY_MASK_FRAG ? qtrue : qfalse);
}

void gsc_player_button_smoke(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_button_smoke() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushBool(client->lastUsercmd.buttons & KEY_MASK_SMOKE ? qtrue : qfalse);
}

void gsc_player_getstance(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getstance() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);
	if ( ps->pm_flags & PMF_CROUCH )
		stackPushString("crouch");
	else if ( ps->pm_flags & PMF_PRONE )
		stackPushString("prone");
	else
		stackPushString("stand");
}

void gsc_player_setstance(scr_entref_t ref)
{
	int id = ref.entnum;
	char *stance;

	if ( !stackGetParams("s", &stance) )
	{
		stackError("gsc_player_setstance() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id];

	if ( entity->client == NULL )
	{
		stackError("gsc_player_setstance() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int event;

	if ( strcmp(stance, "stand") == 0 )
		event = EV_STANCE_FORCE_STAND;
	else if ( strcmp(stance, "crouch") == 0 )
		event = EV_STANCE_FORCE_CROUCH;
	else if ( strcmp(stance, "prone") == 0 )
		event = EV_STANCE_FORCE_PRONE;
	else
	{
		stackError("gsc_player_setstance() invalid argument '%s'. Valid arguments are: 'stand', 'crouch', 'prone'", stance);
		stackPushUndefined();
		return;
	}

	G_AddPredictableEvent(entity, event, 0);

	stackPushBool(qtrue);
}

void gsc_player_getspectatorclient(scr_entref_t ref)
{
	int id = ref.entnum;
	gentity_t *entity = &g_entities[id];

	if ( entity->client == NULL )
	{
		stackError("gsc_player_getspectatorclient() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( entity->client->spectatorClient == -1 )
		stackPushUndefined();
	else
		stackPushEntity(&g_entities[entity->client->spectatorClient]);
}

void gsc_player_getip(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getip() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	char tmp[16];
	snprintf(tmp, sizeof(tmp), "%d.%d.%d.%d", client->netchan.remoteAddress.ip[0], client->netchan.remoteAddress.ip[1], client->netchan.remoteAddress.ip[2], client->netchan.remoteAddress.ip[3]);

	stackPushString(tmp);
}

void gsc_player_getping(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getping() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( customPlayerState[id].overridePing )
	{
		stackPushInt(customPlayerState[id].ping);
	}
	else
	{
		client_t *client = &svs.clients[id];
		stackPushInt(client->ping);
	}
}

void gsc_player_setping(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setping() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( Scr_GetNumParam() == 1 )
	{
		if ( Scr_GetType(0) == STACK_UNDEFINED )
		{
			customPlayerState[id].overridePing = qfalse;
			customPlayerState[id].ping = 0;
		}
		else if ( Scr_GetType(0) == STACK_INT )
		{
			customPlayerState[id].overridePing = qtrue;
			customPlayerState[id].ping = Scr_GetInt(0);
		}
		else
		{
			stackError("gsc_player_setping() argument has a wrong type");
			stackPushUndefined();
			return;
		}
	}
	else
	{
		stackError("gsc_player_setping() needs exactly one argument");
		stackPushUndefined();
		return;
	}

	stackPushBool(qtrue);
}

void gsc_player_processclientcommand(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_processclientcommand() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	ClientCommand(id);

	stackPushBool(qtrue);
}

void gsc_player_getlastconnecttime(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getlastconnecttime() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushInt(client->lastConnectTime);
}

void gsc_player_getlastmsg(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getlastmsg() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushInt(svs.time - client->lastPacketTime);
}

void gsc_player_getclientconnectstate(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getclientconnectstate() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushInt(client->state);
}

void gsc_player_addresstype(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_addresstype() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushInt(client->netchan.remoteAddress.type);
}

void gsc_player_renameclient(scr_entref_t ref)
{
	int id = ref.entnum;
	char *name;

	if ( !stackGetParams("s", &name) )
	{
		stackError("gsc_player_renameclient() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( strlen(name) > 31 )
	{
		stackError("gsc_player_renameclient() player name is longer than 31 characters");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_renameclient() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	Info_SetValueForKey(client->userinfo, "name", name);
	strcpy(client->name, name);

	stackPushBool(qtrue);
}

void gsc_player_connectionlesspackettoclient(scr_entref_t ref)
{
	int id = ref.entnum;
	char *cmd;

	if ( !stackGetParams("s", &cmd) )
	{
		stackError("gsc_player_connectionlesspackettoclient() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_connectionlesspackettoclient() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	NET_OutOfBandPrint(NS_SERVER, client->netchan.remoteAddress, cmd);

	stackPushBool(qtrue);
}

void gsc_player_connectionlesspackettoserver(scr_entref_t ref)
{
	int id = ref.entnum;
	char *cmd;

	if ( !stackGetParams("s", &cmd) )
	{
		stackError("gsc_player_connectionlesspackettoserver() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_connectionlesspackettoserver() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	byte bufData[MAX_MSGLEN];
	msg_t msg;

	MSG_Init(&msg, bufData, sizeof(bufData));
	MSG_WriteByte(&msg, svc_nop);
	MSG_WriteShort(&msg, 0);
	MSG_WriteLong(&msg, -1);
	MSG_WriteString(&msg, cmd);

	SV_ConnectionlessPacket(client->netchan.remoteAddress, &msg);

	stackPushBool(qtrue);
}

void gsc_player_resetnextreliabletime(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_resetnextreliabletime() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	client->nextReliableTime = 0;

	stackPushBool(qtrue);
}

void gsc_player_ismantling(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_ismantling() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);

	stackPushBool(ps->pm_flags & PMF_MANTLE ? qtrue : qfalse);
}

void gsc_player_isonladder(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_isonladder() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);

	stackPushBool(ps->pm_flags & PMF_LADDER ? qtrue : qfalse);
}


void gsc_player_isusingturret(scr_entref_t ref)
{
	int id = ref.entnum;
	gentity_t *entity = &g_entities[id];

	if ( entity->client == NULL )
	{
		stackError("gsc_player_isusingturret() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	stackPushBool(entity->s.eFlags & EF_USETURRET ? qtrue : qfalse);
}

void gsc_player_stopuseturret(scr_entref_t ref)
{
	int id = ref.entnum;
	gentity_t *entity = &g_entities[id];

	if ( entity->client == NULL )
	{
		stackError("gsc_player_stopuseturret() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( entity->client->ps.pm_flags & PMF_VIEWLOCKED && entity->client->ps.eFlags & EF_USETURRET )
	{
		G_ClientStopUsingTurret(&level.gentities[entity->client->ps.viewlocked_entNum]);
		stackPushBool(qtrue);
	}
	else
	{
		stackPushBool(qfalse);
	}

}

void gsc_player_getjumpslowdowntimer(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getjumpslowdowntimer() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);

	stackPushInt(ps->pm_time);
}

void gsc_player_clearjumpstate(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_clearjumpstate() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);

	ps->pm_flags &= ~( PMF_JUMPING | PMF_SLIDING );
	ps->pm_time = 0;
	ps->jumpTime = 0;
	ps->jumpOriginZ = 0;

	stackPushBool(qtrue);
}

void gsc_player_setspeed(scr_entref_t ref)
{
	int id = ref.entnum;
	int speed;

	if ( !stackGetParams("i", &speed) )
	{
		stackError("gsc_player_setspeed() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setspeed() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( speed < 0 )
	{
		stackError("gsc_player_setspeed() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	customPlayerState[id].speed = speed;

	stackPushBool(qtrue);
}

void gsc_player_getspeed(scr_entref_t ref)
{
	int id = ref.entnum;
	gentity_t *entity = &g_entities[id];

	if ( entity->client == NULL )
	{
		stackError("gsc_player_getspeed() entity %i is not a player");
		stackPushUndefined();
		return;
	}
	
	stackPushInt(entity->client->ps.speed);
}

void gsc_player_setgravity(scr_entref_t ref)
{
	int id = ref.entnum;
	int gravity;

	if ( !stackGetParams("i", &gravity) )
	{
		stackError("gsc_player_setgravity() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setgravity() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( gravity < 0 )
	{
		stackError("gsc_player_setgravity() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	customPlayerState[id].gravity = gravity;

	stackPushBool(qtrue);
}

void gsc_player_getgravity(scr_entref_t ref)
{
	int id = ref.entnum;
	gentity_t *entity = &g_entities[id];

	if ( entity->client == NULL )
	{
		stackError("gsc_player_getgravity() entity %i is not a player");
		stackPushUndefined();
		return;
	}

	stackPushInt(entity->client->ps.gravity);
}

void gsc_player_setbulletmask(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setbulletmask() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int old_setting;

	if ( Scr_GetNumParam() == 1 )
	{
		if ( customPlayerState[id].overrideBulletMask )
			old_setting = customPlayerState[id].bulletMask;
		else if ( customPlayerState[id].fireThroughWalls )
			old_setting = CONTENTS_BODY;
		else
			old_setting = MASK_SHOT | CONTENTS_GLASS;

		if ( Scr_GetType(0) == STACK_UNDEFINED )
		{
			customPlayerState[id].overrideBulletMask = qfalse;
			customPlayerState[id].bulletMask = 0;
		}
		else if ( Scr_GetType(0) == STACK_INT )
		{
			customPlayerState[id].overrideBulletMask = qtrue;
			customPlayerState[id].bulletMask = Scr_GetInt(0);
		}
		else
		{
			stackError("gsc_player_setbulletmask() argument has a wrong type");
			stackPushUndefined();
			return;
		}
	}
	else
	{
		stackError("gsc_player_setbulletmask() needs exactly one argument");
		stackPushUndefined();
		return;
	}

	stackPushInt(old_setting);
}

void gsc_player_setfirethroughwalls(scr_entref_t ref)
{
	int id = ref.entnum;
	int old_setting, new_setting;

	if ( !stackGetParams("i", &new_setting) )
	{
		stackError("gsc_player_setfirethroughwalls() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setfirethroughwalls() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	old_setting = customPlayerState[id].fireThroughWalls;
	customPlayerState[id].fireThroughWalls = new_setting;

	stackPushInt(old_setting);
}

void gsc_player_setfirerangescale(scr_entref_t ref)
{
	int id = ref.entnum;
	float old_scale, new_scale;

	if ( !stackGetParams("f", &new_scale) )
	{
		stackError("gsc_player_setfirerangescale() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setfirerangescale() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	old_scale = customPlayerState[id].fireRangeScale;
	if ( new_scale < 0 )
	{
		new_scale = 0.0;
	}
	customPlayerState[id].fireRangeScale = new_scale;

	stackPushFloat(old_scale);
}

void gsc_player_setweaponfiremeleedelay(scr_entref_t ref)
{
	int id = ref.entnum;
	int delay;

	if ( !stackGetParams("i", &delay) )
	{
		stackError("gsc_player_setweaponfiremeleedelay() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}
	
	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setweaponfiremeleedelay() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}
	
	if ( delay < 0 )
	{
		stackError("gsc_player_setweaponfiremeleedelay() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);
	ps->weaponDelay = delay;

	stackPushBool(qtrue);
}

void gsc_player_setmeleeheightscale(scr_entref_t ref)
{
	int id = ref.entnum;
	float old_scale, new_scale;

	if ( !stackGetParams("f", &new_scale) )
	{
		stackError("gsc_player_setmeleeheightscale() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setmeleeheightscale() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	old_scale = customPlayerState[id].meleeHeightScale;
	if ( new_scale < 0 )
	{
		new_scale = 0.0;
	}
	customPlayerState[id].meleeHeightScale = new_scale;

	stackPushFloat(old_scale);
}

void gsc_player_setmeleerangescale(scr_entref_t ref)
{
	int id = ref.entnum;
	float old_scale, new_scale;

	if ( !stackGetParams("f", &new_scale) )
	{
		stackError("gsc_player_setmeleerangescale() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setmeleerangescale() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	old_scale = customPlayerState[id].meleeRangeScale;
	if ( new_scale < 0 )
	{
		new_scale = 0.0;
	}
	customPlayerState[id].meleeRangeScale = new_scale;

	stackPushFloat(old_scale);
}

void gsc_player_setmeleewidthscale(scr_entref_t ref)
{
	int id = ref.entnum;
	float old_scale, new_scale;

	if ( !stackGetParams("f", &new_scale) )
	{
		stackError("gsc_player_setmeleewidthscale() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setmeleewidthscale() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	old_scale = customPlayerState[id].meleeWidthScale;
	if ( new_scale < 0 )
	{
		new_scale = 0.0;
	}
	customPlayerState[id].meleeWidthScale = new_scale;

	stackPushFloat(old_scale);
}

void gsc_player_setturretspreadscale(scr_entref_t ref)
{
	int id = ref.entnum;
	float old_scale, new_scale;

	if ( !stackGetParams("f", &new_scale) )
	{
		stackError("gsc_player_setturretspreadscale() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setturretspreadscale() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	old_scale = customPlayerState[id].turretSpreadScale;
	if ( new_scale < 0 )
	{
		new_scale = 0.0;
	}
	customPlayerState[id].turretSpreadScale = new_scale;

	stackPushFloat(old_scale);
}

void gsc_player_setweaponspreadscale(scr_entref_t ref)
{
	int id = ref.entnum;
	float old_scale, new_scale;

	if ( !stackGetParams("f", &new_scale) )
	{
		stackError("gsc_player_setweaponspreadscale() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setweaponspreadscale() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	old_scale = customPlayerState[id].weaponSpreadScale;
	if ( new_scale < 0 )
	{
		new_scale = 0.0;
	}
	customPlayerState[id].weaponSpreadScale = new_scale;

	stackPushFloat(old_scale);
}

int BG_AnimationCheckForBad(char *anim)
{
	int i, v6, v8;

	#if COD_VERSION == COD2_1_0
	int globalScriptData = 0x855A4E4;
	#elif COD_VERSION == COD2_1_2
	int globalScriptData = 0x856E3A4;
	#elif COD_VERSION == COD2_1_3
	int globalScriptData = 0x860B424;
	#endif

	int (*ConverteStr)(int a1);
	#if COD_VERSION == COD2_1_0
	*(int *)&ConverteStr = 0x80D45C4;
	#elif COD_VERSION == COD2_1_2
	*(int *)&ConverteStr = 0x80D6B9C;
	#elif COD_VERSION == COD2_1_3
	*(int *)&ConverteStr = 0x80D6CE0;
	#endif

	signed int (*SACh)(char *a1, char *a2);
	#if COD_VERSION == COD2_1_0
	*(int *)&SACh = 0x80B5620;
	#elif COD_VERSION == COD2_1_2
	*(int *)&SACh = 0x80B7AB4;
	#elif COD_VERSION == COD2_1_3
	*(int *)&SACh = 0x80B7BF8;
	#endif

	v6 = ConverteStr((int)anim);
	v8 = 0;

	for ( i = *(int *)globalScriptData; ; i += 96 ) 
	{
		
		if ( v8 >= *(long *)((*(int *)globalScriptData) + 49152) )
		{
			printf("BG_AnimationIndexForString: unknown player animation '%s'\n", anim);
			return 0;
		}

		if ( v6 == *(long *)(i + 76) && !SACh(anim, (char *)i) )
			return v8;

		++v8;

	}
	return 0;
}

void gsc_player_setanimation(scr_entref_t ref)
{
	int id = ref.entnum;
	char *animation;

	if ( !stackGetParams("s", &animation) )
	{
		stackError("gsc_player_setanimation() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id];

	if ( entity->s.eType == ET_PLAYER_CORPSE )
	{
		int index = BG_AnimationIndexForString(animation);
		entity->s.legsAnim = index;
		stackPushBool(qtrue);
		return;
	}

	if ( entity->client == NULL )
	{
		stackError("gsc_player_playanimation() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	customPlayerState[id].animation = strcmp(animation, "none") ? BG_AnimationCheckForBad(animation) : 0;

	stackPushBool(qtrue);
}

void gsc_player_getcooktime(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getcooktime() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id);

	stackPushInt(ps->grenadeTimeLeft);
}

void gsc_player_setguid(scr_entref_t ref)
{
	int id = ref.entnum;
	int guid;

	if ( !stackGetParams("i", &guid) )
	{
		stackError("gsc_player_setguid() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setguid() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	client->guid = guid;

	stackPushBool(qtrue);
}

void gsc_player_clienthasclientmuted(scr_entref_t ref)
{
	int id = ref.entnum;
	int id2;

	if ( !stackGetParams("i", &id2) )
	{
		stackError("gsc_player_clienthasclientmuted() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_clienthasclientmuted() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( id2 >= MAX_CLIENTS )
	{
		stackError("gsc_player_clienthasclientmuted() entity %i is not a player", id2);
		stackPushUndefined();
		return;
	}

	stackPushInt(SV_ClientHasClientMuted(id, id2));
}

void gsc_player_muteclient(scr_entref_t ref)
{
	int id = ref.entnum;
	int id2;

	if ( !stackGetParams("i", &id2) )
	{
		stackError("gsc_player_muteclient() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_muteclient() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( id2 >= MAX_CLIENTS )
	{
		stackError("gsc_player_muteclient() entity %i is not a player", id2);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	client->mutedClients[id2] = 1;

	stackPushBool(qtrue);
}

void gsc_player_unmuteclient(scr_entref_t ref)
{
	int id = ref.entnum;
	int id2;

	if ( !stackGetParams("i", &id2) )
	{
		stackError("gsc_player_unmuteclient() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_unmuteclient() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( id2 >= MAX_CLIENTS )
	{
		stackError("gsc_player_unmuteclient() entity %i is not a player", id2);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	client->mutedClients[id2] = 0;

	stackPushBool(qtrue);
}

void gsc_player_getlastgamestatesize(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getlastgamestatesize() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	stackPushInt(customPlayerState[id].gamestateSize);
}

void gsc_player_getfps(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getfps() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	stackPushInt(customPlayerState[id].fps);
}

void gsc_player_noclip(scr_entref_t ref)
{
	int id = ref.entnum;
	char *noclip;

	if ( !stackGetParams("s", &noclip) )
	{
		stackError("gsc_player_noclip() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_noclip() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id];

	if ( !I_stricmp(noclip, "on") || atoi(noclip) )
	{
		entity->client->noclip = qtrue;
	}
	else if ( !I_stricmp(noclip, "off") || !I_stricmp(noclip, "0") )
	{
		entity->client->noclip = qfalse;
	}
	else
	{
		entity->client->noclip = !entity->client->noclip;
	}

	stackPushBool(qtrue);
}

void gsc_player_getinactivitytime(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getinactivitytime() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id];

	stackPushInt(entity->client->inactivityTime);
}

void gsc_player_disableearthquakes(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_disableearthquakes() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	qboolean old_setting = !customPlayerState[id].noEarthquakes;
	customPlayerState[id].noEarthquakes = qtrue;

	stackPushInt(old_setting);
}

void gsc_player_enableearthquakes(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_enableearthquakes() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	qboolean old_setting = !customPlayerState[id].noEarthquakes;
	customPlayerState[id].noEarthquakes = qfalse;

	stackPushInt(old_setting);
}

void gsc_player_earthquakeforplayer(scr_entref_t ref)
{
	int id = ref.entnum;
	float scale;
	float duration;
	vec3_t source;
	float radius;

	if ( !stackGetParams("ffvf", &scale, &duration, &source, &radius) )
	{
		stackError("gsc_player_earthquakeforplayer() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_earthquakeforplayer() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	duration = floorf((duration * 1000.0) + 0.5);

	if ( scale <= 0.0 )
	{
		Scr_ParamError(0, "Scale must be greater than 0");
	}

	if ( (int)duration < 1 )
	{
		Scr_ParamError(1, "duration must be greater than 0");
	}

	if ( radius <= 0.0 )
	{
		Scr_ParamError(3, "Radius must be greater than 0");
	}

	gentity_t *ent = G_TempEntity(source, EV_EARTHQUAKE);
	ent->s.angles2[0] = scale;
	ent->s.time = (int)duration;
	ent->s.angles2[1] = radius;
	ent->s.otherEntityNum = id + 1;

	stackPushBool(qtrue);
}

void gsc_player_playfxforplayer(scr_entref_t ref)
{
	int id = ref.entnum;
	int args;
	qboolean error;
	int index;
	vec3_t origin;
	vec3_t forward_vec;
	vec3_t up_vec;
	vec3_t cross;
	double length;

	args = Scr_GetNumParam();
	error = qfalse;
	switch ( args )
	{
		case 2:
			if ( ! stackGetParams("iv", &index, &origin) )
				error = qtrue;
			break;
		case 3:
			if ( ! stackGetParams("ivv", &index, &origin, &forward_vec) )
				error = qtrue;
			break;
		case 4:
			if ( ! stackGetParams("ivvv", &index, &origin, &forward_vec, &up_vec) )
				error = qtrue;
			break;
		default:
			stackError("gsc_utils_playfxforplayer() incorrect number of parameters");
			stackPushUndefined();
			return;
	}

	if ( error )
	{
		stackError("gsc_utils_playfxforplayer() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}
	
	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_utils_playfxforplayer() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *ent = G_TempEntity(origin, EV_PLAY_FX);
	ent->s.eventParm = index & 0xff;
	ent->s.otherEntityNum = id + 1;

	if ( args == 2 )
	{
		ent->s.apos.trBase[0] = -90.0;
	}
	else
	{
		length = VectorLength(forward_vec);
		if ( length == 0.0 )
		{
			Scr_PlayFxError("playFx called with (0 0 0) forward direction", index);
		}
		VectorScale(forward_vec, 1.0 / length, forward_vec);

		if ( args == 3 )
		{
			VecToAngles(forward_vec, ent->s.apos.trBase);
		}
		else
		{
			length = VectorLength(up_vec);
			if ( length == 0.0 )
			{
				Scr_PlayFxError("playFx called with (0 0 0) up direction", index);
			}
			VectorScale(up_vec, 1.0 / length, up_vec);
			VectorCross(up_vec, forward_vec, cross);

			length = VectorLength(cross);
			if ( length < 0.001 )
			{
				Scr_PlayFxError("playFx called an up direction 0 or 180 degrees from forward", index);
			}
			else if ( length < 0.999 )
			{
				VectorScale(cross, 1.0 / length, cross);
				VectorCross(forward_vec, cross, up_vec);
			}
			AxisToAngles(forward_vec, ent->s.apos.trBase);
		}
	}

	stackPushBool(qtrue);
}

void gsc_player_playfxontagforplayer(scr_entref_t ref)
{
	int id = ref.entnum;
	int argc;
	int index;
	gentity_t *ent;
	unsigned int tag_id;
	char *tag_name;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_utils_playfxontagforplayer() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}
	
	argc = Scr_GetNumParam();
	if ( argc != 3 )
	{
		Scr_Error("USAGE: playFxOnTagForPlayer <effect id from loadFx> <entity> <tag name>");
	}

	index = Scr_GetInt(0);
	if ( index < 1 || 0x3f < index )
	{
		Scr_ParamError(0, custom_va("effect id %i is invalid\n", index));
	}

	ent = Scr_GetEntity(1);
	if ( ent->model == 0 )
	{
		Scr_ParamError(1, "cannot play fx on entity with no model");
	}

	tag_id = Scr_GetConstLowercaseString(2);
	tag_name = SL_ConvertToString(tag_id);
	if ( strchr(tag_name, '"') != 0 )
	{
		Scr_ParamError(2, "cannot use \" characters in tag names\n");
	}

	if ( SV_DObjGetBoneIndex(ent, tag_id) < 0 )
	{
		SV_DObjDumpInfo(ent);
		Scr_ParamError(2, custom_va("tag \'%s\' does not exist on entity with model \'%s\'", tag_name, G_ModelName(ent->model)));
	}

	/* Reusing the attackerEntityNum field that is otherwise only used at
	 obituary TempEntities. This way we have it archived, for correct killcam
	 data */
	ent->s.attackerEntityNum = 1 + id;
	G_AddEvent(ent, EV_PLAY_FX_ON_TAG, G_FindConfigstringIndex(custom_va("%02d%s", index, tag_name), 0x38e, 0x100, 1, NULL));

	stackPushBool(qtrue);
}

void gsc_player_getclienthudelemcount(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getclienthudelemcount() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int count = 0;
	game_hudelem_t *g_hudelem = &g_hudelems;
	for ( int i = 0; i < 0x400; i++, g_hudelem++ )
	{
		if ( ( g_hudelem->elem.type != HE_TYPE_FREE ) && ( g_hudelem->clientNum == id ) )
			count++;
	}
	
	stackPushInt(count);
}

void gsc_player_playscriptanimation(scr_entref_t ref)
{
	int id = ref.entnum;
	int scriptAnimEventType;
	int isContinue;
	int force;

	if ( !stackGetParams("iii", &scriptAnimEventType, &isContinue, &force) )
	{
		stackError("gsc_player_playscriptanimation() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_playscriptanimation() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( scriptAnimEventType < 0 || scriptAnimEventType >= NUM_ANIM_EVENTTYPES )
	{
		stackError("gsc_player_playscriptanimation() argument is not a valid scriptAnimEventType");
		stackPushUndefined();
		return;
	}
	
	gentity_t *entity = &g_entities[id];

	stackPushInt(BG_AnimScriptEvent(&entity->client->ps, (scriptAnimEventTypes_t)scriptAnimEventType, isContinue, force));
}

void gsc_player_disablesilent(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_disablesilent() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	qboolean old_setting = customPlayerState[id].silent;
	if ( old_setting )
	{
		SV_GameSendServerCommand(id, 0, "v cg_footsteps \"1\"");
		SV_GameSendServerCommand(id, 0, "v bg_foliagesnd_minspeed \"40\"");
		SV_GameSendServerCommand(id, 0, "v bg_foliagesnd_maxspeed \"180\"");
	}
	customPlayerState[id].silent = qfalse;

	stackPushInt(old_setting);
}

void gsc_player_enablesilent(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_enablesilent() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	qboolean old_setting = customPlayerState[id].silent;
	if ( !old_setting )
	{
		SV_GameSendServerCommand(id, 0, "v cg_footsteps \"0\"");
		SV_GameSendServerCommand(id, 0, "v bg_foliagesnd_minspeed \"999\"");
		SV_GameSendServerCommand(id, 0, "v bg_foliagesnd_maxspeed \"999\"");
	}
	customPlayerState[id].silent = qtrue;

	stackPushInt(old_setting);
}

void gsc_player_getgroundentity(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getgroundentity() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id];
	if ( entity->client->ps.groundEntityNum == ENTITY_NONE )
		stackPushUndefined();
	else
		stackPushEntity(&g_entities[entity->client->ps.groundEntityNum]);
}

void gsc_player_getentertime(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getentertime() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id];

	stackPushInt(entity->client->sess.enterTime);
}

void gsc_player_getplayerstateflags(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getplayerstateflags() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id];

	stackPushInt(entity->client->ps.pm_flags);
}

void gsc_player_objective_player_add(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_objective_player_add() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int args;
	int objective_number;
	objective_t *obj;
	ushort index;
	objectiveState_t state;

	args = Scr_GetNumParam();
	if ( args < 2 )
	{
		Scr_Error("objective_add needs at least the first two parameters out of its parameter list of: index state [string] [position]\n");
	}

	objective_number = Scr_GetInt(0);
	if ( ( objective_number < 0 ) || ( 0xF < objective_number ) )
	{
		Scr_ParamError(0, custom_va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objective_number, 15));
	}

	obj = &customPlayerState[id].objectives[objective_number];

	if ( obj->entNum != ENTITY_NONE )
	{
		if ( g_entities[obj->entNum].r.inuse != 0 )
		{
			g_entities[obj->entNum].r.svFlags = g_entities[obj->entNum].r.svFlags & 0xef;
		}
		obj->entNum = ENTITY_NONE;
	}

	index = Scr_GetConstString(1);
	if ( index == scr_const.empty )
	{
		state = OBJST_EMPTY;
	}
	else if ( index == scr_const.invisible )
	{
		state = OBJST_INVISIBLE;
	}
	else 
	{
		if ( index != scr_const.current )
		{
			state = OBJST_EMPTY;
			Scr_ParamError(1, custom_va("Illegal objective state \"%s\". Valid states are \"empty\", \"invisible\", \"current\"\n", SL_ConvertToString((uint)index)));
		}
		state = OBJST_CURRENT;
	}
	obj->state = state;

	if ( 2 < args )
	{
		Scr_GetVector(2, &obj->origin);
		obj->origin[0] = (float)(int)obj->origin[0];
		obj->origin[1] = (float)(int)obj->origin[1];
		obj->origin[2] = (float)(int)obj->origin[2];
		obj->entNum = ENTITY_NONE;
		if ( 3 < args )
		{
			SetObjectiveIcon(obj, 3);
		}
	}
	obj->teamNum = 0;

	stackPushBool(qtrue);
}

void gsc_player_objective_player_delete(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_objective_player_delete() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int objective_number;
	objective_t *obj;

	objective_number = Scr_GetInt(0);
	if ( ( objective_number < 0 ) || ( 0xf < objective_number ) )
	{
		Scr_ParamError(0, custom_va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objective_number, 15));
	}

	obj = &customPlayerState[id].objectives[objective_number];

	if ( obj->entNum != ENTITY_NONE )
	{
		if ( g_entities[obj->entNum].r.inuse != 0 )
		{
			g_entities[obj->entNum].r.svFlags = g_entities[obj->entNum].r.svFlags & 0xef;
		}
		obj->entNum = ENTITY_NONE;
	}

	obj->state = OBJST_EMPTY;
	obj->origin[0] = 0.0;
	obj->origin[1] = 0.0;
	obj->origin[2] = 0.0;
	obj->entNum = ENTITY_NONE;
	obj->teamNum = 0;
	obj->icon = 0;

	stackPushBool(qtrue);
}

void gsc_player_objective_player_icon(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_objective_player_icon() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int objective_number = Scr_GetInt(0);
	if ( ( objective_number < 0 ) || ( 0xf < objective_number ) )
	{
		Scr_ParamError(0, custom_va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objective_number, 15));
	}

  	SetObjectiveIcon(&customPlayerState[id].objectives[objective_number], 1);

	stackPushBool(qtrue);
}

void gsc_player_objective_player_position(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_objective_player_position() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int objective_number;
	objective_t *obj;

	objective_number = Scr_GetInt(0);
	if ( ( objective_number < 0 ) || ( 0xf < objective_number ) )
	{
		Scr_ParamError(0, custom_va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objective_number, 15));
	}

	obj = &customPlayerState[id].objectives[objective_number];

	if ( obj->entNum != ENTITY_NONE )
	{
		if ( g_entities[obj->entNum].r.inuse != 0 )
		{
			g_entities[obj->entNum].r.svFlags = g_entities[obj->entNum].r.svFlags & 0xef;
		}
		obj->entNum = ENTITY_NONE;
	}

	Scr_GetVector(1, &obj->origin);
	obj->origin[0] = (float)(int)obj->origin[0];
	obj->origin[1] = (float)(int)obj->origin[1];
	obj->origin[2] = (float)(int)obj->origin[2];

	stackPushBool(qtrue);
}

void gsc_player_objective_player_state(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_objective_player_state() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	int objective_number;
	objective_t *obj;
	ushort index;
	objectiveState_t state;

	objective_number = Scr_GetInt(0);
	if ( ( objective_number < 0 ) || ( 0xf < objective_number ) )
	{
		Scr_ParamError(0, custom_va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objective_number, 15));
	}

	obj = &customPlayerState[id].objectives[objective_number];

	index = Scr_GetConstString(1);
	if ( index == scr_const.empty )
	{
		state = OBJST_EMPTY;
	}
	else if ( index == scr_const.invisible )
	{
		state = OBJST_INVISIBLE;
	}
	else 
	{
		if ( index != scr_const.current )
		{
			state = OBJST_EMPTY;
			Scr_ParamError(1, custom_va("Illegal objective state \"%s\". Valid states are \"empty\", \"invisible\", \"current\"\n", SL_ConvertToString((uint)state)));
		}
		state = OBJST_CURRENT;
	}
	obj->state = state;

	if ( ( state == OBJST_EMPTY ) || ( state == OBJST_INVISIBLE ) )
	{
		if ( obj->entNum != ENTITY_NONE )
		{
			if ( g_entities[obj->entNum].r.inuse != 0 )
			{
				g_entities[obj->entNum].r.svFlags = g_entities[obj->entNum].r.svFlags & 0xef;
			}
			obj->entNum = ENTITY_NONE;
		}
	}

	stackPushBool(qtrue);
}

void gsc_player_getvieworigin(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getvieworigin() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	vec3_t viewOrigin;
	gentity_t *ent = &g_entities[id];

	G_GetPlayerViewOrigin(ent, viewOrigin);

	stackPushVector(viewOrigin);
}

void gsc_player_getservercommandqueuesize(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getservercommandqueuesize() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];

	stackPushInt(client->reliableSequence - client->reliableAcknowledge);
}

void gsc_player_setconfigstring(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setconfigstring() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	char *value;
	int index;

	if ( !stackGetParams("si", &value, &index) )
	{
		stackError("gsc_player_setconfigstring() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id];
	char cmd[MAX_STRINGLENGTH];

	Com_sprintf(cmd, MAX_STRINGLENGTH, "d %i %s", index, value);
	SV_SendServerCommand(client, 1, cmd);

	stackPushBool(qtrue);
}

void gsc_player_getcollisionteam(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getcollisionteam() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( customPlayerState[id].collisionTeam == CUSTOM_TEAM_AXIS_ALLIES )
	{
		Scr_AddConstString(custom_scr_const.axis_allies);
	}
	else if ( customPlayerState[id].collisionTeam == CUSTOM_TEAM_NONE )
	{
		Scr_AddConstString(scr_const.none);
	}
	else if ( customPlayerState[id].collisionTeam == CUSTOM_TEAM_AXIS )
	{
		Scr_AddConstString(scr_const.axis);
	}
	else if ( customPlayerState[id].collisionTeam == CUSTOM_TEAM_ALLIES )
	{
		Scr_AddConstString(scr_const.allies);
	}
}

void gsc_player_setcollisionteam(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setcollisionteam() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	short team = Scr_GetConstString(0);
	if ( team == scr_const.none )
	{
		customPlayerState[id].collisionTeam = CUSTOM_TEAM_NONE;
	}
	else if ( team == scr_const.axis )
	{
		customPlayerState[id].collisionTeam = CUSTOM_TEAM_AXIS;
	}
	else if ( team == scr_const.allies )
	{
		customPlayerState[id].collisionTeam = CUSTOM_TEAM_ALLIES;
	}
	else if ( team == custom_scr_const.axis_allies )
	{
		customPlayerState[id].collisionTeam = CUSTOM_TEAM_AXIS_ALLIES;
	}
	else
	{
		Scr_ParamError(0, "collision team must be \"axis\", \"allies\", \"none\", or \"axis_allies\"");
	}

	stackPushBool(qtrue);
}

void gsc_player_disablebulletimpacts(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_disablebulletimpacts() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	customPlayerState[id].noBulletImpacts = qtrue;

	stackPushBool(qtrue);
}

void gsc_player_enablebulletimpacts(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_enablebulletimpacts() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	customPlayerState[id].noBulletImpacts = qfalse;

	stackPushBool(qtrue);
}

void gsc_player_disabletalkericon(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_disabletalkericon() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *talker = Scr_GetEntity(0);
	int talkerNum = talker->s.number;
	if ( talkerNum >= MAX_CLIENTS )
	{
		stackError("gsc_player_disabletalkericon() specified talker is not a player");
		stackPushUndefined();
		return;
	}

	customPlayerState[id].talkerIcons[talkerNum] = 0;

	stackPushBool(qtrue);
}

void gsc_player_enabletalkericon(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_enabletalkericon() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	gentity_t *talker = Scr_GetEntity(0);
	int talkerNum = talker->s.number;
	if ( talkerNum >= MAX_CLIENTS )
	{
		stackError("gsc_player_disabletalkericon() specified talker is not a player");
		stackPushUndefined();
		return;
	}

	customPlayerState[id].talkerIcons[talkerNum] = 1;

	stackPushBool(qtrue);
}

#if COMPILE_CUSTOM_VOICE == 1

extern VoicePacket_t voiceDataStore[MAX_CUSTOMSOUNDS][MAX_STOREDVOICEPACKETS];

void gsc_player_isplayingsoundfile(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_isplayingsoundfile() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	stackPushInt(customPlayerState[id].currentSoundIndex);
}

void gsc_player_getremainingsoundfileduration(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_getremainingsoundfileduration() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( customPlayerState[id].currentSoundIndex )
	{
		int remainingPackets;
		int endIndex = customPlayerState[id].sentVoiceDataIndex;
		while ( endIndex < MAX_STOREDVOICEPACKETS && voiceDataStore[customPlayerState[id].currentSoundIndex - 1][endIndex].dataLen > 0 )
		{
			endIndex++;
		}
		remainingPackets = endIndex - customPlayerState[id].sentVoiceDataIndex;
		if ( remainingPackets <= 0 )
			stackPushFloat(0.0);
		else
			stackPushFloat(remainingPackets / (((1.0 / FRAMETIME) * 1000) * MAX_VOICEPACKETSPERFRAME));
	}
	else
	{
		stackPushFloat(0.0);
	}
}

void gsc_player_stopsoundfile(scr_entref_t ref)
{
	int id = ref.entnum;

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_stopsoundfile() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( customPlayerState[id].currentSoundIndex )
	{
		customPlayerState[id].pendingVoiceDataFrames = 0.0;
		customPlayerState[id].currentSoundTalker = 0;
		customPlayerState[id].currentSoundIndex = 0;
		customPlayerState[id].sentVoiceDataIndex = 0;
		Scr_Notify(&g_entities[id], custom_scr_const.sound_file_stop, 0);

		stackPushBool(qtrue);
	}
	else
	{
		stackPushBool(qfalse);
	}
}

void gsc_player_playsoundfile(scr_entref_t ref)
{
	int id = ref.entnum;
	int args;
	qboolean error;
	int soundIndex;
	float offset;
	int source;

	args = Scr_GetNumParam();
	error = qfalse;
	switch ( args )
	{
		case 1:
			offset = 0.0;
			source = id;
			if ( !stackGetParams("i", &soundIndex) )
				error = qtrue;
			break;
		case 2:
			source = id;
			if ( !stackGetParams("if", &soundIndex, &offset) )
				error = qtrue;
			break;
		case 3:
			if ( !stackGetParams("ifi", &soundIndex, &offset, &source) )
				error = qtrue;
			break;
		default:
			stackError("gsc_player_playsoundfile() incorrect number of parameters");
			stackPushBool(qfalse);
			return;
	}

	if ( error )
	{
		stackError("gsc_player_playsoundfile() one or more arguments is undefined or has a wrong type");
		stackPushBool(qfalse);
		return;
	}

	if ( id >= MAX_CLIENTS || source >= MAX_CLIENTS )
	{
		stackError("gsc_player_playsoundfile() entity %i is not a player", id);
		stackPushBool(qfalse);
		return;
	}

	if ( svs.clients[source].state < CS_CONNECTED )
	{
		stackError("gsc_player_playsoundfile() entity %i is not connected", source);
		stackPushBool(qfalse);
		return;
	}

	if ( soundIndex < 1 || soundIndex > MAX_CUSTOMSOUNDS )
	{
		stackError("gsc_player_playsoundfile() invalid sound index, valid range is 1-%d", MAX_CUSTOMSOUNDS);
		stackPushBool(qfalse);
		return;
	}

	int packetOffset = (int)(offset * (((1.0 / FRAMETIME) * 1000) * MAX_VOICEPACKETSPERFRAME));
	if ( packetOffset >= MAX_STOREDVOICEPACKETS )
	{
		stackError("gsc_player_playsoundfile() too large offset for sound with index %d", soundIndex);
		stackPushBool(qfalse);
		return;
	}

	VoicePacket_t *voicePacket = &voiceDataStore[soundIndex - 1][packetOffset];
	if ( !*voicePacket->data )
	{
		stackError("gsc_player_playsoundfile() too large offset for sound with index %d", soundIndex);
		stackPushBool(qfalse);
		return;
	}

	if ( customPlayerState[id].currentSoundIndex )
		Scr_Notify(&g_entities[id], custom_scr_const.sound_file_stop, 0);

	customPlayerState[id].pendingVoiceDataFrames = 0.0;
	customPlayerState[id].currentSoundTalker = source;
	customPlayerState[id].sentVoiceDataIndex = packetOffset;
	customPlayerState[id].currentSoundIndex = soundIndex;

	stackPushBool(qtrue);
}
#endif

#if COMPILE_JUMP == 1

void gsc_player_setjumpheight(scr_entref_t ref)
{
	int id = ref.entnum;
	float jump_height;

	if ( !stackGetParams("f", &jump_height) )
	{
		stackError("gsc_player_setjumpheight() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setjumpheight() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( jump_height < 0 )
		customPlayerState[id].overrideJumpHeight = false;
	else
	{
		customPlayerState[id].overrideJumpHeight = true;
		customPlayerState[id].jumpHeight = jump_height;
	}

	stackPushBool(qtrue);
}

void gsc_player_setjumpslowdownenable(scr_entref_t ref)
{
	int id = ref.entnum;
	int slowdown;

	if ( !stackGetParams("i", &slowdown) )
	{
		stackError("gsc_player_setjumpslowdownenable() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setjumpslowdownenable() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( slowdown == -1 )
		customPlayerState[id].overrideJumpSlowdown = false;
	else
	{
		customPlayerState[id].overrideJumpSlowdown = true;
		customPlayerState[id].jumpSlowdown = slowdown;
	}

	stackPushBool(qtrue);
}
#endif

#endif