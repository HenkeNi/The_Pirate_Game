#include "engine/math/noise.h"
#include "engine/core/logger.h"
#include <FastNoiseLite.h>
#include <format>

namespace cursed_engine
{
	Noise::Noise() 
		: m_noise{ std::make_unique<FastNoiseLite>() }
	{
	}

	Noise::~Noise()
	{
	}

	void Noise::setSettings(Settings settings)
	{
		m_settings = std::move(settings);

		setSeed(settings.seed);
		setFrequency(settings.frequency);
		setType(settings.type);
	}

	void Noise::setSeed(int seed)
	{
		if (m_settings.seed == seed)
			return;

		m_noise->SetSeed(seed);
		m_settings.seed = seed;

		cursed_engine::Logger::logInfo(std::format("[Noise::setSeed] - changed seed to: {}", seed));
	}
	
	void Noise::setFrequency(float frequency)
	{
		if (m_settings.frequency == frequency)
			return;

		m_noise->SetFrequency(frequency);
		m_settings.frequency = frequency;

		cursed_engine::Logger::logInfo(std::format("[Noise::setFrequency] - changed frequency to: {}", frequency));
	}

	void Noise::setType(NoiseType type)
	{
		if (m_settings.type == type)
			return;

		m_noise->SetNoiseType(static_cast<FastNoiseLite::NoiseType>(type));
		m_settings.type = type;

		cursed_engine::Logger::logInfo(std::format("[Noise::setType] - changed noise type to: {}", (int)type));
	}

	float Noise::sample(float x, float y) const noexcept
	{
		return m_noise->GetNoise(x, y);
	}
}