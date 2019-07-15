#pragma once
#include <iostream>
#include <vector>
#include <thread>

//Include Graphic library
#include <SFML/Graphics.hpp>

#include "GravitySource/GravitySource.h"
#include "Object/Object.h"

#define STABLE_ORBIT_LIMIT 5000
#define RASTER_SIZE 1
#define THREADS 4

class Core
{
	public:

		Core();

		void Run();
		bool IsOpen();

	private:

		void HandleEvents();
		void Update();
		void Render();

		void CalculatePoints(int threadNumber);

		sf::Vector2f NextPixel(sf::Vector2f currentPixel);
		void RasterPixel(sf::Vector2f pixel, int colorkey);

		sf::RenderWindow _mainWindow;

		sf::Image _gravityMap;
		std::vector<sf::Vector2f> _testPoints;	
		std::vector<GravitySource*> _gravitySources;

};