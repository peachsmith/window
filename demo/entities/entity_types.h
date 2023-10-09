#ifndef EG_DEMO_ENTITY_TYPES_H
#define EG_DEMO_ENTITY_TYPES_H

// A lits of all entity types in the application

//============================================================================
// Demo Entity Types
#define ENTITY_TYPE_PLAYER 0
#define ENTITY_TYPE_BLOCK 1
#define ENTITY_TYPE_BLOCK_BIG 2
#define ENTITY_TYPE_BLOCK_LONG 3
#define ENTITY_TYPE_THROUGHBLOCK_LONG 4
#define ENTITY_TYPE_BLOCK_MOVING 5
#define ENTITY_TYPE_BLOCK_SLOPE 6
#define ENTITY_TYPE_SIGN 7
#define ENTITY_TYPE_JIMBO 8
#define ENTITY_TYPE_BILLY 9
#define ENTITY_TYPE_HENRY 10
#define ENTITY_TYPE_TRANSITION 11

#define ENTITY_TYPE_PAUSE_MENU 12
#define ENTITY_TYPE_FISH_MENU 13
#define ENTITY_TYPE_INFO_MENU 14
#define ENTITY_TYPE_DEBUG_MENU 15
#define ENTITY_TYPE_SCENE_MENU 16
#define ENTITY_TYPE_INPUT_MENU 17

#define ENTITY_TYPE_DEMO_DIALOG 18
#define ENTITY_TYPE_INFO_DIALOG 19
#define ENTITY_TYPE_JIMBO_DIALOG 20
#define ENTITY_TYPE_SIGN_DIALOG 21

#define ENTITY_TYPE_FIREBALL 22

//============================================================================
// Toot n Sploot Entity Types
#define ENTITY_TYPE_FLOOR 23
#define ENTITY_TYPE_WALL 24
#define ENTITY_TYPE_HUD 25
#define ENTITY_TYPE_CORGI 26
#define ENTITY_TYPE_NOTE 27
#define ENTITY_TYPE_FOREST 28
#define ENTITY_TYPE_CRITTER 29
#define ENTITY_TYPE_MAIN_MENU 30

// This is the total number of entity types.
#define ENTITY_TYPE_MAX 31

// flags [0, 15]
#define ENTITY_FLAG_GROUND 0
#define ENTITY_FLAG_JUMP 1
#define ENTITY_FLAG_DOWN 2
#define ENTITY_FLAG_MOVE 3
#define ENTITY_FLAG_UPDATE 4
#define ENTITY_FLAG_SLOPE 5
#define ENTITY_FLAG_MIRROR 6
#define ENTITY_FLAG_INVINCIBLE 7
#define ENTITY_FLAG_PAUSE 8
#define ENTITY_FLAG_MENU 9

#endif