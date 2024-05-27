//i22-0890 Hassaan Anwar Project

/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You don't need to change anything in the driver code/main function except the part(s) where instructed.
 * You are expected to define your functionalities in the "functionality.h" header and call them here in the instructed part(s).
 * The game is started with a small box, you need to create other possible in "pieces.h" file in form of array.
	E.g., Syntax for 2 pieces, each having four coordinates -> int Pieces[2][4];
 * Load the images into the textures objects from img directory.
 * Don't make any changes to the "utils.h" header file except for changing the coordinate of screen or global variables of the game.
 * SUGGESTION: If you want to make changes in "utils.h", do it once you complete rest of the functionalities.
 * TIP: You can study SFML for drawing scenes, sprites and window.
 * */
#include<string>
#include<iostream>
#include<SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"
#include <sstream>
#include<fstream>
using namespace sf;


int main() {
	int shadow[4][2] = { 0 }, next_shape[4][2] = { 0 }, next_grid[4][2] = { 0 }, highscore[10] = { 0 };
	srand(time(0));

	int colorNum, n, NextC = 1 + rand() % 7, NextN = rand() % 7;
	n = NextN;
	colorNum = NextC;
	NextC = 1 + rand() % 7;
	NextN = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		point_1[i][0] = BLOCKS[n][i] % 2;
		point_1[i][1] = BLOCKS[n][i] / 2;
	}
	for (int i = 0; i < 4; i++)
	{
		next_shape[i][0] = BLOCKS[NextN][i] % 2;
		next_shape[i][1] = BLOCKS[NextN][i] / 2;
	}																//generating the first shape + next shape
	make_shadow(shadow);
	shadow_down(shadow);


	RenderWindow window(VideoMode(320, 480), title);

	//declaring texts

	Font font;
	font.loadFromFile("Sports World-Regular.ttf");

	Text text, text2, text3, text4;
	text.setFont(font);
	text.setCharacterSize(22);
	text.setPosition(270, 240);
	text.setFillColor(Color::Blue);

	text2.setFont(font);
	text2.setCharacterSize(23);
	text2.setPosition(230, 215);
	text2.setString("Score:");
	text2.setFillColor(Color::Red);

	text3.setFont(font);
	text3.setCharacterSize(20);
	text3.setPosition(230, 100);
	text3.setString("Next\n Shape:");
	text3.setFillColor(Color::Red);

	text4.setFont(font);
	text4.setFillColor(Color::Red);
	text4.setCharacterSize(25);
	text4.setPosition(130, 75);

	Texture obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10;
	obj1.loadFromFile("img/tiles.png");
	obj2.loadFromFile("img/background.png");
	obj3.loadFromFile("img/frame.png");
	obj4.loadFromFile("img/Game_Paused.png");
	obj5.loadFromFile("img/help.png");
	obj6.loadFromFile("img/Menu.png");
	obj7.loadFromFile("img/Level.png");
	obj8.loadFromFile("img/tiles.png");
	obj9.loadFromFile("img/tiles.png");
	obj10.loadFromFile("img/highscore.png");

	Sprite sprite(obj1), background(obj2), frame(obj3), Pmenu(obj4), help(obj5), menu(obj6), levelp(obj7), shad(obj8), next(obj9), high(obj10);

	SoundBuffer soundBuff, soundBuff2;
	Sound choice, backg;
	soundBuff.loadFromFile("sounds/choice.wav");
	soundBuff2.loadFromFile("sounds/theme.ogg");
	choice.setBuffer(soundBuff);
	backg.setBuffer(soundBuff2);
	backg.setVolume(5);
	backg.setLoop(true);
	backg.play();

	std::string Sscore, out;
	int score = 0, level, t_shapes;
	float timer = 0, delay, delay_const;
	bool game_over = 1;
	Clock clock;
	while (window.isOpen())
	{
		get_highscore(highscore);
		window.draw(menu);							//main menu
		window.display();
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Num1)		//start game option
				{
					choice.play();
					while (!(Keyboard::isKeyPressed(Keyboard::Escape)))
					{
						window.clear(Color::Black);
						window.draw(levelp);
						window.display();
						if (Keyboard::isKeyPressed(Keyboard::A))
						{
							choice.play();
							level = 1;							//assigning levels
							game_over = 0;
							delay_const = 0.5;
							t_shapes = 3;
							break;
						}
						if (Keyboard::isKeyPressed(Keyboard::B))
						{
							choice.play();
							level = 2;
							game_over = 0;
							delay_const = 0.35;
							t_shapes = 8;
							break;
						}
					}
					delay = delay_const;
				}
				else if (e.key.code == Keyboard::Num2)			//highscores
				{
					choice.play();
					std::string temp;
					for (int i = 0; i < 9; i++)
					{
						std::stringstream S;
						S << highscore[i];
						S >> temp;
						out += temp + '\n';
					}
					while (!(Keyboard::isKeyPressed(Keyboard::Escape)))
					{
						window.clear();
						window.draw(high);
						text4.setString(out);
						window.draw(text4);
						window.display();
					}
					choice.play();
					out = "";
				}
				else if (e.key.code == Keyboard::Num3)			//help
				{
					choice.play();
					while (!(Keyboard::isKeyPressed(Keyboard::Escape)))
					{
						window.clear(Color::Black);
						window.draw(help);
						window.display();
					}
					choice.play();
				}
				else if (e.key.code == Keyboard::Num4)		//exit
				{
					choice.play();
					window.close();
				}
			}
		}
		while (!game_over)
		{
			int delta_x = 0;
			bool pause = 0, rotate = 0;

			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			//---Event Listening Part---//
			Event e;
			while (window.pollEvent(e)) {                   		//Event is occurring - until the game is in running state
				if (e.type == Event::Closed)                  	//If cross/close is clicked/pressed
				{
					save_score(highscore);
					window.close();                           	//Opened window disposes
					return 0;
				}
				if (n != 7)										//during bomb movement disabled
				{
					if (e.type == Event::KeyPressed)			//If any other key (not cross) is pressed 
					{
						if (e.key.code == Keyboard::Up)           	//Check if the other key pressed is UP key
							rotate = 1;                        		//Rotation gets on
						else if (e.key.code == Keyboard::Left)    	//Check if the other key pressed is LEFT key
							delta_x = -1;                        	//Change in X-Axis - Negative
						else if (e.key.code == Keyboard::Right) 	//Check if the other key pressed is RIGHT key
							delta_x = 1;  							//Change in X-Axis - Positive
						if (e.key.code == Keyboard::Space)
							delay = 0.0000001;    						//Instantly Falls down
					}
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))   	//Just another way to detect key presses without event listener
				delay = 0.05;                                 //If DOWN key is pressed, delay in falling piece decreases from 0 to 0.05												

			///////////////////////////////////////////////
			///*** START CALLING YOUR FUNCTIONS HERE ***///
			if (e.key.code == Keyboard::P)					//pause
			{
				choice.play();
				pause = 1;
				while (pause)
				{
					window.clear(Color::Black);
					window.draw(Pmenu);
					window.display();
					Event e;
					while (window.pollEvent(e))
					{
						if (e.type == Event::Closed)
						{
							update_highscore(highscore, score);			//will update highscores
							save_score(highscore); 						//will save new highscores into file	
							window.close();
							return 0;
						}
						if (e.type == Event::KeyPressed)
						{
							if (e.key.code == Keyboard::Num1)				//new game
							{
								choice.play();
								while (1)
								{
									window.clear(Color::Black);
									window.draw(levelp);
									window.display();
									if (Keyboard::isKeyPressed(Keyboard::A))
									{
										choice.play();
										pause = 0;
										level = 1;
										game_over = 0;
										delay_const = 0.5;
										t_shapes = 3;
										break;
									}
									if (Keyboard::isKeyPressed(Keyboard::B))
									{
										choice.play();
										pause = 0;
										level = 2;
										game_over = 0;
										delay_const = 0.35;
										t_shapes = 8;
										break;
									}
								}
								update_highscore(highscore, score);			//updates highscore
								save_score(highscore);						//saves new highscore into file
								get_highscore(highscore);					//gets that new highscore from file
								delay = delay_const;
								reset(timer, colorNum, score);
								break;
							}
							else if (e.key.code == Keyboard::Num2)		//showing highscores
							{
								choice.play();
								std::string temp;
								for (int i = 0; i < 9; i++)
								{
									std::stringstream S;
									S << highscore[i];
									S >> temp;
									out += temp + '\n';
								}
								while (!(Keyboard::isKeyPressed(Keyboard::Escape)))
								{
									window.clear();
									window.draw(high);
									text4.setString(out);
									window.draw(text4);
									window.display();
								}
								out = "";
								choice.play();
							}
							else if (e.key.code == Keyboard::Num3)			//help option
							{
								choice.play();
								while (!(Keyboard::isKeyPressed(Keyboard::Escape)))
								{
									window.clear(Color::Black);
									window.draw(help);
									window.display();
								}
								choice.play();
							}
							else if (e.key.code == Keyboard::Num4)			//exit option
							{
								update_highscore(highscore, score);			//updates
								save_score(highscore);						//saves in file
								window.close();
								return 0;
							}
							else if (e.key.code == Keyboard::Num5)			//resume option
							{
								choice.play();
								pause = 0;
								break;
							}
						}
					}
				}
			}
			if (rotate)
				rotation(n, colorNum, shadow);
			fallingPiece(timer, delay, colorNum, n, NextC, NextN, t_shapes, shadow, next_shape, level);   //Example: fallingPiece() function is called here
			left_right(delta_x, colorNum, n, shadow);
			lineclear(score, level);

			std::stringstream ss;
			ss << score;
			ss >> Sscore;
			text.setString(Sscore);					//converting score into string then putting it into object
			game_over = game_check(colorNum);
			error_check(shadow);
			///*** YOUR CALLING STATEMENTS END HERE ***///
			//////////////////////////////////////////////


			window.clear(Color::Black);
			window.draw(background);

			//making next shape
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					next.setTextureRect(IntRect(next_grid[i][j] * 18, 0, 18, 18));
					next.setPosition(270, 150);
					window.draw(next);
				}
			}
			for (int i = 0; i < 4; i++)
			{
				next.setTextureRect(IntRect(NextC * 18, 0, 18, 18));
				next.setPosition(next_shape[i][0] * 18, next_shape[i][1] * 18);
				next.move(271, 150);
				window.draw(next);
			}
			//till here

			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (gameGrid[i][j] == 0)
						continue;
					sprite.setTextureRect(IntRect(gameGrid[i][j] * 18, 0, 18, 18));
					sprite.setPosition(j * 18, i * 18);
					sprite.move(28, 31); //offset
					window.draw(sprite);

					shad.setTextureRect(IntRect(gameGrid[i][j] * 18, 0, 18, 18));			//drawing shadow
					shad.setPosition(j * 18, i * 18);
					shad.move(28, 31); //offset
					window.draw(shad);
				}
			}
			for (int i = 0; i < 4; i++)
			{
				sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
				sprite.setPosition(point_1[i][0] * 18, point_1[i][1] * 18);
				sprite.move(28, 31);
				window.draw(sprite);

				shad.setTextureRect(IntRect(8 * 18, 0, 18, 18));
				shad.setPosition(shadow[i][0] * 18, shadow[i][1] * 18);
				shad.move(28, 31);
				window.draw(shad);
			}
			//---The Final on Which Everything is Drawn Over is Loaded---//
			window.draw(frame);
			window.draw(text);
			window.draw(text2);
			window.draw(text3);
			//---The Window that now Contains the Frame is Displayed---//
			window.display();
		}
		update_highscore(highscore, score);							//updates
		save_score(highscore);										//saving
		reset(timer, colorNum, score);								//will reset when game is over
	}
	return 0;
}
