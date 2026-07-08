#pragma once
#include <memory>

class FastNoiseLite;

namespace cursed_engine
{
	enum class NoiseType
	{
		OpenSimplex2,
		OpenSimplex2S,
		Cellular,
		Perlin,
		ValueCubic,
		Value
	};

	// or NoiseGenerator?
	class Noise
	{
	public:
		struct Settings
		{
			float frequency = 0.01f;
			NoiseType type = NoiseType::OpenSimplex2;
			int seed = 0;
		};

		Noise();
		~Noise();

		void setSettings(Settings settings);
		void setSeed(int seed);

		void setFrequency(float frequency);
		void setType(NoiseType type);

		[[nodiscard]] float sample(float x, float y) const noexcept;

	private:
		std::unique_ptr<FastNoiseLite> m_noise;
		Settings m_settings;
	};
}