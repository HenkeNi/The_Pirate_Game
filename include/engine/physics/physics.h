#pragma once
#include "engine/physics/physics_types.h"
#include "engine/math/vec2.hpp"

namespace cursed_engine
{
	class PhysicsDebugDraw;

	// physics type


	enum class ColliderType // or body type?
	{
		Static = 0,
		Kinematic,
		Dynamic,
		Count
	};

	enum class Shape // Polygon Shape? collider shape?
	{
		Square,
		Rectangle,
		Circle
	};

	struct BodyDefinition
	{
		ColliderType type;
		Shape shape;
		FVec2 position{};
		float rotation = 0.0f;
		float linearDamping = 0.0f;
		float angularDamping = 0.0f;
		
		union ShapeData
		{
			struct 
			{
				float halfExtent;
			} Square;

			struct
			{
				float width;
				float height;
			} Rectangle;

			struct
			{
				float radius;
			} Circle;

		} shapeData;
	};


	

	struct PhysicsBody
	{
	
		int32_t index;
		uint16_t world;
		uint16_t generation;
	};

	// Game / scene should own a physics world...
	class PhysicsWorld
	{
	public:
		explicit PhysicsWorld(FVec2 gravity);
		~PhysicsWorld();

		PhysicsWorld(const PhysicsWorld&) = delete;
		PhysicsWorld(PhysicsWorld&& other) noexcept;

		PhysicsWorld& operator=(const PhysicsWorld& other) = delete;
		PhysicsWorld& operator=(PhysicsWorld&& other) noexcept;
		
		void step(); // or step

		[[nodiscard]] PhysicsBody createBody(const BodyDefinition& definition);

		PhysicsBody createGroundBody(const FVec2& position);
		PhysicsBody createDynamicBody(FVec2 position, ColliderType type, Shape shape);
		PhysicsBody createStaticBody();
		PhysicsBody createKinematicBody();

		void destroyBody(PhysicsBody body);

		// allow user to get world id?? -> need for passing current world to phhyisc debug draw
		[[nodiscard]] inline constexpr WorldId getWorldId() const noexcept
		{
			return m_worldId;
		}
		//[[nodiscard]] bool isValidBody(PhysicsBody body) const;

	private:
		WorldId m_worldId;
	};



	// maybe redundnat? or store bool enable debug here to?
	class PhysicsAPI
	{
	public:
		PhysicsAPI(PhysicsDebugDraw* physicsDebugDraw = nullptr);

		void setDebugDrawEnabled(bool enabled);
		void setWorldId(WorldId worldId);

		[[nodiscard]] constexpr bool isDebugDrawEnabled() const noexcept;

		// [[nodiscard]] PhysicsWorld createWorld() const;

		//	[[nodiscard]] bool isValidWorld(PhysicsWorld world) const;

	private:
		PhysicsDebugDraw* m_physicsDebugDraw;
	};


	// Box2D : public Physics --- REDUNDANT CLASS???
	class Physics
	{
	public:
		//void setDebugDrawEnabled(bool enabled);

	private:
		//bool m_debugDrawEnabled = false;
	};
}