#pragma once
#include "engine/resources/resource_manager.hpp"
#include <functional>

struct SDL_Texture;

namespace cursed_engine
{
	// ==================== Texture ==================== //
	
	// Consider using NonCopyable instead?
	class Texture
	{
	public:
		struct Tag{}; // Remove?

		Texture(SDL_Texture* texture = nullptr); 
		~Texture();

		Texture(const Texture&) = delete;
		Texture(Texture&& other) noexcept;

		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) noexcept;

		[[nodiscard]] inline const SDL_Texture* getTexture() const noexcept { return m_texture; }
		[[nodiscard]] inline SDL_Texture* getTexture() noexcept { return m_texture; }

		[[nodiscard]] inline float getWidth() const noexcept { return m_width; }
		[[nodiscard]] inline float getHeight() const noexcept { return m_height; }

		bool isLoaded() const;

	private:
		void release();
		
		SDL_Texture* m_texture;
		float m_width;
		float m_height;
	};

	// ==================== Texture Key ==================== //
	struct TextureKey // Nest inside texture?? Texture::Key?
	{
		std::string path;

		bool operator==(const TextureKey& other) const noexcept
		{
			return path == other.path;
		}
	};

	// ==================== Texture Loader ==================== //
	class Renderer;

	class TextureLoader
	{
	public:
		explicit TextureLoader(Renderer& renderer);

		[[nodiscard]] Texture operator()(const TextureKey& key) const;

	private:
		Renderer& m_renderer;
	};

	using TextureManager = ResourceManager<Texture, TextureKey, TextureLoader>;
}

template<>
struct std::hash<cursed_engine::TextureKey>
{
	std::size_t operator()(const cursed_engine::TextureKey& key) const noexcept
	{
		return std::hash<std::string>{}(key.path);
	}
};