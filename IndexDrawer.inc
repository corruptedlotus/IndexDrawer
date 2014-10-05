// ReAccess Dev. Studios - FreeDev Team
// SA-MP Serverside Include
// Name: IndexDrawer
// Version: 0.8 BETA
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

#define MAX_SLOTS 100
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
	keychar,
	keyname[64]
}

static Text:iTextDraw[MAX_IDRAWS] = { Text:NULL };
static iDrawTitle[MAX_IDRAWS][64];
static iDrawActive[MAX_SLOTS] = { NULL };
static iCancelKey[MAX_IDRAWS] = { KEY_FIRE };
static iDataSheet[MAX_IDRAWS][MAX_IDRAW_OPTIONS][KEY_LIST];
static iDrawColor[MAX_IDRAWS][3][12];

stock IndexDrawCreate(Float:x, Float:y, title[], Float:xsize, Float:ysize, backcolor, align, font, outline, titlecolor[], enumcolor[], textcolor[])
{
	new xT[64], Text:tmpTD, idr;
	format(xT, sizeof xT, "%s%s", titlecolor, title);
	tmpTD = TextDrawCreate(x, y, xT);
	TextDrawTextSize(tmpTD, xsize, ysize);
	TextDrawSetProportional(tmpTD, 1);
	TextDrawUseBox(tmpTD, 1);
	TextDrawBackgroundColor(tmpTD, backcolor);
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
	   	    format(iDrawTitle[i], strlen(iDrawTitle[i]), title);
	   		iTextDraw[i] = tmpTD;
	   		idr = i;
	   	}
	}
	format(iDrawColor[idr][0], strlen(iDrawColor[idr][0]), titlecolor);
	format(iDrawColor[idr][1], strlen(iDrawColor[idr][1]), enumcolor);
	format(iDrawColor[idr][2], strlen(iDrawColor[idr][2]), textcolor);
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

stock IndexDrawAddOption(idraw, keytitle[], keystate)
{
    for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
        if (iDataSheet[idraw][i][keychar] == keystate)
        {
            printf("IndexDrawer: iDraw Detected a Repeated KeyChar in iDraw #%i.", idraw);
            return -1;
        }
	for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
		if (iDataSheet[idraw][i][keychar] == NULL)
		{
		    iDataSheet[idraw][i][keychar] = keystate;
		    format(iDataSheet[idraw][i][keyname], strlen(iDataSheet[idraw][i][keyname]), keytitle);
		    // xSet
		    new xStr[256];
		    format(xStr, sizeof xStr, "%s%s~n~", iDrawColor[idraw][0], iDrawTitle[idraw]);
			for (new h = 0; h < MAX_IDRAW_OPTIONS; h++)
			{
			    if (iDataSheet[idraw][h][keychar] != NULL)
			    {
			    	strcat(xStr, iDrawColor[idraw][1]);
			    	strcat(xStr, iDataSheet[idraw][h][keychar], 1);
			    	strcat(xStr, ". ");
			    	strcat(xStr, iDrawColor[idraw][2]);
			    	strcat(xStr, iDataSheet[idraw][h][keyname]);
			    	strcat(xStr, "~n~");
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
	iDataSheet[idraw][index][keychar] = NULL;
	format(iDataSheet[idraw][index][keyname], strlen(iDataSheet[idraw][index][keyname]), "");
	// xSet
    new xStr[256];
    format(xStr, sizeof xStr, "%s%s~n~", iDrawColor[idraw][0], iDrawTitle[idraw]);
	for (new h = 0; h < MAX_IDRAW_OPTIONS; h++)
	{
	    if (iDataSheet[idraw][h][keychar] != NULL)
	    {
	    	strcat(xStr, iDrawColor[idraw][1]);
	    	strcat(xStr, iDataSheet[idraw][h][keychar], 1);
	    	strcat(xStr, ". ");
	    	strcat(xStr, iDrawColor[idraw][2]);
	    	strcat(xStr, iDataSheet[idraw][h][keyname]);
	    	strcat(xStr, "~n~");
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
	iDrawActive[playerid] = NULL;
}

stock IndexDrawDispose(idraw)
{
	for (new i = 0; i < MAX_SLOTS; i++)
	    if (iDrawActive[i] == idraw)
	        IndexDrawHideForPlayer(i, idraw);
	for (new i = 0; i < 3; i++)
	    format(iDrawColor[idraw][i], strlen(iDrawColor[idraw][i]), "");
	for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
	{
	    format(iDataSheet[idraw][i][keyname], strlen(iDataSheet[idraw][i][keyname]), "");
	    iDataSheet[idraw][i][keychar] = NULL;
	}
	format(iDrawTitle[idraw], strlen(iDrawTitle[idraw]), "");
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

hook OnPlayerDisconnect(playerid, reason)
{
	iDrawActive[playerid] = NULL;
	return 1;
}

hook OnPlayerText(playerid, text[])
{
	if (iDrawActive[playerid] != NULL)
	{
		new idi = iDrawActive[playerid];
	    for (new i = 0; i < MAX_IDRAW_OPTIONS; i++)
	        if (strcmp(text, iDataSheet[idi][i][keychar], true, 1) == 0)
			{
			    CallRemoteFunction("OnIndexDrawSelected", "iiiis", playerid, idi, i, iDataSheet[idi][i][keychar], iDataSheet[idi][i][keyname]);
			    IndexDrawHideForPlayer(playerid, iDrawActive[playerid]);
				break;
			}
		return 0;
	}
	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (iDrawActive[playerid] != NULL)
	    if (newkeys & iCancelKey[iDrawActive[playerid]])
	    {
	        CallRemoteFunction("OnIndexDrawCanceled", "ii", playerid, iDrawActive[playerid]);
   			IndexDrawHideForPlayer(playerid, iDrawActive[playerid]);
	    }
}
