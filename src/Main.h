

#include "common.h"
#include "Layout.h"
#include "Image.h"
#include "sfmlframework.h"

struct Main : SFMLApp
{
	sf::Font font;
	sf::Text text;
	Layout   layout;
	bool     window_focused;
	
	// Inherited from SFMLApp
	virtual bool setup       ();
	virtual bool loop        (int w, int h, double t);
	virtual bool cleanup     ();
	
	void handleInput ();
	void draw        (vector<ImagePtr>& images);
};


