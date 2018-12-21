#include <SFML/Graphics.hpp>
#include <ctime>
#include <vector>
#include <string>

bool isCollide(sf::Sprite &s1, sf::Sprite &s2) {
	return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
};

int main()
{

	//set fonts
	sf::Font gameFont;
	gameFont.loadFromFile("img/Font/kenvector_future.ttf");
	sf::Text gameOver("Game Over", gameFont);
	gameOver.setCharacterSize(50);
	gameOver.setPosition(250, 250);
	gameOver.setFillColor(sf::Color::Green);
	sf::Text scoreText("Score", gameFont);
	scoreText.setPosition(1, 1);
	scoreText.setFillColor(sf::Color::Cyan);
	scoreText.setCharacterSize(18);
	scoreText.setOutlineThickness(2.f);
	scoreText.setOutlineColor(sf::Color::Black);
	int score = 0;
	sf::Text scoreValText(std::to_string(score), gameFont);
	scoreValText.setCharacterSize(18);
	scoreValText.setPosition(10, 25);
	scoreValText.setFillColor(sf::Color::Cyan);
	scoreValText.setOutlineColor(sf::Color::Black);
	scoreValText.setOutlineThickness(2.f);
	bool isGameOver = false;

	srand(time(NULL));
	sf::Vector2f windowVec(800.f, 600.f);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ark Tutorial");
	window.setFramerateLimit(60);

	//make textures for sprites and background
	sf::Texture texBG, texBlueBlock, texRedBlock, texGreenBlock, texBall, texPaddle;
	texBG.loadFromFile("img/Background-4.png");
	texBlueBlock.loadFromFile("img/blue_button01.png");
	texRedBlock.loadFromFile("img/element_red_rectangle_glossy.png");
	texGreenBlock.loadFromFile("green_Button01.png");
	texBall.loadFromFile("ballBlue.png");
	texPaddle.loadFromFile("paddleBlu.png");
	float ballWidth = 22.f;
	float ballHeight = 22.f;

	//create sprites and add textures to sprites
	sf::Sprite sBackground(texBG),
		sBall(texBall),
		sBlueBlock(texBlueBlock),
		sRedBlock(texRedBlock),
		sGreenBlock(texGreenBlock),
		sPaddle(texPaddle);

	//Set padde and ball position
	sPaddle.setPosition(windowVec.x / 2 - 52, windowVec.y - 24);
	sBall.setPosition(windowVec.x / 2 - 11, windowVec.y - 24 - 22);

	//create array to hold the sprite blocks
	sf::Sprite blockArr[500];

	//create the sprites for the blocks and set the positions in a loop
	int loopCount = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			blockArr[loopCount].setTexture(texRedBlock);
			//block img is 64 x 32
			blockArr[loopCount].setPosition(i * 64.f + 80.f, j * 32.f + 10.f);
			loopCount++;
		}
	}

	//move vector
	sf::Vector2f moveVec(6.f, 5.f);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				if (event.key.code == sf::Keyboard::Space) {
					moveVec.x = 6.f;
					moveVec.y = 5.f;
					sBall.setPosition(sPaddle.getPosition().x + 10.f, sPaddle.getPosition().y - ballHeight);
					isGameOver = false;
					score = 0;
					//reset blocks
					loopCount = 0;
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							blockArr[loopCount].setPosition(i * 64.f + 80.f, j * 32.f + 10.f);
							loopCount++;

						}
					}
				}
			}
		}

			//bounce off walls
			if (sBall.getPosition().x < 0 || sBall.getPosition().x > 800 - 22) {
				moveVec.x *= -1.f;
			}
			if (sBall.getPosition().y < 0 || sBall.getPosition().y > 600 - 22) {
				moveVec.y *= -1.f;
			}
			//set the ball in motion x
			sBall.move(moveVec.x, 0);
			for (int i = 0; i < loopCount; i++) {
				//remove hit blocks
				if (isCollide(sBall, blockArr[i])) {
					blockArr[i].setPosition(-100, 0);
					moveVec.x *= -1; //reverse x movement
					score++;
				}
			}

			//set ball in motion y
			sBall.move(0, moveVec.y);
			for (int i = 0; i < loopCount; i++) {
				//remove hit blocks
				if (isCollide(sBall, blockArr[i])) {
					blockArr[i].setPosition(-100, 0);
					moveVec.y *= -1; //reverse y position
					score++;
				}
			}

			//right/left vectors
			sf::Vector2f leftVec(-8.f, 0);
			sf::Vector2f rightVec(8.f, 0);

			//move paddle left
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				sPaddle.move(leftVec);
			}
			//move paddle right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				sPaddle.move(rightVec);
			}

			//check paddle collision
			if (isCollide(sPaddle, sBall)) {
				moveVec.y = -(rand() % 5 + 2);
			}


			//check for miss
			if (sBall.getPosition().y > 600 - ballHeight) {
				isGameOver = true;
				moveVec.x = 0;
				moveVec.y = 0;
			}
			//clear window
			window.clear(sf::Color::White);

			//draw the stuffs
			window.draw(sBackground);
			window.draw(sPaddle);
			//draw all the blocks in the array
			for (int i = 0; i < loopCount; i++) {
				window.draw(blockArr[i]);
			}
			window.draw(sBall);
			window.draw(scoreText);
			scoreValText.setString(std::to_string(score));
			window.draw(scoreValText);

			if (isGameOver) {
				window.draw(gameOver);
			}
			//display what has been rendered, update window
			window.display();

	}
}