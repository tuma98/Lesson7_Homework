#pragma once
#include "gamestatebase.h"
#include "GameButton.h"


class Player;
class Bullet;
class Enermy;
class Sprite2D;
class Sprite3D;
class Text;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void Init() override;
	void Exit() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents() override;
	void HandleKeyEvents(int key, bool bIsPressed) override;
	void HandleMouseEvents(int x, int y) override;
	void HandleTouchEvents(int x, int y, bool bIsPressed) override;
	void Update(float deltaTime) override;
	void Draw() override;

	static int m_score;


private:

	std::shared_ptr<Sprite2D> m_BackGround;
	std::shared_ptr<Text>  m_scoreText;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	std::shared_ptr<Text>  m_playerHealText;
	std::shared_ptr<Player> m_Player;
	std::vector<std::shared_ptr<Bullet>> m_listBullet;
	std::vector<std::shared_ptr<Enermy>> m_listEnermy;

	float m_SpawnCooldown;
	void CreateRandomEnermy();

};

