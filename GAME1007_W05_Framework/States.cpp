#include <iostream>
#include "States.h"
#include "StateManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "TextureManager.h"
using namespace std;

void State::Render()
{
	SDL_RenderPresent(REMA::Instance().GetRenderer());
}

TitleState::TitleState(){}

void TitleState::Enter()
{
	
}

void TitleState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_N))
	{
		STMA::ChangeState( new GameState() );
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(REMA::Instance().GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(REMA::Instance().GetRenderer());
	State::Render();
}

void TitleState::Exit()
{
	
}

void GameState::ClearTurrets()
{
	for (unsigned i = 0; i < m_turrets.size(); i++)
	{
		delete m_turrets[i];
		m_turrets[i] = nullptr;
	}
	m_turrets.clear();
	m_turrets.shrink_to_fit();
	// You can assimilate some parts of this code for deleting bullets and enemies.
}

GameState::GameState():m_spawnCtr(1) {}

void GameState::Enter()
{
	TEMA::Load("../Assets/img/Turret.png", "turret");
	TEMA::Load("../Assets/img/Enemies.png", "enemy");
	s_enemies.push_back(new Enemy({ 80,0,40,57 }, { 512.0f, -57.0f, 40.0f, 57.0f }));
	// Create the DOM and load the XML file.

	m_xmlDoc.LoadFile("../Assets/dat/turrets.xml");
	tinyxml2::XMLElement* pRoot = m_xmlDoc.FirstChildElement();

	if (pRoot == nullptr)
	{
		return;
	}
	tinyxml2::XMLElement* pTurretElement = pRoot->FirstChildElement();
	while (pTurretElement != nullptr)
	{
		if (strcmp(pTurretElement->Value(), "Turret") == 0)
		{
			float x = pTurretElement->FloatAttribute("xpos");
			float y = pTurretElement->FloatAttribute("ypos");
			int kills = pTurretElement->IntAttribute("kills");
			auto turret = new Turret({0,0,100,100}, {x, y, 100.0f, 100.0f});
			turret->m_kills = kills;
			m_turrets.push_back(turret);
		}
		pTurretElement = pTurretElement->NextSiblingElement();
	}
}

void GameState::Update()
{
	// Parse T and C events.
	if (EVMA::KeyPressed(SDL_SCANCODE_T))
	{
		m_turrets.push_back(new Turret({0,0,100,100},
			{50.0f, 615.0f, 100.0f,100.0f}));
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_C))
	{
		ClearTurrets();
	}
	// Update all GameObjects individually. Spawn enemies. Update turrets. Update enemies. Update bullets.
	if (m_spawnCtr++ % 180 == 0)
	{
		s_enemies.push_back(new Enemy({ 80,0,40,57 },
			{ (float)(rand() % (1024 - 40)), -57.0f, 40.0f, 57.0f }));
	}
	for (auto turret : m_turrets)
		turret->Update();
	for (auto enemy : s_enemies)
		enemy->Update();
	for (auto bullet : s_bullets)
		bullet->Update();

	// Check for collisions with bullets and enemies.
	for (auto bullet : s_bullets)
	{
		for (auto enemy : s_enemies)
		{
			if (COMA::AABBCheck(*bullet->GetDst(), *enemy->GetDst())) {
				enemy->m_deleteMe = true;
				bullet->m_deleteMe = true;
				bullet->m_parent->m_kills++;
				break;
			}
		}
	}
	
	// Cleanup bullets and enemies that go off screen.
	for (int i = 0; i < static_cast<int>(s_bullets.size()); ++i)
	{
		if (s_bullets[i]->m_deleteMe)
		{
			s_bullets.erase(s_bullets.begin() + i);
		}
	}

	for (int i = 0; i < static_cast<int>(s_enemies.size()); ++i)
	{
		if (s_enemies[i]->m_deleteMe)
		{
			s_enemies.erase(s_enemies.begin() + i);
		}
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(REMA::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(REMA::Instance().GetRenderer());

	for (unsigned i = 0; i < m_turrets.size(); i++)
		m_turrets[i]->Render();
	for (unsigned i = 0; i < s_enemies.size(); i++)
		s_enemies[i]->Render();
	for (unsigned i = 0; i < s_bullets.size(); i++)
		s_bullets[i]->Render();

	SDL_Rect spawnBox = { 50, 618, 100, 100 };
	SDL_SetRenderDrawColor(REMA::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(REMA::Instance().GetRenderer(), &spawnBox);

	// This code below prevents SDL_RenderPresent from running twice in one frame.
	if ( dynamic_cast<GameState*>( STMA::GetStates().back() ) ) // If current state is GameState.
		State::Render();
}

void GameState::Exit()
{
	// You can clear all children of the root node by calling .DeleteChildren(); and this will essentially clear the DOM.
	m_xmlDoc.DeleteChildren();
	tinyxml2::XMLNode* pRoot = m_xmlDoc.NewElement("Root");
	m_xmlDoc.InsertEndChild(pRoot);
	tinyxml2::XMLElement* pTurretElement;
	// Iterate through all the turrets and save their positions as child elements of the root node in the DOM.
	for (auto turret : m_turrets)
	{
		pTurretElement = m_xmlDoc.NewElement("Turret");
		pTurretElement->SetAttribute("xpos", turret->GetDst()->x);
		pTurretElement->SetAttribute("ypos", turret->GetDst()->y);
		pTurretElement->SetAttribute("kills", turret->m_kills);
		pRoot->InsertEndChild(pTurretElement);
	}
	// Make sure to save to the XML file.
	m_xmlDoc.SaveFile("../Assets/dat/turrets.xml");
	
	ClearTurrets(); // Deallocate all turrets, then all other objects.
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		delete s_enemies[i];
		s_enemies[i] = nullptr;
	}
	s_enemies.clear();
	s_enemies.shrink_to_fit();
	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		delete s_bullets[i];
		s_bullets[i] = nullptr;
	}
	s_bullets.clear();
	s_bullets.shrink_to_fit();
}

void GameState::Resume()
{
	
}

// This is how static properties are allocated.
std::vector<Bullet*> GameState::s_bullets;
std::vector<Enemy*> GameState::s_enemies;