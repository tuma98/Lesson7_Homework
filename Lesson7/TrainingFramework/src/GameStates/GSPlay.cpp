#include <sstream>
#include <iomanip>
#include <thread>

#include "GSPlay.h"

#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "Player.h"
#include "Enermy.h"
#include "Bullet.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

int GSPlay::m_score = 0;
GSPlay::GSPlay()
{
	m_SpawnCooldown = 0.5;
	m_score = 0;
}


GSPlay::~GSPlay()
{

}


void GSPlay::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Fire_Dragon_Stage");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(screenWidth, screenHeight);

	texture = ResourceManagers::GetInstance()->GetTexture("Player");
	m_Player = std::make_shared<Player >(model, shader, texture);
	m_Player->Set2DPosition(screenWidth / 2, screenHeight - 100);
	m_Player->MoveToPossition(Vector2(screenWidth / 2,screenHeight - 100));
	m_Player->SetSize(50, 50);

	//menu button
	texture = ResourceManagers::GetInstance()->GetTexture("button_menu");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2+175, 100);
	button->SetSize(125, 125);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		});
	m_listButton.push_back(button);


	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_scoreText = std::make_shared< Text>(shader, font, "Score: 0", TEXT_COLOR::RED, 1.0);
	m_scoreText->Set2DPosition(Vector2(5, 25));
}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	
}
void GSPlay::HandleMouseEvents(int x, int y)
{
	m_Player->MoveToPossition(Vector2(x, y));
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSPlay::Update(float deltaTime)
{
	if (m_SpawnCooldown > 0)
	{
		m_SpawnCooldown -= deltaTime;
	}
	if (m_SpawnCooldown <= 0)
	{
		CreateRandomEnermy();
		m_SpawnCooldown = 0.3;
	}

	//update player
	if (m_Player->IsAlive())
	{
		m_Player->Update(deltaTime);

		if (m_Player->CanShoot())
			m_Player->Shoot(m_listBullet);

		m_Player->CheckCollider(m_listBullet, m_listEnermy);
	}

	//update enermies
	for (auto enermy : m_listEnermy)
	{
		if (enermy->IsActive())
		{
			enermy->Update(deltaTime);
			if (enermy->CanShoot())
				enermy->Shoot(m_listBullet);
			enermy->CheckCollider(m_listBullet);
		}
	}

	//update bullets
	for (auto bullet : m_listBullet)
	{
		if (bullet->IsActive())
			bullet->Update(deltaTime);
	}

	//update Score
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << m_score;
	std::string score = "SCORE: " + stream.str();
	m_scoreText->setText(score);
	std::stringstream stream2;
	stream2 << std::fixed << std::setprecision(0) << m_Player->GetHeal();
	std::string heal = "HEAL: " + stream2.str();
	m_playerHealText->setText(heal);
}

void GSPlay::Draw()
{
	m_BackGround->Draw();
	for (auto enermy : m_listEnermy)
		if (enermy->IsActive())
			enermy->Draw();
	if (m_Player->IsAlive())
		m_Player->Draw();
	for (auto bullet : m_listBullet)
		if (bullet->IsActive())
			bullet->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_scoreText->Draw();
	m_playerHealText->Draw();
}
void GSPlay::CreateRandomEnermy()
{

	int range = screenWidth - 10 + 1;
	int num = rand() % range + 10;

	Vector2 pos;
	pos.x = num;
	pos.y = 10;

	for (auto enermy : m_listEnermy)
	{
		if (!enermy->IsActive())
		{
			enermy->SetActive(true);
			enermy->Set2DPosition(pos);
			return;
		}

	}
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Enermy_1");

	std::shared_ptr<Enermy> enermy = std::make_shared<Enermy>(model, shader, texture);
	enermy->Set2DPosition(pos);
	enermy->SetSize(40, 40);
	m_listEnermy.push_back(enermy);
}