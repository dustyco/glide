

#include "Controls.h"


Controls::Button::Button ()
{
	clear();
}

void Controls::Button::clear ()
{
	presses = 0;
	is_pressed = false;
	duration = 0;
}

void Controls::Button::set (bool is_pressed, float dt)
{
	if (is_pressed)
	{
		this->is_pressed = true;
		this->duration += dt;
	}
	else clear();
}

int Controls::Button::getRepeats (float speed)
{
	int temp = presses;
	presses = 0;
	return temp;
	
	// TODO
	if (is_pressed) return 1;
	else            return 0;
}

Controls::Controls ()
{
	clear();
	mouse_screen = Vec2f(0.5, 0.5);
}

void Controls::clear ()
{
	for (auto& pair : buttons) pair.second.clear();
}

Controls& Controls::getSingleton ()
{
	// Guaranteed to be destroyed.
	// Instantiated on first use.
	static Controls instance;
	return instance;
}


