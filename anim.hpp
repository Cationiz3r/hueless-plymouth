#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <math.h>
#include <iostream>
#include <vector>
#include <numeric>

#define PI 3.141592
#define TRI PI*2/3

class Anim {
	int frame, frame_last;
	sf::RenderWindow *window;
	void saveframe();

	// Triangle config
	float tri_size, tri_width;
	int tri_duration;
	// Middle Triangle
	sf::Vertex triangle[3][4];
	void tri_render();

	// Math
	sf::Vector2f mid(sf::Vector2f A, sf::Vector2f B, float factor);
public:
	Anim();
	void play();
};
