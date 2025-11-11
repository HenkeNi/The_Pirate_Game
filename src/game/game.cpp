#include "game/game.h"
#include "game/scenes/title_scene.h"
//#include <game/scenes/scene_stack.h>
#include <iostream>


Game::Game()
{
}

void Game::onUpdate(float deltaTime)
{
	m_sceneStack.update(deltaTime);
}

void Game::onCreated(const AppContext& context)
{
	setupScenes();
}

void Game::onDestroyed()
{
	m_sceneStack.clear();
}

void Game::setupScenes()
{
	//m_sceneStack->push(std::make_unique<TitleScene>());
}