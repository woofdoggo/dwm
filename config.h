/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 6;        /* vertical padding for statusbar */
static const char *fonts[]          = { "JetBrains Mono:size=14" };
static const char col_gray1[]       = "#24283b";
static const char col_gray2[]       = "#292e42";
static const char col_gray3[]       = "#545c7e";
static const char col_gray4[]       = "#7e7aa2";
static const char col_cyan[]        = "#89ddff";
static const char col_green[]       = "#9ece6a";
static const char col_black[]       = "#1f2335";
static const char col_red[]         = "#f7768e";
static const char col_yellow[]      = "#e0af68";
static const char col_white[]       = "#c0caf5";

static const char *colors[][3]      = {
	/*                    fg         bg          border   */
	[SchemeNorm]    =   { col_white, col_gray1,  col_gray2 },
	[SchemeSel]     =   { col_white, col_gray2,  col_cyan  },
};

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "⛶", "⛶", "⛶", "⛶" };
static const char *tagsel[][2] = {
	{ col_red,      "#000000" },
	{ col_yellow,   "#000000" },
	{ col_cyan,     "#000000" },
	{ col_green,    "#000000" },
};

static const unsigned int tagalpha[] = { OPAQUE, baralpha };
static const unsigned int ulinepad	= 6;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
    { NULL,       NULL,       NULL,       0,            False,       -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/bin/fish", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "kitty", NULL };

/* custom funcs */
static void fullscreen(const Arg *arg);

/* keys */
#include "movestack.c"
static Key keys[] = {
	/* modifier                     key         function        argument */

    // terminal
	{ MODKEY|ShiftMask,             XK_Return,  spawn,          {.v = termcmd } },

    // menu
	{ MODKEY,                       XK_Return,  spawn,          SHCMD("omnirun") },

    // lock
    { MODKEY,                       XK_l,       spawn,          SHCMD("lock") },

    // fullscreen screenshot
    { MODKEY,                       XK_Print,   spawn,          SHCMD("screenshot-clipboard") },

    // select screenshot
    { MODKEY|ShiftMask,             XK_Print,   spawn,          SHCMD("screenshot-clipboard -s") },

    // lower volume
    { 0,                            XF86XK_AudioLowerVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },                 

    // raise volume
    { 0,                            XF86XK_AudioRaiseVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },                 

    // focus next window
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },

    // focus prev window
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },

    // move window up stack
	{ MODKEY|ShiftMask,             XK_j,       movestack,      {.i = -1 } },

    // move window down stack
	{ MODKEY|ShiftMask,             XK_k,       movestack,      {.i = +1 } },

    // increase master size
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },

    // decrease master size
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },

    // view next tag
	{ MODKEY|ControlMask,           XK_Right,   viewnext,       {0} },

    // view previous tag
	{ MODKEY|ControlMask,           XK_Left,    viewprev,       {0} },

    // move window to next tag
	{ MODKEY|ShiftMask,             XK_Right,   tagtonext,      {0} },

    // move window to prev tag
	{ MODKEY|ShiftMask,             XK_Left,    tagtoprev,      {0} },

    // close window
	{ MODKEY,                       XK_c,       killclient,     {0} },

    // fullscreen window
    { MODKEY,                       XK_f,       fullscreen,     {0} },

    // quit dwm
	{ MODKEY|ShiftMask,             XK_q,       quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
};

/* function impls */
void
fullscreen(const Arg *arg) {
    Client *c = selmon->sel;
    if (c != NULL)
        setfullscreen(c, !c->isfullscreen);
}
