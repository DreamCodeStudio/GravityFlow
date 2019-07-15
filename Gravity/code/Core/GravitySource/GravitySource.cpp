#include "GravitySource.h"

void GravitySource::Create(sf::Vector2f position)
{
	_gravitySprite.setRadius(10);
	_gravitySprite.setPosition(position);
	_gravitySprite.setFillColor(sf::Color(100, 100, 100));

	_clicked = false;
}

void GravitySource::CalculateVelocity(Object *o)
{
	sf::Vector2f distance = _gravitySprite.getPosition() - o->GetPosition();
	sf::Vector2f gravityForce = sf::Vector2f(Normalize(distance).x * (GRAVITY_MASS_FACTOR / pow(GetLength(distance), 2)),
											  Normalize(distance).y * (GRAVITY_MASS_FACTOR / pow(GetLength(distance), 2)));

	//Add gravitational fource to the velocity
	*o->GetVelocity() += gravityForce;
}

void GravitySource::Render(sf::RenderWindow *window)
{
	window->draw(_gravitySprite);
}

void GravitySource::Update(sf::RenderWindow* window)
{
	if (sf::Mouse::getPosition(*window).x > _gravitySprite.getPosition().x - _gravitySprite.getRadius() * 2 &&
		sf::Mouse::getPosition(*window).x < _gravitySprite.getPosition().x + _gravitySprite.getRadius() * 2 &&
		sf::Mouse::getPosition(*window).y > _gravitySprite.getPosition().y - _gravitySprite.getRadius() * 2 &&
		sf::Mouse::getPosition(*window).y < _gravitySprite.getPosition().y + _gravitySprite.getRadius() * 2 &&
		sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		_clicked = true;
	}
	
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		_clicked = false;
	}

	if (_clicked)
	{
		_gravitySprite.setPosition(sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(*window).x),
											    static_cast<float>(sf::Mouse::getPosition(*window).y)));
	}
}

float GravitySource::GetDistance(Object* o)
{
	return GetLength(_gravitySprite.getPosition() - o->GetPosition());
}

/* ################# Private ################## */

float GravitySource::GetLength(sf::Vector2f v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

sf::Vector2f GravitySource::Normalize(sf::Vector2f v)
{
	float length = GetLength(v);

	v.x = v.x / length;
	v.y = v.y / length;

	return v;
}