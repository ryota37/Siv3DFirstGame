#include <Siv3D.hpp> // Siv3D v0.6.15

class Player {
private:
	double speed;
	double posX;
	bool isFacingRight = true;
public:
	Player(double initialSpeed, double initialPosX)
		: speed(initialSpeed), posX(initialPosX) {}
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

class FallingObject {
private:
	double posX;
	double posY;
	double speed;
public:
	FallingObject(double initialSpeed, double initialPosX, double initialPosY)
		: speed(initialSpeed), posX(initialPosX), posY(initialPosY) {
	}
	void update() {
		posY += speed * Scene::DeltaTime();
	}
	void draw(const Texture& texture) {
		texture.scaled(0.75).drawAt(posX, posY);
	}
	void vanish(const Texture& texture) {
		texture.resized(1).drawAt(0, 0);
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

	Player player{ 200.0, 400.0 };
	FallingObject fallingMeat{ 200.0, Random(0.0, 800.0), 100.0 };

	bool isGameStarted = false;
	bool isSuccess = false;

	while (System::Update())
	{

		if (!isGameStarted) {
			// ゲームのタイトルやロゴ画像みたいなのをここで表示したい
			font(U"Press Enter to start!!").drawAt(400, 300);

			// "Licenses" ボタンが押されたら
			if (SimpleGUI::Button(U"Licenses", Vec2{ 20, 20 }))
			{
				// ライセンス情報を表示
				LicenseManager::ShowInBrowser();
			}
		}

		if (KeyEnter.down()) {
			isGameStarted = true;
			fallingMeat = FallingObject{ 200.0, Random(0.0, 800.0), 100.0 };
		}

		if (isGameStarted && !isSuccess) {
			player.update();
			player.draw(dinasour);

			fallingMeat.update();

			if (!fallingMeat.getCircle().intersects(player.getCircle())) {
				fallingMeat.draw(meat);
			}
			if (fallingMeat.getCircle().intersects(player.getCircle())) {
				isSuccess = true;
			}
			if (!isSuccess && fallingMeat.getCircle().y > 600) {
				font(U"Failure!!").drawAt(400, 300, Palette::Black);
			}
		}

		if (isGameStarted && isSuccess) {
			font(U"Success!!").drawAt(400, 300, Palette::Black);
			fallingMeat.vanish(meat);
			player.update();
			player.draw(dinasour);

			if (KeyEnter.down()) {
				isSuccess = false;
			}
		}
	}
}
