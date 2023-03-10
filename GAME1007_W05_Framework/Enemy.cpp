#include "Enemy.h"
#include "Engine.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "TextureManager.h"

Enemy::Enemy(const SDL_Rect src, const SDL_FRect dst):Sprite(src, dst)
{ 

}

void Enemy::Update()
{
	m_dst.y += s_speed * (float)Engine::Instance().GetDeltaTime();
	// don't really need to check any other boundaries because
	// enemies only travel down
	if (m_dst.y > kWidth)
	{
		m_deleteMe = true;
	}
}

void Enemy::Render()
{
	SDL_RenderCopyExF(REMA::Instance().GetRenderer(), TEMA::GetTexture("enemy"), &m_src, &m_dst, 180.0, NULL, SDL_FLIP_NONE);
}

const float Enemy::s_speed = 50.0f;