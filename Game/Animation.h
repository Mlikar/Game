#pragma once
#include <SFML/Graphics.hpp> // Použití døíve zmínìné knihovny

using namespace sf;
using namespace std;

class Animation   // Classa pro animace
{
public:
	float frame, speed;
	Sprite sprite;
	vector<IntRect> frames;

	Animation() {};

	Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
	{
		frame = 0;
		speed = Speed;

		for (int i = 0; i < count; i++)
			frames.push_back(IntRect(x + i * w, y, w, h));

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}
};

