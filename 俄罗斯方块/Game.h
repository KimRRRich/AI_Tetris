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
	Continue_Dark,		//������
	Continue_Light,		//������
	Hold_Dark,			//��ͣ��
	Hold_Light,			//��ͣ��
	Close_Dark,			//������
	Close_Light,		//������
	Start_Dark,			//��ʼ��
	Start_Light,		//��ʼ��
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
	bool isGameBegin, isGameHold;			//��Ϸ�Ƿ�ʼ
	int isGameOverState;					//��Ϸ������״̬
	Vector2i mCornPoint;					//��Ϸ����λ��
	int gridSize;							//��Ĵ�С
	int imgBGNo, imgSkinNo, imgSetNo;

	Texture tBackground, tTiles, tButtons, tSwitcher, tFrame, tCover, tScore, tGameOver;		//�����������
	Sprite sBackground, sTiles, sButtons, sSwitcher, sFrame, sCover, sScore, sGameOver;			//�����������
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

//�Ŵ��㷨��������Ⱥ��ģ��0~100������ֳ��������������������������ʡ��������
# define GROUP_SCALE    100   
# define MAX_GENS       50
# define N_VARS         6
# define P_MATING       0.8
# define P_MUTATION     0.15

Game GameGA;

struct Individual
{
	double Xn[N_VARS];      //��ű���ֵ
	double Fitness;         //��Ӧֵ
	double ReFitness;       //��Ӧֵ�����ܶ�
	double SumFitness;      //�ۼӷֲ���Ϊ����ת
};
struct X_Range
{
	double Upper;           //�������Ͻ�ȡֵ
	double Lower;           //�������½�ȡֵ
};

template<typename T>
T randT(T Lower, T Upper); //���������������������

void crossover(int& seed);
void elitist();        //������
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

