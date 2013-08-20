

#include "common.h"
#include <map>


struct Controls
{
	struct Button
	{
		bool  pressed;
		float duration;
		
		     Button    ();
		void clear     ();
		void set       (bool pressed, float dt);
//		int  getRepeat (float speed = 1.0f); TODO
	};
	
	map<string,Button> buttons;
	Vec2f              mouse_screen;
	
	     Controls ();
	void clear    ();
	
	static Controls& getSingleton ();
};
