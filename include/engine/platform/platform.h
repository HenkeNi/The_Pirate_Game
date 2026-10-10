#pragma once
#include "engine/platform/cursor.h"
#include "engine/platform/window.h"
#include "engine/platform/input_handler.h"

// [Consider] - having window process window events (enum class EventType, struct WindowResizedEvent : public Event)

namespace cursed_engine
{
	struct EngineConfig;
	class EventBus;

	template <typename T>
	class Result;

#pragma region Platform

	class Platform
	{
	public:
		Platform() = default;
		virtual ~Platform() = default;

		Platform(const Platform&) = delete;
		Platform(Platform&&) = delete;
		
		Platform& operator=(const Platform&) = delete;
		Platform& operator=(Platform&&) = delete;
		 
		virtual Result<void> init(const EngineConfig& config) = 0;
		virtual void shutdown() = 0;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;
		
		virtual void processEvents() = 0;

		[[nodiscard]] virtual bool exitRequested() const noexcept = 0;
		[[nodiscard]] virtual Window& getWindow() noexcept = 0;

		[[nodiscard]] virtual Cursor& getCursor() noexcept = 0;
		[[nodiscard]] virtual InputHandler& getInputHandler() noexcept = 0;
	};

#pragma endregion

#pragma region SDL_Platform

	class SDLPlatform final : public Platform
	{
	public:
		SDLPlatform(EventBus& eventBus);
		~SDLPlatform();

		Result<void> init(const EngineConfig& config) override;
		void shutdown() override;

		void beginFrame() override;
		void endFrame() override;

		void processEvents() override;

		[[nodiscard]] bool exitRequested() const noexcept override;
		[[nodiscard]] Window& getWindow() noexcept override;

		[[nodiscard]] Cursor& getCursor() noexcept override;
		[[nodiscard]] InputHandler& getInputHandler() noexcept override;

	private:
		void pollEvents();

		SDLWindow m_window;
		SDLCursor m_cursor;

		SDLInputHandler m_inputHandler;
		EventBus& m_eventBus;

		bool m_initialized;
		bool m_shouldExit;
	};

#pragma endregion
}