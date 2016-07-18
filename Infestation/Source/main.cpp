#include <PYRO/Application.h>

#include "States/GameState.h"

int main()
{
	pyro::Application app(sf::VideoMode(1280, 720), "Infestation", sf::Style::Close);
	app.registerState<GameState>(pyro::StateID::Game);

	app.pushState(pyro::StateID::Game);
	app.run();

	return 0;
}