#ifndef __BULLET_H__
#define __BULLET_H__
#include <SDL.h>
#include "GameObject.h"
#include "Turret.h"

class Bullet : public GameObject
{
private:
	double m_dx, m_dy;
	static const float s_speed;

public:
	Bullet(const SDL_FRect, const double, const double, Turret*);
	void Update();
	void Render();
	bool m_deleteMe = false; // Flag that will let the program know, we can delete it.
	Turret* m_parent;
};

#endif