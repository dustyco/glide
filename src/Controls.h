

#include "common.h"
#include <map>


struct Controls
{
	struct Button
	{
		int   presses;
		bool  is_pressed;
		float duration;
		
		     Button     ();
		void clear      ();
		void set        (bool is_pressed, float dt);
		int  getRepeats (float speed = 1.0f);
	};
	
	map<string,Button> buttons;
	Vec2f              mouse_screen;
	
	     Controls ();
	void clear    ();
	
	static Controls& getSingleton ();
};
