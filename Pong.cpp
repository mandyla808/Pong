
#include <iostream>
#include <string>
#include <time.h>
#include <conio.h>
#include <windows.h>

using namespace std;
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT =5, DOWNRIGHT = 6 };

class Ball {
private:
	int x, y;
	int originalX, originalY;
	eDir direction;
public:
	Ball(int posX, int posY) {
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		direction = STOP;
	}
	void Reset() {
		x = originalX; 
		y = originalY;
		direction = STOP;
	}
	void changeDirection(eDir d) {
		direction = d;
	}
	void randomDirection() {
		direction = (eDir)((rand() % 6) + 1);
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline eDir getDirection() { return direction; }
	void Move() {
		switch (direction) {
		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}
	friend ostream & operator<<(ostream & o, Ball c) {
		o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]";
		return o;
	}
};

class Paddle {
private:
	int x, y;
	int originalX, originalY;
public:
	Paddle() {
		x = y = 0;
	}
	Paddle(int posX, int posY) : Paddle() {
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void Reset() { x = originalX; y = originalY; }
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp() { y--; }
	inline void moveDown() { y++; }
	friend ostream & operator<<(ostream & o, Paddle c) {
		o << "Paddle [" << c.x << "," << c.y << "]";
		return o;
	}
};

// Sets up game logistics
class GameHandler {
private:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	Ball *ball;
	Paddle *player1;
	Paddle *player2;
public:
	GameHandler(int w, int h) {
		srand(time(NULL));
		quit = false;
		up1 = 'w'; up2 = 72;
		down1 = 's'; down2 = 80;
		score1 = score2 = 0;
		width = w; height = h;
		ball = new Ball(w / 2, h / 2);
		player1 = new Paddle(1, h / 2 - 3);
		player2 = new Paddle(w - 2, h / 2 - 3);
	}
	~GameHandler() {
		delete ball, player1, player2;
	}
	void IncrementScore(Paddle *player) {
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;
		ball->Reset();
		player1->Reset();
		player2->Reset();
	}
	void Draw() {
		system("cls");
		string screen;
		// top wall
		for (int i = 0; i < width + 2; i++)
			screen.append("-");
		screen.append("\n");
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				int ballx = ball->getX();
				int bally = ball->getY();
				int player1x = player1->getX();
				int player2x = player2->getX();
				int player1y = player1->getY();
				int player2y = player2->getY();
				// left wall
				if (j == 0)
					screen.append("!");
				// ball
				if (ballx == j && bally == i)
					screen.append("O");
				// player 1 paddle
				else if (player1x == j && i-3 <= player1y && player1y <= i)
					screen.append("|");
				// player 2 paddle
				else if (player2x == j && i-3 <= player2y && player2y <= i)
					screen.append("|"); 
				else
					screen.append(" ");
				// right wall
				if (j == width - 1)
					screen.append("!");
			}
			screen.append("\n");
		}
		// bottom wall
		for (int i = 0; i < width + 2; i++)
			screen.append("-");
		screen.append("\n");
		cout << screen;
		// scoreboard
		cout << "Scoreboard" << endl << "Player 1: "
			<< score1 << endl << "Player 2: " << score2 << "\n";
	}
	void Input() {
		ball->Move();
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();
		if (_kbhit()) {
			int current = _getch();
			if (current == up1)
				if (player1y > 0)
					player1->moveUp();
			if (current == up2)
				if (player2y > 0)
					player2->moveUp();
			if (current == down1)
				if (player1y + 4 < height)
					player1->moveDown();
			if (current == down2)
				if (player2y + 4 < height)
					player2->moveDown();
			if (ball->getDirection() == STOP)
				ball->randomDirection();
			if (current == 'q')
				quit = true;
		}
	}
	// handles logic for bouncing ball and scoring
	void Logic() {
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();
		// left paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					ball->changeDirection((eDir)((rand() % 3) + 4));
		// right paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i)
					ball->changeDirection((eDir)((rand() % 3) + 1));
		// BOTTOM WALL
		if (bally == height - 1)
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
		// TOP WALL
		if (bally == 0)
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
		// RIGHT WALL
		if (ballx == width - 1)
			IncrementScore(player1);
		// LEFT WALL
		if (ballx == 0)
			IncrementScore(player2);
	}
	void Run() {
		while (!quit) {
			Draw();
			Input();
			Logic();
		}
	}
};

int main()
{
	GameHandler c(40, 20);
	c.Run();
	return 0;
}

