#include <iostream>

#include "Core/Core.h"

int main()
{
	Core core;
	while (core.IsOpen())
	{
		core.Run();
	}

	return 0;
}