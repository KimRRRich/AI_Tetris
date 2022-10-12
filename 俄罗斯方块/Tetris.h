#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<windows.h>
#include <iostream>
#include <sstream>
#define GRIDSIZE		35
#define STAGE_WIDTH		10
#define STAGE_HEIGHT	20
#define P1_STAGE_CORNER_X	156
#define P1_STAGE_CORNER_Y	174
#define P2_STAGE_CORNER_X	844
#define P2_STAGE_CORNER_Y	174
#define P1_NEXT_CORNER_X	587
#define P1_NEXT_CORNER_Y	125
#define P2_NEXT_CORNER_X	702
#define P2_NEXT_CORNER_Y	125
#define HOLD_CORNER_X       660
#define HOLD_CORNER_Y       275
#define DELAYVALUE			0.000000001
//#define LH_WEIGHT -45
//#define RE_WEIGHT 34
//#define RT_WEIGHT -32
//#define CT_WEIGHT -93
//#define NH_WEIGHT -79
//#define WS_WEIGHT -34


typedef enum PLAYROLE
{
	roleNONE,		//空
	rolePLAYER1,	//玩家1
	rolePLAYER2		//玩家2
};

typedef enum GridShape
{
	shapeI,		//I
	shapeS,		//S
	shapeZ,		//Z
	shapeT,		//T
	shapeL,		//L
	shapeJ,		//J
	shapeO,		//O
};

using namespace sf;

class Tetris
{
public:
	Tetris();
	~Tetris();
	Vector2i mCornPoint, nextSquareCornPoint, holdSquareCornPoint;			//舞台的左顶点坐标
	int role;
	int gridSize;					//块的大小
	int imgBGNo, imgSkinNo;
	Texture* tTiles;
	Texture tBackground, tButtons, tNum, tTimer, tCounter, tGameOver;				//创建纹理对象
	Sprite sBackground, sTiles, sButtons, sNum, sTimer, sCounter, sGameOver;		//创建精灵对象

	int Field[STAGE_HEIGHT][STAGE_WIDTH] = { 0 };
	bool flagtest = false;
	int temp[STAGE_HEIGHT][STAGE_WIDTH];
	Vector2i currentSquare[4], nextSquare[4], tempSquare[4], shadowSquare[4],algoSquare[4];
	int Figures[7][4] =
	{
		5,1,3,7,	//I
		5,2,4,7,	//S
		5,3,4,6,	//Z
		5,3,4,7,	//T
		5,2,3,7,	//L
		5,3,6,7,	//J
		5,2,3,4,	//O
	};
	int dx;
	bool rotate, hold, hardDrop, newShapeFlag, animationFlag;
	bool gameOver;
	int animationRow[4];
	float animationCtrlValue;
	int score;
	int clearline, cells;//消除行*贡献方块数评分
	RenderWindow* window;

	int colorNum, nextcolorNum, tempcolorNum;
	int currentShapeNum, nextShapeNum, tempShapeNum;
	static int holdcolorNum, holdShapeNum;
	static Vector2i holdSquare[4];
	int bag[7] = { 0 }, bagNum;
	float timer, delay;
	int horizonIndex[STAGE_WIDTH] = {19};
	int bestScore = 0x80000000;					// 最佳评分,初始值为最小INT值
	int needRotate = 0, needMoveLeft = 0;		// 每次到达最佳位置需要转动、向左移动次数
	double LH_WEIGHT;
	double RE_WEIGHT;
	double CT_WEIGHT;
	double RT_WEIGHT;
	double NH_WEIGHT;
	double WS_WEIGHT;


	void Initial(Texture *tex);
	void Input(Event event);
	void Logic();
	void Draw(RenderWindow *window);
	bool hitTest();
	void xMove();
	void yMove();
	void rotateFunc();
	void holdFunc();
	void hardDropFunc();
	int Bag7();
	void checkLine();
	void slowLoading();
	void shadow();

	void traditionLogic();
	void clearLine();
	void newShapeFunc();
	void animationFunc(int i);
	void isWin();


	int getNH();
	int getWS();
	void backupField();
	void backupSquare();
	int getCT();
	int getRT();
	int Dellacherie();
	int getLH();
	int getRE();
	void getMaxMin(Vector2i square[4], int& minX, int& maxX, int& minY, int& maxY);
	
	void AI_rotateFunc();
	int lineStatus(int i);
	
	
	void showmap();
	void fakedrop();
	void putTetrisToLeft();
	int getWidth();
	void getBestPositionAgain();
	bool AIhitTest();
	void AI_putTetrisToLeft();

};



class TetrisObject
{
public:
	TetrisObject();
	~TetrisObject();
	Vector2i  nextSquare[4], nextSquareCornPoint;
	static int holdcolorNum, holdShapeNum;
	static Vector2i holdSquare[4];
};
