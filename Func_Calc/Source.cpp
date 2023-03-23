#include <SDL.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#define WINDOW_HEIGHT 700
#define WINDOW_WIDTH 1000
#define ARRAY_SIZE 100
#define SEGMENT 40
#define STEP 0.01


SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;

void DeInit(int error)
{
	if(ren != NULL)SDL_DestroyRenderer(ren);
	if (win != NULL)SDL_DestroyWindow(win);
	SDL_Quit();
	exit(error);
}

void Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf_s("Couldn't init SDL! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	win = SDL_CreateWindow("SDL project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (win == NULL)
	{
		printf_s("Couldn't create window! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if (ren == NULL)
	{
		printf_s("Couldn't create renderer! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

}

bool IsNumber(char symb)
{
	if (symb == '0' || symb == '1' || symb == '2' || symb == '3' || symb == '4' || symb == '5' || symb == '6' || symb == '7' || symb == '8' || symb == '9')
		return 1;
	return 0;
}

bool IsArithmetic(char symb)
{
	if (symb == '+' || symb == '-' || symb == '*' || symb == '/')
		return 1;
	return 0;
}

bool IsVariable(char symb)
{
	if (symb == 'x' || symb == 'y')
		return 1;
	return 0;
}

void AddEnd(char func[])
{
	for (int i = 0;i < ARRAY_SIZE;i++)
	{
		if (func[i] == 0)
			func[i] = '\n';
	}
}

void GetNumbers(char func[], int numbers[])
{
	for (int i = 0, temp = 0, n = 0; func[i] != '\n';i++)
	{
		if (IsNumber(func[i]))
			temp = temp * 10 + func[i] - '0';
		
		else
		{
			numbers[n++] = temp;
			temp = 0;
		}

		if(func[i+1] == '\n')
		{
			numbers[n++] = temp;
			temp = 0;
		}



		if (func[i + 1] == '\n')
			numbers[n] = INT_MAX;
	}

}

void DrawAxis()
{


	SDL_SetRenderDrawColor(ren, 255, 165, 0, 0);

	//Буква Х
	SDL_RenderDrawLine(ren, WINDOW_WIDTH, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 30 * 2, WINDOW_WIDTH - WINDOW_WIDTH / 50, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 30 * 3);
	SDL_RenderDrawLine(ren, WINDOW_WIDTH - WINDOW_WIDTH / 50, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 30 * 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 30 * 3);

	//Буква Y
	SDL_RenderDrawLine(ren, WINDOW_WIDTH / 2 + WINDOW_WIDTH / 50 * 2, 0, WINDOW_WIDTH / 2 + WINDOW_WIDTH/50*2.5, WINDOW_HEIGHT / 30 / 2);
	SDL_RenderDrawLine(ren, WINDOW_WIDTH / 2 + WINDOW_WIDTH / 50 * 2, WINDOW_HEIGHT / 30, WINDOW_WIDTH / 2 + WINDOW_WIDTH / 50 * 3, 0);

	SDL_SetRenderDrawColor(ren, 170, 170, 170, 0);

	//Шаг по X
	for (int i = WINDOW_WIDTH/2; i < WINDOW_WIDTH; i += SEGMENT)
		SDL_RenderDrawLine(ren, i, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT * SEGMENT / 2.5, i, WINDOW_HEIGHT / 2 + WINDOW_HEIGHT * SEGMENT / 2.5);
	for (int i = WINDOW_WIDTH / 2; i > 0; i -= SEGMENT)
		SDL_RenderDrawLine(ren, i, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT * SEGMENT / 2.5, i, WINDOW_HEIGHT / 2 + WINDOW_HEIGHT * SEGMENT / 2.5);

	//Шаг по Y
	for (int i = WINDOW_HEIGHT / 2; i < WINDOW_HEIGHT - 1; i += SEGMENT)
		SDL_RenderDrawLine(ren, WINDOW_WIDTH / 2 - WINDOW_WIDTH * SEGMENT / 1.66, i, WINDOW_WIDTH / 2 + WINDOW_WIDTH * SEGMENT / 1.66, i);
	for (int i = WINDOW_HEIGHT / 2; i > 0 ; i -= SEGMENT)
		SDL_RenderDrawLine(ren, WINDOW_WIDTH / 2 - WINDOW_WIDTH * SEGMENT / 1.66, i, WINDOW_WIDTH / 2 + WINDOW_WIDTH * SEGMENT / 1.66, i);

	SDL_SetRenderDrawColor(ren, 200, 0, 200, 0);

	//Ось абсцисс и ординат
	SDL_RenderDrawLine(ren, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);
	SDL_RenderDrawLine(ren, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);

	//Стрелка ординат
	SDL_RenderDrawLine(ren, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2 - WINDOW_WIDTH / 50, 0 + WINDOW_HEIGHT / 30);
	SDL_RenderDrawLine(ren, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2 + WINDOW_WIDTH / 50, 0 + WINDOW_HEIGHT / 30);

	//Стрелка обсцисс
	SDL_RenderDrawLine(ren, WINDOW_WIDTH, WINDOW_HEIGHT / 2, WINDOW_WIDTH - WINDOW_WIDTH / 50, WINDOW_HEIGHT / 2 + WINDOW_HEIGHT / 30);
	SDL_RenderDrawLine(ren, WINDOW_WIDTH, WINDOW_HEIGHT / 2, WINDOW_WIDTH - WINDOW_WIDTH / 50, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 30);
}

void DrawGraph()
{
	SDL_SetRenderDrawColor(ren, 128, 0, 128, 0);

	double tempX1 = -10, tempX = 0, tempY = 0;
	double tempY1 = sin(tempX1);

	for (double i = -10; i <= 10; i += STEP)
	{
		SDL_RenderDrawPoint(ren, i * SEGMENT + WINDOW_WIDTH / 2, -cos(i) * SEGMENT + WINDOW_HEIGHT / 2);


		//tempX = tempX1;
		//tempY = tempY1;
		//tempX1 = i;
		//tempY1 = sin(tempX1);
		//SDL_RenderDrawLine(ren, (tempX+WINDOW_WIDTH/2)*SEGMENT*10, (tempY+WINDOW_HEIGHT/2) * SEGMENT * 10, (tempX1+WINDOW_WIDTH/2) * SEGMENT * 10, (tempY1+WINDOW_HEIGHT/2) * SEGMENT * 10);
	}
}

int main(int argc, char* argv[])
{
	system("chcp 866");
	system("cls");
	Init();

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
	SDL_RenderClear(ren);

	DrawAxis();

	DrawGraph();
	
	SDL_RenderPresent(ren);

	system("pause");

	DeInit(0);
	return 0;
}

/*char func[ARRAY_SIZE] = {};
	int numbers[ARRAY_SIZE];
	int otvet = 0;

	printf_s("Enter func: ");
	while (getchar() != '\n');
	gets_s(func);

	AddEnd(func);

	GetNumbers(func, numbers);


	for (int i = 0, temp = numbers[0], n = 1;func[i] != '\n';i++)
	{
		
		if (IsArithmetic(func[i]) && func[i] != '(')
		{
			numbers[n++];
			if (func[i] == '+')

		}
	}*/