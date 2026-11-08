#pragma once
#include "engine/resources/resource_types.h"
#include <filesystem>
#include <memory>
#include <span>
#include <string_view>

namespace cursed_engine
{
	template <typename T>
	class Result;

	class AudioCreator;
	class TextureCreator;
	struct Surface;
	
	class AudioLoader final
	{
	public:
		explicit AudioLoader(const AudioCreator* creator);
		[[nodiscard]] Result<AudioPtr> operator()(const struct AudioDescriptor& descriptor) const;

		// THIS?
		std::span<const std::string_view> extensions() const noexcept;

	private:
		const AudioCreator* m_creator;
	};

	class FontLoader final
	{
	public:
		[[nodiscard]] Result<FontPtr> operator()(const struct FontDescriptor& descriptor) const;

		std::span<const std::string_view> extensions() const noexcept;
	};

	struct SurfaceLoader final
	{
		[[nodiscard]] Result<Surface> operator()(const std::filesystem::path& path) const;
	};

	class TextureLoader final
	{
	public:
		explicit TextureLoader(const TextureCreator* creator);
		[[nodiscard]] Result<TexturePtr> operator()(const struct TextureDescriptor& descriptor) const;

		std::span<const std::string_view> extensions() const noexcept;

	private:
		const TextureCreator* m_creator;
	};
}