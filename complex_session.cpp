#include "engine.h"
#include "stream.h"

int main()
{
	Engine engine(std::make_shared<StreamConsole>());
	engine.Start();

	return 0;
}