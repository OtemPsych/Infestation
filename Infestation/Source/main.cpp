#include <PYRO/Application.h>

#include "States/GameState.h"

int main()
{
	srand(static_cast<unsigned>(time(0)));

	pyro::Application app(sf::VideoMode(1280, 720), "Infestation", sf::Style::Close);
	app.registerState<GameState>(pyro::StateID::Game);

	app.pushState(pyro::StateID::Game);
	app.run();

	return 0;
}