#include "game/game.h"
#include "game/scenes/title_scene.h"
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
	m_sceneStack.addPath("TitleScene", context.assetRoot.string() + "scenes/title_scene.json"); // Force user to specify path?
	//m_sceneStack.registerScene<TitleScene>("TitleScene", context.assetRoot.string() + "scenes/title_scene.json"); // Force user to specify path?
	m_sceneStack.push(std::make_unique<TitleScene>(&context.systemManager)); // NOTE; (maybe problem) but every scene will need to accept systemmanager!

	//setupScenes();
}

void Game::onDestroyed()
{
	m_sceneStack.clear();
}

void Game::setupScenes()
{
}