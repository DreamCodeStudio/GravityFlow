#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>

class Object
{
	public:

		void Create(sf::Vector2f position, sf::Vector2f velocity);

		void SetPosition(sf::Vector2f newPosition);
		sf::Vector2f GetPosition();
		sf::Vector2f* GetVelocity();
		int GetLiveTime();
		void Tick();


		void Render(sf::RenderWindow* window);

	private:

		sf::CircleShape _objectSprite;
		sf::Vector2f _velocity;

		int _livetime;
};