#include<graphics.h>
#include<conio.h>
#include <time.h>
#include <Windows.h>

#pragma comment (lib, "Winmm.lib")
//struct of tank
/*
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
*/
#define ENEMY_NUM 4

struct tank_s {
	int x, y; //location
	int direction;//zhi na da na
	int live;//si mei si
};

struct bullet_s {
	int direction;
	int speed;
	int pos_x, pos_y;//movement
	int status; //clappse & boom
};

struct apple_s {
	int status;
	static const int x, y;
};

int map[19][26] = {
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1},
	{2, 2, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 2, 2},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 1, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void init_map();
void menu();
void play();
void set_prop_map(int x, int y, int set);
int bullet_action(bullet_s* bullet, tank_s* enemy_tank);
int tank_walk(tank_s* tank, int ditection, IMAGE* img, int step);
void enemy_tank_walk(tank_s* tank, int direction, IMAGE* img);
int enemy_direction(tank_s* tank, int x, int y);
void tank_fire(tank_s* tank, bullet_s* bullet);
static int enemy_total;

int main(void) {
	initgraph(640, 480);

	menu();

	init_map();

	play();

	system("pause");
}



void init_map() {
	IMAGE img_home, img_wall_1, img_wall_2;

	loadimage(&img_home, _T("resource/img/save.jpg"), 50, 50);//home
	loadimage(&img_wall_1, _T("resource/blocks/stoneTx.gif"), 25, 25);//stone
	loadimage(&img_wall_2, _T("resource/blocks/brickTx.gif"), 25, 25);//brick

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 26; j++) {
			if (map[i][j] == 1) putimage(25 * j, 25 * i, &img_wall_2);
			else if (map[i][j] == 2) putimage(25 * j, 25 * i, &img_wall_1);
			else if (map[i][j] == 3) {
				putimage(25 * j, 25 * i, &img_home);
				map[i][j] = 4;
				map[i + 1][j] = 4;
				map[i][j + 1] = 4;
				map[i + 1][j + 1] = 4;
			}
		}
	}
}

void menu() {
	IMAGE logo_img;
	loadimage(&logo_img, _T("resource/img/Miscellaneous.png"), 480, 1200);
	putimage(110, 20, &logo_img);

	setlinecolor(WHITE);
	setfillcolor(BLACK);

	fillrectangle(270, 350, 410, 395);
	fillrectangle(270, 400, 410, 445);
	outtextxy(270, 370, _T("1 PLAYER"));
	outtextxy(270, 420, _T("2 PLAYERS"));
	outtextxy(100, 440, _T("This is project for applying summer intern for SIE from HeTengjiao, Utokyo"));

	ExMessage m;

	while (true) {
		m = getmessage(EX_MOUSE | EX_KEY);
		switch (m.message) {
		case WM_KEYDOWN:
			if (m.vkcode == VK_ESCAPE)
				return;
		case WM_LBUTTONDOWN:
			if (m.x > 270 && m.x < 410 && m.y > 350 && m.y < 395) {
				cleardevice();
				return;
			}
		}
	}
}

int tank_walk(tank_s* tank, int direction1, IMAGE* img, int step) {
	int new_x = tank->x;
	int new_y = tank->y;

	int old_prop = map[tank->y][tank->x];

	if (step == 1) {//go forward
		if (direction1 == 0) {
			new_y -= 1;
		}
		else if (direction1 == 1) {
			new_y += 1;
		}
		else if (direction1 == 2) {
			new_x -= 1;
		}
		else if (direction1 == 3) {
			new_x += 1;
		}
		else return 0;

		map[tank->y][tank->x] = 0;
		map[tank->y][tank->x + 1] = 0;
		map[tank->y + 1][tank->x] = 0;
		map[tank->y + 1][tank->x + 1] = 0;

		setfillcolor(BLACK);
		solidrectangle(tank->x * 25, tank->y * 25, tank->x * 25 + 50, tank->y * 25 + 50);

		set_prop_map(new_x, new_y, old_prop);

		tank->x = new_x;
		tank->y = new_y;
		tank->direction = direction1;
		putimage(tank->x * 25, tank->y * 25, img);
	}
	else {
		setfillcolor(BLACK);
		solidrectangle(tank->x * 25, tank->y * 25, tank->x * 25 + 50, tank->y * 25 + 50);
		tank->direction = direction1;
		putimage(tank->x * 25, tank->y * 25, img);
	}
	return 1;
}

//############################
//main loop
void play() {

	tank_s my_tank;
	bullet_s my_bullet;

	tank_s enemy_tank[4];
	bullet_s enemy_bullet[4];

	my_bullet.status = 0;
	IMAGE my_tank_img[4];
	IMAGE enemy_tank_img[4];
	IMAGE apple;
	int key;//control
	int times = 0;
	enemy_total = 0;

	//init seed random
	srand(time(NULL));

	//load image
	loadimage(&my_tank_img[0], _T("resource/img/6.png"), 50, 50);
	loadimage(&my_tank_img[1], _T("resource/img/5.png"), 50, 50);
	loadimage(&my_tank_img[2], _T("resource/img/4.png"), 50, 50);
	loadimage(&my_tank_img[3], _T("resource/img/3.png"), 50, 50);

	loadimage(&enemy_tank_img[0], _T("resource/img/kurakTx.png"), 50, 50);
	loadimage(&enemy_tank_img[1], _T("resource/img/kurakTx.png"), 50, 50);
	loadimage(&enemy_tank_img[2], _T("resource/img/kurakTx.png"), 50, 50);
	loadimage(&enemy_tank_img[3], _T("resource/img/kurakTx.png"), 50, 50);

	loadimage(&apple, _T("resource/img/apple.jpg"), 50, 50);

	my_tank.x = 9;
	my_tank.y = 17;

	my_tank.live = 1;
	my_tank.direction = 2;

	map[my_tank.y][my_tank.x] = 200;
	map[my_tank.y][my_tank.x + 1] = 200;
	map[my_tank.y + 1][my_tank.x] = 200;
	map[my_tank.y + 1][my_tank.x + 1] = 200;

	for (int i = 0; i < ENEMY_NUM; i++) {
		if (i % 4 == 0) {
			enemy_tank[i].x = 0;
		}
		else if (i % 4 == 1) {
			enemy_tank[i].x = 4;
		}
		else if (i % 4 == 2) {
			enemy_tank[i].x = 20;
		}
		else if (i % 4 == 3) {
			enemy_tank[i].x = 24;
		}
		enemy_tank[i].direction = 1;
		enemy_tank[i].y = 0;
		enemy_tank[i].live = 1;
		enemy_bullet[i].status = 0;
		set_prop_map(enemy_tank[i].x, enemy_tank[i].y, 100 + i);
	}

	tank_walk(&enemy_tank[0], 1, &enemy_tank_img[1], 0);
	
	tank_walk(&enemy_tank[1], 1, &enemy_tank_img[1], 0);
	
	tank_walk(&enemy_tank[2], 1, &enemy_tank_img[1], 0);
	
	tank_walk(&enemy_tank[3], 1, &enemy_tank_img[1], 0);
	
	enemy_total = 4;

	putimage(my_tank.x * 25, my_tank.y * 25, &my_tank_img[my_tank.direction]);
	putimage(12 * 25, 0, &apple);

	while (true) {
		if (times > 0 && times % 1000 == 0 && enemy_total < ENEMY_NUM) {
			set_prop_map(enemy_tank[enemy_total].x, enemy_tank[enemy_total].y, 100 + enemy_total);
			enemy_total++;
		}
		if (times % 200 == 0) {//2ms
			for (int i = 0; i < enemy_total; i++)
			{	
				if (enemy_tank[i].live == 0) continue;
				if (i % 2 == 0) {
					int d = enemy_direction(&enemy_tank[i], 12, 17);
					enemy_tank_walk(&enemy_tank[i], d, &enemy_tank_img[i]);
				}
				else {
					int d = enemy_direction(&enemy_tank[i], my_tank.x, my_tank.y);
					enemy_tank_walk(&enemy_tank[i], d, &enemy_tank_img[i]);
				}
				tank_fire(&enemy_tank[i], &enemy_bullet[i]);
			}
		}
		if (times % 49 == 0 && times != 0) {//enemy moves
			for (int i = 0; i < enemy_total; i++)
			{
				if (enemy_tank[i].live) {
					enemy_tank_walk(&enemy_tank[i], enemy_tank[i].direction, &enemy_tank_img[i]);
				}
			}
		}
		if (my_bullet.status == 1) bullet_action(&my_bullet, enemy_tank);//our bullet
		for (int i = 0; i < ENEMY_NUM; i++)//enemy bullet
		{
			if (enemy_bullet[i].status == 1) { 
				bullet_action(&enemy_bullet[i], enemy_tank);
			}
		}
		Sleep(10);
		times++;


		if (_kbhit()) {
			key = _getch();

			switch (key) {
			case 'a':
				if ((my_tank.direction == 2) && (my_tank.x - 1) >= 0 && map[my_tank.y][my_tank.x - 1] == 0 && map[my_tank.y + 1][my_tank.x - 1] == 0) {
					tank_walk(&my_tank, 2, &my_tank_img[2], 1);
				}
				else {
					tank_walk(&my_tank, 2, &my_tank_img[2], 0);
				}
				break;
			case 'w':
				if ((my_tank.direction == 0) && (my_tank.y - 1) >= 0 && map[my_tank.y - 1][my_tank.x] == 0 && map[my_tank.y - 1][my_tank.x + 1] == 0) {
					tank_walk(&my_tank, 0, &my_tank_img[0], 1);
				}
				else {
					tank_walk(&my_tank, 0, &my_tank_img[0], 0);
				}
				break;
			case 's':
				if ((my_tank.direction == 1) && (my_tank.y + 2) <= 18 && map[my_tank.y + 2][my_tank.x] == 0 && map[my_tank.y + 2][my_tank.x + 1] == 0) {
					tank_walk(&my_tank, 1, &my_tank_img[1], 1);
				}
				else {
					tank_walk(&my_tank, 1, &my_tank_img[1], 0);
				}
				break;
			case 'd':
				if ((my_tank.direction == 3) && (my_tank.x + 2) <= 25 && map[my_tank.y][my_tank.x + 2] == 0 && map[my_tank.y + 1][my_tank.x + 2] == 0) {
					tank_walk(&my_tank, 3, &my_tank_img[3], 1);
				}
				else {
					tank_walk(&my_tank, 3, &my_tank_img[3], 0);
				}
				break;
			case 'j'://fire!
				tank_fire(&my_tank, &my_bullet);
				break;
			case'p':
				system("pause");
				break;
			default:
				break;
			}
		}
	}
}
void set_prop_map(int x, int y, int set) {
	map[y][x] = set;
	map[y][x + 1] = set;
	map[y + 1][x] = set;
	map[y + 1][x + 1] = set;
}

int bullet_action(bullet_s* bullet, tank_s* enemy_tank) {
	int x, y, x1, y1;//position now

	x = bullet->pos_x / 25;
	y = bullet->pos_y / 25;

	setfillcolor(BLACK);
	solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 3, bullet->pos_y + 3);

	//movement of bullet
	if (bullet->direction == 0) {
		bullet->pos_y -= 2;
		x1 = x + 1;
		y1 = y;
	}
	else if (bullet->direction == 1) {
		bullet->pos_y += 2;
		x1 = x + 1;
		y1 = y;
	}
	else if (bullet->direction == 2) {
		bullet->pos_x -= 2;
		x1 = x;
		y1 = y + 1;
	}
	else if (bullet->direction == 3) {
		bullet->pos_x += 2;
		x1 = x;
		y1 = y + 1;
	}
	else {
		return 0;
	}

	if (bullet->pos_x < 0 || bullet->pos_x > 640 || bullet->pos_y < 0 || bullet->pos_y > 480) {
		bullet->status = 0;
		return 0;
	}

	//check collasion
	if (map[y][x] == 4 || map[y1][x1] == 4) {
		return 1;
	}
	
	if (map[y][x] == 200 || map[y1][x1] == 200) {
		return 1;
	}

	if ((map[y][x] >= 100 && map[y][x] <= 109) || (map[y][x] >= 100 && map[y][x] <= 109)) {
		tank_s* tank = NULL;
		bullet->status = 0;
		if (map[y][x] >= 100 && map[y][x] <= 109) {
			tank = enemy_tank + (map[y][x] - 100);
		}
		else {
			tank = enemy_tank + (map[y1][x1] - 100);
		}
		tank->live = 0;
		enemy_total--;
		set_prop_map(tank->x, tank->y, 0);
		setfillcolor(BLACK);
		solidrectangle(tank->x * 25, tank->y * 25, tank->x * 25 + 50, tank->y * 25 + 50);
	}

	//hit the block
	if (map[y][x] == 1) {
		map[y][x] = 0;
		bullet->status = 0;
		setfillcolor(BLACK);
		solidrectangle(x * 25, y * 25, x * 25 + 25, y * 25 + 25);
	}
	else if (map[y][x] == 2) {
		bullet->status = 0;
	}

	//hit the block
	if (map[y1][x1] == 1) {
		map[y1][x1] = 0;
		bullet->status = 0;
		setfillcolor(BLACK);
		solidrectangle(x1 * 25, y1 * 25, x1 * 25 + 25, y1 * 25 + 25);
	}
	else if (map[y1][x1] == 2) {
		bullet->status = 0;
	}

	//renew bullet
	if (bullet->status == 1) {
		setfillcolor(WHITE);
		solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 3, bullet->pos_y + 3);
	}
}

void enemy_tank_walk(tank_s* tank, int direction, IMAGE* img) {
	switch (direction) {
	case 2:
		if ((tank->direction == 2) && (tank->x - 1) >= 0 && map[tank->y][tank->x - 1] == 0 && map[tank->y + 1][tank->x - 1] == 0) {
			tank_walk(tank, 2, img, 1);
		}
		else if (tank->direction != 2) {
			tank->direction = 2;
			tank_walk(tank, 2, img, 0);
		}
		break;
	case 0:
		if ((tank->direction == 0) && (tank->y - 1) >= 0 && map[tank->y - 1][tank->x] == 0 && map[tank->y - 1][tank->x + 1] == 0) {
			tank_walk(tank, 0, img, 1);
		}
		else if (tank->direction != 0) {
			tank->direction = 0;
			tank_walk(tank, 0, img, 0);
		}
		break;
	case 1:
		if ((tank->direction == 1) && (tank->y + 2) <= 18 && map[tank->y + 2][tank->x] == 0 && map[tank->y + 2][tank->x + 1] == 0) {
			tank_walk(tank, 1, img, 1);
		}
		else if (tank->direction != 1) {
			tank->direction = 1;
			tank_walk(tank, 1, img, 0);
		}
		break;
	case 3:
		if ((tank->direction == 3) && (tank->x + 2) <= 25 && map[tank->y][tank->x + 2] == 0 && map[tank->y + 1][tank->x + 2] == 0) {
			tank_walk(tank, 3, img, 1);
		}
		else if (tank->direction != 3) {
			tank->direction = 3;
			tank_walk(tank, 3, img, 0);
		}
		break;
	default:
		break;
	}
}

int enemy_direction(tank_s* tank, int x, int y) {//adjust direction depand on aim's location
	int r = rand() % 100;

	if (tank->x > x) {// right up
		if (tank->y > y) {//left up
			if (r <= 50) return 0;
			else return 2;
		}
		else {//left down
			if (r <= 50) return 1;
			else return 2;
		}
	}
	else {//right down
	 if (tank->y > y) {
		 if (r <= 50) return 0;
		 else return 3;
	 }
	 else {
		 if (r <= 50) return 1;
		 else return 3;
	 }
}
}

void tank_fire(tank_s* tank, bullet_s* bullet) {
	if (bullet->status == 0) {
		PlaySound(_T("resource/sound/shot.wav"), NULL, SND_FILENAME | SND_ASYNC);
		if (tank->direction == 0) {
			bullet->pos_x = tank->x * 25 + 25;
			bullet->pos_y = tank->y * 25 - 3;
		}
		else if (tank->direction == 1) {
			bullet->pos_x = tank->x * 25 + 25;
			bullet->pos_y = tank->y * 25 + 53;
		}
		else if (tank->direction == 2) {
			bullet->pos_x = tank->x * 25 - 3;
			bullet->pos_y = tank->y * 25 + 25;
		}
		else if (tank->direction == 3) {
			bullet->pos_x = tank->x * 25 + 53;
			bullet->pos_y = tank->y * 25 + 25;
		}

		bullet->direction = tank->direction;
		bullet->status = 1;
	}
}