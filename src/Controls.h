

#include "common.h"


struct Controls
{
	struct Button
	{
		bool  pressed;
		float duration;
		
		     Button ();
		void clear  ();
		void set    (bool pressed, float dt);
	};
	
	Button up, down, left, right;
	Button mouse_select, mouse_context;
	Button lanes1, lanes2, lanes3, lanes4;
	Button edit, key_select;
	
	Vec2f mouse_screen;
	
	     Controls ();
	void clear    ();
	
	static Controls& getSingleton ();
};
