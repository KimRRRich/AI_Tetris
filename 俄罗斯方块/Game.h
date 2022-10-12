#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<windows.h>
#include <iostream>
#include <sstream>
#include "Tetris.h"

#define B_START_CORNER_X	621
#define B_START_CORNER_Y	763
#define B_HOLD_CORNER_X		621
#define B_HOLD_CORNER_Y		822
#define B_LEFT_CORNER_X		70
#define B_LEFT_CORNER_Y		460
#define B_RIGHT_CORNER_X	1295
#define B_RIGHT_CORNER_Y	460
#define P1_SCORE_CORNER_X	320
#define P1_SCORE_CORNER_Y	119
#define P2_SCORE_CORNER_X	1020
#define P2_SCORE_CORNER_Y	119
#define INFO_CORNER_X		540
#define INFO_CORNER_Y		440

typedef enum ButtonState
{
	Continue_Dark,		//继续暗
	Continue_Light,		//继续亮
	Hold_Dark,			//暂停暗
	Hold_Light,			//暂停亮
	Close_Dark,			//结束暗
	Close_Light,		//结束亮
	Start_Dark,			//开始暗
	Start_Light,		//开始亮
};

using namespace sf;
using namespace std;

class Game
{
public:
	RenderWindow window;
	Game();
	~Game();
	Tetris player1, player2;
	bool gameOver, gameQuit;
	Clock clock;
	int window_width, window_height, stage_width, stage_height;
	bool isGameBegin, isGameHold;			//游戏是否开始
	int isGameOverState;					//游戏结束的状态
	Vector2i mCornPoint;					//游戏区域位置
	int gridSize;							//块的大小
	int imgBGNo, imgSkinNo, imgSetNo;

	Texture tBackground, tTiles, tButtons, tSwitcher, tFrame, tCover, tScore, tGameOver;		//创建纹理对象
	Sprite sBackground, sTiles, sButtons, sSwitcher, sFrame, sCover, sScore, sGameOver;			//创建精灵对象
	Font font;
	Text text;
	
	IntRect ButtonRectStart, ButtonRectHold, ButtonRectLeft, ButtonRectRight;
	int ButtonState_Start, ButtonState_Hold;

	SoundBuffer sbWin, sbBoom;
	Sound soundWin, soundBoom;
	Music bkMusic;
	Clock gameClock, mouseClickTimer;
	bool test = false;

	void gameInitial(double LH_WEIGHT, double RE_WEIGHT, double CT_WEIGHT, double RT_WEIGHT, double NH_WEIGHT, double WS_WEIGHT);
	void LoadMediaData();
	void gameInput();
	void gameLogic();
	void gameDraw();
	void DrawButton();
	int gameRun(double LH_WEIGHT, double RE_WEIGHT, double CT_WEIGHT, double RT_WEIGHT, double NH_WEIGHT, double WS_WEIGHT,int scale);

	void DrawResults();
	void TextOut();
};

using namespace std;

#define  PI    3.14159265358979323846

//遗传算法参数，种群规模（0~100）、繁殖代数、函数变量个数、交叉概率、编译概率
# define GROUP_SCALE    100   
# define MAX_GENS       50
# define N_VARS         6
# define P_MATING       0.8
# define P_MUTATION     0.15

Game GameGA;

struct Individual
{
	double Xn[N_VARS];      //存放变量值
	double Fitness;         //适应值
	double ReFitness;       //适应值概率密度
	double SumFitness;      //累加分布，为轮盘转
};
struct X_Range
{
	double Upper;           //变量的上界取值
	double Lower;           //变量的下界取值
};

template<typename T>
T randT(T Lower, T Upper); //产生任意类型随机数函数

void crossover(int& seed);
void elitist();        //基因保留
void evaluate();

void initGroup(int& seed);

void selectBest();
void mutate(int& seed);

double r8_uniform_ab(double a, double b, int& seed);
int i4_uniform_ab(int a, int b, int& seed);

void report(int Xnration);
void selector(int& seed);
//void showTime();
void Xover(int one, int two, int& seed);

