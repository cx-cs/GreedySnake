#include <iostream>
#include <conio.h>
#include <ctime>
#include <list>
#include <queue>
#include <set>
#include <vector>
#include <map>
#include <graphics.h>
using namespace std;
const int WINDOW_WIDTH = 640;	// ���ڿ��
const int WINDOW_HEIGHT = 480;	// ���ڸ߶�
int MAP_ROWNUM ;		// ��ͼ����>1
int MAP_COLNUM ;		// ��ͼ����>1
int AREA;//��ͼ���
const int GRIDGAP = 5;			// ���Ӽ�϶
int GRIDH ;	// ���Ӹ߶�
int GRIDW;	// ���ӿ��
int _GRIDH;			// С���Ӹ߶�
int _GRIDW;			// С���ӿ��
int SPEED;
bool AI;
bool HanMilton;
int successtimes;
enum Direct { RIGHT = 77, DOWN = 80, LEFT = 75, UP = 72 };
struct Coor {
	int x, y, step;
	Coor() { step = 0; }
	Coor(int _x, int _y, int _step = 0) {
		x = _x;
		y = _y;
		step = _step;
	}
	bool operator==(const Coor &t)const {
		return (x == t.x) && (y == t.y);
	}
	bool operator!=(const Coor &t)const {
		return (x != t.x) || (y != t.y);
	}
	bool operator<(const Coor &t)const {
		return x == t.x ? y < t.y : x < t.x;
	}
};
class Food {
public:
	Coor coor;			
	bool EXISTFOOD;		
	~Food() { EXISTFOOD = false; }
	void RandFood(int limx, int limy) {
		coor.x = rand() % limx;
		coor.y = rand() % limy;
	}
};
class Snake {	
public:
	int length;			
	int speed;	
	int movestepbeforeeat;
	Coor lasttail;
	list<Coor> coor;	
	Direct direct;	
	Snake() {
		length = 1;
		speed = SPEED;
		direct = UP;
		movestepbeforeeat = 0;
		coor.push_back(Coor(rand() % MAP_COLNUM, rand() % MAP_ROWNUM));
	}
	void Move() {
		Coor head = coor.front();
		switch (direct) {
		case UP:	head.y--;	break;
		case DOWN:	head.y++;	break;
		case LEFT:	head.x--;	break;
		case RIGHT:	head.x++;	break;
		}
		lasttail = coor.back();
		coor.pop_back();	
		coor.push_front(head);
		movestepbeforeeat++;
	}
};
class SnakeGame {
public:
	bool AIalgorithm,SafeMove;
	Snake snake;
	Food food;
	map<Coor, int> emptyspace;
	vector<Coor> HanMilTonpath1;
	vector<Coor> HanMilTonpath2[2];
	int HanMilTonpath2choose;
	void InitSnakeFood() {// ��Ϸ��ʼ��
		srand((unsigned)time(NULL));
		AIalgorithm = false;
		CreatFood();
		if(HanMilton)HanMilTon();
	}
	void DrawSnakeFood() {// ��ʳ�����
		setfillcolor(DARKGRAY);
		solidrectangle(0, 0, GRIDW*MAP_COLNUM, GRIDH*MAP_ROWNUM);
		setcolor(LIGHTGRAY);
		line(WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		// ��ʳ��
		setfillcolor(RED);
		fillrectangle(food.coor.x*GRIDW + GRIDGAP, food.coor.y*GRIDH + GRIDGAP,
			(food.coor.x+1)*GRIDW - GRIDGAP, (food.coor.y+1)*GRIDH - GRIDGAP);
		// ����
		setfillcolor(WHITE);
		Coor temp = snake.coor.front();
		for (auto iter : snake.coor) {
			solidrectangle(iter.x*GRIDW + GRIDGAP, iter.y*GRIDH + GRIDGAP,
				(iter.x + 1)*GRIDW - GRIDGAP, (iter.y + 1)*GRIDH - GRIDGAP);
			// ��������ķ�϶
			int iter_x = iter.x*GRIDW + GRIDGAP;
			int iter_y = iter.y*GRIDH + GRIDGAP;
			int temp_x = temp.x*GRIDW + GRIDGAP;
			int temp_y = temp.y*GRIDH + GRIDGAP;
			if (temp.x == iter.x) {
				if (iter.y > temp.y) {
					solidrectangle(temp_x, temp_y + _GRIDH, iter_x + _GRIDW, iter_y);
				}
				if (iter.y < temp.y) {
					solidrectangle(iter_x, iter_y + _GRIDH, temp_x + _GRIDW, temp_y);
				}
			}
			if (temp.y==iter.y) {
				if (iter.x > temp.x) {
					solidrectangle(temp_x + _GRIDW, temp_y, iter_x, iter_y + _GRIDH);
				}
				if (iter.x < temp.x) {
					solidrectangle(iter_x + _GRIDW, iter_y, temp_x, temp_y + _GRIDH);
				}
			}
			temp = iter;
		}
		//����ͷ
		setfillcolor(GREEN);
		fillrectangle(snake.coor.front().x * GRIDW + GRIDGAP, snake.coor.front().y * GRIDH + GRIDGAP,
			(snake.coor.front().x + 1) * GRIDW - GRIDGAP, (snake.coor.front().y + 1) * GRIDH - GRIDGAP);
	}
	void DrawGameEnd() {// ����Ϸ��������
		setfillcolor(BLACK);
		TCHAR str_end[10];
		swprintf_s(str_end, _T("GAME OVER"));
		if (EatFullScreen()) {
			swprintf_s(str_end, _T("YOU WIN"));
			successtimes++;
		}
		int Tstrlen = (int)_tcslen(str_end);
		outtextxy(WINDOW_HEIGHT / 2 - Tstrlen * 20 / 4, WINDOW_HEIGHT / 2 - 20 / 4, str_end);
	}
	void DrawGameInfo() {// ������Ϸ�����Ϣ
		setfillcolor(BLACK);
		solidrectangle(WINDOW_HEIGHT, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		// �ߴ���Ϣ
		TCHAR str_mapsize[20];
		swprintf_s(str_mapsize, _T("MAPSIZE:  %d��%d"), MAP_ROWNUM,MAP_COLNUM);
		settextcolor(WHITE);
		outtextxy(WINDOW_HEIGHT + 20, 20, str_mapsize);
		// AI״̬��Ϣ
		TCHAR str_ai[10];
		if (AI == true)swprintf_s(str_ai, _T("AI:  ON"));
		else swprintf_s(str_ai, _T("AI:  OFF"));
		outtextxy(WINDOW_HEIGHT + 20, 50, str_ai);
		// HanMilTon״̬��Ϣ
		TCHAR str_HanMilton[25];
		if (HanMilton == true)swprintf_s(str_HanMilton, _T("HanMilton:  ON"));
		else swprintf_s(str_HanMilton, _T("HanMilton:  OFF"));
		outtextxy(WINDOW_HEIGHT + 20, 80, str_HanMilton);
		// �ٶ���Ϣ
		TCHAR str_speed[20];
		swprintf_s(str_speed, _T("SPEED:  %d"), snake.speed);
		outtextxy(WINDOW_HEIGHT + 20, 110, str_speed);
		// �߳�����Ϣ
		TCHAR str_length[20];
		swprintf_s(str_length, _T("LENGTH:  %d/%d"), snake.length, AREA);
		outtextxy(WINDOW_HEIGHT + 20, 140, str_length);
		// ������Ϣ
		TCHAR str_dir[20];
		switch (snake.direct) {
		case RIGHT:	swprintf_s(str_dir, _T("Direction:  RIGHT"));	break;
		case DOWN:	swprintf_s(str_dir, _T("Direction:  DOWN"));		break;
		case LEFT:	swprintf_s(str_dir, _T("Direction:  LEFT")); 	break;
		case UP:	swprintf_s(str_dir, _T("Direction:  UP"));		break;
		default:	swprintf_s(str_dir, _T("Direction:  NONE"));
		}
		outtextxy(WINDOW_HEIGHT + 20, 170, str_dir);
		//�����ɹ�����
		TCHAR str_success[20];
		swprintf_s(str_success, _T("SUCCESS:  %d"), successtimes);
		outtextxy(WINDOW_HEIGHT + 20, 200, str_success);
		settextcolor(RED);
		//AI�㷨�ı�
		if(AIalgorithm)outtextxy(WINDOW_HEIGHT + 20, 230, _T("AI2 ON"));
		//��ȫ�ƶ�
		if (SafeMove)outtextxy(WINDOW_HEIGHT + 20, 260, _T("SafeMove"));
		settextcolor(WHITE);
		// ��Ϸ����˵��
		outtextxy(WINDOW_HEIGHT + 20, 290, _T("����˵��:"));
		outtextxy(WINDOW_HEIGHT + 20, 320, _T("���������ʼ"));
		outtextxy(WINDOW_HEIGHT + 20, 380, _T("��  :      ��"));
		outtextxy(WINDOW_HEIGHT + 20, 400, _T("��  :      ��"));
		outtextxy(WINDOW_HEIGHT + 20, 420, _T("��  :      ��"));
		outtextxy(WINDOW_HEIGHT + 20, 440, _T("��  :      ��"));
	}
	bool EatFood() {// �Ե�ʳ��û
		Coor head = snake.coor.front();
		if (head==food.coor) {
			snake.movestepbeforeeat = 0;
			food.EXISTFOOD = false;
			snake.coor.push_back(snake.lasttail);
			snake.length++;
			CreatFood();
			return true;
		}
		return false;
	}
	void CreatFood() {// ����ʳ��
		if (food.EXISTFOOD == false&&!EatFullScreen()) {
			while (true) {
				food.RandFood(MAP_COLNUM,MAP_ROWNUM);
				if (!onSnake(food.coor))
					break;
			}
			food.EXISTFOOD = true;
		}
	}
	void TurnDirect(char cmd = 0x00) {
		if (cmd == 0x00) {
			if (!AI && !HanMilton) {
				while (_kbhit())
					cmd = _getch();
			}
			else if (AI)
				cmd = SafeWander();
		}
		switch (cmd)
		{
		case UP:
			if (snake.length==1||snake.direct != DOWN)//����ײ�Լ�
				snake.direct = UP;
			break;
		case DOWN:
			if (snake.length == 1 || snake.direct != UP)
				snake.direct = DOWN;
			break;
		case LEFT:
			if (snake.length == 1 || snake.direct != RIGHT)
				snake.direct = LEFT;
			break;
		case RIGHT:
			if (snake.length == 1 || snake.direct != LEFT)
				snake.direct = RIGHT;
			break;
		}
	}
	char SafeWander() {
		SafeMove = true;
		const int next[4][2] = { { 1,0 } ,{ 0,1 },{ -1,0 },{ 0,-1 } };
		Coor aroundPoint[4];
		for (int i = 0; i < 4; i++) {
			aroundPoint[i].x = snake.coor.front().x + next[i][0];
			aroundPoint[i].y = snake.coor.front().y + next[i][1];
			if (inBorder(aroundPoint[i]) && !onSnake_ExceptTail(aroundPoint[i])) {
				SnakeGame tmp = *this;
				if (aroundPoint[i] != food.coor)
					tmp.snake.coor.pop_back();
				aroundPoint[i].step = tmp.canFindPath(aroundPoint[i], tmp.snake.coor.back());
			}
			else
				aroundPoint[i].step = -1;
		}
		for (int i = 0; i < 4; i++)
			if (aroundPoint[i].step != -1)
				return CmdToNextPoint(aroundPoint[i]);
	}
	bool chooseAlgorithm() {
		if (snake.movestepbeforeeat > 10 * AREA) {
			emptyspace.clear();
			AIalgorithm = !AIalgorithm;
			snake.movestepbeforeeat = 0;
		}
		return AIalgorithm;
	}
	char getNextCmd() {// �õ���һ��ָ���AIָ����ܶ���ָ��
		if (HanMilton == true)return NextCmdHanMilTon();
		if (AI == false)
			return 0x00;
		char cmd = 0x00;
		if (snake.length == AREA - 1 && getManhattanDistance(snake.coor.front(), food.coor) == 1)
			return CmdToNextPoint(food.coor);
		// ������ҵ�ʳ��
		if (canFindFood(snake.coor.front())) {
			SnakeGame simulate = *this;
			// ������ȥ�Ե�ʳ��
			bool first = true;
			char firstcmd;
			while (!simulate.EatFood()) {
				char tmpcmd;
				tmpcmd=simulate.NextCmdToEatNearFood();
				simulate.TurnDirect(tmpcmd);
				if (first) {
					firstcmd = tmpcmd;
					first = false;
				}
				simulate.snake.Move();
			}
			// ������껹���ҵ�β��
			if (simulate.canFindTail(simulate.snake.coor.front())) {
				// ����ȥ��
				cmd = firstcmd;
			}
			else {// �����Ҳ���
				if(chooseAlgorithm())cmd = NextCmdToleft2();
				else cmd= NextCmdToFarestTail();
			}
		}
		else {// �Ҳ���ʳ��
			if(chooseAlgorithm())cmd = NextCmdToleft2();
			else cmd = NextCmdToFarestTail();
		}
		return cmd;
	}
	int canFindPath(const Coor& _start,const Coor& _end) {// ����·�����ز����������ڷ���-1
		if (_start == _end) {
			return 0;
		}
		const int next[4][2] = { {1,0} ,{0,1}, {-1,0}, {0,-1} };
		queue<Coor> bfs_que;
		set<Coor> snake_set;
		Coor tcoor;
		bfs_que.push(_start);
		for (auto iter : snake.coor) {
			snake_set.insert(iter);
		}
		// ����յ���β�;Ͱ�β����Ϊ���߽�� *
		// ����ʱ��Ҫʹ������
		if (&_end == &snake.coor.back()) {
			snake_set.erase(_end);
		}
		while (!bfs_que.empty()) {
			for (int k = 0; k < 4; k++) {
				tcoor.x = bfs_que.front().x + next[k][0];
				tcoor.y = bfs_que.front().y + next[k][1];
				// ������ͼ�����´�ѭ��
				if (!inBorder(tcoor)) {
					continue;
				}
				// ���ϰ�û���߹��Ľ��������
				if (!snake_set.count(tcoor)) {
					tcoor.step= bfs_que.front().step+1;
					snake_set.insert(tcoor);
					bfs_que.push(tcoor);
				}
				// ����Ŀ�귵�ز���
				if (tcoor == _end) {
					return tcoor.step;
				}
			}
			bfs_que.pop();
		}
		return -1;
	}
	bool canFindTail(Coor _start) {// ���ڵ�β�͵�·��
		return canFindPath(_start, snake.coor.back()) >= 0 ? true : false;
	}
	bool canFindFood(Coor _start) {// ���ڵ�ʳ���·��
		return canFindPath(_start, food.coor) >= 0 ? true : false;
	}
	char NextCmdToEatNearFood() {//bfs��̾����ʳ��
		const int next[4][2] = { { 1,0 } ,{ 0,1 },{ -1,0 },{ 0,-1 } }; 
		Coor aroundPoint[4];
		for (int i = 0; i < 4; i++) {
			aroundPoint[i].x = snake.coor.front().x + next[i][0];
			aroundPoint[i].y = snake.coor.front().y + next[i][1];
			if (!inBorder(aroundPoint[i]) || onSnake_ExceptTail(aroundPoint[i])||(snake.length==2&& aroundPoint[i]==snake.coor.back()))	// ���⴦��β��
				aroundPoint[i].step = -1;
			else
				aroundPoint[i].step = canFindPath(aroundPoint[i], food.coor);
		}
		// ѡ���������
		vector<Coor> v;
		for (int i = 0; i < 4; i++) {
			if (aroundPoint[i].step != -1) {
				if (!v.empty() && aroundPoint[i].step < v.front().step) {
					v.clear();
					v.push_back(aroundPoint[i]);
				}
				else if (v.empty() || aroundPoint[i].step == v.front().step)
					v.push_back(aroundPoint[i]);
			}
		}
		char ret = 0x00;
		if (!v.empty())
			ret = CmdToNextPoint(v[rand()%v.size()]);
		return ret;
	}
	char NextCmdToFarestTail() {// �ܵ�β������Զ��β��
		const int next[4][2] = { { 1,0 } ,{ 0,1 },{ -1,0 },{ 0,-1 } };
		Coor aroundPoint[4];
		for (int i = 0; i < 4; i++) {
			aroundPoint[i].x = snake.coor.front().x + next[i][0];
			aroundPoint[i].y = snake.coor.front().y + next[i][1];
			if (inBorder(aroundPoint[i]) && !onSnake_ExceptTail(aroundPoint[i])) {
				SnakeGame tmp = *this;
				if(aroundPoint[i]!=food.coor)
					tmp.snake.coor.pop_back();
				aroundPoint[i].step = tmp.canFindPath(aroundPoint[i], tmp.snake.coor.back());
			}
			else
				aroundPoint[i].step = -1;
		}
		char ret = 0x00;
		if (snake.movestepbeforeeat>=AREA&&rand()%2==1) {
			vector<Coor> v;
			for (int i = 0; i < 4; i++)
				if (aroundPoint[i].step != -1)
					v.push_back(aroundPoint[i]);
			if (!v.empty())
				ret = CmdToNextPoint(v[rand() % v.size()]);
		}
		else {
			vector<Coor> v;
			for (int i = 0; i < 4; i++) {
				if (aroundPoint[i].step != -1) {
					if (!v.empty() && aroundPoint[i].step > v.front().step) {
						v.clear();
						v.push_back(aroundPoint[i]);
					}
					else if (v.empty() || aroundPoint[i].step == v.front().step)
						v.push_back(aroundPoint[i]);
				}
			}
			if (!v.empty())
				ret = CmdToNextPoint(v[rand() % v.size()]);
		}
		return ret;
	}
	char NextCmdToleft2() {// �ܵ�β����ʹ��λ�ı�
		const int next[4][2] = { { 1,0 } ,{ 0,1 },{ -1,0 },{ 0,-1 } };
		Coor aroundPoint[4];
		for (int i = 0; i < 4; i++) {
			aroundPoint[i].x = snake.coor.front().x + next[i][0];
			aroundPoint[i].y = snake.coor.front().y + next[i][1];
			if (inBorder(aroundPoint[i]) && !onSnake_ExceptTail(aroundPoint[i])) {
				SnakeGame tmp = *this;
				if (aroundPoint[i] != food.coor)
					tmp.snake.coor.pop_back();
				aroundPoint[i].step = tmp.canFindPath(aroundPoint[i], tmp.snake.coor.back());
			}
			else
				aroundPoint[i].step = -1;
		}
		char ret = 0x00;
		Coor empty;
		bool found = false;
		for (int i = 0; i < MAP_COLNUM; i++) {
			if (found == true)break;
			for (int j = 0; j < MAP_ROWNUM; j++) {
				empty = Coor(i, j);
				if (!onSnake(empty) && empty != food.coor) {
					found = true;
					break;
				}
			}
		}
		emptyspace[empty] = 1;
		queue<Coor> q;
		vector<Coor> v;
		for (int i = 0; i < 4; i++)
			if (aroundPoint[i].step != -1) {
				q.push(aroundPoint[i]);
				ret = CmdToNextPoint(aroundPoint[i]);
			}
		while (!q.empty()) {
			SnakeGame tmp = *this;
			char tmpret = CmdToNextPoint(q.front());
			tmp.TurnDirect(tmpret);
			tmp.snake.Move();
			Coor newempty;
			bool found = false;
			for (int i = 0; i < MAP_COLNUM; i++) {
				if (found == true)break;
				for (int j = 0; j < MAP_ROWNUM; j++) {
					newempty = Coor(i, j);
					if (!tmp.onSnake(newempty) && newempty != tmp.food.coor) {
						found = true;
						break;
					}
				}
			}
			if (emptyspace.count(newempty) == 0) {
				v.push_back(q.front());
				break;
			}
			q.pop();
		}
		if(!v.empty())
			ret = CmdToNextPoint(v[rand() % v.size()]);
		return ret;
	}
	void HanMilTon() {
		map<Coor, int> vis;
		vector<Coor> tmppath;
		int cnt = 0;
		Coor tmp(0, 0);
		if ((MAP_COLNUM & 1) == 1 && (MAP_ROWNUM & 1) == 1)HanMilTonCore1();
		else HanMilTonCore2(tmp,tmppath,vis,cnt);
	}
	void HanMilTonCore1() {
		int y,x=0;
		for(y=0;y<2;y++)
			HanMilTonpath2[0].push_back(Coor(x,y));
		while (y < MAP_ROWNUM - 1) {
			for (x = 0; x < MAP_COLNUM - 1; x++)
				HanMilTonpath2[0].push_back(Coor(x, y));
			for (++y,x=MAP_COLNUM - 2; x >= 0; x--)
				HanMilTonpath2[0].push_back(Coor(x, y));
			++y;
		}
		for (x = 0; x < MAP_COLNUM-1; x++)
			HanMilTonpath2[0].push_back(Coor(x, y));
		for(x = MAP_COLNUM - 1,y= MAP_ROWNUM - 1;y>=1;y--)
			HanMilTonpath2[0].push_back(Coor(x, y));
		y = 1;
		while (x > 2) {
			HanMilTonpath2[0].push_back(Coor(x, --y));
			HanMilTonpath2[0].push_back(Coor(--x, y));
			HanMilTonpath2[0].push_back(Coor(x, ++y));
			HanMilTonpath2[0].push_back(Coor(--x, y));
		}
		HanMilTonpath2[1] = HanMilTonpath2[0];
		HanMilTonpath2[0].push_back(Coor(2, 0));
		HanMilTonpath2[0].push_back(Coor(1, 0));
		HanMilTonpath2[1].push_back(Coor(1, 1));
		HanMilTonpath2[1].push_back(Coor(1, 0));
	}
	void HanMilTonCore2(Coor& dst, vector<Coor>& tmppath, map<Coor, int>& vis,int& cnt) {
		tmppath.push_back(dst); 
		Coor tmp=Coor(0, 0);
		if (tmppath.size() == AREA && getManhattanDistance(dst, tmp) == 1) {
			cnt++;
			HanMilTonpath1 = tmppath;
			tmppath.pop_back();
			return;
		}
		vis[dst] = 1;
		const int next[4][2] = { { 1,0 } ,{ 0,1 },{ -1,0 },{ 0,-1 } };
		Coor aroundPoint[4];
		for (int i = 0; i < 4&&cnt==0; i++) {
			aroundPoint[i].x = dst.x + next[i][0];
			aroundPoint[i].y = dst.y + next[i][1];
			if (inBorder(aroundPoint[i]) && !vis[aroundPoint[i]]) {
				HanMilTonCore2(aroundPoint[i], tmppath, vis,cnt);
			}
		}
		vis[dst] = 0;
		tmppath.pop_back();
	}
	char NextCmdHanMilTon() {
		if ((MAP_COLNUM & 1) == 1 && (MAP_ROWNUM & 1 )== 1) {
			if (food.coor==Coor(2,0) && snake.coor.front() == Coor(2, 1))
				HanMilTonpath2choose = 0;
			else if (food.coor==Coor(1,1)&&snake.coor.front()==Coor(2,1))
				HanMilTonpath2choose = 1;
			for (size_t i = 0; i < HanMilTonpath2[HanMilTonpath2choose].size(); i++)
				if (HanMilTonpath2[HanMilTonpath2choose][i] == snake.coor.front())
					return CmdToNextPoint(HanMilTonpath2[HanMilTonpath2choose][(i + 1) % HanMilTonpath2[HanMilTonpath2choose].size()]);
		}
		else {
			for (size_t i = 0; i < HanMilTonpath1.size(); i++) {
				if (HanMilTonpath1[i] == snake.coor.front()) {
					return CmdToNextPoint(HanMilTonpath1[(i + 1) % HanMilTonpath1.size()]);
				}
			}
		}
	}
	char CmdToNextPoint(Coor& nextPoint) {// ���ص���һ��ָ��
		char cmd = 0x00;
		int dx = nextPoint.x - snake.coor.front().x;
		int dy = nextPoint.y - snake.coor.front().y;
		if (dx == 0 && dy < 0)
			cmd = UP;
		if (dx == 0 && dy > 0)
			cmd = DOWN;
		if (dx < 0 && dy == 0)
			cmd = LEFT;
		if (dx > 0 && dy == 0)
			cmd = RIGHT;
		return cmd;
	}
	int getManhattanDistance(Coor& _a,Coor& _b) {// ���������پ���
		return abs(_a.x-_b.x)+abs(_a.y-_b.y);
	}
	bool GameEnd() {// �ж���Ϸ����
		Coor head = snake.coor.front();//ͷ���������߽�����
		if (!inBorder(head) || onSnake_ExceptHead(head) || EatFullScreen()) {
			return true;
		}
		return false;
	}
	bool EatFullScreen() {// �Ƿ����
		return snake.length == AREA;
	}
	bool onSnake(Coor& coor) {// ����������
		for (auto iter : snake.coor) {
			if (coor == iter) {
				return true;
			}
		}
		return false;
	}
	bool onSnake_ExceptHead(Coor& coor){// ���������ϲ�������ͷ
		auto iter = snake.coor.begin();
		for (iter++; iter != snake.coor.end(); iter++) {
			if (coor == *iter) {
				return true;
			}
		}
		return false;
	}
	bool onSnake_ExceptTail(Coor& coor) {// ���������ϲ�������β
		auto iter = snake.coor.begin();
		auto flag = --snake.coor.end();
		for (; iter != flag; iter++) {
			if (coor == *iter) {
				return true;
			}
		}
		return false;
	}
	bool inBorder(Coor& coor)const {// �����ڷ�����δ����
		return (coor.x >= 0 && coor.x < MAP_COLNUM && coor.y >= 0 && coor.y < MAP_ROWNUM);
	}
};
void Input() {
	int mode;
	cout << "����������(����1)";
	while (1) {
		cin >> mode;
		if (mode > 1)break;
		else cout << "����";
	}
	MAP_ROWNUM=mode;
	cout << "����������(����1)";
	while (1) {
		cin >> mode;
		if (mode > 1)break;
		else cout << "����";
	}
	MAP_COLNUM = mode;
	cout << "�����ٶȣ�(����0)";
	while (1) {
		cin >> mode;
		if (mode > 0)break;
		else cout << "����";
	}
	SPEED = mode;
	cout << "����ģʽ��1��AIģʽ��2��HanMilTon��3" << endl;
	while (1) {
		cin >> mode;
		if (mode == 1|| mode == 2|| mode == 3)break;
		else cout << "����";
	}
	if (mode == 2)AI = true;
	else if (mode == 3)HanMilton = true;
	AREA = MAP_ROWNUM * MAP_COLNUM;
	GRIDH = WINDOW_HEIGHT / MAP_ROWNUM;
	GRIDW = WINDOW_HEIGHT / MAP_COLNUM;
	_GRIDH = GRIDH - 2 * GRIDGAP;
	_GRIDW = GRIDW - 2 * GRIDGAP;
}
int main() {
	Input();
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	BeginBatchDraw();
	bool gameover = false;
	SnakeGame* SG = new SnakeGame();
	SG->InitSnakeFood();
	SG->DrawSnakeFood();
	SG->DrawGameInfo();
	FlushBatchDraw();
	_getch();
	while (true) {
		if (gameover) {
			if(!SG->EatFullScreen())_getch();
			if(SG!=nullptr)delete SG;
			SG=new SnakeGame();
			SG->InitSnakeFood();
			gameover = false;
		}
		SG->TurnDirect(SG->getNextCmd()); 
		SG->snake.Move();
		SG->EatFood();
		SG->DrawSnakeFood();
		SG->DrawGameInfo();
		if (SG->GameEnd()) {
			SG->DrawGameEnd();
			SG->DrawGameInfo();
			gameover = true;
		}
		FlushBatchDraw();
		Sleep(1000/(SG->snake.speed));
	}
	EndBatchDraw();
	closegraph();
	return 0;
}