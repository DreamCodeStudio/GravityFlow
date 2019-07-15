#pragma once
#include <iostream>

//Include Graphic library
#include <SFML/Graphics.hpp>

#include "..\Object\Object.h"

#define GRAVITY_MASS_FACTOR 1000.0f

class GravitySource
{
	public:

		void Create(sf::Vector2f position);

		void CalculateVelocity(Object *o);
		void Render(sf::RenderWindow *window);
		void Update(sf::RenderWindow *window);

		float GetDistance(Object* o);

	private:

		float GetLength(sf::Vector2f v);
		sf::Vector2f Normalize(sf::Vector2f v);

		sf::CircleShape _gravitySprite;
		bool _clicked;

};