#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#define KEY_DOWN 80
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_SPACE 32

#define COLOR_GREEN 2
#define COLOR_WHITE 15
#define COLOR_RED 4

#define FIELD_WIDTH 50
#define FIELD_HEIGHT 20

void printField(HANDLE h, COORD c)
{
	int x;
	for (int y = 0; y <= FIELD_HEIGHT + 1; y++)
	{
		x = 0;
		c.Y = y;
		if (y == 0 || y == FIELD_HEIGHT + 1)
		{
			for (; x <= FIELD_WIDTH + 1; x++)
			{
				c.X = x;
				SetConsoleCursorPosition(h, c);
				putchar('-');
			}
		}
		else
		{
			c.X = x;
			SetConsoleCursorPosition(h, c);
			putchar('|');
			x = FIELD_WIDTH + 1;
			c.X = x;
			SetConsoleCursorPosition(h, c);
			putchar('|');
		}
	}

	c.X = FIELD_WIDTH + 5;
	c.Y = 2;
	SetConsoleCursorPosition(h, c);
	printf("Sectoring... Press spacebar to finish");
}

int main()
{
	// initialization
	COORD c;
	c.X = 1;
	c.Y = 1;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), 0 };
	SetConsoleCursorInfo(h, &cci);

	_Bool field[FIELD_HEIGHT][FIELD_WIDTH];
	memset(field, 0, FIELD_WIDTH * FIELD_HEIGHT * sizeof(_Bool));

	int* changedCells_x = NULL;
	int* changedCells_y = NULL;

	int allLivingCells;
	int changedCellsCounter;
	int livingCellsCounterAround;
	int swap_x, swap_y;

	_Bool sectoringIsFinished = 0;
	_Bool game = 0;

	printField(h, c);

	c.X = 1;
	c.Y = 1;
	SetConsoleCursorPosition(h, c);
	putchar(219);
	while (1)
	{
		while (!sectoringIsFinished)
		{
			if (_kbhit())
			{
				int key = _getch();

				switch (key)
				{
				case KEY_DOWN:
					if (field[c.Y - 1][c.X - 1] == 1)
					{
						SetConsoleCursorPosition(h, c);
						putchar(219);
					}
					else
					{
						SetConsoleCursorPosition(h, c);
						putchar(' ');
					}
					if (c.Y < FIELD_HEIGHT)
						c.Y++;
					else
						c.Y = 1;
					SetConsoleCursorPosition(h, c);
					putchar(219);
					break;
				case KEY_UP:
					if (field[c.Y - 1][c.X - 1] == 1)
					{
						SetConsoleCursorPosition(h, c);
						putchar(219);
					}
					else
					{
						SetConsoleCursorPosition(h, c);
						putchar(' ');
					}
					if (c.Y > 1)
						c.Y--;
					else
						c.Y = FIELD_HEIGHT;
					SetConsoleCursorPosition(h, c);
					putchar(219);
					break;
				case KEY_RIGHT:
					if (field[c.Y - 1][c.X - 1] == 1)
					{
						SetConsoleCursorPosition(h, c);
						putchar(219);
					}
					else
					{
						SetConsoleCursorPosition(h, c);
						putchar(' ');
					}
					if (c.X < FIELD_WIDTH)
						c.X++;
					else
						c.X = 1;
					SetConsoleCursorPosition(h, c);
					putchar(219);
					break;
				case KEY_LEFT:
					if (field[c.Y - 1][c.X - 1] == 1)
					{
						SetConsoleCursorPosition(h, c);
						putchar(219);
					}
					else
					{
						SetConsoleCursorPosition(h, c);
						putchar(' ');
					}
					if (c.X > 1)
						c.X--;
					else
						c.X = FIELD_WIDTH;
					SetConsoleCursorPosition(h, c);
					putchar(219);
					break;
				case KEY_ENTER:
					field[c.Y - 1][c.X - 1] = !field[c.Y - 1][c.X - 1];
					break;
				case KEY_SPACE:
					sectoringIsFinished = 1;
					game = 1;
					c.X = FIELD_WIDTH + 5;
					c.Y = 2;
					SetConsoleCursorPosition(h, c);
					printf("                                     ");
					SetConsoleCursorPosition(h, c);
					SetConsoleTextAttribute(h, COLOR_GREEN);
					printf("The game goes!");
					SetConsoleTextAttribute(h, COLOR_WHITE);
					c.X = FIELD_WIDTH + 5;
					c.Y = 3;
					SetConsoleCursorPosition(h, c);
					printf("Press enter to suspend");
				default:
					break;
				}
			}
		}

		while (game)
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case KEY_ENTER:
					c.X = FIELD_WIDTH + 5;
					c.Y = 2;
					SetConsoleCursorPosition(h, c);
					printf("              ");
					SetConsoleTextAttribute(h, COLOR_RED);
					SetConsoleCursorPosition(h, c);
					printf("The game is suspended");
					SetConsoleTextAttribute(h, COLOR_WHITE);
					c.X = FIELD_WIDTH + 5;
					c.Y = 3;
					SetConsoleCursorPosition(h, c);
					printf("Press any key to resume");
					_getch();
					c.X = FIELD_WIDTH + 5;
					c.Y = 3;
					SetConsoleCursorPosition(h, c);
					printf("                        ");
					SetConsoleCursorPosition(h, c);
					printf("Press enter to suspend");
					c.X = FIELD_WIDTH + 5;
					c.Y = 2;
					SetConsoleCursorPosition(h, c);
					printf("                     ");
					SetConsoleTextAttribute(h, COLOR_GREEN);
					SetConsoleCursorPosition(h, c);
					printf("The game goes!");
					SetConsoleTextAttribute(h, COLOR_WHITE);
					break;
				default:
					break;
				}
			}

			Sleep(100);

			allLivingCells = 0;
			changedCellsCounter = 0;

			for (int i = 0; i < FIELD_HEIGHT; i++)
			{
				for (int j = 0; j < FIELD_WIDTH; j++)
				{
					if (field[i][j])
						allLivingCells++;

					livingCellsCounterAround = 0;
					for (int y = i - 1; y <= i + 1; y++)
					{
						for (int x = j - 1; x <= j + 1; x++)
						{
							if (x == j && y == i)
								continue;

							swap_x = x;
							swap_y = y;

							if (x == -1)
								x = FIELD_WIDTH - 1;
							if (y == -1)
								y = FIELD_HEIGHT - 1;
							if (x == FIELD_WIDTH)
								x = 0;
							if (y == FIELD_HEIGHT)
								y = 0;

							if (field[y][x])
								livingCellsCounterAround++;

							x = swap_x;
							y = swap_y;
						}
					}

					if (!field[i][j])
					{
						if (livingCellsCounterAround == 3)
						{
							changedCellsCounter++;

							changedCells_x = (int*)realloc(changedCells_x, changedCellsCounter * sizeof(int));
							changedCells_y = (int*)realloc(changedCells_y, changedCellsCounter * sizeof(int));

							changedCells_x[changedCellsCounter - 1] = j;
							changedCells_y[changedCellsCounter - 1] = i;
						}
					}
					else
					{
						if (livingCellsCounterAround < 2 || livingCellsCounterAround > 3)
						{
							changedCellsCounter++;

							changedCells_x = (int*)realloc(changedCells_x, changedCellsCounter * sizeof(int));
							changedCells_y = (int*)realloc(changedCells_y, changedCellsCounter * sizeof(int));

							changedCells_x[changedCellsCounter - 1] = j;
							changedCells_y[changedCellsCounter - 1] = i;
						}
					}
				}
			}
			
			c.X = FIELD_WIDTH + 5;
			c.Y = 4;
			SetConsoleCursorPosition(h, c);
			printf("Population:    ");
			c.X = FIELD_WIDTH + 17;
			c.Y = 4;
			SetConsoleCursorPosition(h, c);
			printf("%d", allLivingCells);

			if (changedCellsCounter == 0)
			{
				c.X = FIELD_WIDTH + 5;
				c.Y = 2;
				SetConsoleCursorPosition(h, c);
				printf("              ");
				SetConsoleCursorPosition(h, c);
				SetConsoleTextAttribute(h, COLOR_RED);
				printf("The game is over.");
				c.X = FIELD_WIDTH + 5;
				c.Y = 3;
				SetConsoleCursorPosition(h, c);
				printf("                      ");
				game = 0;
			}
			else
			{
				// changing the state of cells
				for (int i = 0; i < changedCellsCounter; i++)
				{
					field[changedCells_y[i]][changedCells_x[i]] = !field[changedCells_y[i]][changedCells_x[i]];

					// drawing new state
					c.X = changedCells_x[i] + 1;
					c.Y = changedCells_y[i] + 1;
					SetConsoleCursorPosition(h, c);
					if (field[changedCells_y[i]][changedCells_x[i]])
						putchar(219);
					else
						putchar(' ');
				}
			}
		}

		SetConsoleTextAttribute(h, COLOR_WHITE);
		c.X = FIELD_WIDTH + 5;
		c.Y = 6;
		SetConsoleCursorPosition(h, c);
		printf("Do you want to begin again?");
		c.X = FIELD_WIDTH + 5;
		c.Y = 7;
		SetConsoleCursorPosition(h, c);
		printf("Press enter if yes.");

		if (_kbhit())
		{
			switch (_getch())
			{
			case KEY_ENTER:
				memset(field, 0, FIELD_WIDTH * FIELD_HEIGHT * sizeof(_Bool));
				system("cls");
				printField(h, c);
				c.X = 1;
				c.Y = 1;
				SetConsoleCursorPosition(h, c);
				putchar(219);
				sectoringIsFinished = 0;
				break;
			default:
				break;
			}
		}
	}
}