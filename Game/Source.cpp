
// Projekt hry v jazyce C++ pomocí SFML knihovny
// Za projekt jsem si vybral hru zvanou Asteriods, je dìlaná na jejím principu,
// avšak grafika je dìlaná ve stylu hry World of Warcraft
// 


#include <iostream>
#include <SFML/Graphics.hpp> // Použití døíve zmínìné knihovny
#include <list>


using namespace sf;
using namespace std;

const int W = 812;    // Rozmìry okna, které jsem dal konstantní kvùli pozadí
const int H = 541;

float prevod = 0.017453f;  // Pozdìji se použije k pøevodùm na radiány

class Animation   // Classa pro animace
{
public:
	float frame, speed;
	Sprite sprite;
	vector<IntRect> frames;

	Animation() {};

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
		frame = 0;
		speed = Speed;

		for (int i = 0; i < count; i++)
			frames.push_back(IntRect(x+i*w,y,w,h));

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}
};

class Entity  // Classa pro naše objekty støely a nepøítele
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

class Murloc : public Entity  // Nepøítel
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

class fire : public Entity // Støela
{
public:
	fire()
	{
		name = "fireball";
	}

	void update() // pohyb støely
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

bool Collison(Entity* a, Entity* b)  // kolize objetkù
{
	return	(b->x - a->x) * (b->x - a->x)+
			(b->y - a->y) * (b->y - a->y)<
			(b->R - a->R) * (b->R - a->R);
}

void closeWindow(RenderWindow& window, int t) // funkce pro zavøení okna startu a konce po èasovém intervalu
{
	Clock clock;
	Time time = clock.getElapsedTime();
	while (time.asSeconds() < t)
	{
		time = clock.getElapsedTime();
	}
	window.close();
}

int main()
{
	RenderWindow app(VideoMode(W, H), "Murlocs are everywhere"); // tvoøí okno s hrou
	app.setFramerateLimit(60);
	int pocet = 0;
	int start = 0;
	
	// Naše objekty
	Texture t1, t2, t3,t4, t5, t6;
	t1.loadFromFile("D:/Hra/Project/Arathi.jpg");
	t2.loadFromFile("D:/Hra/Project/Mage.png");
	t3.loadFromFile("D:/Hra/Project/Murloc.png");   
	t4.loadFromFile("D:/Hra/Project/Fireball.png");
	t5.loadFromFile("D:/Hra/Project/VarianE.jpg");	
	t6.loadFromFile("D:/Hra/Project/VarianS.jpg");

	Sprite End(t5);    // Sprite nám spojuje window a texturu (obrázek)
	Sprite Start(t6);  // Pokud je tøeba obrázek menší než window tak se nám obrázek vykreslí uprostøed a zbytek bude èerný

	Sprite Background(t1), Player(t2);
	Player.setOrigin(20, 20);  // Urèuje, kde je støed obrázku player

	Animation fireball(t4, 0, 0, 32, 64, 16, 0.8);

	Animation murloc(t3, 0, 0, 64, 64, 16, 0.2);
	murloc.sprite.setPosition(400, 400);

	list<Entity*>entities;

	for (int i = 0; i < 15; i++) // poèet nepøátel ve høe
	{
		Murloc *a = new Murloc();
		a->settings(murloc, rand() % W, rand() % H, rand() % 360, 25);
		entities.push_back(a);
		pocet++;
	}
	

	float x = 300, y = 300;
	float dx = 0, dy = 0, angle = 0;
	bool thrust;
	
	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				app.close();
			}

			if (event.type == Event::KeyPressed)  // funkce pro støelbu pomocí mezerníku
				if (event.key.code == Keyboard::Space)
				{
					fire* b = new fire();
					b->settings(fireball, x, y, angle, 10);
					entities.push_back(b);
				}
		}

		//Movement // pohy pomocí šipek nelze dozadu
		if (Keyboard::isKeyPressed(Keyboard::Right)) angle += 3;
		if (Keyboard::isKeyPressed(Keyboard::Left)) angle -= 3;
		if (Keyboard::isKeyPressed(Keyboard::Up)) thrust = true; else thrust = false;

		if (thrust)
		{
			dx += cos(angle * prevod) * 0.2;
			dy += sin(angle * prevod) * 0.2;
		}

		else
		{
			dx *= 0.99;
			dy *= 0.99;
		}

		int maxSpedd = 5;
		float speed = sqrt(dx * dx + dy * dy);
		if (speed > maxSpedd)
		{
			dx *= maxSpedd / speed;
			dy *= maxSpedd / speed;
		}
		
		x += dx;
		y += dy;

		if (x > W) x = 0; if (x < 0) x = W;
		if (y > H) y = 0; if (y < 0) y = H;

		Player.setPosition(x, y);
		Player.setRotation(angle + 90);

		for (auto i = entities.begin(); i != entities.end();)  // pohyb pro fireball a murloc
		{
			Entity *e = *i;
			e->update();
			//e->anim.update();

			if (e->life == false)
			{
				i = entities.erase(i);
				delete e;
			}
			else i++;
		}

		for (auto a : entities)  // pøi kolizi murloca a fireballu zmizí oba objekty
		{
			for (auto b : entities)
			{
				if (a->name == "murloc" && b->name == "fireball")
				{
					if (Collison(a, b))
					{
						a->life = false;
						b->life = false;
						pocet--;	

						if (pocet == 0)   // po znièení všech enemy se zobrazí window s koncem hry a po 5 sekundách se vypne window s konce a následnì i samotná hra
						{
							RenderWindow konec(VideoMode(1494, 1076), "Konec hry");  
							while (konec.isOpen())
							{
								konec.clear();
								konec.draw(End);
								konec.display();
								closeWindow(konec,5);
								closeWindow(app, 1);

							}
						}
					}
				}
			}
		}
		
		
		
		if(start == 0)  // Zobrazuje screen na zaèátku hry, který se po 5 sekundách vypne
		{
			RenderWindow start(VideoMode(1494, 1076), "Zacatekc hry");
			start.clear();
			start.draw(Start);
			start.display();
			closeWindow(start, 5);
		}
		start = 1;


		//Draw   // Vykreslení hry s hráèem
		app.clear();
		app.draw(Background);
		app.draw(Player);
		for (auto i : entities) i->draw(app);
		app.display();
		
	}
	return 0;
}