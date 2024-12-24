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
	void draw(const Texture& texture, const Texture& xmasTexture, bool isXmas) {
		const Texture& currentTexture = isXmas ? xmasTexture : texture;
		currentTexture.scaled(0.75).mirrored(isFacingRight).drawAt(posX, 540);
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
	void draw(const Texture& texture, const Texture& xmasTexture, bool isXmas) {
		const Texture& currentTexture = isXmas ? xmasTexture : texture;
		currentTexture.scaled(0.75).drawAt(posX, posY);
	}
	void vanish(const Texture& texture, const Texture& xmasTexture, bool isXmas) {
		const Texture& currentTexture = isXmas ? xmasTexture : texture;
		currentTexture.resized(1).drawAt(0, 0);
	}
	const Circle getCircle() {
		return Circle{ posX, posY, 50 };
	}
};

void Main()
{
	const Size windowSize{ 800, 600 };
	Window::Resize(windowSize);
	const Texture background{ Resource(U"example/xmas.jpg") };
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };
	const Font minifont{ FontMethod::MSDF, 24, Typeface::Bold };
	const Font emojiFont{ 48, Typeface::ColorEmoji };
	font.addFallback(emojiFont);

	Texture dinasour{ U"🦖"_emoji };
	Texture meat{ U"🍖"_emoji };

	Texture Santa{ U"🎅"_emoji };
	Texture Present{ U"🎁"_emoji };

	Player player{ 200.0, 400.0 };
	FallingObject fallingMeat{ 200.0, Random(0.0, 800.0), 100.0 };

	bool isXmas = false;
	bool isGameStarted = false;
	bool isSuccess = false;

	while (System::Update())
	{
		if (isXmas) {
			background.resized(windowSize).draw();
		}
		if (!isGameStarted) {
			// ゲームのタイトルやロゴ画像みたいなのをここで表示したい

			if (isXmas) {
				font(U"🎁 Present catcher 🎅").drawAt(400, 250);
			}
			else {
				font(U"🍖 Meat catcher 🦖").drawAt(400, 250);
			}
			minifont(U"Press Enter to start!!").drawAt(400, 400);

			// "Licenses" ボタンが押されたら
			if (SimpleGUI::Button(U"Licenses", Vec2{ 20, 20 }))
			{
				// ライセンス情報を追加
				LicenseManager::AddLicense({
					.title = U"Background image of Xmas mode",
					.copyright = U"フリー素材ぱくたそ",
					.text = U"https://www.pakutaso.com" });
				// ライセンス情報を表示
				LicenseManager::ShowInBrowser();
			}

			if (KeyX.down())
			{
				isXmas = !isXmas;
			}
		}

		if (KeyEnter.down()) {
			isGameStarted = true;
			fallingMeat = FallingObject{ 200.0, Random(0.0, 800.0), 100.0 };
		}

		if (isGameStarted && !isSuccess) {
			player.update();
			player.draw(dinasour, Santa, isXmas);

			fallingMeat.update();

			if (!fallingMeat.getCircle().intersects(player.getCircle())) {
				fallingMeat.draw(meat, Present, isXmas);
			}
			if (fallingMeat.getCircle().intersects(player.getCircle())) {
				isSuccess = true;
			}
			if (!isSuccess && fallingMeat.getCircle().y > 600) {
				font(U"Failure!!").drawAt(400, 300);
			}
		}

		if (isGameStarted && isSuccess) {

			if (isXmas) {
				font(U"MerryChristmas!!").drawAt(400, 300);
			}
			else {
				font(U"Success!!").drawAt(400, 300);
			}

			fallingMeat.vanish(meat, Present, isXmas);
			player.update();
			player.draw(dinasour, Santa, isXmas);

			if (KeyEnter.down()) {
				isSuccess = false;
			}
		}
		if (isGameStarted) {
			if (KeySpace.down()) {
				isGameStarted = false;
				isSuccess = false;
			}
		}
	}
}
