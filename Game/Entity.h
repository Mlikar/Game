#pragma once

#include "Animation.h"


const int W = 812;    // Rozm�ry okna, kter� jsem dal konstantn� kv�li pozad�
const int H = 541;

float prevod = 0.017453f;  // Pozd�ji se pou�ije k p�evod�m na radi�ny

class Entity  // Classa pro na�e objekty st�ely a nep��tele
{
public:
	float x, y, dx, dy, R, angle;
	bool life;
	string name;
	Animation anim;

	Entity()
	{
		life = 1;
	}

	void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1)
	{
		x = X; y = Y; anim = a;
		angle = Angle; R = radius;
	}

	virtual void update() {};

	void draw(RenderWindow& app)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);
	}
};

class Murloc : public Entity  // Nep��tel
{
public:
	Murloc()
	{
		dx = rand() % 8 - 4;
		dy = rand() % 8 - 4;
		name = "murloc";
	}

	void update() // pohyb murloca
	{
		x += dx;
		y += dy;
		if (x > W) x = 0; if (x < 0) x = W;
		if (y > H) y = 0; if (y < 0) y = H;
	}
};

class fire : public Entity // St�ela
{
public:
	fire()
	{
		name = "fireball";
	}

	void update() // pohyb st�ely
	{
		dx = cos(angle * prevod) * 6;
		dy = sin(angle * prevod) * 6;
		x += dx;
		y += dy;
		if (x > W || x<0 || y>H || y < 0)
		{
			life = 0;
		}
	}
};

bool Collison(Entity* a, Entity* b)  // kolize objetk�
{
	return	(b->x - a->x) * (b->x - a->x) +
		(b->y - a->y) * (b->y - a->y) <
		(b->R - a->R) * (b->R - a->R);
}


