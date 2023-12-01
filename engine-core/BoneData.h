#pragma once

#define NUM_BONES_PER_VERTEX 4

namespace Engine
{
	namespace Graphics
	{
		struct BoneData
		{
			unsigned int IDs[NUM_BONES_PER_VERTEX];
			float Weights[NUM_BONES_PER_VERTEX];
		};
	}
}

