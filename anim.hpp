#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <math.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <time.h>

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

	// Lines config
	std::vector<int> line_lengths; // i:Probabilty for length i+1
	int line_types; // 1/n Probability for start of lines block
	float line_spacing, line_width, line_unit;
	int line_max_horz, line_max_vert;
	// Lines
	std::vector<std::vector<int>> lines; // Hardcoded range
	std::vector<int> line_frames; // frames after which line is drawn
	void line_gen();
	void line_render();

	// Math
	sf::Vector2f mid(sf::Vector2f A, sf::Vector2f B, float factor);
public:
	Anim();
	void play();
};
