#include "Core.h"

Core::Core()
{
	_mainWindow.create(sf::VideoMode(1920, 1080), "Gravity");
	_mainWindow.setFramerateLimit(60);

	//Create gravity image
	_gravityMap.create(1920, 1080);

	_gravitySources.push_back(new GravitySource);
	_gravitySources[_gravitySources.size() - 1]->Create(sf::Vector2f(955, 540));

	//Create test points
	_testPoints.push_back(sf::Vector2f(2 * RASTER_SIZE, 2 * RASTER_SIZE));
	while (NextPixel(_testPoints[_testPoints.size() - 1]).x != -1)
	{
		_testPoints.push_back(NextPixel(_testPoints[_testPoints.size() - 1]));
	}
}

void Core::Run()
{
	this->HandleEvents();
	this->Update();
	this->Render();
}

bool Core::IsOpen()
{
	return _mainWindow.isOpen();
}

/* ############### Private ################ */

void Core::HandleEvents()
{
	sf::Event event;
	while (_mainWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_mainWindow.close();
			return;
		}
	}
}

void Core::Update()
{
	//If the user pressed the enter key -> create a gravitational source
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		_gravitySources.push_back(new GravitySource);
		_gravitySources[_gravitySources.size() - 1]->Create(sf::Vector2f(960, 540));
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter));
	}

	//Update all gravitational sources so they can move if the user clicked them
	for (unsigned int i = 0; i < _gravitySources.size(); i++)
	{
		_gravitySources[i]->Update(&_mainWindow);
	}

	//If the user released the mouse button
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		//Start rendering threads
		for (unsigned int i = 0; i < THREADS; i++)
		{
			std::thread Thread(&Core::CalculatePoints, this, i, false);
			Thread.detach();
		}

		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
	}

	//If the S key was pressed save a picture of the gravity map
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_gravityMap.saveToFile("GravityMap.png");
	}

	//If the user pressed the right mouse button start the gravity simulation of an object on this location
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		_testObject.Create(sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(_mainWindow).x), 
										static_cast<float>(sf::Mouse::getPosition(_mainWindow).y)), sf::Vector2f(0, 0));

		std::thread Thread(&Core::CalculatePoints, this, 1, true);
		Thread.detach();

		while (sf::Mouse::isButtonPressed(sf::Mouse::Right));
	}
}

void Core::Render()
{
	_mainWindow.clear();

	sf::Texture t;
	t.loadFromImage(_gravityMap);
	sf::Sprite s;
	s.setTexture(t);
	_mainWindow.draw(s);

	for (unsigned int i = 0; i < _gravitySources.size(); i++)
	{
		_gravitySources[i]->Render(&_mainWindow);
	}

	_testObject.Render(&_mainWindow);

	_mainWindow.display();
}


void Core::CalculatePoints(int threadNumber, bool useTestObject)
{
	//Get the thread Number as starting index 
	int pointIndex = threadNumber;

	//Create an object to test the gravity
	Object *o = new Object;

	//Framelimit 
	sf::Clock clock;

	//Go through all testing points 
	while (pointIndex < _testPoints.size())
	{
		//If the user started a simulation with the right mouse button and wants to see the object -> use the test object
		if (useTestObject)
		{
			delete o;
			o = &_testObject;
		}
		else
		{
			//Set our object to the current test point
			o->Create(_testPoints[pointIndex], sf::Vector2f(0, 0));
		}

		//We abort the simulation if the object stayed more than 5000 frames in the gravitational field
		while (o->GetLiveTime() < 5000)
		{
			clock.restart();

			//Let the object get influenced by all gravitational sources
			for (unsigned int i = 0; i < _gravitySources.size(); i++)
			{
				_gravitySources[i]->CalculateVelocity(o);
			}

			o->SetPosition(o->GetPosition() + *o->GetVelocity());

			//Check if the object is to close to a gravitational source
			for (unsigned int i = 0; i < _gravitySources.size(); i++)
			{
				//If one object got to close to a gravitational source -> abort
				if (_gravitySources[i]->GetDistance(o) < 20)
				{
					//Calculate the color and maybe raster a square
					int colorkey = static_cast<int>((o->GetLiveTime() / 5000.0f) * 765);

					this->RasterPixel(_testPoints[pointIndex], colorkey);
					goto end;
				}
			}

			//Add 1 frame to the lifetime
			o->Tick();

			//If the object flew out of the screen -> abort
			if (o->GetPosition().x < 0 || o->GetPosition().x > 1920 || o->GetPosition().y < 0 || o->GetPosition().y > 1080)
			{
				//Calculate the color and maybe raster a square
				int colorkey = static_cast<int>((o->GetLiveTime() / 5000.0f) * 765);

				this->RasterPixel(_testPoints[pointIndex], colorkey);
				break;
			}

			//If we use the test object the user spawned with right mouse button, we need to limit the fps
			if (useTestObject)
			{
				while (clock.getElapsedTime().asMilliseconds() < sf::milliseconds(8).asMilliseconds());
			}
		}
		end:

		//If we used the test object we can exit here
		if (useTestObject)
		{
			return;
		}

		//If the object stayed more than 5000 frames set the pixel color to white
		if (o->GetLiveTime() >= 5000)
		{
			this->RasterPixel(_testPoints[pointIndex], 765);
		}

		pointIndex += THREADS;
	}

	if (!useTestObject)
	{
		delete o;
	}

}

sf::Vector2f Core::NextPixel(sf::Vector2f currentPixel)
{
	currentPixel.x += 2 * RASTER_SIZE;
	if (currentPixel.x + RASTER_SIZE > 1920)
	{
		currentPixel.x = 2 * RASTER_SIZE;
		currentPixel.y += 2 * RASTER_SIZE;

		if (currentPixel.y + RASTER_SIZE > 1080)
		{
			return sf::Vector2f(-1, -1);
		}
	}

	return currentPixel;
}

void Core::RasterPixel(sf::Vector2f pixel, int colorkey)
{
	for (register int y = static_cast<int>(pixel.y - RASTER_SIZE); y < pixel.y + RASTER_SIZE; y++)
	{
		for (register int x = static_cast<int>(pixel.x - RASTER_SIZE); x < pixel.x + RASTER_SIZE; x++)
		{
			_gravityMap.setPixel(x, y, sf::Color(colorkey > 255 ? (colorkey > 510 ? colorkey - 510 : 0) : 0,
												 colorkey > 255 ? (colorkey > 510 ? 255 : colorkey - 255) : 0,
												 colorkey > 255 ? 255 : colorkey));
		}
	}
}