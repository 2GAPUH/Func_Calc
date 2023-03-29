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
#define SEGMENT 30
#define STEP 0.01

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;

struct coefficients
{
	double a, b, c, d, e;
};

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

void ValueSubstitution(double tempNum[], double x)
{
	for (int i = 0;tempNum[i] != INT_MAX;i++)
	{
		if (tempNum[i] == INT_MIN)
			tempNum[i] = x;
	}
}

int IsNumber(char symb)
{
	if (symb == '0' || symb == '1' || symb == '2' || symb == '3' || symb == '4' || symb == '5' || symb == '6' || symb == '7' || symb == '8' || symb == '9')
		return 1;
	if (symb == 'x' || symb == 'X')
		return 2;
	return 0;
}

bool IsArithmetic(char symb)
{
	if (symb == '+' || symb == '-' || symb == '/' || symb == '*' || symb == '^' )
		return 1;
	return 0;
}

void GetFunc(char func[])
{
	int i = -1;
	char temp = 0;
	for (i; temp != 13; )
	{
		temp = _getch();
		if (IsArithmetic(temp) && not(IsArithmetic( func[i])) && i != -1)
		{
			i++;
			printf_s("%c", temp);
			func[i] = temp;
			continue;
		}
		if (IsNumber(temp) == 1)
		{
			i++;
			printf_s("%c", temp);
			func[i] = temp;
			continue;
		}
		if (IsNumber(temp) == 2 && IsNumber(func[i]) != 1)
		{
			i++;
			printf_s("%c", temp);
			func[i] = temp;
			continue;
		}

		
	}
	func[i+1] = '\n';
	printf_s("\n");
}

void GetNumbers(char func[], double numbers[])
{
	int n = 0;
	int temp;
	for (int i = 0;func[i] != '\n';i++)
	{
		temp = IsNumber(func[i]);
		if (temp == 1)
		{
			numbers[n] = numbers[n]*10 + func[i] - '0';
		}
		else if(temp == 2)
		{
			numbers[n] = INT_MIN;
		}
		else  n++;

	}
	numbers[n+1] = INT_MAX;
}

void GetArithmetic(char func[], char arithmetic[])
{
	int n = 0;
	for (int i = 0;func[i] != '\n';i++)
	{
		if (IsArithmetic(func[i]))
		{
			arithmetic[n++] = func[i];
		}
	}
	arithmetic[n] = '\n';
}

void CopyArrayNum(double numbers[], double tempNum[])
{
	int i = 0;
	for (i; numbers[i] != INT_MAX;i++)
		tempNum[i] = numbers[i];
	tempNum[i] = INT_MAX;
}

void Calculations(char arithmetic[], double tempNum[])
{
	//Обратная Польская нотация
	double temp = 0, temp1 = 0;

	for (int i = 0; arithmetic[i] != '\n';i++)
	{
		if (arithmetic[i] == '^')
		{
			int n = i;
			while (tempNum[n] == INT_MAX - 1)
				n--;
			temp = tempNum[i];
			for (int n = 1;n < tempNum[i + 1];n++)
				tempNum[i] *= temp;
			tempNum[i + 1] = INT_MAX - 1;
		}
	}

	for (int i = 0; arithmetic[i] != '\n';i++)
	{
		if (arithmetic[i] == '*')
		{
			int n = i;
			while (tempNum[n] == INT_MAX - 1)
				n--;
			tempNum[n] *= tempNum[i + 1];
			tempNum[i + 1] = INT_MAX - 1;
		}
		if (arithmetic[i] == '/')
		{
			int n = i;
			while (tempNum[n] == INT_MAX - 1)
				n--;
			if (tempNum[i + 1] == 0)
			{
				tempNum[n] = 3;
				tempNum[i + 1] = INT_MAX - 1;
				continue;
			}
			tempNum[n] /= tempNum[i + 1];
			tempNum[i + 1] = INT_MAX - 1;
		}
	}


	for (int i = 0; arithmetic[i] != '\n';i++)
	{
		if (arithmetic[i] == '+')
		{
			int n = i;
			while (tempNum[n] == INT_MAX-1)
				n--;
			tempNum[n] += tempNum[i + 1];
			tempNum[i + 1] = INT_MAX - 1;
		}
		if (arithmetic[i] == '-')
		{
			int n = i;
			while (tempNum[n] == INT_MAX - 1)
				n--;
			tempNum[n] -= tempNum[i + 1];
			tempNum[i + 1] = INT_MAX - 1;
		}
	}

}

double ctg(double value)
{
	return cos(value)/sin(value);
}


void DrawGraph(char arithmetic[], double numbers[], double tempNum[], coefficients funcCof, int type, double start = (-WINDOW_WIDTH / 2 / SEGMENT), double stop = WINDOW_WIDTH / 2 / SEGMENT)
{
	if (start < (-WINDOW_WIDTH / 2 / SEGMENT))
		start = (-WINDOW_WIDTH / 2 / SEGMENT);

	if (stop > WINDOW_WIDTH / 2 / SEGMENT)
		stop = WINDOW_WIDTH / 2 / SEGMENT;

	SDL_SetRenderDrawColor(ren, 128, 0, 128, 0);

	double tempX1 = start, tempX = 0, tempY = 0;
	double tempY1 = -1;

	for (double i = start; i <= stop; i += STEP)
	{
		if (i == 0)
			continue;

		int temp0 = 1;
		tempX = tempX1;
		tempY = tempY1;
		tempX1 = i;


		
		if (type == 0)
		{
			CopyArrayNum(numbers, tempNum);
			ValueSubstitution(tempNum, i);
			Calculations(arithmetic, tempNum);
			tempY1 = -tempNum[0];
		}
		
		else if (type == 1)
		{
			tempY1 = -(funcCof.a * sin(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		else if (type == 2)
		{
			tempY1 = -(funcCof.a * cos(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		else if (type == 3)
		{
			tempY1 = -(funcCof.a * tan(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		else if (type == 4)
		{
			tempY1 = -(funcCof.a * ctg(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		else if (type == 5)
		{
			//tempY1 = -(abs(funcCof.a * i + funcCof.b ) + funcCof.c);
			tempY1 = -( sqrt(cos(i)) * cos(200 * i) + sqrt(abs(i)) - 3.14 / 4 * pow((4 - i * i), 0.01));
			//tempY1 = -sqrt(10.1 * 10 - i * i);
		}

		else if (type == 6)
		{
			if (funcCof.b * i + funcCof.c < -1 || funcCof.b * i + funcCof.c > 1)
				continue;
			tempY1 = -(funcCof.a * asin(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		else if (type == 7)
		{
			if (funcCof.b * i + funcCof.c < -1 || funcCof.b * i + funcCof.c > 1)
				continue;
			tempY1 = -(funcCof.a * acos(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		else if (type == 8)
		{
			if (funcCof.b * i + funcCof.c < -1 || funcCof.b * i + funcCof.c > 1)
				continue;
			tempY1 = -(funcCof.a * atan(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		else if (type == 9)
		{
			if (funcCof.b * i + funcCof.c < -1 || funcCof.b * i + funcCof.c > 1)
				continue;
			tempY1 = -(funcCof.a * atan(funcCof.b * i + funcCof.c) + funcCof.d);
		}

		if (tempY1 > WINDOW_HEIGHT / SEGMENT)
			tempY1 = WINDOW_HEIGHT / SEGMENT;
		SDL_RenderDrawLine(ren, tempX * SEGMENT + WINDOW_WIDTH / 2, tempY * SEGMENT + WINDOW_HEIGHT / 2, tempX1 * SEGMENT + WINDOW_WIDTH / 2, tempY1 * SEGMENT + WINDOW_HEIGHT / 2);
		//if(type == 5)SDL_RenderDrawLine(ren, tempX * SEGMENT + WINDOW_WIDTH / 2, -tempY * SEGMENT + WINDOW_HEIGHT / 2, tempX1 * SEGMENT + WINDOW_WIDTH / 2, -tempY1 * SEGMENT + WINDOW_HEIGHT / 2);
	}
}

coefficients GetCoefficients(coefficients funcCof, int count)
{
	int i = 1;
	if (i++ <= count) 
	{
		printf_s("Enter a: ");
		scanf_s("%lf", &funcCof.a);
	}
	if (i++ <= count)
	{
		printf_s("Enter b: ");
		scanf_s("%lf", &funcCof.b);
	}
	if (i++ <= count)
	{
		printf_s("Enter c: ");
		scanf_s("%lf", &funcCof.c);
	}
	if (i++ <= count)
	{
		printf_s("Enter d: ");
		scanf_s("%lf", &funcCof.d);
	}
	if (i++ <= count)
	{
		printf_s("Enter d: ");
		scanf_s("%lf", &funcCof.d);
	}
	return funcCof;
}

int main(int argc, char* argv[])
{
	system("chcp 866");
	system("cls");
	char func[ARRAY_SIZE] = {};
	char arithmetic[ARRAY_SIZE / 2] = {};
	double numbers[ARRAY_SIZE] = {};
	double tempNum[ARRAY_SIZE] = {};
	coefficients funcCof = { 0, 0, 0, 0, 0 };
	int type;

	printf_s("0 - Without trigonometry modules and brackets \n\
1 - a * sin(b * x + c) + d \n\
2 - a * cos(b * x + c) + d \n\
3 - a * tg(b * x + c) + d \n\
4 - a * ctg(b * x + c) + d \n\
5 - |a * x + b| + c \n\
6 - a * asin(b * x + c) + d \n\
7 - a * acos(b * x + c) + d \n\
8 - a * atg(b * x + c) + d \n\
9 - Y = A * ln(x) + B, [1;15]. ");



	do {
		printf_s("Choose your function type:");
		scanf_s("%d", &type);
		system("cls");
		switch (type)
		{
		case 0:
		{
			printf_s("Enter your function: ");
			GetFunc(func);
			GetArithmetic(func, arithmetic);
			GetNumbers(func, numbers);
			break;
		}
		case 1:
		{
			printf_s("a * sin(b * x + c) + d\n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}
		case 2:
		{
			printf_s("a * cos(b * x + c) + d\n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}
		case 3:
		{
			printf_s("a * tg(b * x + c) + d\n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}
		case 4:
		{
			printf_s("a * ctg(b * x + c) + d\n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}
		case 5:
		{
			printf_s("|a * x + b| + c \n");
			funcCof = GetCoefficients(funcCof, 3);
			break;
		}
		case 6:
		{
			printf_s("a * asin(b * x + c) + d\n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}
		case 7:
		{
			printf_s("a * acos(b * x + c) + d\n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}
		case 8:
		{
			printf_s("a * atg(b * x + c) + d\n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}
		case 9:
		{
			printf_s("Y = A * ln(x) + B, [1;15] \n");
			funcCof = GetCoefficients(funcCof, 4);
			break;
		}

		default:
			printf_s("Try again!\n");
		}
	} while (type > 8 || type < 0);
	
	Init();

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
	SDL_RenderClear(ren);

	DrawAxis();

	DrawGraph(arithmetic , numbers, tempNum, funcCof, type);
	
	SDL_RenderPresent(ren);

	system("pause");

	DeInit(0);
	return 0;
}
