#pragma once
#include <vector>

namespace cursed_engine
{
	// Could store just indexes?!
	// Rename FlatTree?

	template <typename T>
	class Tree
	{
	public:
		struct Node
		{
			std::vector<Node*> children;
			T value;
		};

	private:
	};
}