// Class.cpp 
// Implement the class methods
#include "Class.h"

// Snack Class /////////////////////////////////////////////////////////////////////
Snack::Snack()
{
	// initialize Snack variables
	score    = 0;
	curr_dir = DIR_RIGHT;
	LEN      = 3;
	head.x   = START_X + (PIXELS_PER_RECT * 40);
	head.y   = (START_Y-1) + (PIXELS_PER_RECT * 20);

	// initializes the start and end points of the head of snack
	snack[0].pt0.x = head.x;
	snack[0].pt0.y = head.y;
	snack[0].pt1.x = head.x + NODE_WIDTH;
	snack[0].pt1.y = head.y + NODE_HEIGHT;
	// initializes the second node of the snack
	snack[1].pt0.x = snack[0].pt0.x - NODE_WIDTH;
	snack[1].pt0.y = snack[0].pt0.y;
	snack[1].pt1.x = snack[0].pt0.x;
	snack[1].pt1.y = snack[0].pt1.y;
	// initializes the third node of the snack
	snack[2].pt0.x = snack[1].pt0.x - NODE_WIDTH;
	snack[2].pt0.y = snack[1].pt0.y;
	snack[2].pt1.x = snack[1].pt0.x;
	snack[2].pt1.y = snack[1].pt1.y;
	// set the tail node to the final node in the array
	tail.x = snack[2].pt0.x;
	tail.y = snack[2].pt0.y;

	food_pt.x = 0;
	food_pt.y = 0;
}

void Snack::set_Dir(const int& dir)
{
	switch( dir )
	{
	// if the user pressed the 'W' key to go up
	case DIR_UP:
		if(curr_dir == DIR_DOWN)
			break;
		else
			curr_dir = DIR_UP;
		break;

	// if the user pressed the 'S' key to go down
	case DIR_DOWN:
		if(curr_dir == DIR_UP)
			break;
		else
			curr_dir = DIR_DOWN;
		break;

	// if the user pressed the 'A' key to go left
	case DIR_LEFT:
		if(curr_dir == DIR_RIGHT)
			break;
		else
			curr_dir = DIR_LEFT;
		break;

	// if the user pressed the 'D' key to go right
	case DIR_RIGHT:
		if(curr_dir == DIR_LEFT)
			break;
		else
			curr_dir = DIR_RIGHT;
	}
}

void Snack::display_snack(HDC hbbDC)
{
	// save the tail node
	tail.x = snack[LEN-1].pt0.x;
	tail.y = snack[LEN-1].pt0.y;
	// draw each node
	for(int j=LEN-1; j>=1; j--)
	{
		snack[j] = snack[j-1];
	}

	switch( curr_dir )
	{
	case DIR_UP:
		head.y -= NODE_HEIGHT;
		break;

	case DIR_DOWN:
		head.y += NODE_HEIGHT;
		break;

	case DIR_RIGHT:
		head.x += NODE_WIDTH;
		break;

	case DIR_LEFT:
		head.x -= NODE_WIDTH;
	}

	// check if the snack has goes out of the game bounds
	check_for_bounds();

	// check if the snack ated the food
	check_for_food();
	// assign to first node the new head coordinates
	snack[0].pt0.x = head.x;
	snack[0].pt0.y = head.y;
	snack[0].pt1.x = head.x + NODE_WIDTH;
	snack[0].pt1.y = head.y + NODE_HEIGHT;

	// draw the head of snack alone
	Rectangle(hbbDC, snack[0].pt0.x, snack[0].pt0.y, snack[0].pt1.x, snack[0].pt1.y);
	// draw the body of snack
	for( int d=1; d<LEN; d++ )
		Ellipse(hbbDC, snack[d].pt0.x, snack[d].pt0.y,
			             snack[d].pt1.x, snack[d].pt1.y);
}

void Snack::setFoodPt(const POINT& food)
{  food_pt = food;  }

void Snack::check_for_food()
{
	if( head.x == food_pt.x && head.y == food_pt.y )
	{
		// increase the length of the snack
		LEN++;
		snack[LEN-1].pt0.x = tail.x;
		snack[LEN-1].pt0.y = tail.y;
		snack[LEN-1].pt1.x = tail.x + NODE_WIDTH;
		snack[LEN-1].pt1.y = tail.y + NODE_HEIGHT;
		food_pt.x = 0;  food_pt.y = 0;

		PlaySoundA( "chomp.wav", 0, SND_ASYNC);

		score += 50;
	}
}

void Snack::check_for_bounds()
{
	if( head.x < START_X )
		head.x = END_X - NODE_WIDTH;

	if( head.x > END_X-15 )
		head.x = START_X;

	if( head.y < (START_Y-5) )
		head.y = END_Y - NODE_HEIGHT - 1;

	if( head.y > END_Y-15 )
		head.y = START_Y - 1;
}

POINT Snack::get_food_pt()
{ return food_pt; }

bool Snack::check_for_snack_hit(HDC hbbDC)
{
	for(int i=3; i<LEN; i++)
	{
		if( head.x == snack[i].pt0.x && head.y == snack[i].pt0.y )
		{
			ostrstream ostr(buff, 256);
			ostr << "Your Score is " << score << ends;
			TextOutA(hbbDC, (GAME_WIDTH/2) - 80,
				            10, buff, strlen(buff));
			return true;
		}
	}
	return false;
}

// End Snack Class //////////////////////////////////////////////////////////////////

// GameArea Class ///////////////////////////////////////////////////////////////////
GameArea::GameArea()
{
	// set the background brush to black
	hBG = (HBRUSH)GetStockObject(BLACK_BRUSH);

	// set the game area brush to drak gray
	hGA = (HBRUSH)GetStockObject(DKGRAY_BRUSH);

	// initialize the LOGPEN object
	glp.lopnColor   = RGB(100, 100, 100);
	glp.lopnStyle   = PS_SOLID;
	glp.lopnWidth.x = 1;
	glp.lopnWidth.y = 1;
	hLine = CreatePenIndirect( &glp );

	hOldPen   = 0;
	hOldBrush = 0;
}

GameArea::~GameArea()
{
	DeleteObject(hBG);
	DeleteObject(hGA);
}

void GameArea::Draw_Game_Area(HDC hbbDC)
{
	// draw the background
	hOldBrush = (HBRUSH)SelectObject(hbbDC, hBG);
	Rectangle(hbbDC, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);

	// draw the foreground
	SelectObject(hbbDC, hGA);
	Rectangle(hbbDC, START_X, START_Y, END_X, END_Y);

	// draw the horizontal and vertical lines
	hOldPen = (HPEN)SelectObject(hbbDC, hLine);

	// draw the vertical lines
	for( int j=0; j<NUM_VRLINES; j++ )
	{
		MoveToEx(hbbDC,
				 START_X + (j*PIXELS_PER_RECT),
				 START_Y,
				 0);
		LineTo(hbbDC, 
			   START_X + (j*PIXELS_PER_RECT),
			   END_Y);
	}
	// Draw horizontal lines
	for( int i=0; i<NUM_HRLINES; i++ )
	{
		MoveToEx(hbbDC,
			     START_X,
				 START_Y + (i*PIXELS_PER_RECT),
				 0);
		LineTo(hbbDC,
			   END_X,
			   START_Y + (i*PIXELS_PER_RECT));
	}

	SelectObject(hbbDC, hOldBrush);
	SelectObject(hbbDC, hOldPen);
}

// End GameArea /////////////////////////////////////////////////////////////////////

// Food Class ///////////////////////////////////////////////////////////////////////

Food::Food()
{
	// initialize randome number generator
	srand( time(NULL) );

	lb.lbColor = RGB(255, 255, 255);
	lb.lbStyle = BS_SOLID;
	lb.lbHatch = NULL;

	hbrFood   = (HBRUSH)CreateBrushIndirect( &lb );
	hOldBrush = 0;

	// there is no current food displayed
	food.x = 0;
	food.y = 0;
}

Food::~Food()
{
	DeleteObject(hbrFood);
}

void Food::display_food(HDC hbbDC)
{
	if( food.x != 0 && food.y != 0 )
	{
		hOldBrush = (HBRUSH)SelectObject(hbbDC, hbrFood);
		
		Ellipse(hbbDC, food.x, food.y, food.x + NODE_WIDTH, food.y + NODE_HEIGHT);
		
		SelectObject(hbbDC, hOldBrush);
	}
}

void Food::setFoodPt()
{
	food.x = (rand() % GAME_WIDTH) + START_X;
	food.y = (rand() % GAME_HEIGHT) + START_Y;

	while( (food.x % NODE_WIDTH) != 0 )
		food.x--;
	while( (food.y % NODE_HEIGHT) != 0 )
		food.y--;
	PlaySoundA( "appear2.wav", 0, SND_ASYNC);
}

void Food::setFoodPt(const POINT& pt)
{
	food = pt;
}

POINT Food::get_food_pt()
{ return food; }

void Food::erase_food()
{ food.x = 0; }