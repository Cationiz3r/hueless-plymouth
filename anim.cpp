#include "anim.hpp"

Anim::Anim() {
	srand(time(NULL));

	frame = 0;
	frame_last = 300;
	window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Boot animation", sf::Style::Fullscreen);
	window->setFramerateLimit(60);
	window->setMouseCursorVisible(0);
	window->setVerticalSyncEnabled(0);

	tri_size = 150;
	tri_width = 20;
	tri_duration = 100;

	line_spacing = 20;
	line_width = 3;
	line_unit = 5;
	line_max_vert = 400;
	line_max_horz = 1000;
	line_lengths = { 1, 2, 1, 1, 1, 1, 4, 5, 8, 9 };
	line_types = 3;
	line_gen();
}

void Anim::tri_render() {
	sf::Vector2f center = { (float)window->getSize().x/2, (float)window->getSize().y/2 };

	float angle = PI/2,
		factor = 1 - (frame % tri_duration) / (float)tri_duration,
		side = (frame / tri_duration) % 3;
	triangle[0][0].color = sf::Color(0, 0, 0);
	triangle[0][0].position = center + mid(
		sf::Vector2f(tri_size * std::cos(angle+side*TRI),     tri_size * std::sin(angle+side*TRI)),
		sf::Vector2f(tri_size * std::cos(angle+(side+1)*TRI), tri_size * std::sin(angle+(side+1)*TRI)),
		factor
	);
	triangle[0][1].color = sf::Color(0, 0, 0);
	triangle[0][1].position = center + mid(
		sf::Vector2f((tri_size-tri_width) * std::cos(angle+side*TRI),     (tri_size-tri_width) * std::sin(angle+side*TRI)),
		sf::Vector2f((tri_size-tri_width) * std::cos(angle+(side+1)*TRI), (tri_size-tri_width) * std::sin(angle+(side+1)*TRI)),
		factor
	);
	triangle[2][3].color = sf::Color(255, 255, 255);
	triangle[2][3].position = center + mid(
		sf::Vector2f(tri_size * std::cos(angle+(side+2)*TRI), tri_size * std::sin(angle+(side+2)*TRI)),
		sf::Vector2f(tri_size * std::cos(angle+side*TRI),     tri_size * std::sin(angle+side*TRI)),
		factor
	);
	triangle[2][2].color = sf::Color(255, 255, 255);
	triangle[2][2].position = center + mid(
		sf::Vector2f((tri_size-tri_width) * std::cos(angle+(side+2)*TRI), (tri_size-tri_width) * std::sin(angle+(side+2)*TRI)),
		sf::Vector2f((tri_size-tri_width) * std::cos(angle+side*TRI),       (tri_size-tri_width) * std::sin(angle+side*TRI)),
		factor
	);
	for (int i=1; i<3; ++i) {
		int c = 255*(i+factor-1)/2;
		triangle[i][0].color = triangle[(i+2)%3][3].color =
		triangle[i][1].color = triangle[(i+2)%3][2].color = sf::Color(c, c, c);
		triangle[i][0].position = triangle[(i+2)%3][3].position = center + sf::Vector2f(
			tri_size * std::cos(angle + (i+side)*TRI),
			tri_size * std::sin(angle + (i+side)*TRI)
		);
		triangle[i][1].position = triangle[(i+2)%3][2].position = center + sf::Vector2f(
			(tri_size-tri_width) * std::cos(angle + (i+side)*TRI),
			(tri_size-tri_width) * std::sin(angle + (i+side)*TRI)
		);
	}

	for (int i=0; i<3; ++i) window->draw(triangle[i], 4, sf::Quads);
}

void Anim::line_gen() {
	std::vector<int> probs, delays;
	std::partial_sum(line_lengths.begin(), line_lengths.end(), std::back_inserter(probs));

	for (int i=0; i<line_max_vert; ++i) {
		lines.push_back({});
		int max_horz = rand()%(line_max_horz*3/4)+line_max_horz/4;
		if (rand()%line_types && lines.size()>1) lines[lines.size()-1].push_back(0);
		while (true) {
			int nextword_prob = rand()%probs[(int)probs.size()-1];
			auto nextword_itr = std::find_if(probs.begin(), probs.end(), [&nextword_prob](int const &i) {
				return i > nextword_prob;
			});
			lines[lines.size()-1].push_back(std::distance(probs.begin(), nextword_itr)+1);
			if (std::accumulate(lines[lines.size()-1].begin(), lines[lines.size()-1].end(), 0) >= max_horz) break;
		}

		// Delays
		delays.push_back(rand()%5+3); // 1-5 frames
	}
	std::partial_sum(delays.begin(), delays.end(), std::back_inserter(line_frames));

	// DEBUG
	//for (auto l: lines) {
	//	for (auto i: l) std::cout <<i <<" ";
	//	std::cout <<"\n";
	//}
}

void Anim::line_render() {
	sf::Color fill(150, 150, 150);

	sf::RectangleShape r;
	r.setFillColor(fill);
	r.setOutlineThickness(0);

	sf::CircleShape c(line_unit/2);
	c.setFillColor(sf::Color(0, 0, 0));
	c.setOutlineColor(fill);
	c.setOutlineThickness(line_width);

	int frame_cur = frame;
	auto last = std::find_if(line_frames.begin(), line_frames.end(), [&frame_cur](int const &i) {
		return i > frame_cur;
	});
	int last_line=std::distance(line_frames.begin(), last);

	int x, y = 0;
	for (int i=0; i<last_line; ++i) {
		x = 0;
		if (!lines[i][0]) x += line_unit*6; // Indented line to previous main line
		else {
			c.setPosition(x+line_unit/2, y+line_spacing/2-line_unit/4);
			window->draw(c);
			x += line_unit*2;
		}
		for (auto i: lines[i]) {
			r.setPosition(sf::Vector2f(x, y+(line_spacing-line_width)/2));
			r.setSize(sf::Vector2f(i*line_unit, line_width));
			window->draw(r);
			//std::cout <<x <<" " <<x+i*line_unit <<" " <<i <<"\n";
			x += (i+1)*line_unit;
		}
		//exit(0);
		y += line_spacing;
	}
}

void Anim::saveframe() {
	//sf::Vector2u windowSize = window->getSize();
	//sf::Texture texture;
	//texture.create(windowSize.x, windowSize.y);
	//texture.update(window);
	//sf::Image screenshot = texture.copyToImage();
	//std::stringstream frame_n;
	//frame_n << frame;
	//std::string id = std::string(3, '0').append(frame_n);
	//std::cout <<"'" <<id <<"'\n";
	//std::cout <<std::string(12) <<"\n";
	//screenshot.saveToFile("frame"+);
}
void Anim::play() {
	while (window->isOpen()) {
		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) window->close();
			if (e.key.code == sf::Keyboard::Q) window->close();
		}

		window->clear();
		line_render();
		tri_render();
		window->display();

		saveframe();

		frame++;
		//if (frame > frame_last) window->close();
	}
}


sf::Vector2f Anim::mid(sf::Vector2f A, sf::Vector2f B, float factor) {
	return sf::Vector2f(
		A.x*factor + B.x*(1-factor),
		A.y*factor + B.y*(1-factor)
	);
}
