#include "anim.hpp"

Anim::Anim() {
	frame = 0;
	frame_last = 300;
	window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Boot animation", sf::Style::Fullscreen);
	window->setFramerateLimit(60);
	window->setMouseCursorVisible(0);
	window->setVerticalSyncEnabled(0);

	tri_size = 150;
	tri_width = 20;
	tri_duration = 100;
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
