#include "Tetris.h"

Tetris::Tetris()
{
	dx = 0;				//X����ƫ����
	rotate = false;		//�Ƿ���ת
	colorNum = 1;		//ɫ�����ɫ
	timer = 0;			
	delay = DELAYVALUE;		//������ٶ�
}

Tetris::~Tetris()
{

}

int Tetris::holdcolorNum = 0, Tetris::holdShapeNum = 0;
Vector2i Tetris::holdSquare[4] = { { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 } };

void Tetris::Initial(Texture* tex)
{
	tTiles = tex;
	dx = 0;					//x����ƫ����
	bagNum = 0;
	rotate = false;			//�Ƿ���ת
	hold = false;			//�Ƿ���hold��ͼ��

	colorNum = 1;			//ɫ����ɫ
	timer = 0;
	delay = 0.3;			//�����ٶ�
	score = 0;
	gameOver = false;

	animationFlag = true;	//��������
	animationCtrlValue = 1.0;

	if (role == rolePLAYER1)
	{
		mCornPoint = { P1_STAGE_CORNER_X, P1_STAGE_CORNER_Y };
		nextSquareCornPoint = { P1_NEXT_CORNER_X, P1_NEXT_CORNER_Y };
	}
	if (role == rolePLAYER2)
	{
		mCornPoint = { P2_STAGE_CORNER_X, P2_STAGE_CORNER_Y };
		nextSquareCornPoint = { P2_NEXT_CORNER_X, P2_NEXT_CORNER_Y };
	}
	
	holdSquareCornPoint = { HOLD_CORNER_X,HOLD_CORNER_Y };
	holdShapeNum = -1;				//��Ϸ��ʼʱ�ڣ�hold����ͼ������һ���쳣ֵ������ʾHold�ķ���ͼ��
	
	sTiles.setTexture(*tTiles);

	//��ʼ������ͼ��
	colorNum = 1 + rand() % 7;
	currentShapeNum = Bag7();		//rand() % 7;
	//�����¸�����ͼ��
	nextcolorNum = 1 + rand() % 7;
	nextShapeNum = Bag7();			//rand() % 7;

	for (int i = 0; i < 4; i++)
	{
		currentSquare[i].x = Figures[currentShapeNum][i] % 2 + STAGE_WIDTH / 2;
		currentSquare[i].y = Figures[currentShapeNum][i] / 2;
		nextSquare[i].x = Figures[nextShapeNum][i] % 2;
		nextSquare[i].y = Figures[nextShapeNum][i] / 2;
		animationRow[i] = -1;		//Ҫ���Ŷ�����������-1��ʾ����
	}
	putTetrisToLeft();

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
			Field[i][j] = 0;
	}
	//std::cout << "����߶ȷ�����" << LH_WEIGHT << " ���з�����" << RE_WEIGHT << " �б任������" << RT_WEIGHT << " �б任������" << CT_WEIGHT << " �ն�������" << NH_WEIGHT << " ���������" << WS_WEIGHT << std::endl;
}

void Tetris::Input(Event event)
{
	if (role == rolePLAYER1)		//���1�İ�����Ӧ
	{
		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::W)
				if (currentShapeNum != shapeO)
					rotate = true;
			if (event.key.code == Keyboard::A)
				dx = -1;
			else if (event.key.code == Keyboard::D)
				dx = 1;
			if (event.key.code == Keyboard::S)
				delay = 0.05;
		}
		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::LControl)
				hold = true;
			if (event.key.code == Keyboard::Space)
				hardDrop = true;
			if (event.key.code == Keyboard::A|| event.key.code == Keyboard::D)
				dx = 0;
			if (event.key.code == Keyboard::S)
				delay = 0.3;
			if (event.key.code == Keyboard::C)
				flagtest=true;
			if (event.key.code == Keyboard::V)
				flagtest = false;
		}
	}

	if (role == rolePLAYER2)		//���2�İ�����Ӧ
	{
		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::Up)
				if (currentShapeNum != shapeO)
					rotate = true;
			if (event.key.code == Keyboard::Left)
				dx = -1;
			else if (event.key.code == Keyboard::Right)
				dx = 1;
			if (event.key.code == Keyboard::Down)
				delay = 0.05;
		}
		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::RControl)
				hold = true;
			if (event.key.code == Keyboard::Enter)
				hardDrop = true;
			if (event.key.code == Keyboard::Left || event.key.code == Keyboard::Right)
				dx = 0;
			if (event.key.code == Keyboard::Down)
				delay = DELAYVALUE;
		}
	}
}

void Tetris::traditionLogic()
{
	////Rotate ��ת////
	/*
	if (rotate)
	{
		rotateFunc();
		rotate = false;
	}
	*/
	if (rotate) {
		for (int i = 0; i < needRotate; i++) {
			rotateFunc();
		}
		rotate = false;
		putTetrisToLeft();
	}
	
	////Hold ����////
	if (hold)
	{
		holdFunc();
		hold = false;
	}

	////Move ˮƽ�ƶ�////
	xMove();

	

	////Slow_Load  �ײ�����////
	slowLoading();

	////Tick ����////
	if (timer > delay)
	{
		yMove();
		timer = 0;
	}

	////Shadow  ͶӰ////
	shadow();
	if (hardDrop)
	{
		hardDropFunc();
		hardDrop = false;
	}
}

void Tetris::Logic()
{
	if (!animationFlag)
		traditionLogic();
	//��������·����������������ж��Ƿ��ж�������

	if (newShapeFlag)
	{
		if (animationFlag == false)
		{
			checkLine();				////Check Lines ����////
			if (animationFlag == false) {
				newShapeFunc();			//��������µķ���
				getBestPositionAgain();
			}
				
			isWin();
		}
		else
		{
			animationCtrlValue -= 0.1;
			if (animationCtrlValue < 0)
			{
				animationFlag = false;
				animationCtrlValue = 1.0;
				for (int i = 0; i < 4; i++)
					animationRow[i] = -1;
				clearLine();
				newShapeFunc();
				getBestPositionAgain();
			}
		}
	}
}

void Tetris::Draw(RenderWindow* w)
{
	window = w;
	if (animationFlag == false)
	{
		//������Ӱ�ķ���
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(shadowSquare[i].x * GRIDSIZE, shadowSquare[i].y * GRIDSIZE);
			sTiles.setColor(Color(50, 50, 50, 255));
			sTiles.move(mCornPoint.x, mCornPoint.y);
			window->draw(sTiles);
			sTiles.setColor(Color(255, 255, 255, 255));
		}
		//���ƻ�ķ���
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(currentSquare[i].x * GRIDSIZE, currentSquare[i].y * GRIDSIZE);
			sTiles.move(mCornPoint.x, mCornPoint.y);
			window->draw(sTiles);
		}
	}

	//���ƹ̶��ķ���
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		if (i == animationRow[0] || i == animationRow[1] || i == animationRow[2] || i == animationRow[3])
			animationFunc(i);		//���������У���ִ����Ч����
		else
			for (int j = 0; j < STAGE_WIDTH; j++)
			{
				if (Field[i][j] == 0)
					continue;
				sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
				sTiles.move(mCornPoint.x, mCornPoint.y);
				window->draw(sTiles);
			}
	}		
	
	//����NEXT���ķ���
	for (int i = 0; i < 4; i++)
	{
		sTiles.setTextureRect(IntRect(nextcolorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
		sTiles.setPosition(nextSquare[i].x * GRIDSIZE, nextSquare[i].y * GRIDSIZE);
		sTiles.move(nextSquareCornPoint.x, nextSquareCornPoint.y);
		window->draw(sTiles);
	}
	//����HOLD���ķ���
	if(holdShapeNum>-1)				//hold��ͼ������ʱ����л���
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(holdcolorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(holdSquare[i].x * GRIDSIZE - 16, holdSquare[i].y * GRIDSIZE + 4);
			sTiles.move(holdSquareCornPoint.x, holdSquareCornPoint.y);
			window->draw(sTiles);
		}
}

bool Tetris::hitTest()
{
	for (int i = 0; i < 4; i++)
	{
		if (currentSquare[i].x < 0 || currentSquare[i].x >= STAGE_WIDTH || currentSquare[i].y >= STAGE_HEIGHT)
			return false;
		else if (Field[currentSquare[i].y][currentSquare[i].x])
			return false;
	}
	return true;
}

bool Tetris::AIhitTest()
{
	for (int i = 0; i < 4; i++)
	{
		if (algoSquare[i].x < 0 || algoSquare[i].x >= STAGE_WIDTH || algoSquare[i].y >= STAGE_HEIGHT)
			return false;
		else if (temp[algoSquare[i].y][algoSquare[i].x])
			return false;
	}
	return true;
}

void Tetris::xMove()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].x += dx;
	}

	if (!hitTest())		//���ײ����
	{
		for (int i = 0; i < 4; i++)
			currentSquare[i] = tempSquare[i];		//�����ҵı߽磬�����Ƴ��߽�
	}
	dx = 0;
}

void Tetris::yMove()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].y += 1;
	}

	if (!hitTest())		//���ײ����
	{
		for (int i = 0; i < 4; i++)
			Field[tempSquare[i].y][tempSquare[i].x] = colorNum;
		newShapeFlag = true;
	}
}

void Tetris::rotateFunc()
{
	int originalHeight = currentSquare[0].y;
	for (int j = 0; j < 4; j++)			//wall kick ����ƫ��
	{
		Vector2i p = currentSquare[j];	//������ת���ĵ�
		for (int i = 0; i < 4; i++)
		{
			//˳ʱ����ת90��
			int x = currentSquare[i].y - p.y;		//ԭY����������ĵ�Ĳ�ֵ
			int y = currentSquare[i].x - p.x;		//ԭX����������ĵ�Ĳ�ֵ
			currentSquare[i].x = p.x - x;			//������X = ���ĵ����� - �µ�X�����ֵ
			currentSquare[i].y = p.y + y;			//������Y = ���ĵ����� - �µ�Y�����ֵ
		}

		if (hitTest())		//���û��ײ��
		{
			int detaY = 0;
			detaY = currentSquare[0].y - originalHeight;	//�������ĵ�ĸ߶Ȳ�
			if (detaY != 0)
			{
				for (int i = 0; i < 4; i++)
					currentSquare[i].y -= detaY;			//�߶Ȳ�����
			}
			if (!hitTest())
			{
				for (int i = 0; i < 4; i++)
					currentSquare[i] = tempSquare[i];		//�����ҵı߽磬�����Ƴ��߽�
			}
			else
				break;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				currentSquare[i] = tempSquare[i];			//�����ҵı߽磬�����Ƴ��߽�
		}
	}
}

void Tetris::holdFunc()
{
	Vector2i backUpSquare[4];
	tempcolorNum = holdcolorNum;
	tempShapeNum = holdShapeNum;
	holdcolorNum = colorNum;
	holdShapeNum = currentShapeNum;

	for (int i = 0; i < 4; i++)
	{
		holdSquare[i].x = Figures[holdShapeNum][i] % 2;
		holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		tempSquare[i].x = Figures[tempShapeNum][i] % 2;
		tempSquare[i].y = Figures[tempShapeNum][i] / 2;
		backUpSquare[i] = currentSquare[i];
	}

	if (tempShapeNum < 0)		//hold�����쳣ֵ��ʾhold��Ϊ�յ�״̬����NEXT��ȡֵ
	{
		newShapeFunc();
	}
	else
	{
		colorNum = tempcolorNum;
		currentShapeNum = tempShapeNum;
		//Ϊ��hold���û������ķ���ͼ�μ���������̨�ϵ�����
		int minCurrentX = currentSquare[0].x,
			minCurrentY = currentSquare[0].y,
			minTempX = tempSquare[0].x,
			minTempY = tempSquare[0].y;
		int dx, dy;
		for (int i = 1; i < 4; i++)
		{
			if (currentSquare[i].x < minCurrentX)
				minCurrentX = currentSquare[i].x;
			if (currentSquare[i].y < minCurrentY)
				minCurrentY = currentSquare[i].y;
			if (tempSquare[i].x < minTempX)
				minTempX = tempSquare[i].x;
			if (tempSquare[i].y < minTempY)
				minTempY = tempSquare[i].y;
		}
		dx = minCurrentX - minTempX;
		dy = minCurrentY - minTempY;
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i].x = tempSquare[i].x + dx;
			currentSquare[i].y = tempSquare[i].y + dy;
			holdSquare[i].x = Figures[holdShapeNum][i] % 2;
			holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		}
	}

	if (!hitTest())		//���ײ����
	{
		colorNum = holdcolorNum;
		holdcolorNum = tempcolorNum;
		holdShapeNum = tempShapeNum;
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i] = backUpSquare[i];
			holdSquare[i].x = Figures[holdShapeNum][i] % 2;
			holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		}
	}
}

void Tetris::hardDropFunc()
{
	for (int i = 0; i < 4; i++)
		currentSquare[i] = shadowSquare[i];
}

int Tetris::Bag7()
{
	int num;
	num = rand() % 7;
	for (int i = 0; i < bagNum; i++)
	{
		if (bag[i] == num)
		{
			i = -1;			//i++�����ù��㣬�������½��б���
			num = rand() % 7;
		}
	}
	bag[bagNum] = num;
	bagNum++;
	if (bagNum == 7)
	{
		bagNum = 0;
		for (int i = 0; i < 7; i++)
			bag[i] = 0;
	}
	return num;
}

void Tetris::checkLine()
{
	int k = STAGE_HEIGHT - 1;
	int yCount = 0;
	for (int i = STAGE_HEIGHT - 1; i > 0; i--)
	{
		int xCount = 0;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (Field[i][j])
				xCount++;
			//Field[k][j] = Field[i][j];
		}
		if (xCount < STAGE_WIDTH)
			k--;
		else
		{
			animationRow[yCount] = i;	//Ҫ���Ŷ���������
			yCount++;
			animationFlag = true;
			clearline++;	
			for (int l = 0; l < 4; l++)
			{
				if (currentSquare[l].y == i)
				{
					cells++;
				}
			}
		}
	}

	switch (yCount)
	{
	case 1:
		score += 10;
		break;
	case 2:
		score += 30;
		break;
	case 3:
		score += 60;
		break;
	case 4:
		score += 100;
		break;
	}
}

void Tetris::slowLoading()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].y += 1;
	}
	if (!hitTest())
		delay = DELAYVALUE * 2;
	else
		delay = DELAYVALUE;

	for (int i = 0; i < 4; i++)
		currentSquare[i] = tempSquare[i];
}

void Tetris::shadow()
{
	for (int i = 0; i < 4; i++)
		shadowSquare[i] = currentSquare[i];
	for (int j = 0; j < STAGE_HEIGHT; j++)		//һֱ����
	{
		for (int i = 0; i < 4; i++)
			currentSquare[i].y += 1;
		if (!hitTest())
		{
			for (int i = 0; i < 4; i++)			//�����򷵻���һ��λ��
				currentSquare[i].y -= 1;
			break;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i] = shadowSquare[i];
		shadowSquare[i] = tempSquare[i];
	}
}



void Tetris::clearLine()
{
	int k = STAGE_HEIGHT - 1;
	for (int i = STAGE_HEIGHT - 1; i > 0; i--)
	{
		int xCount = 0;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (Field[i][j])
				xCount++;
			Field[k][j] = Field[i][j];	//�½�
		}
		if (xCount < STAGE_WIDTH)
			k--;
	}
}

void Tetris::newShapeFunc()
{
	//ȡ�¸�����ͼ��
	colorNum = nextcolorNum;
	currentShapeNum = nextShapeNum;

	//�����¸�����ͼ��
	nextcolorNum = 1 + rand() % 7;
	nextShapeNum = Bag7();		//Bag7();

	//��ǰ�����
	for (int i = 0; i < 4; i++)
	{
		currentSquare[i] = nextSquare[i];
		currentSquare[i].x = currentSquare[i].x + STAGE_WIDTH / 2;
		nextSquare[i].x = Figures[nextShapeNum][i] % 2;
		nextSquare[i].y = Figures[nextShapeNum][i] / 2;
	}
	putTetrisToLeft();
	//shadow();
	newShapeFlag = false;
}

void Tetris::animationFunc(int i)
{
	Vector2f p;
	sTiles.scale(animationCtrlValue, animationCtrlValue);
	p = sTiles.getOrigin();
	sTiles.setOrigin(GRIDSIZE / 2, GRIDSIZE / 2);
	sTiles.rotate(360 * animationCtrlValue);
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
		sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
		sTiles.move(mCornPoint.x + GRIDSIZE / 2, mCornPoint.y + GRIDSIZE / 2);
		//������ת��С
		window->draw(sTiles);
	}
	sTiles.scale(1.0 / animationCtrlValue, 1.0 / animationCtrlValue);
	sTiles.rotate(-360 * animationCtrlValue);
	sTiles.setOrigin(p);
}

void Tetris::isWin()
{
	if (Field[2][5] || Field[2][6]||Field[2][0] || Field[2][1] || Field[2][2])
		gameOver = true;
}

void Tetris::getMaxMin(Vector2i square[4], int& minX, int& maxX, int& minY, int& maxY)
{
	minX = maxX = square[0].x;
	minY = maxY = square[0].y;
	for (int i = 0; i < 4; i++)
	{
		if (minX > square[i].x)
		{
			minX = square[i].x;
		}
		if (maxX < square[i].x)
		{
			maxX = square[i].x;
		}
		if (minY > square[i].y)
		{
			minY = square[i].y;
		}
		if (maxY < square[i].y)
		{
			maxY = square[i].y;
		}
	}
}

// �߶�����
int Tetris::getLH()
{
	int maxX, minX, maxY, minY;
	getMaxMin(algoSquare, minX, maxX, minY, maxY);
	return STAGE_HEIGHT-maxY;
}
// ������*���׷���������
int Tetris::lineStatus(int i)
{
	int cnt = 0;
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		if (temp[i][j])
		{
			cnt++;
		}
		else {
			break;
		}
	}
	if (cnt == STAGE_WIDTH)
	{
		return 1;
	}
	else {
		return 0;
	}
}
// ������*���׷���������
int Tetris::getRE()
{
	int  status, clearline = 0, cells = 0;

	for (int i = STAGE_HEIGHT - 1; i >= 0; i--)
	{
		status = lineStatus(i);
		if (status == 0)
		{
			continue;
		}
		if (status == 1)
		{
			// ��
			clearline++; // ��+1
			// ���й��׷�����
			for (int j = 0; j < 4; j++)
			{
				if (algoSquare[j].y == i)
				{
					cells++;
				}
			}
		}
	}
	return clearline * cells;
}

int Tetris::getWS() {
	//backupField();
	// TODO: ������ⲻ��ȷ
	int wells = 0;
	int center, left, right;
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j <STAGE_WIDTH; j++)
		{

			center = temp[i][j];
			if (j > 0)
			{
				left = temp[i][j - 1];
			}
			else
			{
				left = 1;
			}
			if (j < STAGE_WIDTH - 1)
			{
				right = temp[i][j + 1];
			}
			else
			{
				right = 1;
			}
			// �Ƿ�Ϊ��
			if (left && right && !center)
			{
				for (int k = i; k < STAGE_HEIGHT; k++)
				{
					if (temp[k][j] == 0)
					{
						wells++;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return wells;

}

int Tetris::getNH() {
	//backupField();
	int holes = 0;
	bool holeFlag = false; // �Ƿ���Կ�ʼ����ն�
	for (int i = 0; i < STAGE_WIDTH; i++)
	{
		holeFlag = false;
		for (int j = 0; j < STAGE_HEIGHT; j++)
		{
			if (!holeFlag)
			{
				if (temp[j][i])
				{
					holeFlag = true;
				}
			}
			else
			{
				if (temp[j][i] == 0)
				{
					holes++;
				}
			}
		}
	}
	return holes;

}

int Tetris::getRT() {//�б任
	int lineChane = 0, lastStatus;
	bool nulllinejudge = false;
	int nulllinecount = 0;
	//backupField();
	for (int i = STAGE_HEIGHT - 1; i >= 0; i--)
	{
		lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
		if (nulllinejudge) {
			lineChane += 2;
		}
		else {
			nulllinecount = 0;
			for (int j = 0; j < STAGE_WIDTH; j++)
			{
				//�ж��Ƿ�Ϊ����
				if (nulllinecount == 10) {
					nulllinejudge = true;
				}
				if (temp[i][j] == 0) {
					nulllinecount++;
				}
				// ����һ�����Ӳ�ͬ
				if (!(lastStatus * temp[i][j]) && lastStatus != temp[i][j])
				{
					lineChane++;
					lastStatus = temp[i][j];
				}
				// ��ǰ�����һ���ڵ�Ϊ��
				if (j == (STAGE_WIDTH - 1) && temp[i][j] == 0)
				{
					lineChane++;
				}
			}
		}
	}
	return lineChane;
}

int Tetris::getCT() {//�б任
	//backupField();
	int colChane = 0, lastStatus;
	for (int i = 0; i < STAGE_WIDTH; i++)
	{
		lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
		for (int j = 0; j < STAGE_HEIGHT; j++)
		{
			// ����һ�����Ӳ�ͬ
			if (!(lastStatus * temp[j][i]) && lastStatus != temp[j][i])
			{
				colChane++;
				lastStatus = temp[j][i];
			}
			// ��ǰ�����һ���ڵ�Ϊ��
			if (j == (STAGE_HEIGHT - 1) && temp[j][i] == 0)
			{
				colChane++;
			}
		}
	}
	return colChane;
}

int Tetris::Dellacherie()
{
	int lh_score, re_score, rt_score, ct_score, nh_score, ws_score;
	lh_score = getLH();
	re_score = getRE();
	rt_score = getRT();
	ct_score = getCT();
	nh_score = getNH();
	ws_score = getWS();
	//if(flagtest)

	int sumscore = lh_score * LH_WEIGHT + re_score * RE_WEIGHT + rt_score * RT_WEIGHT + ct_score * CT_WEIGHT + nh_score * NH_WEIGHT + ws_score * WS_WEIGHT;
	//std::cout << "����߶ȷ�����"<< LH_WEIGHT << " ���з�����"<< RE_WEIGHT << " �б任������" << RT_WEIGHT << " �б任������" << CT_WEIGHT << " �ն�������"<< NH_WEIGHT << " ���������"<< WS_WEIGHT <<" �ܷ֣�"<<sumscore<< std::endl;
	
	return sumscore;
}

void Tetris::backupField() {
	for (int i = 0; i < STAGE_HEIGHT; i++) {
		for (int j = 0; j < STAGE_WIDTH; j++) {
			temp[i][j] = Field[i][j];
		}
	}
}

void Tetris::backupSquare() {
	for (int i = 0; i < 4; i++)
	{
		algoSquare[i].x = currentSquare[i].x;
		algoSquare[i].y = currentSquare[i].y;
	}
}

void Tetris::AI_rotateFunc()
{
	int originalHeight = algoSquare[0].y;
	for (int j = 0; j < 4; j++)			//wall kick ����ƫ��
	{
		Vector2i p = algoSquare[j];	//������ת���ĵ�
		for (int i = 0; i < 4; i++)
		{
			//˳ʱ����ת90��
			int x = algoSquare[i].y - p.y;		//ԭY����������ĵ�Ĳ�ֵ
			int y = algoSquare[i].x - p.x;		//ԭX����������ĵ�Ĳ�ֵ
			algoSquare[i].x = p.x - x;			//������X = ���ĵ����� - �µ�X�����ֵ
			algoSquare[i].y = p.y + y;			//������Y = ���ĵ����� - �µ�Y�����ֵ
		}

		if (hitTest())		//���û��ײ��
		{
			int detaY = 0;
			detaY = algoSquare[0].y - originalHeight;	//�������ĵ�ĸ߶Ȳ�
			if (detaY != 0)
			{
				for (int i = 0; i < 4; i++)
					algoSquare[i].y -= detaY;			//�߶Ȳ�����
			}
			if (!hitTest())
			{
				for (int i = 0; i < 4; i++)
					algoSquare[i] = tempSquare[i];		//�����ҵı߽磬�����Ƴ��߽�
			}
			else
				break;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				algoSquare[i] = tempSquare[i];			//�����ҵı߽磬�����Ƴ��߽�
		}
	}
}

void Tetris::showmap() {//�����ã���ʾÿ������Ľ��
	for (int i = 0; i < STAGE_HEIGHT; i++) {
		for (int j = 0; j < STAGE_WIDTH; j++) {
			std::cout << temp[i][j];
		}
		std::cout << std::endl;
	}
}


void Tetris::fakedrop() //ģ������������¼��temp������
{
	for (int j = 0; j < STAGE_HEIGHT; j++)		//һֱ����
	{
		for (int i = 0; i < 4; i++)
			algoSquare[i].y += 1;
		if (!AIhitTest())
		{
			for (int i = 0; i < 4; i++)			//�����򷵻���һ��λ��
				algoSquare[i].y -= 1;
			break;
		}
	}
	for (int i = 0; i < 4; i++) {
		temp[algoSquare[i].y][algoSquare[i].x] = 1;
	}
}

void Tetris::putTetrisToLeft() {     //���Խ������Ƶ�����ߣ��������
	if(currentSquare[0].x > 0 && currentSquare[1].x > 0 && currentSquare[2].x > 0 && currentSquare[3].x > 0)
	while (currentSquare[0].x > 0 && currentSquare[1].x > 0 && currentSquare[2].x > 0 && currentSquare[3].x > 0) {
		for (int i = 0; i < 4; i++) {
			currentSquare[i].x--;
		}
	}
}

void Tetris::AI_putTetrisToLeft() {
	if (algoSquare[0].x > 0 && algoSquare[1].x > 0 && algoSquare[2].x > 0 && algoSquare[3].x > 0)
		while (algoSquare[0].x > 0 && algoSquare[1].x > 0 && algoSquare[2].x > 0 && algoSquare[3].x > 0) {
			for (int i = 0; i < 4; i++) {
				algoSquare[i].x--;
			}
		}
}

int Tetris::getWidth() {
	int MinX, MaxX;
	MinX = currentSquare[0].x;
	MaxX = currentSquare[0].x;
	for (int i = 1; i < 4; i++) {
		if (currentSquare[i].x > MaxX) MaxX = currentSquare[i].x;
		if (currentSquare[i].x < MinX) MinX = currentSquare[i].x;
	}
	return MaxX - MinX+1;
}

void Tetris::getBestPositionAgain() {
	backupField();
	backupSquare();
	int rightstep=0;
	int score = 0;
	int bestrotate = 0;
	bestScore = 0;
	
	//std::cout << "width:" << length << std::endl;
	bestScore = Dellacherie();
	//std::cout << "current:"<<"(" << currentSquare[0].y << "," << currentSquare[0].x << ") "<< "(" << currentSquare[1].y << "," << currentSquare[1].x << ") "<< "(" << currentSquare[2].y << "," << currentSquare[2].x << ") "<< "(" << currentSquare[3].y << "," << currentSquare[3].x << ") "<<std::endl;
	for (int k = 0; k < 4; k++) {
		backupField();
		backupSquare();
		//std::cout << "��ת��" << k << std::endl;
		for (int count = 0; count < k; count++) {
			AI_rotateFunc();
		}
		AI_putTetrisToLeft();
		int length = getWidth();
		for (int z = 0; z < 4; z++) {
			tempSquare[z] = algoSquare[z];
		}
		for (int i = 0; i < STAGE_WIDTH - length + 1; i++) {
			backupField();
			//backupSquare();
			for (int z = 0; z < 4; z++) {
				algoSquare[z] = tempSquare[z];
			}
			for (int j = 0; j < 4; j++) {
				algoSquare[j].x += i;
				//std::cout << "(" << algoSquare[j].y << "," << algoSquare[j].x << ") ";
			}
			fakedrop();
			//std::cout << std::endl;
			//showmap();
			score = Dellacherie();
			if (score > bestScore) {
				bestScore = score;
				bestrotate = k;
				rightstep = i;
			}
			//std::cout << "\n";
		}
		//AI_rotateFunc();
	}
	dx = rightstep;
	needRotate = bestrotate;
	if (bestrotate != 0) rotate = true;
	//std::cout << "������ת��" <<needRotate <<"�������ƣ�"<<dx<<std::endl;
}
