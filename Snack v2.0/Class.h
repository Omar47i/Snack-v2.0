// Class.h
// this file define the classes for the game

#ifndef CLASS_H
#define CLASS_H

#include <windows.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <strstream>
#include <time.h>
#include "Define.h"
#pragma comment(lib, "winmm.lib")
using namespace std;

struct S_Node{                      //Snack node
	POINT pt0;                      //the x and y coordinates
	POINT pt1;
};

class Snack
{
private:
	S_Node snack[300];              //snack can grow up to 300 nodes
	POINT tail;
	POINT head;                     //current coordinates of the snack head

	POINT food_pt;                  //food coordinates
	int LEN;                        //length of the snack nodes
	int score;                      //score of the snack
	char buff[256];
	int curr_dir;                   //current direction of the snack

	//check if the snack eated the food
	void check_for_food();

	// checks if the snack goes out of the wall boundries
	void check_for_bounds();

public:
	Snack();                        //default constructor
	//display snack based on the user's direction
	void display_snack(HDC hbbDC);

	// set the direction of the snack
	void set_Dir(const int& dir);

	// set food coordinates
	void setFoodPt(const POINT& food);

	// see if the snack has hitted his self
	bool check_for_snack_hit(HDC hbbDC);

	// get the point of the food
	POINT get_food_pt();
};

class GameArea
{
private:
	HBRUSH hBG;                    //handle to background brush
	HBRUSH hGA;                    //handle to game area
	HBRUSH hOldBrush;              //handle to old brush
	LOGPEN glp;

	HPEN hLine;
	HPEN hOldPen;

public:
	GameArea();                    //default constructor
	~GameArea();

	//draw the game area
	void Draw_Game_Area(HDC hbbDC); 
};

class Food
{
private:
	POINT food;                    //coordinates of the
	LOGBRUSH lb;                   //for cutimizing the food color

	HBRUSH   hbrFood;
	HBRUSH   hOldBrush;
	char buff[256];

public:
	Food();
	~Food();

	void display_food(HDC hbbDC);  //display food

	void setFoodPt();
	void setFoodPt(const POINT& pt);
	void erase_food();
	POINT get_food_pt();
};

#endif