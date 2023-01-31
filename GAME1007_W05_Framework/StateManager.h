#pragma once
#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__
#include "States.h"
#include <vector>
class StateManager//FINITE state machine (FSM)
{// "static" class not a Singleton.
public:
	static void Update();
	static void Render();
	//specific statemanager method:
	static void pushState(State* pState);
	static void PopState();
	static void ChangeState(State* pState);
	static void Quit();

	//Accessor for the states.
	static std::vector<State*>& GetStates();

private:
	StateManager() = default;
	static std::vector<State*> s_states;
	// Note: the back of the state vector is the current state.
};

typedef StateManager STMA;
#endif

