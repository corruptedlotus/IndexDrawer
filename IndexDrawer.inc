// ReAccess Dev. Studios - FreeDev Team
// SA-MP Serverside Include
// Name: IndexDrawer
// Version: 0.9 RC
// Author: [MMM] Anna SB
// Description: Simple Index-Based Text-To-Go Selection Menus. Simply With TextDraws, Fully Customizable.

#include <a_samp>
#include "YSI\y_hooks"

#define NULL '\0'

// MAX. INDEXDRAWS PER PLAYER
#if defined MAX_IDRAWS
#else
#define MAX_IDRAWS 100
#endif

// #define MAX_SLOTS 20
// MAX. PLAYER SLOTS
#if defined MAX_SLOTS
#else
#error MAX_SLOTS is not defined before <IndexDrawer>. To prevent performance impacts, define MAX_SLOTS with value of your max players.
#endif

// MAX. INDEXDRAW SELECTION SLOTS
#define MAX_IDRAW_OPTIONS 20

// Callbacks
forward OnIndexDrawCanceled(playerid, idraw);
forward OnIndexDrawSelected(playerid, idraw, index, keystate, keytitle[]);

enum KEY_LIST
{
	keychar[2],
	keyname[20]
}

new Text:iTextDraw[MAX_IDRAWS] = { Text:NULL };
new iDrawTitle[MAX_IDRAWS][64];
new iDrawActive[MAX_SLOTS] = { -1 };
new iCancelKey[MAX_IDRAWS] = { KEY_FIRE };
new iDataSheet[MAX_IDRAWS][MAX_IDRAW_OPTIONS][KEY_LIST];
new iDrawColor[MAX_IDRAWS][3][12];

public OnScriptInit()
{
	for (new i = 0; i < MAX_SLOTS; i++) iDrawActive[i] = -1;
	for (new i = 0; i < MAX_IDRAWS; i++)
	{
	    iCancelKey[i] = KEY_FIRE;
	    for (new f = 0; f < MAX_IDRAW_OPTIONS; f++)
	    {
	        format(iDataSheet[i][f][keychar], 2, "");
	        format(iDataSheet[i][f][keyname], 20, "");
	    }
	}
}

stock IndexDrawCreate(Float:x, Float:y, title[], Float:xsize, Float:ysize, backcolor, align, font, outline, titlecolor[], enumcolor[], textcolor[])
{
	new xT[64], Text:tmpTD, idr;
	format(xT, sizeof xT, "%s%s", titlecolor, title);
	tmpTD = TextDrawCreate(x, y, xT);
	TextDrawTextSize(tmpTD, xsize, ysize);
	TextDrawSetProportional(tmpTD, 1);
	TextDrawUseBox(tmpTD, 1);
	TextDrawBackgroundColor(tmpTD, backcolor);
	TextDrawBoxColor(tmpTD, backcolor);
	TextDrawAlignment(tmpTD, align);
	TextDrawFont(tmpTD, font);
	TextDrawSetOutline(tmpTD, outline);
	TextDrawHideForAll(tmpTD);
	for (new i = 0; i <= MAX_IDRAWS; i++)
	{
	    if (i == MAX_IDRAWS)
		{
			print("IndexDrawer: iDraw Buffer Overflow, Cannot Hold Any More iDraws. Try Increasing MAX_IDRAWS.");
			TextDrawDestroy(tmpTD);
			return -1;
		}
	    if (iTextDraw[i] == Text:NULL)
	   	{
	   	    format(iDrawTitle[i], sizeof(iDrawTitle[]), title);
	   		iTextDraw[i] = tmpTD;
	   		idr = i;
	   		break;
	   	}
	}
	format(iDrawColor[idr][0], sizeof(iDrawColor[][]), titlecolor);
	format(iDrawColor[idr][1], sizeof(iDrawColor[][]), enumcolor);
	format(iDrawColor[idr][2], sizeof(iDrawColor[][]), textcolor);
	return idr;
}

stock IndexDrawLetterSize(idraw, Float:x, Float:y)
{
	TextDrawLetterSize(iTextDraw[idraw], x, y);
}

stock IndexDrawCancelKey(idraw, keystate)
{
	iCancelKey[idraw] = keystate;
}

stock IndexDrawAddOption(idraw, keytitle[], keystate[])
{
    for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
        if (strcmp(iDataSheet[idraw][i][keychar], keystate) == 0 && strlen(iDataSheet[idraw][i][keychar]) != 0)
        {
            printf("IndexDrawer: iDraw Detected a Repeated KeyChar in iDraw #%i.", idraw);
            return -1;
        }
	for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
		if (strcmp(iDataSheet[idraw][i][keychar], "") == 0)
		{
		    format(iDataSheet[idraw][i][keychar], 2, keystate);
		    format(iDataSheet[idraw][i][keyname], 20, keytitle);
		    // xSet
		    new xStr[256];
		    format(xStr, sizeof xStr, "%s%s~n~", iDrawColor[idraw][0], iDrawTitle[idraw]);
		    // printf("T. %s", xStr);
			for (new h = 0; h < MAX_IDRAW_OPTIONS; h++)
			{
			    if (iDataSheet[idraw][h][keychar] != NULL)
			    {
			    	strcat(xStr, iDrawColor[idraw][1]);
			    	// printf("%i-1. %s", h, xStr);
			    	strcat(xStr, iDataSheet[idraw][h][keychar]);
			    	// printf("%i-2. %s", h, xStr);
			    	strcat(xStr, ". ");
			    	// printf("%i-3. %s", h, xStr);
			    	strcat(xStr, iDrawColor[idraw][2]);
			    	// printf("%i-4. %s", h, xStr);
			    	strcat(xStr, iDataSheet[idraw][h][keyname]);
			    	// printf("%i-5. %s", h, xStr);
			    	strcat(xStr, "~n~");
			    	// printf("%i-6. %s", h, xStr);
			    }
			}
		    TextDrawSetString(iTextDraw[idraw], xStr);
		    // ----
		    return i;
		}
    printf("IndexDrawer: iDraw Options Overflow for iDraw #%i. Maximum is 20 Per iDraw.", idraw);
    return -1;
}

stock IndexDrawRemoveOption(idraw, index)
{
	format(iDataSheet[idraw][index][keychar], 2, "");
	format(iDataSheet[idraw][index][keyname], 20, "");
	// xSet
    new xStr[256];
    format(xStr, sizeof xStr, "%s%s~n~", iDrawColor[idraw][0], iDrawTitle[idraw]);
    // printf("T. %s", xStr);
	for (new h = 0; h < MAX_IDRAW_OPTIONS; h++)
	{
	    if (iDataSheet[idraw][h][keychar] != NULL)
	    {
	    	strcat(xStr, iDrawColor[idraw][1]);
	    	// printf("%i-1. %s", h, xStr);
	    	strcat(xStr, iDataSheet[idraw][h][keychar]);
	    	// printf("%i-2. %s", h, xStr);
	    	strcat(xStr, ". ");
	    	// printf("%i-3. %s", h, xStr);
	    	strcat(xStr, iDrawColor[idraw][2]);
	    	// printf("%i-4. %s", h, xStr);
	    	strcat(xStr, iDataSheet[idraw][h][keyname]);
	    	// printf("%i-5. %s", h, xStr);
	    	strcat(xStr, "~n~");
	    	// printf("%i-6. %s", h, xStr);
	    }
	}
    TextDrawSetString(iTextDraw[idraw], xStr);
    // ----
}

stock IndexDrawShowForPlayer(playerid, idraw)
{
	TextDrawShowForPlayer(playerid, Text:iTextDraw[idraw]);
	iDrawActive[playerid] = idraw;
}

stock IndexDrawHideForPlayer(playerid, idraw)
{
    TextDrawHideForPlayer(playerid, Text:iTextDraw[idraw]);
	iDrawActive[playerid] = -1;
}

stock IndexDrawDispose(idraw)
{
	for (new i = 0; i < MAX_SLOTS; i++)
	    if (iDrawActive[i] == idraw)
	        IndexDrawHideForPlayer(i, idraw);
	for (new i = 0; i < 3; i++)
	    format(iDrawColor[idraw][i], sizeof(iDrawColor[][]), "");
	for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
	{
	    format(iDataSheet[idraw][i][keyname], 20, "");
	    format(iDataSheet[idraw][i][keychar], 2, "");
	}
	format(iDrawTitle[idraw], sizeof(iDrawTitle[]), "");
	iCancelKey[idraw] = KEY_FIRE;
	TextDrawDestroy(iTextDraw[idraw]);
	iTextDraw[idraw] = Text:NULL;
	return 1;
}

stock GetPlayerActiveIndexDraw(playerid)
{
	return iDrawActive[playerid];
}

stock IsPlayerIndexDrawActive(playerid, idraw)
{
	return iDrawActive[playerid] == idraw;
}

hook OnPlayerDisconnect00(playerid, reason)
{
	iDrawActive[playerid] = -1;
	return 1;
}

hook OnGameModeInit()
{
	// printf("%d CancelKey", iCancelKey[5]);
}

hook OnPlayerText00(playerid, text[])
{
	if (iDrawActive[playerid] != -1)
	{
		new idi = iDrawActive[playerid];
	    for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
	        if (strcmp(text, iDataSheet[idi][i][keychar], true, strlen(iDataSheet[idi][i][keychar])) == 0)
			{
			    CallRemoteFunction("OnIndexDrawSelected", "iiiis", playerid, idi, i, iDataSheet[idi][i][keychar], iDataSheet[idi][i][keyname]);
       			IndexDrawHideForPlayer(playerid, iDrawActive[playerid]);
				break;
			}
		return 0;
	}
	return 1;
}

hook OnPlayerKeyStateChange00(playerid, newkeys, oldkeys)
{
	if (iDrawActive[playerid] != -1)
	    if (newkeys & iCancelKey[iDrawActive[playerid]])
	    {
	        CallRemoteFunction("OnIndexDrawCanceled", "ii", playerid, iDrawActive[playerid]);
   			IndexDrawHideForPlayer(playerid, iDrawActive[playerid]);
	    }
	return 1;
}
