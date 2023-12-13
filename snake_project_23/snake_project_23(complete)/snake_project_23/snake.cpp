//libraries 
#include "SFML/Graphics.hpp" //graphics library 
#include "SFML/Window.hpp"  //window rendering library
#include<SFML/System.hpp> //system requirements for sfml
#include<SFML/Audio.hpp>  //audio for when or if we do implement it
#include <ctime> //for food's position
#include <vector> //for body

//namespaces
using namespace sf; //so i wont have to type sf::
using namespace std; //for vectors so i dont have to type std::

//global variables 
bool is_death_playing = false; //is used to prevent the repetitive sfx of death playing 

int direction = 3; //int used to select the direction of movement (is a global variable)
vector <Sprite> Bodies; // vector array of bodies other than the first one
int score = 0; //score 

//bool to start movement and transition from the menu to the gameplay
bool gameplaying = false;

//between each tick there is a possibility of pressing 2 inputs, negating that possibility with this simple bool
bool Direction_changed = false;

//functions
//movement of every body other than the head is done by implementing this very simple function
void tail_movement(Sprite& tail, Vector2f position_of_head, float rotation_of_head)
{

	tail.setRotation(rotation_of_head);    //sets the position of tail to be of the position_of_head, and the rotation as the rotation_of_head
	tail.setPosition(position_of_head);

}

//function that returns true if the chosen position is not occupied by anything else
bool isValid_position(int s_x, int s_y, Sprite& food, FloatRect tail, FloatRect head, FloatRect middle)
{
	//if out of bounds then return false
	if (s_x < 0)return false;

	if (s_y < 0)return false;


	//if the food intersects with any boundaries of the existing sprites, return false 
	if (head.intersects(food.getGlobalBounds())) return false;

	if (tail.intersects(food.getGlobalBounds())) return false;

	if (middle.intersects(food.getGlobalBounds())) return false;


	for (Sprite sprite : Bodies)
	{
		if ((sprite.getGlobalBounds().intersects(food.getGlobalBounds()))) return false;
	}

	return true;
}

//FloatRect stores the bounds of each sprite by drawing invisible rectangles 
void Food_movement(Sprite& food, FloatRect snake_head_position, FloatRect snake_tail_position, FloatRect snake_middle_position, FloatRect food_position)
{
	srand(static_cast<unsigned>(time(nullptr))); //seeding the srand function
	int minRange = 1;
	int maxRange_x = 1800;
	int maxRange_y = 990;

	int size_x = 0;  //
	int size_y = 0;  //rng for location of food

	do
	{
		size_x = minRange + rand() % (maxRange_x); //
		size_y = minRange + rand() % (maxRange_y);//rng for location of food
		food.setPosition(size_x, size_y);

	} while (!isValid_position(size_x, size_y, food, snake_tail_position, snake_head_position, snake_middle_position)); //checks if the position of the food is valid or not


	food.setPosition(size_x, size_y);

}

//respawnning (or spawning first time of the player)
void respawn(Sprite& head, FloatRect snake_head_position, FloatRect snake_tail_position, FloatRect snake_middle_position)
{
	srand(static_cast<unsigned>(time(nullptr))); //seeding the srand function
	int minRange = 200; //in order to prevent spawning on the borders
	int maxRange_x = 1600;
	int maxRange_y = 700;

	int size_x = 0;  //
	int size_y = 0;  //rng for location of head 

	do
	{
		size_x = minRange + rand() % (maxRange_x);
		size_y = minRange + rand() % (maxRange_y);
		head.setPosition(size_x, size_y);

	} while (!isValid_position(size_x, size_y, head, snake_tail_position, snake_head_position, snake_middle_position)); //checks if the position of the head is valid or not

	//making the respawn directions more fair
	//only for x-axis
	if (size_x >= 1920 / 2)
	{
		direction = 1;
	}
	if (size_x <= 1920 / 2)
	{
		direction = 3;
	}

}


//main function OFC
void main()
{

	RenderWindow window(VideoMode(1920, 1080), "Snake Xenia", Style::Fullscreen);	//rendering the window
	window.setFramerateLimit(60); //setting limit to 60 fps 
	Event occurances; //declaring the variable to store events 

	Font font; //vina sans font
	font.loadFromFile("font/VinaSans-Regular.ttf");

	//score text on the top left while playing
	Text score_txt;
	score_txt.setFont(font);
	score_txt.setPosition(20, 0);
	score_txt.setScale(1, 1);// to set position of score display
	score_txt.setString("Score: " + to_string(score));
	score_txt.setFillColor(Color::Black);

	//pixelated font for old games aesthetics hehea boi
	Font font_over;
	font_over.loadFromFile("font/PressStart2P-Regular.ttf");

	//gameover text
	Text over_text;
	over_text.setFont(font_over);
	over_text.setPosition(100, 230);
	over_text.setScale(5.3, 5.3);
	over_text.setString(" Game Over ");
	over_text.setFillColor(Color::Red);

	//menu text 
	Text begin_text;
	begin_text.setFont(font_over);
	begin_text.setPosition(97, 240);
	begin_text.setScale(4.5, 4.5);
	begin_text.setString(" Snake Xenia ");
	begin_text.setFillColor(Color::Red);

	//credits on the menu 
	Text credits;
	credits.setFont(font_over);
	credits.setPosition(400, 450);
	credits.setScale(1.5, 1.5);// to set position of score display
	credits.setString("BY: Huzaifa  and  Mawahid ");
	credits.setFillColor(Color::Black);

	//input information screen at the menu 
	Text menu_input_text;
	menu_input_text.setFont(font);
	menu_input_text.setPosition(1350, 900);
	menu_input_text.setScale(1.5, 1.5);// to set position of score display
	menu_input_text.setString(" Press 'P' to play.\n Press 'Escape' to exit  ");
	menu_input_text.setFillColor(Color::White);

	//score at gameover screen
	Text end_score_text;
	end_score_text.setFont(font);
	end_score_text.setPosition(830, 450);
	end_score_text.setScale(3, 3);// to set position of score display
	end_score_text.setString("Score: " + to_string(score));
	end_score_text.setFillColor(Color::Black);

	//text for input information on gameover
	Text ins_text;
	ins_text.setFont(font);
	ins_text.setPosition(1350, 900);
	ins_text.setScale(1.5, 1.5);
	ins_text.setString("Press 'Enter' to restart\nPress 'Escape' to exit");
	ins_text.setFillColor(Color::White);

	//black background behind input information
	RectangleShape over_back(Vector2f(500, 200));
	over_back.setPosition(1300, 850);
	over_back.setFillColor(Color::Black);


	//loading textures for the sprites
	Texture head;
	head.loadFromFile("sprites/snake_head.png"); //loads from a file 

	Texture tail;
	tail.loadFromFile("sprites/snake_tail.png");

	Texture body;
	body.loadFromFile("sprites/snake_body.png");

	Texture food_1; //we planned to add multiple foods but decided against it as it added unnecessary complexity
	food_1.loadFromFile("sprites/snake_food_1.png");


	//loading background 
	Texture background;
	background.loadFromFile("sprites/background.jpg");


	//sprites of snake
	Sprite snake_sprite_head(head); //loads the texture 
	snake_sprite_head.setScale(.18, .18); //sets scale (size)
	snake_sprite_head.setPosition(470, 340); //sets location
	snake_sprite_head.setOrigin(snake_sprite_head.getTexture()->getSize().x / 2.0, snake_sprite_head.getTexture()->getSize().y / 2.0); //setting the axis of rotation to the sprite's centre

	Sprite snake_sprite_middle(body);
	snake_sprite_middle.scale(Vector2f(.18, .18));
	snake_sprite_middle.setPosition(50, 50);
	snake_sprite_middle.setOrigin(snake_sprite_middle.getTexture()->getSize().x / 2.0, snake_sprite_middle.getTexture()->getSize().y / 2.0);

	Sprite snake_sprite_tail(tail);
	snake_sprite_tail.scale(Vector2f(.18, .18));
	snake_sprite_tail.setPosition(50, 50);
	snake_sprite_tail.setOrigin(snake_sprite_tail.getTexture()->getSize().x / 2.0, snake_sprite_tail.getTexture()->getSize().y / 2.0);

	Sprite background_sprite(background);
	background_sprite.scale(3.5, 3);

	Sprite food_sprite1(food_1); //sprite of food
	food_sprite1.scale(Vector2f(.20, .20));
	food_sprite1.setPosition(Vector2f(5, 200));

	// menu button
	Texture play_button_texture;
	play_button_texture.loadFromFile("sprites/play_button.png");

	Sprite button(play_button_texture);
	button.setPosition(950, 750); //no need to set size as its set by a loop which is called all the time 
	button.setOrigin(button.getTexture()->getSize().x / 2.0, button.getTexture()->getSize().y / 2.0);
	//button.scale(Vector2f(.1, .1));


	Texture restart_button_texture;
	restart_button_texture.loadFromFile("sprites/restart_button.png");
	Sprite restart_button(restart_button_texture);
	restart_button.setPosition(950, 780);
	restart_button.setOrigin(restart_button.getTexture()->getSize().x / 2.0, restart_button.getTexture()->getSize().y / 2.0);
	restart_button.setScale(Vector2f(.5, .5)); //setting position for the start as the loop which controls its size is under (isGameOver)


	//used for tick calculations
	Clock clock; //used for tick system
	float time_elapsed = 0;

	//sfx 
	// 
	//food 
	SoundBuffer eat_buffer;
	eat_buffer.loadFromFile("sfx/consume.wav");

	
	Sound eat_sound;
	eat_sound.setBuffer(eat_buffer);
	eat_sound.setVolume(100);

	//death sfx
	SoundBuffer death_buffer;
	death_buffer.loadFromFile("sfx/over.wav");

	Sound death_sfx;
	death_sfx.setBuffer(death_buffer);
	death_sfx.setVolume(100);

	//button sfx
	SoundBuffer click_buffer;
	click_buffer.loadFromFile("sfx/click1.wav");

	Sound click_sfx;
	click_sfx.setBuffer(click_buffer);
	click_sfx.setVolume(100);

	//bool to detect game overs
	bool IsGameOver = false;

	while (window.isOpen())
	{

		window.display(); //displaying all objects
		window.draw(background_sprite); //drawing the background always ofc


		if ((!gameplaying) && (!IsGameOver)) //displaying stuff at the start when neither the game is over or the game is running
		{

			//window.draw(menu_input_text);
			window.draw(begin_text);
			window.draw(credits);
			window.draw(button);

		}


		if (gameplaying) //checks which only work when the game running bool is true
		{

			window.draw(food_sprite1); //drawing the food (apple)
			window.draw(snake_sprite_head); //drawing the head of the snake
			window.draw(snake_sprite_middle); //drawing the body of the snake
			window.draw(snake_sprite_tail); //drawing the tail of the snake
			window.draw(score_txt); //drawing the score

			score_txt.setString("Score: " + to_string(score)); //updating score 
			end_score_text.setString("Score: " + to_string(score));

			for (int i = 0; i < Bodies.size(); i++)
			{

				window.draw(Bodies[i]); //drawing all the bodies


				if (snake_sprite_head.getGlobalBounds().intersects(Bodies[i].getGlobalBounds()))
				{
					IsGameOver = true; //collision detection with each body (done when the game is running ofc)
				}

			}

		}

		time_elapsed += clock.restart().asSeconds();

		//for movement and the tick system 
		if ((!IsGameOver) && (gameplaying))
		{
			//detection for each tick
			if (time_elapsed >= 0.15f)
			{
				Direction_changed = false; //resetting the bool to allow for input again

				Vector2f last_mid_pos_loc = Vector2f(0, 0);
				float last_mid_rot_loc = 0;

				if (Bodies.size() > 0)
				{

					last_mid_pos_loc = Bodies[Bodies.size() - 1].getPosition();
					last_mid_rot_loc = Bodies[Bodies.size() - 1].getRotation();
					//last positions to be used for the tail
				}

				//for the use of the middle (non vector)
				Vector2f prev_pos = snake_sprite_head.getPosition();
				float prev_rot = snake_sprite_head.getRotation();

				//for the use of the first middle vector
				Vector2f first_mid_pos = snake_sprite_middle.getPosition();
				float first_mid_rot = snake_sprite_middle.getRotation();

				if (direction == 4) snake_sprite_head.move(Vector2f(0, 80)), snake_sprite_head.setRotation(0);//down 
				if (direction == 2) snake_sprite_head.move(Vector2f(0, -80)), snake_sprite_head.setRotation(180); //up
				if (direction == 3) snake_sprite_head.move(Vector2f(80, 0)), snake_sprite_head.setRotation(270);	//right
				if (direction == 1) snake_sprite_head.move(Vector2f(-80, 0)), snake_sprite_head.setRotation(90); //left

				tail_movement(snake_sprite_middle, prev_pos, prev_rot); //setting the position of the middle (non vector) as the position of the head 

				time_elapsed = 0; //for the clock to add time into 

				if (Bodies.size() <= score + 1)
				{
					Bodies.emplace_back(snake_sprite_middle);
					//adding sprites 	
				}

				tail_movement(Bodies[0], first_mid_pos, first_mid_rot); //first vector's position setting

				tail_movement(snake_sprite_tail, last_mid_pos_loc, last_mid_rot_loc); //tails'position setting

				for (int i = Bodies.size() - 1; i > 0; i--)
				{

					//counts the number of values gone through
					Vector2f prev_mid_pos_loc = Bodies[i - 1].getPosition();
					float prev_mid_rot_loc = Bodies[i - 1].getRotation();

					tail_movement(Bodies[i], prev_mid_pos_loc, prev_mid_rot_loc);
					//setting previous position and rotation to the current one after each tick except the first one


				}


			}

		}

		if (IsGameOver)
		{


			if (!is_death_playing)
			{
				death_sfx.play(); //playing death sound effect
				is_death_playing = true;
			}

			window.draw(over_text);
			//window.draw(over_back);
			window.draw(end_score_text);
			//.draw(ins_text);
			window.draw(restart_button);

			bool retry = false; //bool to check if user wants to retry

			while (window.pollEvent(occurances)) //retrying inputs 
			{
				if (occurances.key.code == Keyboard::Enter)
				{
					retry = true;
				}

				if (occurances.key.code == Keyboard::Escape)
				{
					window.close();
				}

				//restart button
				if (restart_button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition())))
				{
					restart_button.setScale(Vector2f(.6, .6));
				}
				else
				{
					restart_button.setScale(Vector2f(.5, .5));
				}

				if (occurances.type == Event::MouseButtonPressed) //check dat only mouse button is pressed because the A on the keyboard and the left mouse button share event ids :skull_emoji:
				{
					if ((occurances.mouseButton.button == Mouse::Left) && (restart_button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition()))) && (!retry))
					{

						click_sfx.play();
						retry = true;

					}
				}

			}

			if (retry)
			{
				snake_sprite_middle.setPosition(Vector2f(5000, 5000)); //putting the middle and the tail out of the field as they can collide with the head in the VERY rare chance that the head spawns exactly 1 space away and collides with them as the head moves first and the tail/body moves in the end of the tick  
				snake_sprite_tail.setPosition(Vector2f(6000, 6000));

				//respawn function
				respawn(snake_sprite_head, snake_sprite_head.getGlobalBounds(), snake_sprite_tail.getGlobalBounds(), snake_sprite_middle.getGlobalBounds());
				//food_movement function (basically respawning but for food)
				Food_movement(food_sprite1, snake_sprite_head.getGlobalBounds(), snake_sprite_tail.getGlobalBounds(), snake_sprite_middle.getGlobalBounds(), food_sprite1.getGlobalBounds());


				window.clear();
				IsGameOver = false;
				Bodies.clear();
				score = 0;
				is_death_playing = false;
				gameplaying = true;
				restart_button.setScale(.5, .5);
			}

		}

		// collision detection with the border of the screen
		if (snake_sprite_head.getGlobalBounds().top <= 0)
		{
			IsGameOver = true;
		}

		if (snake_sprite_head.getGlobalBounds().top + snake_sprite_head.getGlobalBounds().height >= window.getSize().y)
		{
			IsGameOver = true;
		}

		if (snake_sprite_head.getGlobalBounds().left <= 0)
		{
			IsGameOver = true;
		}

		if (snake_sprite_head.getGlobalBounds().left + snake_sprite_head.getGlobalBounds().width >= window.getSize().x)
		{
			IsGameOver = true;
		}


		//collision of the snake with itself
		if ((snake_sprite_head.getGlobalBounds().intersects(snake_sprite_middle.getGlobalBounds())) || (snake_sprite_head.getGlobalBounds().intersects(snake_sprite_tail.getGlobalBounds())))
		{
			IsGameOver = true;
		}


		//with the FOOOD
		if (snake_sprite_head.getGlobalBounds().intersects(food_sprite1.getGlobalBounds()))
		{
			eat_sound.play();
			Food_movement(food_sprite1, snake_sprite_head.getGlobalBounds(), snake_sprite_tail.getGlobalBounds(), snake_sprite_middle.getGlobalBounds(), food_sprite1.getGlobalBounds());
			score++;
		}


		while (window.pollEvent(occurances)) //checks if events are occuring 
		{
			//inputs for movement and leaving game
			if (occurances.type == Event::KeyPressed) //to check if the only input is from the keyboard 
			{
				if (occurances.key.code == Keyboard::Escape)
				{
					window.close();
				}

				if (((occurances.key.code == Keyboard::Right) || (occurances.key.code == Keyboard::D)) && (direction != 1) && !(Direction_changed))
				{

					direction = 3;
					Direction_changed = true;

				}
				else if (((occurances.key.code == Keyboard::Left || (occurances.key.code == Keyboard::A))) && (direction != 3) && !(Direction_changed))
				{

					direction = 1;
					Direction_changed = true;

				}
				else if (((occurances.key.code == Keyboard::Up) || (occurances.key.code == Keyboard::W)) && (direction != 4) && !(Direction_changed))
				{

					direction = 2;
					Direction_changed = true;

				}
				else if (((occurances.key.code == Keyboard::Down) || (occurances.key.code == Keyboard::S)) && (direction != 2) && !(Direction_changed))
				{

					direction = 4;
					Direction_changed = true;

				}

				//if (occurances.key.code == Keyboard::M)
				//{
				//	score++; //test to check if scoring works
				//}

				if ((occurances.key.code == Keyboard::P) && !(gameplaying))
				{

					gameplaying = true; //starting the game from menu
					respawn(snake_sprite_head, snake_sprite_head.getGlobalBounds(), snake_sprite_tail.getGlobalBounds(), snake_sprite_middle.getGlobalBounds());
					Food_movement(food_sprite1, snake_sprite_head.getGlobalBounds(), snake_sprite_tail.getGlobalBounds(), snake_sprite_middle.getGlobalBounds(), food_sprite1.getGlobalBounds());

				}

			}

			//mouse detection with the buttons

			if (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition())))
			{
				button.setScale(Vector2f(0.6, 0.6));
			}
			else
			{
				button.setScale(Vector2f(0.5, 0.5));
			}


			if (occurances.type == Event::MouseButtonPressed) //without this check the inputs were being mixed up
			{

				if ((occurances.mouseButton.button == Mouse::Left) && (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition()))) && (!gameplaying)) //the getposition() function only returns a point and not coordinates so it has to be converted into coordinates first by the mapPixeltoCoords() function 
				{

					click_sfx.play();
					gameplaying = true;

				}
			}
		}
	}
}