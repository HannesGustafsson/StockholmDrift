
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <string>
#include <windows.h>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

bool gameHelp = false;
bool gameOver = false;
bool typed = false;
bool positionsResetBool = false;

const int mapWidth = 4;
const int mapHeight = 16;
float difficulty = 0.3;//Sets the number by which the fuel deceases each time. ((0,30 = EASY) (0.45 = NORMAL) ( 0,6 = HARD))

int points;
int pointsHighscore;
int delayTime = 70;
int obstacleCounter = 0;
float pointsMultiplier;

//Player position array
//[0] is X.position [1] is Y.position.
int playerPosition[2] = { (mapWidth/2), (mapHeight-1) };

//Obstacle position array
//[Obstacle number][0-position x, 1-position y]
int obstacle[4][2] = 
{
	{rand() % (mapWidth - 1) + 1, 0},
	{rand() % (mapWidth - 1) + 1, 0},
	{rand() % (mapWidth - 1) + 1, 0},
	{rand() % (mapWidth - 1) + 1, 0}
};
//Creates fuel
float fuelAmount;
int fuelPosition[2] = { rand() % (mapWidth - 1) + 1, rand() % (mapHeight - 1) + 5 };

//Defines key inputs
enum Key { UP = 119, DOWN = 115, LEFT = 97, RIGHT = 100, ESC = 27 };

//COLORS LIST
//1: Blue
//2: Green
//3: Cyan
//4: Reddd
//5: Purple
//6: Yellow (Dark)
//7: Default white
//8: Gray/Grey
//9: Bright blue
//10: Brigth green
//11: Bright cyan
//12: Bright red
//13: Pink/Magenta
//14: Yellow
//15: Bright white
//Numbers after 15 include background colors
void Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Draws start menu
void setup()
{
	cout << "~---~~~~====<'Stockholm Drift'>====~~~~---" << endl;
	Sleep(500);
	if (gameHelp == false)
	{
		fuelAmount = 20;
	}
	gameOver = false;
	Color(12);
	cout << "Ready... ";
	Sleep(500);
	Color(14);
	cout << "Set... ";
	Sleep(500);
	Color(10);
	cout << "GO!";
	Color(7);
	Sleep(500);
}

//Resets obstacles
void obstacleReset(int id)
{
	if (obstacle[id][1] == mapHeight)
	{
		obstacle[id][0] = rand() % (mapWidth-1) + 1;
		obstacle[id][1] = 0;
	}
}


//Moves obstacles downward
void obstacleMove(int id, int counter, int count, int moveAmount)
{
	if (counter >= count)
	{
		obstacle[id][1] += moveAmount;
	}
}

//Checks collision and counts points
void pointCounter(int playerY)
{
	if (playerY == obstacle[0][1] || playerY == obstacle[1][1] || playerY == obstacle[2][1] || playerY == obstacle[3][1])
	{
		points += ((32 * pointsMultiplier) + fuelAmount);
	}

	if (playerPosition[0] == obstacle[0][0] && playerPosition[1] == obstacle[0][1])
	{
		gameOver = true;
	}
	else if(playerPosition[0] == obstacle[1][0] && playerPosition[1] == obstacle[1][1])
	{
		gameOver = true;
	}
	else if(playerPosition[0] == obstacle[2][0] && playerPosition[1] == obstacle[2][1])
	{
		gameOver = true;
	}
	else if(playerPosition[0] == obstacle[3][0] && playerPosition[1] == obstacle[3][1])
	{
		gameOver = true;
	}
	else if (playerPosition[0] == fuelPosition[0] && playerPosition[1] == fuelPosition[1])
	{
		fuelPosition[0] = rand() % (mapWidth - 1) + 1;
		fuelPosition[1] = rand() % (mapHeight - 5) + 5;
		fuelAmount += 10;
		if (fuelAmount > 20)
		{
			fuelAmount = 20;
		}

	}
}

//Draws the map
void draw()
{
	system("cls");//Clear screen

	for (size_t y = 0; y < mapHeight; y++)
	{
		for (size_t x = 0; x <= mapWidth; x++)
		{
			//Draws edgelines
			if (x == 0 || x == mapWidth)
			{
				cout << "|";
				typed = true;
			}

			Color(12);//Sets color to bright red
			//Draws Obstacle 0
			if (obstacle[0][0] == x && obstacle[0][1] == y)
			{
				cout << "O";
				typed = true;
			}
			//Draws Obstacle 1
			if ((obstacle[1][0] == x && obstacle[1][1] == y) && obstacleCounter >= 4)
			{
				cout << "O";
				typed = true;
			}
			//Draws Obstacle 2
			if ((obstacle[2][0] == x && obstacle[2][1] == y) && obstacleCounter >= 8)
			{
				cout << "O";
				typed = true;
			}
			//Draws Obstacle 3
			if ((obstacle[3][0] == x && obstacle[3][1] == y) && obstacleCounter >= 12)
			{
				cout << "O";
				typed = true;
			}
			Color(7);//Resets color to white
			//Draws Player
			if ((x == playerPosition[0] && y == playerPosition[1]) && typed == false)
			{
				Color(15);
				cout << "8";
				Color(7);
				typed = true;
			}
			//Drawss fuel
			if ((x == fuelPosition[0] && y == fuelPosition[1]) && typed == false)
			{
				Color(10);
				cout << "*";
				typed = true;
				Color(7);
			}
			//Drawss middle line
			if (x == (mapWidth / 2) && typed == false)
			{
				cout << ".";
				typed = true;
			}
			//Draws blank
			if (typed == false)
			{
				cout << " ";
				typed = true;
			}

			if (y == 5 && x == mapWidth)
			{
				cout << "Score: ";
				Color(15);
				cout << points;
				Color(7);
			}

			//Draws score
			if (y == 6 && x == mapWidth)
			{
				float mapHeightFloat = mapHeight;
				float playerPositionFloat = playerPosition[1];

				pointsMultiplier = ((((mapHeightFloat * 2) - playerPositionFloat) / mapHeightFloat) + (fuelAmount / 32));
				cout << "Score multiplier: ";
				Color(15);
				printf("%.2f", pointsMultiplier);
				Color(7);
			}

			if (y == 7 && x == mapWidth)
			{
				cout << "Press 'H' for help. ";
			}
			//Draws fuel amount & Game over if fuel goes < 0
			if (y == mapHeight - 1 && x == mapWidth)
			{
				cout << "FUEL: ";
				if (fuelAmount < 0)
				{
					Color(12);
					cout << "OUT OF FUEL";
					gameOver = true;
					Color(7);
				}
				for (int i = 0; i < 21; i++)
				{
					if (i == 20)
					{
						cout << "|";
					}
					else if (i < fuelAmount)
					{
						if (fuelAmount > 13)
						{
							Color(10);
						}
						else if (fuelAmount > 7)
						{
							Color(2);
						}
						else
						{
							Color(4);
						}
						cout << "=";
						Color(7);
					}
					else
					{
						cout << " ";
					}
				}
				Color(7);
			}


			typed = false;

			//Reset the obstacle position is they hit bottom
			obstacleReset(0);
			obstacleReset(1);
			obstacleReset(2);
			obstacleReset(3);
		}
		cout << endl;
	}
}

//Checks for user input
void input()
{
	if (_kbhit())
	{
		//Controls
		switch (_getch())
		{
		case LEFT:
			if (playerPosition[0] != 1)
			{
				playerPosition[0]--;
			}
			break;
		case RIGHT:
			if (playerPosition[0] != (mapWidth-1))
			{
				playerPosition[0]++;
			}
			break;
		case UP:
			if (playerPosition[1] != 1)
			{
				playerPosition[1]--;
			}
			break;
		case DOWN:
			if (playerPosition[1] != (mapHeight-1))
			{
				playerPosition[1]++;
			}
			break;
		case 'h':
			gameOver = true;
			gameHelp = true;
			break;
		}
	}
}

//Resets objects before restart
void positionsReset()
{
	obstacle[0][1] = 0;
	obstacle[1][1] = 0;
	obstacle[2][1] = 0;
	obstacle[3][1] = 0;
	obstacleCounter = 0;

	playerPosition[0] = (mapWidth / 2);
	playerPosition[1] = (mapHeight - 1);
}

int main()
{
	while (true)
	{
		setup();
		srand(time(NULL));
		gameHelp = false;

		while (gameOver == false)
		{
			input();

			draw();
			pointCounter(playerPosition[1]);
			fuelAmount -= difficulty;

			obstacleCounter++;
			obstacleMove(0, obstacleCounter, 0, 1);
			obstacleMove(1, obstacleCounter, 4, 1);
			obstacleMove(2, obstacleCounter, 8, 1);
			obstacleMove(3, obstacleCounter, 12, 1);

			Sleep(delayTime);
		}

		if (points > pointsHighscore)
		{
			pointsHighscore = points;
		}
		while (gameHelp == true)
		{
			system("cls");
			cout << "~---~~~~====<'Stockholm Drift'>====~~~~---" << endl;
			cout << "You gain points for each passing car. '";
			Color(12);
			cout << "O";
			Color(7);
			cout << "'" << endl;
			cout << "The multiplier increases number of points gained." << endl;
			cout << "Being higher up and having more fuel increases the multiplier." << endl;
			cout << "Collect fuel '";
			Color(10);
			cout << "*";
			Color(7);
			cout << "' before you run empty!" << endl << endl;
			cout << "Make sure not to crash though!" << endl << endl;
			cout << "Press 'Enter' to return to game.";
			cin.ignore(101, '\n');
			break;
		}

		if (gameHelp == false)
		{
			//End screen
			cout << "\nGame over!" << endl << endl << "Your score was: ";
			Color(15);
			cout << points << endl;
			Color(7);
			cout << "Highscore: ";
			Color(15);
			cout << pointsHighscore << endl;
			Color(7);
			points = 0;
			positionsReset();

			cout << "Press 'Enter' to continue.";
			cin.ignore(101, '\n');
		}
	}
}
