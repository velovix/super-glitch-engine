/* Super Glitch Engine Client
 * constants.h
 * Copyright Tyler Compton 2013
 *
 * Declares universal constants to be used throughout the entirety of the
 * Client code. The same values are used for these constants in other parts of
 * SGE as well.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Basic Object Constants
#define NPC				1
#define ENVIRON			2
#define ALL				255

// Directional Constants
#define RIGHT			1
#define LEFT			2
#define UP				3
#define DOWN			4

// Types
#define T_NORMAL		0
#define T_FIRE			1
#define T_WATER			2
#define T_ROCK			3
#define T_GROUND		4
#define T_ELECTRIC		5
#define T_FLYING		6
#define T_ICE			7
#define T_DRAGON		8
#define T_GHOST			9
#define T_PSYCHIC		10
#define T_FIGHTING		11
#define T_POISON		12
#define T_GRASS			13
#define T_BUG			14
#define T_NONE			15

// Move Results
#define RES_HYPER		1
#define RES_SUPER		2
#define RES_NORMAL		3
#define RES_NOTVERY		4
#define RES_HARDLY		5
#define RES_NONE		6

// Modes
#define SES_OVERWORLD	1
#define SES_BATTLE		3

// Screen Stuff
#define SCREEN_ZOOM		1
#define SCREEN_WIDTH	160*SCREEN_ZOOM
#define SCREEN_HEIGHT	144*SCREEN_ZOOM

// Maximums
#define MAX_NPCS		10
#define MAX_DOORS		10
#define MAX_WINDOWS		10
#define MAX_MOVES		100
#define MAX_MONSTERS	151
#define MAX_TYPES		50

// Battle Types
#define BAT_WILD		1
#define BAT_TRAINER		2

// Battle Steps
#define BATS_PRE		0
#define BATS_WPA		1
#define BATS_GO			2
#define BATS_SEL		3
#define BATS_ATT		4
#define BATS_RESULT		5
#define BATS_FINISH		6

// Window Selections
#define WSEL_NONE		-1
#define WSEL_BACK		255

// Misc Constants
#define PHYS_DELAY		25
#define WIND_WIDTH		10
#define WIND_HEIGHT	 	9
#define BLOCK_SIZE		16
#define CHAR_SIZE		8
#define PKMN_SIZE		64
#define CHAR_SPEED		BLOCK_SIZE

// Map Sprites
#define MOFF			0
#define GRASS			MOFF
#define ONE_POLL		MOFF+1
#define DARK_DIRT		MOFF+2
#define TREE			MOFF+3
#define SIGN			MOFF+4
#define WATER			MOFF+5
#define R_SHORE			MOFF+6
#define L_SHORE			MOFF+7
#define U_SHORE			MOFF+8
#define U_R_SHORE		MOFF+9
#define U_L_SHORE		MOFF+10
#define TWO_POLL		MOFF+11
#define CLEAR			MOFF+12
#define GRASS_OVER		MOFF+13
#define WORD_BUBBLE		MOFF+14
#define HOUSE_DOOR		MOFF+18
#define SLIGHT_DIRT		MOFF+24
#define HOUSE_FLOOR		MOFF+30
#define HOUSE_MAT		MOFF+36
#define BLACK			MOFF+42

// Player Sprites
#define PSOFF			0
#define C_PLAYER		PSOFF
#define C_GARY			PSOFF+10
#define C_SCI			PSOFF+20
#define C_GIRL			PSOFF+30
#define C_FATMAN		PSOFF+40
#define C_MOM			PSOFF+50
#define C_SIGN			PSOFF+60

// Character Sprites
#define COFF			0
#define LETTERS_C		COFF
#define LETTERS_L		COFF+26
#define NUMBERS			COFF+52
#define SPACE			COFF+62
#define PERIOD			COFF+63
#define EXCLAMATION		COFF+64
#define QUESTION		COFF+65
#define QUOTATION		COFF+66
#define CURRICON		COFF+67
#define SELARROW		COFF+68
#define WCORNER_TL		COFF+69
#define WCORNER_TR		COFF+70
#define WCORNER_BR		COFF+71
#define WCORNER_BL		COFF+72
#define WHORIZ			COFF+73
#define WVERT			COFF+74
#define SLASH			COFF+75
#define LINE_V			COFF+76
#define LINE_BL			COFF+77
#define LINE_H			COFF+78
#define LINE_RARROW		COFF+79
#define LINE_LARROW		COFF+80
#define LINE_BR			COFF+81
#define LEVEL			COFF+82
#define BATTLE_H		COFF+83
#define BATTLE_P		COFF+84
#define HPBAR_L			COFF+85
#define HPBAR_MID		COFF+86
#define HPBAR_R			COFF+87

// Character Exceptions
#define CE_NEWLINE		200
#define CE_OPTION		201
#define CE_ENDSTR		202
#define CE_FULLSCROLL	203

// AI Patterns
#define AI_NOTHING		0
#define AI_TURN			1
#define AI_STURN		2
#define AI_BANDF		3
#define AI_WANDER		4
#define AI_DEST			5

// Window Types
#define WT_PLAINTXT		1
#define WT_OPTIONS		2

// Door Types
#define DT_WALKINTO		1
#define DT_COLLIDE		2

// Move Events
#define ME_DAMAGE		1
#define ME_POISON		2

#define ME_TARGET		1
#define ME_USER			2

// Pokemon
#define PK_NIDOQUEEN	0
#define PK_CHARIZARD	1
#define PK_BLASTOISE	2

#endif