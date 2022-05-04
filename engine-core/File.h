#pragma once

#include <fstream>
#include <string>

namespace Engine
{
	namespace Filesystem
	{

		class File
		{
		private:

			std::fstream _mFile;
			bool _mIsValid;
			size_t _mLength;

		public:

			File(std::string const& fileName);
		
			static bool Create(std::string const& fileName);
			static bool Exists(std::string const& fileName);

			bool   IsValid() const;
			size_t Length() const;

			size_t Read(char** out, size_t const len);
			size_t ReadLine(std::string& out);
			size_t Write(std::string const& toWrite);

			void Close();
		};
	}
}


