#include "Object.h"

void Object::Create(sf::Vector2f position, sf::Vector2f velocity)
{
	_objectSprite.setPosition(position);
	_objectSprite.setFillColor(sf::Color(255, 0, 0, 255));
	_objectSprite.setRadius(5);

	_velocity = velocity;

	_livetime = 0;
}

void Object::SetPosition(sf::Vector2f newPosition)
{
	_objectSprite.setPosition(newPosition);
}

sf::Vector2f Object::GetPosition()
{
	return _objectSprite.getPosition();
}

sf::Vector2f* Object::GetVelocity()
{
	return &_velocity;
}

int Object::GetLiveTime()
{
	return _livetime;
}

void Object::Tick()
{
	_livetime++;
}

void Object::Render(sf::RenderWindow* window)
{
	window->draw(_objectSprite);
}