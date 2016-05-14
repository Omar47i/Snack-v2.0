//=================================================================================
// File : Define.h
// Desc : This file includes the important definitions
//=================================================================================

// avoid multiple includes
#ifndef DEFINE_H
#define DEFINE_H

#include <Windows.h>

#define GAME_NAME "Snack v2.0"      //program name

#define CLIENT_WIDTH  1366          //client area width
#define CLIENT_HEIGHT 768           //client area height
#define GAME_WIDTH    1305          //game area width
#define GAME_HEIGHT   705           //game area height

#define START_X       30            //start points of the
#define START_Y       31            // game area
#define END_X         1335          //end points of the 
#define END_Y         736           //game area

#define NUM_VRLINES   87+1          //number of vertical lines
#define NUM_HRLINES   47+1          //number of horizontal lines

#define NODE_WIDTH  15              //the height and width
#define NODE_HEIGHT 15              //of the snack node

#define DIR_UP        1             //directions of the game
#define DIR_RIGHT     2
#define DIR_LEFT      3
#define DIR_DOWN      4

#define PIXELS_PER_RECT 15          //number of pixels per rectangle

#endif