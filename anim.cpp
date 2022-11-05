#include "anim.hpp"

Anim::Anim() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	frame = 0;
	window = new sf::RenderTexture();
	window->create(400, 400, settings);

	tri_size = 150;
	tri_width = 30;
	tri_duration = 300; //Loop
	tri_duration_end = 30;
	trail_endlength = 5;

	frame_last = tri_duration + tri_duration_end;
}

void Anim::tri_render() {
	sf::Vector2f center = { (float)window->getSize().x/2, (float)window->getSize().y/2 };

	float angle = PI/2;
	float offset = 1 - frame % (tri_duration/3) / (tri_duration/3.0);
	float offset_tail = offset;
	int step = (frame / (tri_duration/3)) % 3; // Rotated everything by 120 deg
	float trail_length = 2;
	int n = 3;

	// End of loop, finish animation
	if (frame > tri_duration) {
		angle += (1 - std::cos((frame-tri_duration)/(float)tri_duration_end*PI))/2*TRI;
		trail_length += (trail_endlength - trail_length)*std::pow((frame-tri_duration)/(float)tri_duration_end, 2);
		offset_tail = trail_length - offset;
		--n;
		while (offset_tail > 1) {
			offset_tail -= 1;
			n += 1;
		}
		offset_tail = 1 - offset_tail;
	}

	//int n = std::floor(trail_length) + 1; // Number of segments
	std::vector<sf::Vertex[4]> trail(n); // Each segment is a quad

	// Begin is darkest, End is brightest
	// Begin: 0    End:  3  <-- outer side
	//         1        2   <-- inner side
	trail[0  ][0].color = trail[0  ][1].color = sf::Color(0, 0, 0);
	trail[n-1][3].color = trail[n-1][2].color = sf::Color(255, 255, 255);

	// Begin & End has position between corners based on offset
	float tri_size_inner = tri_size - tri_width; // Avoid recalculation
	trail[0][0].position = center + sf::Vector2f(
		mid(tri_size       * std::cos(angle + TRI*(step)),     tri_size       * std::cos(angle + TRI*(step+1)), offset),
		mid(tri_size       * std::sin(angle + TRI*(step)),     tri_size       * std::sin(angle + TRI*(step+1)), offset));
	trail[0][1].position = center + sf::Vector2f(
		mid(tri_size_inner * std::cos(angle + TRI*(step)),     tri_size_inner * std::cos(angle + TRI*(step+1)), offset),
		mid(tri_size_inner * std::sin(angle + TRI*(step)),     tri_size_inner * std::sin(angle + TRI*(step+1)), offset));
	trail[n-1][3].position = center + sf::Vector2f(
		mid(tri_size       * std::cos(angle + TRI*(step+n-1)), tri_size       * std::cos(angle + TRI*(step+n)), offset_tail),
		mid(tri_size       * std::sin(angle + TRI*(step+n-1)), tri_size       * std::sin(angle + TRI*(step+n)), offset_tail));
	trail[n-1][2].position = center + sf::Vector2f(
		mid(tri_size_inner * std::cos(angle + TRI*(step+n-1)), tri_size_inner * std::cos(angle + TRI*(step+n)), offset_tail),
		mid(tri_size_inner * std::sin(angle + TRI*(step+n-1)), tri_size_inner * std::sin(angle + TRI*(step+n)), offset_tail));

	for (int i=1; i<n; ++i) {
		int c = 255*(i+offset-1)/trail_length;
		// Making everything full bright when end goes over start
		//   then delay that frame for a while (post-this)
		if (trail_length > 3) c += (trail_length-3)/(trail_endlength-3)*(255-c);

		// Last one's end color is this one's begin color (gradient)
		trail[i][0].color = trail[(i+n-1)%n][3].color =
		trail[i][1].color = trail[(i+n-1)%n][2].color = sf::Color(c, c, c);

		// Same for position
		trail[i][0].position = trail[(i+n-1)%n][3].position = center + sf::Vector2f(
			tri_size       * std::cos(angle + TRI*(i+step)),
			tri_size       * std::sin(angle + TRI*(i+step)));
		trail[i][1].position = trail[(i+n-1)%n][2].position = center + sf::Vector2f(
			tri_size_inner * std::cos(angle + TRI*(i+step)),
			tri_size_inner * std::sin(angle + TRI*(i+step)));
	}

	//std::cout <<frame <<": " <<n <<" " <<trail_length <<" " <<offset <<" " <<offset_tail <<"\n";
	for (auto t: trail) window->draw(t, 4, sf::Quads);
}

void Anim::saveframe() {
	std::string id, type = "throbber-";
	int n = frame;
	if (frame >= tri_duration) {
		type = "animation-";
		n -= tri_duration;
	}
	for (int i=0; i<3; ++i) {
		id = (char)(n%10+'0') + id;
		n /= 10;
	}

	window->getTexture().copyToImage().saveToFile(type + id + ".png");
}
void Anim::play() {
	for (frame = 0; frame < frame_last; ++frame) {
		window->clear();
		tri_render();
		window->display();

		saveframe();
	}
}

float Anim::mid(float A, float B, float factor) {
	return A*factor + B*(1-factor);
}
