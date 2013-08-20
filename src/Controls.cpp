

#include "Controls.h"


Controls::Button::Button ()
{
	clear();
}

void Controls::Button::clear ()
{
	pressed = false;
	duration = 0;
}

void Controls::Button::set (bool pressed, float dt)
{
	if (pressed)
	{
		this->pressed = true;
		this->duration += dt;
	}
	else clear();
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


