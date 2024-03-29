/*
							Seifensive - Anti Cheat INCLUDE
*/
/*x---------------------------------Important-------------------------------------x*/
//**INCLUDES**//
#include <a_samp>
//**PRAGMAS**//
//#pragma unused Seifensive_GivePlayerMoney
//#pragma unused Seifensive_TakePlayerMoney
//#pragma unused Seifensive_GivePlayerWeapon
//**MISC**//

/*x---------------------------------Defining-------------------------------------x*/
//**COLORS*//
#define GREEN 0x21DD00FF
#define RED 0xE60000FF
#define ADMIN_RED 0xFB0000FF
#define YELLOW 0xFFFF00FF
#define ORANGE 0xF97804FF
#define LIGHTRED 0xFF8080FF
#define LIGHTBLUE 0x00C2ECFF
#define PURPLE 0xB360FDFF
#define PLAYER_COLOR 0xFFFFFFFF
#define BLUE 0x1229FAFF
#define LIGHTGREEN 0x38FF06FF
#define DARKPINK 0xE100E1FF
#define DARKGREEN 0x008040FF
#define ANNOUNCEMENT 0x6AF7E1FF
#define COLOR_SYSTEM 0xEFEFF7AA
#define GREY 0xCECECEFF
#define PINK 0xD52DFFFF
#define DARKGREY    0x626262FF
#define AQUAGREEN   0x03D687FF
#define NICESKY 0x99FFFFAA
#define WHITE 			0xFFFFFFFF
//**MISC**//
#define ON      					1
#define OFF     					0
#define MAX_WEAPONS                 46
#define MONEY_HACK                  5000 // Maximum money amount found as extra between current cash and script cash
#define MAX_AMMO_HACK               69  // Maximum bullets found as extra between normal ammo and scripted one. (69 = s0beit)
#define KICK                        1
#define BAN                         2
//**VARIABLES**//
enum acInfo
{
	acWeapon[13],
	acCash,
	acWAmmo[MAX_WEAPONS],
}
new Seifensive[MAX_PLAYERS][acInfo];
new hhtime[MAX_PLAYERS];
new HACKER = BAN;

new TOGGLE_WEAPON_DETECTION = ON;
new TOGGLE_JETPACK_DETECTION = ON;
// **FORWARDS** //
forward HackingDetected(playerid, reason[]);
forward WeaponDetection();
forward MoneyDetection();
forward HealthDetection(playerid);
forward HealthHackSuspection(suspectid);
forward Seifensive_OnInit();
forward Seifensive_OnPlayerSpawn(playerid);
forward Seifensive_OnPlayerConnect(playerid);
forward Seifensive_OnPlayerDisconnect(playerid, reason);
/*x---------------------------------CallBacks-------------------------------------x*/
public WeaponDetection()
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
	    if (IsPlayerConnected(i))
	    {
	        if (TOGGLE_JETPACK_DETECTION == ON) if (GetPlayerSpecialAction(i) == SPECIAL_ACTION_USEJETPACK)
			{
				HackingDetected(i, "Jetpack hacking");
				break;
			}
	        new w, a;
	        for(new s = 0; s < 13; s++)
	        {
         		GetPlayerWeaponData(i, s, w, a);
           		if (Seifensive[i][acWeapon][s] != w)
             	{
              		if (a > Seifensive[i][acWAmmo][w])
           			{
              			if ((a - Seifensive[i][acWAmmo][w]) >= MAX_AMMO_HACK)
              			{
                 			HackingDetected(i, "Weapon/Ammo hacking");
	                        continue;
                    	}
                   	}
             	}
	        }
	    }
	}
}

public MoneyDetection()
{
    for(new i = 0; i < MAX_PLAYERS; i++)
	{
	    if (IsPlayerConnected(i))
	    {
	        new cash = GetPlayerMoney(i);
	        if (cash > Seifensive[i][acCash])
	        {
	            if ((cash - Seifensive[i][acCash]) > MONEY_HACK)
	            {
	                HackingDetected(i, "Money hacking");
	            }
	        }
	    }
	}
}

public HealthDetection(playerid)
{
	if (IsPlayerConnected(playerid))
	{
	    KillTimer(hhtime[playerid]);
	    new Float:hp;
	    GetPlayerHealth(playerid, hp);
	    if (hp >= 100)
	    {
	        if (GetPlayerPing(playerid) < 700)
	        {
	            SetPlayerHealth(playerid, 99);
	            hhtime[playerid] = SetTimerEx("HealthDetection",1000,0,"d",playerid);
	            return;
	        }
	    }
	    hhtime[playerid] = SetTimerEx("HealthDetection",1000,0,"d",playerid);
	}
}

public HealthHackSuspection(suspectid)
{
	if (IsPlayerConnected(suspectid))
	{
	    new Float:hp;
	    GetPlayerHealth(suspectid, hp);
	    if (hp >= 100)
	    {
	        if (GetPlayerPing(suspectid) < 700) HackingDetected(suspectid, "Health hacking");
	    }
	}
}

public HackingDetected(playerid, reason[])
{
	new string[128];
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	if (HACKER == BAN)
	{
	    format(string, sizeof(string), "|- SeifensiveBot has banned %s | Reason: %s -|", name, reason);
	    SendClientMessageToAll(ADMIN_RED, string);
	    BanEx(playerid, reason);
	}
	else if (HACKER == KICK)
	{
	    format(string, sizeof(string), "|- SeifensiveBot has kicked %s | Reason: %s -|", name, reason);
	    SendClientMessageToAll(ADMIN_RED, string);
	    Kick(playerid);
	}
}

public Seifensive_OnInit()
{
    SetTimer("MoneyDetection",2000,1);
	if (TOGGLE_WEAPON_DETECTION == ON) SetTimer("WeaponDetection",5000,1);
}

public Seifensive_OnPlayerSpawn(playerid)
{
    hhtime[playerid] = SetTimerEx("HealthDetection",1000,0,"d",playerid);
}

public Seifensive_OnPlayerConnect(playerid)
{
    for(new i = 0; i <= 12; i++) Seifensive[playerid][acWeapon][i] = 0;
    for(new w = 0; w <= MAX_WEAPONS; w++) Seifensive[playerid][acWAmmo][w] = 0;
    Seifensive[playerid][acCash] = 0;
}

public Seifensive_OnPlayerDisconnect(playerid, reason)
{
    for(new i = 0; i <= 12; i++) Seifensive[playerid][acWeapon][i] = 0;
    for(new w = 0; w <= MAX_WEAPONS; w++) Seifensive[playerid][acWAmmo][w] = 0;
    Seifensive[playerid][acCash] = 0;
    KillTimer(hhtime[playerid]);
}

stock GetWeapSlotID(weaponid)
{
	switch (weaponid)
	{
	    case 0,1: return 0;
	    case 2..9: return 1;
	    case 10..15: return 10;
	    case 16..18,39: return 8;
	    case 22..24: return 2;
	    case 25..27: return 3;
	    case 28,29,32: return 4;
	    case 33,34: return 6;
	    case 35..38: return 7;
	    case 41..43: return 9;
	    case 44..46: return 11;
	    default: return 12;
	}
}

stock IsValidWeapon(weaponid)
{
	if (weaponid > 0) return 1;
	return 0;
}

Seifensive_GivePlayerMoney(playerid, amount)
{
	Seifensive[playerid][acCash] += amount;
	GivePlayerMoney(playerid, amount);
}

Seifensive_TakePlayerMoney(playerid, amount)
{
	Seifensive[playerid][acCash] -= amount;
	GivePlayerMoney(playerid, -amount);
}

Seifensive_GivePlayerWeapon(playerid, weaponid, ammos)
{
	Seifensive[playerid][acWeapon][GetWeapSlotID(weaponid)] = weaponid;
	Seifensive[playerid][acWAmmo][weaponid] = ammos;
	GivePlayerWeapon(playerid, weaponid, ammos);
}