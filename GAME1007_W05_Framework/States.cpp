#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include <iostream>
using namespace std;

void State ::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState(){}

void TitleState::Enter()
{
	cout << "Entering TitleState!" << endl;
	m_Music.emplace("music", Mix_LoadMUS("../Assets/aud/Music.mp3"));
	
	//load music track,add it to map
	Mix_PlayMusic(m_Music["music"],-1);
	//and play it
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
	{
		cout << "Changing to GameState!" << endl;
		STMA::ChangeState(new GameState());
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 200, 0, 200, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	//Any unique rendering in TitleState goes here....

	State::Render();//This invokes SDL_RenderPresent
}

void TitleState::exit()
{
	cout << "Exiting TitleState!" << endl;
	Mix_FreeMusic(m_Music["music"]);
	//make sure to invoke Mix_freemusic
}

GameState::GameState(){}

void GameState::Enter()
{
	cout << "Entering GameState!" << endl;
	m_sfx.emplace("Slacker", Mix_LoadWAV("../Assets/aud/Blow.wav"));
	m_sfx.emplace("jump", Mix_LoadWAV("../Assets/aud/Yay.ogg"));
	//load sound effects track, add the to map(x2)
	m_Music.emplace("music", Mix_LoadMUS("../Assets/aud/Music.mp3"));
	//load music track,add it to map
	Mix_PlayMusic(m_Music["music"], -1);
	//and play it
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		cout << "Changing to PauseState!" << endl;
		//pause the music track
		Mix_PauseMusic();
		STMA::ChangeState(new PauseState());
	}
	//parse "x" scan code and change to new endstate
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		cout << "Changing to EndState!" << endl;
		
		STMA::ChangeState(new EndState());
	}
	//parse '1' key and play first sfx
	if(Engine::Instance().KeyDown(SDL_SCANCODE_1))
	{
		Mix_PlayChannel(-1, m_sfx["Slacker"], 0);
	}
	//parse '2' key and play second sfx
	if (Engine::Instance().KeyDown(SDL_SCANCODE_2))
	{
		Mix_PlayChannel(-1, m_sfx["jump"], 0);
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 70, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	//Any unique rendering in GameState goes here....

	 //if the current state in the vector is the gamestate.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
	
}

void GameState::exit()
{
	cout << "Exiting GameState!" << endl;
	//make sure to invoke Mix_freeMusic
	Mix_FreeChunk(m_sfx["jump"]);
	Mix_FreeChunk(m_sfx["slacker"]);
	Mix_FreeMusic(m_Music["music"]);
	//make sure to invoke Mix_freeMusic. (x2)
}

void GameState::Resume()
{

	cout << "Resuming GameState!" << endl;
	// resume playing music track
	Mix_ResumeMusic();
}



PauseState::PauseState(){}

void PauseState::Enter()
{
	cout << "Entering PauseState!" << endl;
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		cout << "Changing to GameState!" << endl;
		STMA::ChangeState(new GameState());
	}
}

void PauseState::Render()
{
	
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 50, 255, 100, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	//Any unique rendering in TitleState goes here....

	
	State::Render();
}

void PauseState::exit()
{
	cout << "Exiting PauseState!" << endl;
}




EndState::EndState(){}

void EndState::Enter()
{
	cout << "Entering EndState!" << endl;
}

void EndState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		cout << "Changing to TitleState!" << endl;
		STMA::ChangeState(new TitleState());
	}
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 100, 255, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	//Any unique rendering in TitleState goes here....

	
	State::Render();
}

void EndState::exit()
{
	cout << "Exiting EndState!" << endl;
}

// ina subclass to invoke the render above, we just do:
// state :: render();