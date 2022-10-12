#include "Game.h"

# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <cstring>
//#include "GA.h"

Game::Game()
{
	window_width = 1350;
	window_height = 1000;
	isGameBegin = false;

	imgBGNo = 1;
	imgSkinNo = 1;
	window.create(VideoMode(window_width, window_height), L"俄罗斯方块『双人版』");
}

Game::~Game()
{

}

void Game::gameInitial(double LH_WEIGHT, double RE_WEIGHT, double CT_WEIGHT, double RT_WEIGHT, double NH_WEIGHT, double WS_WEIGHT)
{
	window.setFramerateLimit(800);
	LoadMediaData();
	srand(time(NULL));

	//isGameBegin = false;
	//gameOver = false;
	isGameHold = false;
	ButtonState_Start = Start_Dark;
	ButtonState_Hold = Hold_Dark;
	player1.role = rolePLAYER1;		//定义Tetris对象为player1
	player2.role = rolePLAYER2;		//定义Tetris对象为player2
	player1.Initial(&tTiles);		//方块素材传给Tetris对象
	player2.Initial(&tTiles);		//方块素材传给Tetris对象

	player1.LH_WEIGHT = LH_WEIGHT;
	player1.RE_WEIGHT = RE_WEIGHT;
	player1.CT_WEIGHT = CT_WEIGHT;
	player1.RT_WEIGHT = RT_WEIGHT;
	player1.NH_WEIGHT = NH_WEIGHT;
	player1.WS_WEIGHT = WS_WEIGHT;

}

void Game::LoadMediaData()
{
	stringstream ss;
	ss << "data/images/bg" << imgSetNo << ".jpg";
	if (!tBackground.loadFromFile(ss.str()))
		cout << "bg image 没找到" << endl;

	ss.str("");		//清空字符串
	ss << "data/images/tiles" << imgSetNo << ".jpg";
	if (!tTiles.loadFromFile(ss.str()))
		cout << "tiles image 没找到" << endl;
		
	if (!tFrame.loadFromFile("data/images/frame.png"))
		cout << "frame 没找到" << endl;
	if (!tCover.loadFromFile("data/images/cover.png"))
		cout << "cover 没找到" << endl;
	if (!tGameOver.loadFromFile("data/images/end.png"))
		cout << "end 没找到" << endl;
	if (!tButtons.loadFromFile("data/images/button.png"))
		cout << "button 没找到" << endl;
	if (!tSwitcher.loadFromFile("data/images/bgSwitch.png"))
		cout << "bgSwitch 没找到" << endl;

	sBackground.setTexture(tBackground);
	//sTiles.setTexture(tTiles);		//由Tetris对象各自绘制方块
	sFrame.setTexture(tFrame);
	sCover.setTexture(tCover);
	sGameOver.setTexture(tGameOver);
	sButtons.setTexture(tButtons);
	sSwitcher.setTexture(tSwitcher);
	sSwitcher.setOrigin(sSwitcher.getLocalBounds().width / 2.0, sSwitcher.getLocalBounds().height / 2.0);
	
	if (!font.loadFromFile("data/Fonts/SIMYOU.ttf"))
		cout << "字体没找到" << endl;
	text.setFont(font);
}

void Game::gameInput()
{
	Event event;
	window.setKeyRepeatEnabled(false);		//按键按下只响应一次
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
			gameQuit = true;
		}

		if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
		{
			if (ButtonRectStart.contains(event.mouseButton.x, event.mouseButton.y))
			{
				if (!isGameBegin)
				{
					isGameBegin = true;
					player1.animationFlag = false;
					player2.animationFlag = false;
					ButtonState_Start = Close_Light;
				}
				else
				{
					isGameBegin = false;
					ButtonState_Start = Start_Light;
				}
			}
			if (ButtonRectHold.contains(event.mouseButton.x, event.mouseButton.y))
			{
				if (!isGameHold)
				{
					isGameHold = true;
					ButtonState_Hold = Continue_Light;
				}
				else
				{
					isGameHold = false;
					ButtonState_Hold = Hold_Light;
				}
			}

			if (ButtonRectLeft.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSetNo--;
				if (imgSetNo < 0)		//重新轮换皮肤图
					imgSetNo = 3;
				LoadMediaData();
			}
			if (ButtonRectRight.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSetNo++;
				if (imgSetNo > 3)		//重新轮换皮肤图
					imgSetNo = 0;
				LoadMediaData();
			}
		}

		if (event.type == sf::Event::MouseMoved)
		{
			if (ButtonRectStart.contains(event.mouseButton.x, event.mouseButton.y))
			{
				if (!isGameBegin)
					ButtonState_Start = Start_Light;
				else
					ButtonState_Start = Close_Light;
			}
			else
			{
				if (!isGameBegin)
					ButtonState_Start = Start_Dark;
				else
					ButtonState_Start = Close_Dark;
			}

			if (ButtonRectHold.contains(event.mouseButton.x, event.mouseButton.y))
			{
				if (!isGameHold)
					ButtonState_Hold = Hold_Light;
				else
					ButtonState_Hold = Continue_Light;
			}
			else
			{
				if (!isGameHold)
					ButtonState_Hold = Hold_Dark;
				else
					ButtonState_Hold = Continue_Dark;
			}
			
		}

		player1.Input(event);
		player2.Input(event);
	}
}

void Game::gameLogic()
{
	if (isGameHold == true)
		return;
	float time = clock.getElapsedTime().asSeconds();
	clock.restart();
	player1.timer += time;
	//player2.timer += time;

	player1.Logic();
	//player2.Logic();
}

void Game::gameDraw()
{
	window.clear();		//清屏
	//绘制背景
	sBackground.setPosition(0, 0);
	window.draw(sBackground);
	window.draw(sFrame);
	player1.Draw(&window);
	player2.Draw(&window);

	sCover.setPosition(P1_STAGE_CORNER_X, P1_STAGE_CORNER_Y);
	window.draw(sCover);
	sCover.setPosition(P2_STAGE_CORNER_X, P2_STAGE_CORNER_Y);
	window.draw(sCover);

	DrawButton();
	TextOut();
	DrawResults();

	window.display();
}

int Game::gameRun(double LH_WEIGHT, double RE_WEIGHT, double CT_WEIGHT, double RT_WEIGHT, double NH_WEIGHT, double WS_WEIGHT,int scale)
{
	do 
	{
		gameInitial(LH_WEIGHT, RE_WEIGHT, CT_WEIGHT, RT_WEIGHT, NH_WEIGHT, WS_WEIGHT);
		if (test == true) {
			gameInitial(LH_WEIGHT, RE_WEIGHT, CT_WEIGHT, RT_WEIGHT, NH_WEIGHT, WS_WEIGHT);
			isGameBegin = true;
			player1.animationFlag = false;
			ButtonState_Start = Close_Light;
		}
		while (window.isOpen() && gameOver == false)
		{
			gameInput();
			if (isGameBegin) {
				gameLogic();
				test = true;
				if (player1.gameOver||player1.score>200000) {
					//cout << player1.score << endl;
					return player1.score;
				}
			}
			else {
				gameInitial(LH_WEIGHT, RE_WEIGHT, CT_WEIGHT, RT_WEIGHT, NH_WEIGHT, WS_WEIGHT);
			}
			gameDraw();
		}
	} while (!gameQuit);
}

void Game::DrawButton()
{
	int ButtonWidth, ButtonHeight;
	ButtonWidth = 110;
	ButtonHeight = sButtons.getLocalBounds().height;

	//ButtonRectStart
	sButtons.setTextureRect(IntRect(ButtonState_Start * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(B_START_CORNER_X, B_START_CORNER_Y);
	ButtonRectStart.left = B_START_CORNER_X;
	ButtonRectStart.top = B_START_CORNER_Y;
	ButtonRectStart.width = ButtonWidth;
	ButtonRectStart.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectHold
	sButtons.setTextureRect(IntRect(ButtonState_Hold * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(B_HOLD_CORNER_X, B_HOLD_CORNER_Y);
	ButtonRectHold.left = B_HOLD_CORNER_X;
	ButtonRectHold.top = B_HOLD_CORNER_Y;
	ButtonRectHold.width = ButtonWidth;
	ButtonRectHold.height = ButtonHeight;
	window.draw(sButtons);

	//背景素材切换
	ButtonWidth = sSwitcher.getLocalBounds().width;
	ButtonHeight = sSwitcher.getLocalBounds().height;

	//ButtonRectLeft
	sSwitcher.setPosition(B_LEFT_CORNER_X, B_LEFT_CORNER_Y);
	ButtonRectLeft.left = B_LEFT_CORNER_X - ButtonWidth / 2;
	ButtonRectLeft.top = B_LEFT_CORNER_Y - ButtonHeight / 2;
	ButtonRectLeft.width = ButtonWidth;
	ButtonRectLeft.height = ButtonHeight;
	window.draw(sSwitcher);
	//ButtonRectRight
	sSwitcher.setPosition(B_RIGHT_CORNER_X, B_RIGHT_CORNER_Y);
	ButtonRectRight.left = B_RIGHT_CORNER_X - ButtonWidth / 2;
	ButtonRectRight.top = B_RIGHT_CORNER_Y - ButtonHeight / 2;
	ButtonRectRight.width = ButtonWidth;
	ButtonRectRight.height = ButtonHeight;
	sSwitcher.rotate(180);		//得到镜像的箭头素材
	window.draw(sSwitcher);
	sSwitcher.rotate(180);		//箭头素材还原
}

void Game::DrawResults()
{
	int ButtonWidth, ButtonHeight;
	ButtonWidth = 250;
	ButtonHeight = sGameOver.getLocalBounds().height;
	if (player1.gameOver || player2.gameOver)
	{
		sGameOver.setTextureRect(IntRect(player1.gameOver * ButtonWidth, 0, ButtonWidth, ButtonHeight));
		sGameOver.setPosition(P1_STAGE_CORNER_X + GRIDSIZE * 1.5, 0);
		window.draw(sGameOver);

		sGameOver.setTextureRect(IntRect(player2.gameOver * ButtonWidth, 0, ButtonWidth, ButtonHeight));
		sGameOver.setPosition(P2_STAGE_CORNER_X + GRIDSIZE * 1.5, 0);
		window.draw(sGameOver);
	}
}

void Game::TextOut()
{
	int initialX, initialY;
	int CharacterSize = 36;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 0, 0, 255));	//红色字体
	text.setStyle(Text::Bold);
	text.setPosition(P1_SCORE_CORNER_X, P1_SCORE_CORNER_Y);
	stringstream ss;
	ss << player1.score;
	text.setString(ss.str());
	window.draw(text);
	text.setPosition(P2_SCORE_CORNER_X, P2_SCORE_CORNER_Y);
	ss.str("");
	ss << player2.score;
	text.setString(ss.str());
	window.draw(text);

	CharacterSize = 24;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	initialY = INFO_CORNER_Y;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"  玩家1：");
	window.draw(text);
	initialY += CharacterSize * 1.4;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	方向键：WASD");
	window.draw(text);
	initialY += CharacterSize * 1.4;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	速降：Space键");
	window.draw(text);
	initialY += CharacterSize * 1.4;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	交换：左Ctrl键");
	window.draw(text);

	initialY += CharacterSize * 1.8;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"  玩家2：");
	window.draw(text);
	initialY += CharacterSize * 1.4;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	方向键：↑←↓→");
	window.draw(text);
	initialY += CharacterSize * 1.4;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	速降：Enter键");
	window.draw(text);
	initialY += CharacterSize * 1.4;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	交换：右Ctrl键");
	window.draw(text);
}







//申请种群内存，其中多加1个是放置上一代中最优秀个体
struct Individual Population[GROUP_SCALE + 1];

//extern Individual PopulationXXX[GROUP_SCALE + 1];

X_Range  XnRange[N_VARS] = { {-100,100} };

//有交配权的所有父代进行交叉
void crossover(int& seed)
{
	const double a = 0.0;
	const double b = 1.0;
	int mem;
	int one;
	int first = 0;
	double x;

	for (mem = 0; mem < GROUP_SCALE; ++mem)
	{
		x = randT(0.0, 1.0);
		//x = r8_uniform_ab(a, b, seed);//产生交配概率

		if (x < P_MATING)
		{
			++first;

			if (first % 2 == 0)//交配
			{
				Xover(one, mem, seed);
			}
			else
			{
				one = mem;
			}

		}
	}
	return;
}

//对最差的一代和最优的一代的处理，起到优化的目的
void elitist()
{
	int i;
	double best;
	int best_mem;
	double worst;
	int worst_mem;

	best = Population[0].Fitness;
	worst = Population[0].Fitness;

	for (i = 0; i < GROUP_SCALE - 1; ++i)
	{
		if (Population[i + 1].Fitness < Population[i].Fitness)
		{

			if (best <= Population[i].Fitness)
			{
				best = Population[i].Fitness;
				best_mem = i;
			}

			if (Population[i + 1].Fitness <= worst)
			{
				worst = Population[i + 1].Fitness;
				worst_mem = i + 1;
			}

		}
		else
		{

			if (Population[i].Fitness <= worst)
			{
				worst = Population[i].Fitness;
				worst_mem = i;
			}

			if (best <= Population[i + 1].Fitness)
			{
				best = Population[i + 1].Fitness;
				best_mem = i + 1;
			}

		}

	}

	//对于当前代的最优值的处理，如果当前的最优值小于上一代则将上一代的值最优个体取代当前的最弱个体
	//基因保留
	if (Population[GROUP_SCALE].Fitness <= best)
	{
		for (i = 0; i < N_VARS; i++)
		{
			Population[GROUP_SCALE].Xn[i] = Population[best_mem].Xn[i];
		}
		Population[GROUP_SCALE].Fitness = Population[best_mem].Fitness;
	}
	else
	{
		for (i = 0; i < N_VARS; i++)
		{
			Population[worst_mem].Xn[i] = Population[GROUP_SCALE].Xn[i];
		}
		Population[worst_mem].Fitness = Population[GROUP_SCALE].Fitness;
	}
	return;
}
//计算适应度值
void evaluate()
{
	int member;
	int i;
	double x[N_VARS + 1];

	for (member = 0; member < GROUP_SCALE; member++)
	{
		for (i = 0; i < N_VARS; i++)
		{
			x[i + 1] = Population[member].Xn[i];
		}
		//Population[member].Fitness = 21.5 + x[1] * sin(4 * PI * x[1]) + x[2] * sin(20 * PI * x[2]);
		Population[member].Fitness = GameGA.player1.score;
	}
	return;
}


//产生整形的随机数
int i4_uniform_ab(int a, int b, int& seed)
{
	int c;
	const int i4_huge = 2147483647;
	int k;
	float r;
	int value;

	if (seed == 0)
	{
		cerr << "\n";
		cerr << "I4_UNIFORM_AB - Fatal error!\n";
		cerr << "  Input value of SEED = 0.\n";
		exit(1);
	}
	//保证a小于b
	if (b < a)
	{
		c = a;
		a = b;
		b = c;
	}

	k = seed / 127773;
	seed = 16807 * (seed - k * 127773) - k * 2836;

	if (seed < 0)
	{
		seed = seed + i4_huge;
	}

	r = (float)(seed) * 4.656612875E-10;
	//
	//  Scale R to lie between A-0.5 and B+0.5.
	//
	r = (1.0 - r) * ((float)a - 0.5)
		+ r * ((float)b + 0.5);
	//
	//  Use rounding to convert R to an integer between A and B.
	//
	value = round(r);//四舍五入
	//保证取值不越界
	if (value < a)
	{
		value = a;
	}
	if (b < value)
	{
		value = b;
	}

	return value;
}

//初始化种群个体
void initGroup(int& seed)

{
	int i;
	int j;
	double lbound;
	double ubound;
	// 
	//  initGroup variables within the bounds 
	//
	//for (i = 0; i < N_VARS; i++)
	//{
		//input >> lbound >> ubound;

		for (j = 0; j < GROUP_SCALE; j++)
		{
			Population[j].Fitness = 0;
			Population[j].ReFitness = 0;
			Population[j].SumFitness = 0;
			XnRange[j].Lower = -100;
			XnRange[j].Upper = 100;
			for (i = 0; i < N_VARS; i++) Population[j].Xn[i] = randT(XnRange[j].Lower, XnRange[j].Upper);
			//Population[j].Xn[i] = r8_uniform_ab(XnRange[i].Lower, XnRange[i].Upper, seed);
		}
	//}

	return;
}


//挑选出最大值，保存在种群数组的最后一个位置
void selectBest()
{
	int cur_best;
	int mem;
	int i;

	cur_best = 0;

	for (mem = 0; mem < GROUP_SCALE; mem++)
	{
		if (Population[GROUP_SCALE].Fitness < Population[mem].Fitness)
		{
			cur_best = mem;
			Population[GROUP_SCALE].Fitness = Population[mem].Fitness;
		}
	}

	for (i = 0; i < N_VARS; i++)
	{
		Population[GROUP_SCALE].Xn[i] = Population[cur_best].Xn[i];
	}

	return;
}

//个体变异
void mutate(int& seed)
{
	const double a = 0.0;
	const double b = 1.0;
	int i;
	int j;
	double lbound;
	double ubound;
	double x;

	for (i = 0; i < GROUP_SCALE; i++)
	{
		for (j = 0; j < N_VARS; j++)
		{
			//x = r8_uniform_ab(a, b, seed);
			x = randT(a, b);//突变概率
			if (x < P_MUTATION)
			{
				lbound = XnRange[j].Lower;
				ubound = XnRange[j].Upper;
				Population[i].Xn[j] = randT(lbound, ubound);
				//Population[i].Xn[j] = r8_uniform_ab(lbound, ubound, seed);
			}
		}
	}

	return;
}

//模板函数，用于生成各种区间上的数据类型
template<typename T>
T randT(T Lower, T Upper)
{
	return rand() / (double)RAND_MAX * (Upper - Lower) + Lower;
}

//产生小数随机数
double r8_uniform_ab(double a, double b, int& seed)

{
	int i4_huge = 2147483647;
	int k;
	double value;

	if (seed == 0)
	{
		cerr << "\n";
		cerr << "R8_UNIFORM_AB - Fatal error!\n";
		cerr << "  Input value of SEED = 0.\n";
		exit(1);
	}

	k = seed / 127773;
	seed = 16807 * (seed - k * 127773) - k * 2836;

	if (seed < 0)
	{
		seed = seed + i4_huge;
	}

	value = (double)(seed) * 4.656612875E-10;

	value = a + (b - a) * value;

	return value;
}

//输出每一代进化的结果
void report(int Xnration)
{
	double avg;
	double best_val;
	int i;
	double square_sum;
	double stddev;
	double sum;
	double sum_square;

	if (Xnration == 0)
	{
		cout << "\n";
		cout << "  Xnration       Best            Average       Standard \n";
		cout << "  number           value           Fitness       deviation \n";
		cout << "\n";
	}
	sum = 0.0;
	sum_square = 0.0;

	for (i = 0; i < GROUP_SCALE; i++)
	{
		sum = sum + Population[i].Fitness;
		sum_square = sum_square + Population[i].Fitness * Population[i].Fitness;
	}

	avg = sum / (double)GROUP_SCALE;
	square_sum = avg * avg * GROUP_SCALE;
	stddev = sqrt((sum_square - square_sum) / (GROUP_SCALE - 1));
	best_val = Population[GROUP_SCALE].Fitness;

	cout << "  " << setw(8) << Xnration
		<< "  " << setw(14) << best_val
		<< "  " << setw(14) << avg
		<< "  " << setw(14) << stddev << "\n";

	return;
}

//选择有交配权的父代
void selector(int& seed)
{
	struct Individual NewPopulation[GROUP_SCALE + 1];//临时存放挑选的后代个体
	const double a = 0.0;
	const double b = 1.0;
	int i;
	int j;
	int mem;
	double p;
	double sum;

	sum = 0.0;
	for (mem = 0; mem < GROUP_SCALE; mem++)
	{
		sum = sum + Population[mem].Fitness;
	}
	//计算概率密度
	for (mem = 0; mem < GROUP_SCALE; mem++)
	{
		Population[mem].ReFitness = Population[mem].Fitness / sum;
	}
	// 计算累加分布，思想是轮盘法
	Population[0].SumFitness = Population[0].ReFitness;
	for (mem = 1; mem < GROUP_SCALE; mem++)
	{
		Population[mem].SumFitness = Population[mem - 1].SumFitness +
			Population[mem].ReFitness;
	}
	// 选择个体为下一代繁殖，选择优秀的可能性大，这是轮盘法的奥秘之处
	for (i = 0; i < GROUP_SCALE; i++)
	{
		p = r8_uniform_ab(a, b, seed);
		if (p < Population[0].SumFitness)
		{
			NewPopulation[i] = Population[0];
		}
		else
		{
			for (j = 0; j < GROUP_SCALE; j++)
			{
				if (Population[j].SumFitness <= p && p < Population[j + 1].SumFitness)
				{
					NewPopulation[i] = Population[j + 1];
				}
			}
		}
	}
	//更新后代个体 
	for (i = 0; i < GROUP_SCALE; i++)
	{
		Population[i] = NewPopulation[i];
	}
	return;
}

//交叉产生子代
void Xover(int one, int two, int& seed)
{
	int i;
	int point;
	double t;
	//随机选择交叉点，这里的点是以变量的整个长度为单位
	point = randT<int>(0, N_VARS - 1);
	//point = i4_uniform_ab(0, N_VARS - 1, seed);
	//交叉
	for (i = 0; i < point; i++)
	{
		t = Population[one].Xn[i];
		Population[one].Xn[i] = Population[two].Xn[i];
		Population[two].Xn[i] = t;
	}
	return;
}


int main()
{
	Game tetris;
	int seed = 123456789;
	int average = 0;
	int score = 0;
	initGroup(seed);
	int fitness = 0;
	for (int Xnration = 0; Xnration < MAX_GENS; Xnration++) {
		average = 0;
		cout << "第" << Xnration << "代：" << endl;
		for (int scale = 0; scale < GROUP_SCALE; scale++) {
			cout << Population[scale].Xn[0] << " " << Population[scale].Xn[1] << " " << Population[scale].Xn[2] << " " << Population[scale].Xn[3] << " " << Population[scale].Xn[4] << " " << Population[scale].Xn[5];
			for (int count = 0; count < 4; count++) {
				score = tetris.gameRun(Population[scale].Xn[0], Population[scale].Xn[1], Population[scale].Xn[2], Population[scale].Xn[3], Population[scale].Xn[4], Population[scale].Xn[5], scale);
				cout << " " << score << " ";
				Population[scale].SumFitness += score;
				//Population[scale].SumFitness
			}
			Population[scale].Fitness = Population[scale].SumFitness / 4.0;
			average += Population[scale].Fitness;
			cout << " Fitness:" << Population[scale].Fitness << endl;
		}
		cout << "平均：" << average / GROUP_SCALE << endl;
		selector(seed);
		crossover(seed);
		mutate(seed);
		//report(Xnration);
		evaluate();
		elitist();
	}
	while (1);
	return 0;
}
	