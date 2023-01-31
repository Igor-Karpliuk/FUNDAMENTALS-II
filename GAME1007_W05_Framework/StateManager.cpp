#include "StateManager.h"

void StateManager::Update()
{
	if (!s_states.empty())// if state vector is not empty.
		s_states.back()->Update();//Invoke Update of the "current" state .
}

void StateManager::Render()
{
	if (!s_states.empty())
		s_states.back()->Render();
}

//used where there needs to be 2 states in the vector, eg game and pause
void StateManager::pushState(State* pState)
{
	s_states.push_back(pState);
	s_states.back()->Enter();//Enter is used for initialization
}
// used where there needs to be two states in the vector
void StateManager::PopState()
{
	if (s_states.size() <= 1)return;//exit this if there are less than two states in the vector.
	s_states.back()->exit();
	delete s_states.back();
	s_states.back() = nullptr;
	s_states.pop_back();

	s_states.back()->Resume();
}

void StateManager::ChangeState(State* pState)
{
	if(!s_states.empty())//we're changing state with a vector with one state.
	{
		s_states.back()->exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();
	}
	s_states.push_back(pState);
	s_states.back()->Enter();
}

void StateManager::Quit()
{
	while (!s_states.empty())
	{
		s_states.back()->exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();
	}
}

std::vector<State*>& StateManager::GetStates()
{
	return s_states;
}

std::vector<State*> StateManager::s_states;// NOW this static object will be allocated;
