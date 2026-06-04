#pragma once
#include <array>
#include "engine/math/vec3.hpp"

namespace cursed_engine
{
	template <typename T>
	struct Mat3
	{
		Mat3();
		Mat3(std::initializer_list<T> list);

		static Mat3 identity();
		static Mat3 zero();

		static Mat3 rotateX(T radians);

		const T& operator()(std::size_t row, std::size_t col) const;
		T& operator()(std::size_t row, std::size_t col);

		std::array<T, 9> m;
	};


#pragma region Definitions

	template <typename T>
	Mat3<T>::Mat3()
		: m{ T{ 1 }, T{ 0 }, T{ 0 },
			 T{ 0 }, T{ 1 }, T{ 0 },
			 T{ 0 }, T{ 0 }, T{ 1 } }
	{
	}

	template <typename T>
	Mat3<T>::Mat3(std::initializer_list<T> list)
	{
		//asert size...
		std::copy(list.begin(), list.end(), m.begin());
	}

	template <typename T>
	Mat3<T> Mat3<T>::identity()
	{
		return Mat3{ T{ 1 }, T{ 0 }, T{ 0 },
					 T{ 0 }, T{ 1 }, T{ 0 },
					 T{ 0 }, T{ 0 }, T{ 1 } };
	}

	template <typename T>
	Mat3<T> Mat3<T>::zero()
	{
		return Mat3{ T{ 1 }, T{ 0 }, T{ 0 },
					 T{ 0 }, T{ 1 }, T{ 0 },
					 T{ 0 }, T{ 0 }, T{ 1 } };
	}

	template <typename T>
	const T& Mat3<T>::operator()(std::size_t row, std::size_t col) const
	{
		return m[row * 3 + col];
	}
	
	template <typename T>
	T& Mat3<T>::operator()(std::size_t row, std::size_t col)
	{
		return m[row * 3 + col];
	}

#pragma endregion
}