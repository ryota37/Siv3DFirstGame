# include <Siv3D.hpp> // Siv3D v0.6.15

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	const Texture dinasour{ U"🦖"_emoji };
	const Texture meat{ U"🍖"_emoji };
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };
	const Font emojiFont{ 48, Typeface::ColorEmoji };
	font.addFallback(emojiFont);

	double speed = 200.0;
	double playerPosX = 400;
	bool isPlayerFacingRight = true;
	
	double meatPosX = Random(0.0, 800.0);
	double meatPosY = 100;
	double meatspeed = 200.0;

	while (System::Update())
	{
		meatPosY += meatspeed * Scene::DeltaTime();

		if (KeyLeft.pressed())
		{
			playerPosX = Max((playerPosX - speed * Scene::DeltaTime()), 60.0);
			isPlayerFacingRight = false;
		}

		if (KeyRight.pressed())
		{
			playerPosX = Min((playerPosX + speed * Scene::DeltaTime()), 740.0);
			isPlayerFacingRight = true;
		}

		const Circle dinasour_circle{ playerPosX, 540,50 };
		dinasour_circle.draw(ColorF{ 0.6, 0.8, 0.7 });
		dinasour.scaled(0.75).mirrored(isPlayerFacingRight).drawAt(playerPosX, 540);

		const Circle meat_circle{ meatPosX,meatPosY,50 };

		if (!meat_circle.intersects(dinasour_circle)) {
			meat_circle.draw(ColorF{ 0.6, 0.8, 0.7 });
			meat.scaled(0.75).drawAt(meatPosX, meatPosY);
		}
	}
}
