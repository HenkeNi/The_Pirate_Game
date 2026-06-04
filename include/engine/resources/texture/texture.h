#pragma once
#include <functional>

struct SDL_Texture;

namespace cursed_engine
{	
	// Consider using NonCopyable instead?
	class Texture
	{
	public:
		struct Tag{}; // Remove?

		Texture();
		Texture(SDL_Texture* texture);  // TODO; pass in width / height as well?
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

	struct TextureDescriptor // Nest inside texture?? Texture::Key?
	{
		std::string path;

		bool operator==(const TextureDescriptor& other) const noexcept
		{
			return path == other.path;
		}
	};	
}

template<>
struct std::hash<cursed_engine::TextureDescriptor>
{
	std::size_t operator()(const cursed_engine::TextureDescriptor& key) const noexcept
	{
		return std::hash<std::string>{}(key.path);
	}
};