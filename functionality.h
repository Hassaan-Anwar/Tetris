//i22-0890 Hassaan Anwar Project

/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */

 //---Piece Starts to Fall When Game Starts---//
#include<fstream>
#include<string>


void bomb_check(int);
void shadow_down(int[][2]);
void make_shadow(int[][2]);
void fallingPiece(float& timer, float& delay, int& colorNum, int& n, int& NextC, int& NextN, int t_shapes, int shadow[][2], int next_shape[][2], int level)
{

	if (timer > delay)
	{
		for (int i = 0; i < 4; i++) {
			point_2[i][0] = point_1[i][0];
			point_2[i][1] = point_1[i][1];
			point_1[i][1] += 1;                   //How much units downward
		}
		if (!anamoly())
		{
			if (level == 1)
				delay = 0.5;
			else
				delay = 0.35;
			if (n == 7)
			{
				bomb_check(colorNum);
			}
			else
			{
				for (int i = 0; i < 4; i++)
					gameGrid[point_2[i][1]][point_2[i][0]] = colorNum;		//we dont want to keep bomb within game
			}
			colorNum = NextC;
			n = NextN;
			NextC = 1 + rand() % 7;
			NextN = rand() % t_shapes;
			for (int i = 0; i < 4; i++)
			{
				point_1[i][0] = BLOCKS[n][i] % 2;
				point_1[i][1] = BLOCKS[n][i] / 2;
			}
			for (int i = 0; i < 4; i++)
			{
				next_shape[i][0] = BLOCKS[NextN][i] % 2;
				next_shape[i][1] = BLOCKS[NextN][i] / 2;
			}


			shadow_down(shadow);						//when shadow is created it instantly goes to most bottom row
			if (n == 7)
			{
				int x = rand() % N;						//bomb will be spawning at random colum
				for (int i = 0; i < 4; i++)
				{
					point_2[i][0] = point_1[i][0];
					point_2[i][1] = point_1[i][1];
					point_1[i][0] += x;
				}
			}
			make_shadow(shadow);						//creates shadow of object
			shadow_down(shadow);
		}
		timer = 0;
	}

}

/////////////////////////////////////////////
///*** START CODING YOUR FUNTIONS HERE ***///
void left_right(int& X_coord, int& colorNum, int n, int shadow[][2])
{

	if (X_coord > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((point_1[i][0] + 1) < 0 || (point_1[i][0] + 1) >= N || gameGrid[point_1[i][1]][point_1[i][0] + 1])		//if object is going outside boundry/into another block after moving
				return;
		}
		for (int i = 0; i < 4; i++)
		{
			point_2[i][0] = point_1[i][0];
			point_2[i][1] = point_1[i][1];
			point_1[i][0] += 1;
		}
		make_shadow(shadow);				//creating new shadow after movement
		shadow_down(shadow);				//making shadows fall to bottom most
	}
	else if (X_coord < 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((point_1[i][0] - 1) < 0 || (point_1[i][0] - 1) >= N || gameGrid[point_1[i][1]][point_1[i][0] - 1])		//same as explained before
				return;
		}
		for (int i = 0; i < 4; i++)
		{
			point_2[i][0] = point_1[i][0];
			point_2[i][1] = point_1[i][1];
			point_1[i][0] -= 1;
		}
		make_shadow(shadow);
		shadow_down(shadow);
	}
}

void rotation(int& n, int& color, int shadow[][2])
{
	int temp;
	for (int i = 0; i < 4; i++)
	{
		if (point_1[i][1] < 0 || point_1[i][0] >= N || gameGrid[point_1[i][0]][point_1[i][1]])
			return;
	}
	for (int i = 0; i < 4; i++)
	{
		temp = point_1[i][1];
		point_1[i][1] = point_1[i][0];
		point_1[i][0] = temp;
		point_2[i][0] = point_1[i][0];
		point_2[i][1] = point_1[i][1];
	}
	make_shadow(shadow);					//making new shadow after rotation
	shadow_down(shadow);

}



void lineclear(int& score, int level)
{
	int t_line = 0, count, temp;
	for (int i = 0; i < M; i++)
	{
		count = 0;
		for (int j = 0; j < N; j++)
		{
			if (gameGrid[i][j] > 0 && gameGrid[i][j] < 8)			//counting number of blocks on each line
				count++;
		}
		if (count == N)
		{
			t_line += 1;
			for (int j = 0; j < N; j++)
				gameGrid[i][j] = 0;							//clearing line
			for (int j = i; j > 0; j--)
			{
				for (int k = 0; k < N; k++)
					gameGrid[j][k] = gameGrid[j - 1][k];		//moving everything else one step below
			}
		}
	}
	switch (t_line)						//scores
	{
	case 1:
		score += 10 * level;
		break;
	case 2:
		score += 30 * level;
		break;
	case 3:
		score += 60 * level;
		break;
	case 4:
		score += 100 * level;
		break;
	}
}

void reset(float& timer, int& color, int& score)			//reseting the game
{

	timer = 0;
	score = 0;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
			gameGrid[i][j] = 0;
	}
	color = 1 + rand() % 7;
	int sh = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		point_1[i][0] = BLOCKS[sh][i] % 2;
		point_1[i][1] = BLOCKS[sh][i] / 2;
	}
}


bool game_check(int colorNum)			//checking if blocks reach top
{
	for (int i = 0; i < N; i++)
	{
		if (gameGrid[0][i] > 0 & gameGrid[0][i] < 8)
		{
			if (gameGrid[0][i] != colorNum)
				return 1;
		}
	}
	return 0;
}

void bomb_check(int color)				//seeing where bomb is landing 
{

	if (gameGrid[point_2[0][1] + 1][point_2[0][0]] == color)
	{
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				gameGrid[i][j] = 0;
	}
	else if (gameGrid[point_2[0][1] + 1][point_2[0][0]] == 0)
		return;
	else
	{
		for (int i = 0; i < 4; i++)
		{
			gameGrid[point_2[i][1] + 1][point_2[i][0]] = 0;
			gameGrid[point_2[i][1] + 1][point_2[i][0] + 1] = 0;
			gameGrid[point_2[i][1] + 1][point_2[i][0] - 1] = 0;
			gameGrid[point_2[i][1]][point_2[i][0] - 1] = 0;
		}
	}
}

void shadow_down(int shadow[][2])			//moves shadow to most bottom possible row
{
	bool contact = 0;
	while (!contact)
	{
		for (int i = 0; i < 4; i++)
		{
			if (gameGrid[shadow[i][1] + 1][shadow[i][0]] || shadow[i][1] + 1 >= M)
				contact = 1;
		}
		if (!contact)
		{
			for (int i = 0; i < 4; i++)
			{
				shadow[i][0] = shadow[i][0];
				shadow[i][1] = shadow[i][1];
				shadow[i][1] += 1;
			}
		}
	}
}
void make_shadow(int shadow[][2])			//makes shadow
{
	for (int i = 0; i < 4; i++)
	{
		shadow[i][0] = point_1[i][0];
		shadow[i][1] = point_1[i][1];
	}
}

void error_check(int shadow[][2])
{
	for (int i = 0; i < 4; i++)
	{
		if (shadow[i][1] > 19)
		{
			while (1)
			{
				for (int j = 0; j < 4; j++)
					shadow[j][1]--;
				if (shadow[i][1] == 19)
					break;
			}
		}
	}
}

void update_highscore(int highscore[], int score)			//sorting the highscore array
{
	int i, j, temp;
	if (score > highscore[9])
	{
		highscore[9] = score;
		for (i = 0; i < N; i++)
		{
			for (int j = 0; j < N - 1 - i; j++)
				if (highscore[j] < highscore[j + 1])
				{
					temp = highscore[j];
					highscore[j] = highscore[j + 1];
					highscore[j + 1] = temp;
				}
		}
	}
}

void get_highscore(int highscore[])				//getting highscores from file
{
	int index = 0;
	std::ifstream scores;
	scores.open("scores.txt");
	while (!scores.eof())
	{
		scores >> highscore[index];
		index++;
	}
	scores.close();
}


void save_score(int highscore[])				//writing the highscores back into file
{
	std::ofstream scores;
	scores.open("scores.txt");
	for (int i = 0; i < 10; i++)
		scores << highscore[i] << '\n';
	scores.close();
}



///*** YOUR FUNCTIONS END HERE ***///
/////////////////////////////////////
#pragma once
