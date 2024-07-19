# include <Siv3D.hpp> // Siv3D v0.6.15


class Player {
	private:
		double speed = 200.0;
		double posX = 400;
		bool isFacingRight = true;
	public:
		void update() {
			if (KeyLeft.pressed())
			{
				posX = Max((posX - speed * Scene::DeltaTime()), 60.0);
				isFacingRight = false;
			}
			if (KeyRight.pressed())
			{
				posX = Min((posX + speed * Scene::DeltaTime()), 740.0);
				isFacingRight = true;
			}
		}
		void draw(const Texture& texture) {
			texture.scaled(0.75).mirrored(isFacingRight).drawAt(posX, 540);
		}
		const Circle getCircle() {
			return Circle{ posX, 540, 50 };
		}
};

class Falling_object {
	private:
		double posX = Random(0.0, 800.0);
		double posY = 100;
		double speed = 200.0;
	public:
		void update() {
			posY += speed * Scene::DeltaTime();
		}
		void draw(const Texture& texture) {
			texture.scaled(0.75).drawAt(posX, posY);				
		}
		const Circle getCircle() {
			return Circle{ posX, posY, 50 };
		}

};

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };
	const Font emojiFont{ 48, Typeface::ColorEmoji };
	font.addFallback(emojiFont);
	const Texture dinasour{ U"🦖"_emoji };
	const Texture meat{ U"🍖"_emoji };

	Player player;
	Falling_object falling_object;

	while (System::Update())
	{
		player.update();
		player.draw(dinasour);
		falling_object.update();
		if (!falling_object.getCircle().intersects(player.getCircle())) {
			falling_object.draw(meat);	
		}
	}
}
