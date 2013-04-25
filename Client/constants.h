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

// Battle Types
#define BAT_WILD		1
#define BAT_TRAINER		2

// Battle Steps
#define BATS_PRE		0
#define BATS_WPA		1
#define BATS_GO			2
#define BATS_SEL		3
#define BATS_FOEATT		4
#define BATS_PATT		5
#define BATS_FINISH		6

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

// Player Sprites
#define PSOFF			0
#define C_PLAYER		PSOFF
#define C_GARY			PSOFF+10
#define C_SCI			PSOFF+20
#define C_GIRL			PSOFF+30
#define C_FATMAN		PSOFF+40
#define C_MOM			PSOFF+50

// Character Sprites
#define COFF			0
#define LETTERS_C		COFF
#define LETTERS_L		COFF+26
#define NUMBERS			COFF+52
#define SPACE			COFF+62
#define SELARROW		COFF+63
#define WCORNER_TL		COFF+64
#define WCORNER_TR		COFF+65
#define WCORNER_BR		COFF+66
#define WCORNER_BL		COFF+67
#define WHORIZ			COFF+68
#define WVERT			COFF+69

// AI Patterns
#define AI_TURN			1
#define AI_STURN		2
#define AI_BANDF		3
#define AI_WANDER		4

// Window Types
#define WT_PLAINTXT		1
#define WT_OPTIONS		2

// Moves
#define PK_NOMOVE		0
#define PK_TACKLE		1
#define PK_EXPLOSION	2

// Pokemon
#define PK_NIDOQUEEN	0
#define PK_CHARIZARD	1
#define PK_BLASTOISE	2

#endif