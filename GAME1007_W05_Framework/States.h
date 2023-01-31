#pragma once
#ifndef __STATES_H__
#define __STATES_H__
#include <map>
#include <SDL_mixer.h>
#include <string>

class State //Abstract base class
{// no object of state can ever be created.
public:
	virtual void Enter()=0;//pure virtual method.
	virtual void Update()=0;
	virtual void Render();// make a definition for render().
	virtual void exit() = 0;
	virtual void Resume() {};
protected://private but inherited.
	State() = default;
};
class TitleState: public State
{
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void exit();
private:
	//map for music track goes here
	std::map<std::string, Mix_Music*> m_Music;
};

//MAke defenition for pause state
class GameState : public State
{
public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void exit();
	virtual void Resume();
private:
	std::map<std::string, Mix_Chunk*> m_sfx;
	std::map<std::string, Mix_Music*> m_Music;
	
	//map for music track goes here
	// map for sound efects goes here

};
class PauseState : public State
{
public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void exit();
private:
	
};
class EndState : public State
{
public:
	EndState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void exit();
private:
	
};
#endif

