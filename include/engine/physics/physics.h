#pragma once
#include "engine/utils/non_copyable.h"

namespace cursed_engine
{
	struct PhysicsBody
	{
		enum class BodyType
		{
			Static = 0, 
			Kinematic,
			Dynamic,
			Count
		};

		int32_t index;
		uint16_t world;
		uint16_t generation;
	}; 

	class PhysicsWorld : public NonCopyable
	{
	public:
		PhysicsWorld(uint16_t index, uint16_t generation);
		~PhysicsWorld();

		PhysicsWorld(PhysicsWorld&& other) noexcept;
		PhysicsWorld& operator=(PhysicsWorld&& other) noexcept;

		PhysicsBody createBody(PhysicsBody::BodyType type); // createCharacter?
		void destroyBody(PhysicsBody body);

		//[[nodiscard]] bool isValidBody(PhysicsBody body) const;
	
	private:
		void destroy();

		uint16_t m_index; // or not unsigend?
		uint16_t m_generation;
	};

	class Physics
	{
	public:
		[[nodiscard]] PhysicsWorld createWorld() const;

	//	[[nodiscard]] bool isValidWorld(PhysicsWorld world) const;

	private:
	};
}