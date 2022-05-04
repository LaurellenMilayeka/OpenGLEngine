#pragma once

#include <string>
#include <math.h>

#include <rpc.h>

namespace Engine
{
	namespace Helpers
	{
		inline float DegreeToRad(float value)
		{
			return static_cast<float>(value * (M_PI / 180.0f));
		}

		inline float RadToDegree(float value)
		{
			return static_cast<float>(value * (180.0f * M_PI));
		}

		inline std::string GenerateGUID()
		{
			UUID uuid;
			unsigned char* str = nullptr;
			std::string retval = "";

			if (UuidCreate(&uuid) == RPC_S_OK)
			{
				if (UuidToStringA(&uuid, &str) == RPC_S_OK)
				{
					retval.assign(reinterpret_cast<char*>(str));
					RpcStringFreeA(&str);
				}
			}
			return retval;
		}

		inline std::string Trim(std::string const& src)
		{
			return (src.substr(src.find_first_not_of(" \n\r\t\f\v\0"), src.find_last_not_of(" \n\r\t\f\v\0") + 1));
		}
	}
}