#include "game/Game.h"
#include "game/scenes/title_scene.h"
#include <engine/scenes/scene_stack.h>
#include <iostream>


Game::Game()
	: m_sceneStack{ nullptr }
{
}

void Game::onUpdate(float deltaTime)
{
}

void Game::onCreated(const EngineSystems& systems)
{
	m_sceneStack = &systems.sceneStack;
	setupScenes();
}

void Game::setupScenes()
{
	m_sceneStack->push(std::make_unique<TitleScene>());
}