
#include <iostream>
#include <cmath>
using namespace std;

#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>


// A simple realtime OpenGL render window template for quick projects
// Usage: inherit, override, and call go()

/*
Copy/paste sample:

#include "sfmlframework.h"
class App : public SFMLApp {
	// Override these
	bool setup ();
	bool loop (int w, int h, double t);
	bool cleanup ();
};
bool App::setup () {
	return true;
}
bool App::loop (int w, int h, double t) {
	return true;
}
bool App::cleanup () {
	return true;
}


-lsfml-system -lsfml-window -lGL
*/

struct SFMLApp {
	sf::RenderWindow  window;
	sf::View          view;
	sf::VideoMode     video_mode;
	sf::Event         event;
	sf::Clock         clock;
	string            window_title;
	bool              running;
	int               mouse_x, mouse_y;
	
	// Call this to kick things off
	int go (string window_title = "SFMLApp");
	
	// Override these
	virtual bool setup ();
	virtual bool loop (int w, int h, double t);
	virtual bool cleanup ();
	
	// NON-USER MEMBERS ///////////////////////////////////////////////////////
	bool _setup ();
	bool _input ();
	bool _loop ();
	bool _cleanup ();
};




// DEFAULT FUNCTIONS (OVERRIDE THEM IN YOUR SUBCLASS) /////////////////////////
bool SFMLApp::setup () {return true;}
bool SFMLApp::loop (int w, int h, double t)
{
	float x = (float)w/2.0 + cos(t)*(float)w/3.0;
	float y = (float)h/2.0 + sin(t)*(float)h/3.0;
	
	// Clear the buffer
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set the viewport
	glViewport		(	0,0,w,h		);
	glMatrixMode	(	GL_PROJECTION	);
	glLoadIdentity	();
	glOrtho			(	0,w,0,h,-1,1	);
	glMatrixMode	(	GL_MODELVIEW	);
	glLoadIdentity	();
	glDisable		(	GL_DEPTH_TEST	);
	
	// Draw a rectangle
	int BOXR = 20;
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glVertex2f( x-BOXR,y-BOXR );
		glVertex2f( x+BOXR,y-BOXR );
		glVertex2f( x+BOXR,y+BOXR );
		glVertex2f( x-BOXR,y+BOXR );
	glEnd();
	
	glColor3f(.2,.2,.2);
	glBegin(GL_LINES);
		glVertex2f(0,h/2);
		glVertex2f(w,h/2);
		
		glVertex2f(w/2,0);
		glVertex2f(w/2,h);
	glEnd();
	
	return true;
}
bool SFMLApp::cleanup () {return true;}








// NON-USER IMPLEMENTATION ////////////////////////////////////////////////////
//SFMLApp::SFMLApp () {}
//SFMLApp::~SFMLApp () {}

int SFMLApp::go (string window_title) {
	this->window_title = window_title;
	if (_setup()) {
		_loop();
		_cleanup();
		return 0;
	} else return -1;
}
bool SFMLApp::_setup () {
	// Pre-user setup
	video_mode = sf::VideoMode::getDesktopMode();
	if (!video_mode.isValid()) {
		cerr << "Invalid video mode?" << endl;
		return false;
	}
//	window.create( video_mode, window_title, sf::Style::Fullscreen | sf::Style::Close );
	window.create( video_mode, window_title, sf::Style::Resize | sf::Style::Close );
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(true);
	
#ifdef SFML_SYSTEM_WINDOWS
	ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
#endif
	
	// User setup
	if (!setup()) return false;
	
	// Post-user setup
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	clock.restart();
	
	return true;
}
bool SFMLApp::_input () {
	return true;
}
bool SFMLApp::_loop () {
	running = true;
	while (running) {
		// Pre-user loop
		window.clear();
		view.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
		view.setViewport(sf::FloatRect(0.0, 0.0, 1.0, 1.0));
		window.setView(view);
		double time = (double)(clock.getElapsedTime().asMicroseconds())*1e-6;
		_input();
	
		// User loop
		if (!loop(window.getSize().x, window.getSize().y, time)) return false;
	
		// Post-user loop
		window.display();
	}
	return true;
}
bool SFMLApp::_cleanup () {
	// Pre-user cleanup
	
	// User cleanup
	if (!cleanup()) return false;
	
	// Post-user cleanup
	
	return true;
}





