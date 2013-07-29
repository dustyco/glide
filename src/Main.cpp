

#include "Main.h"
#include "Camera.h"
#include "Controls.h"
#include <X11/Xlib.h>
#include <SFML/Graphics/Texture.hpp>


bool Main::setup ()
{
	Camera& camera = Camera::getSingleton();

/*	if (font.loadFromFile("/usr/share/fonts/TTF/DejaVuSansMono-Bold.ttf"))
	{
		text.setFont(font);
		text.setCharacterSize(24);
	}
*/	
	// Init camera dimensions BEFORE the first Layout::tick()
	camera.lock();
	camera.dim = Vec2i(window.getSize().x, window.getSize().y);
	camera.unlock();
	
	// Add images from this folder
	vector<Image*> images;
	Image::createImages(images, ".");
	layout.add(images);
	
	window_focused = true;
	
	return true;
}

bool Main::loop (int w, int h, double t)
{
	Camera& camera = Camera::getSingleton();
	
	// Update camera dimensions
	camera.lock();
	camera.dim = Vec2i(w, h);
	camera.unlock();
	
	// Input
	handleInput();
	
	// Simulate
	// TODO Time
	layout.tick(1.0/60.0);
	
	// Update viewport
	camera.lock();
	sf::View view;
	view.reset(sf::FloatRect(
		camera.current.p.x - camera.current.rx,
		camera.current.p.y - camera.current.ry,
		camera.current.rx*2,
		camera.current.ry*2
	));
	camera.unlock();
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	window.setView(view);
	
	// Draw visible images
	vector<Image*> images;
	layout.getVisible(images);
	draw(images);
	
	// TODO
/*	// Image name
	text.setString(current->name);
	text.setPosition(-screen_aspect/2, 0.5);
	text.setScale(1.0/h, -1.0/h);
	if (current->failed) text.setColor(sf::Color(200, 0, 0));
	else text.setColor(sf::Color(200, 200, 200));
	window.draw(text);
*/	
	return true;
}

bool Main::cleanup ()
{
	// TODO
/*	for (ImagePtr it=all.begin(); it!=all.end(); ++it)
	{
		if (it->loaded)
		{
			delete (sf::Texture*)(it->data);
			it->data = NULL;
		}
		it->saveMods();
		it->cleanMods();
	}
*/	return true;
}

void Main::handleInput ()
{
	Controls& c = Controls::getSingleton();
	float dt = 1.0/60.0;
	
	/////////////////////////////////////////////////////////////////
	// Event based controls
	/////////////////////////////////////////////////////////////////
	
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:             running = false; return;
			case sf::Event::LostFocus:          c.clear(); window_focused = false; break;
			case sf::Event::GainedFocus:        window_focused = true; break;
//			case sf::Event::Resized:            break;
//			case sf::Event::MouseMoved:         event.mouseMove.x; break;
			case sf::Event::MouseButtonPressed:
				switch (event.mouseButton.button)
				{
//					case sf::Mouse::Left:       finalizeObject(); break;
//					case sf::Mouse::Right:      placePoint(); break;
				}
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:  running = false; return;
				}
				break;
		}
	}
	
	/////////////////////////////////////////////////////////////////
	// State based controls
	/////////////////////////////////////////////////////////////////
	
	// These will grab the state even if the window isn't focused so we have to check
	if (!window_focused) return;
	
	c.up.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W), dt);
	c.down.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S), dt);
	c.left.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A), dt);
	c.right.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D), dt);
	
	c.mouse_select.set(sf::Mouse::isButtonPressed(sf::Mouse::Left), dt);
	c.mouse_context.set(sf::Mouse::isButtonPressed(sf::Mouse::Right), dt);
	
	c.lanes1.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1), dt);
	c.lanes2.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2), dt);
	c.lanes3.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3), dt);
	c.lanes4.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4), dt);
	
	c.edit.set(sf::Keyboard::isKeyPressed(sf::Keyboard::E), dt);
	c.key_select.set(sf::Keyboard::isKeyPressed(sf::Keyboard::Return), dt);
	
//	sf::Vector2i sf_mouse_pos = sf::Mouse::getPosition(window);
//	mouse = viewTransformInverse(Vec(sf_mouse_pos.x, sf_mouse_pos.y));
}

void Main::draw (vector<Image*>& images)
{
//	cout << endl << "Rendering " << images.size() << " images" << endl;
	typedef vector<Image*> ImageVec;
	for (ImageVec::iterator it=images.begin(); it!=images.end(); ++it)
	{
		Image& image = *(*it);
		image.lock();
		
		/////////////////////////////////////////////////////////////////
		// Create/resize texture if necessary and new pixels aren't loading
		/////////////////////////////////////////////////////////////////
		
		if (!image.loading)
		{
			// If there is a texture, pixels are available, but they're different sizes
			if (image.texture != NULL && image.loaded == true && ((sf::Texture*)image.texture)->getSize().x != image.dim_loaded.x)
			{
				// Delete the texture
				delete (sf::Texture*)image.texture;
				image.texture = NULL;
			}
			
			// If there's no texture and pixels are available
			if (image.texture == NULL && image.loaded == true)
			{
				// Try to create a texture
				image.texture = (void*)new sf::Texture;
				((sf::Texture*)image.texture)->setSmooth(true);
				if (!((sf::Texture*)image.texture)->create(image.dim_loaded.x, image.dim_loaded.y))
				{
					// Failed to create texture
					cout << "Main::draw(): Failed to create texture";
					delete (sf::Texture*)image.texture;
					image.texture = NULL;
					image.unload();
					image.failed = true;
				}
				else ((sf::Texture*)image.texture)->update(image.pixels);
			}
		}
		
		/////////////////////////////////////////////////////////////////
		// Draw it if the texture is ready
		/////////////////////////////////////////////////////////////////
		
		if (image.texture != NULL)
		{
			// Draw it
			sf::Texture& texture = *(sf::Texture*)image.texture;
			sf::Sprite sprite(texture);
			
			sprite.setOrigin(float(texture.getSize().x)/2, float(texture.getSize().y)/2);
			sprite.setScale(image.current.rx*2/float(texture.getSize().x), image.current.ry*2/float(texture.getSize().y));
			sprite.setPosition(image.current.p.x, image.current.p.y);
			
			window.draw(sprite);
			
		}
		
		image.unlock();
	}
}

int main (int argc, char const** argv)
{
	XInitThreads();
	Main app;
	try
	{
		return app.go("glide");
	}
	catch (string s)
	{
		cout << s << endl;
	}
	catch (...)
	{
		cout << "main(): Caught an exception of unkown type or origin" << endl;
	}
	return 1;
}


