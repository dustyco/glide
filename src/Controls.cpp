

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
}

void Controls::clear ()
{
	up.clear();
	down.clear();
	left.clear();
	right.clear();
	
	mouse_select.clear();
	mouse_context.clear();
	
	lanes1.clear();
	lanes2.clear();
	lanes3.clear();
	lanes4.clear();
	
	edit.clear();
	key_select.clear();
}

Controls& Controls::getSingleton ()
{
	// Guaranteed to be destroyed.
	// Instantiated on first use.
	static Controls instance;
	return instance;
}


