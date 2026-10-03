#pragma once
#include "engine/math/vec2.hpp"

struct SDL_Texture;

namespace cursed_engine
{
	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		Texture(const Texture&) = delete;
		Texture(Texture&& other) noexcept = default;

		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) noexcept = default;

		[[nodiscard]] virtual float getWidth() const noexcept = 0;
		[[nodiscard]] virtual float getHeight() const noexcept = 0;

		[[nodiscard]] virtual bool isValid() const noexcept = 0;
	};

	class SDLTexture final : public Texture
	{
	public:
		SDLTexture(SDL_Texture* texture = nullptr);
		~SDLTexture();

		SDLTexture(SDLTexture&& other) noexcept;
		SDLTexture& operator=(SDLTexture&& other) noexcept;

		[[nodiscard]] inline float getWidth() const noexcept { return m_size.x; }
		[[nodiscard]] inline float getHeight() const noexcept { return m_size.y; }

		[[nodiscard]] inline SDL_Texture* getInternal() const noexcept { return m_texture; }
		[[nodiscard]] bool isValid() const noexcept override;

	private:
		SDL_Texture* m_texture;
		FVec2 m_size;
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