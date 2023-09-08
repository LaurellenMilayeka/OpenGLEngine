#include "File.h"
#include <iostream>
#include <errno.h>
#include <string.h>

#include <filesystem>

using namespace Engine::Filesystem;

File::File(std::string const& fileName, OpenMode openMode)
{
	try {
		if (openMode == OpenMode::Binary)
			_mFile = std::fstream(fileName, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
		else
			_mFile = std::fstream(fileName);

		_mIsValid = _mFile.is_open();
		if (_mIsValid)
		{
			_mFile.seekp(0, std::ios::end);
			_mLength = _mFile.tellg();
			_mFile.seekp(0, std::ios::beg);
		}
		else
		{
			_mLength = 0;
		}
	}
	catch (std::exception& e)
	{
		std::cout << "EXCEPTION CAUGHT : " << e.what() << std::endl;
	}
}

bool File::Create(std::string const& fileName)
{
	std::ofstream toCreate(fileName);
	return File::Exists(fileName);
}

bool File::Exists(std::string const& fileName)
{
	std::ifstream toCheck;

	toCheck.open(fileName);
	return toCheck.is_open();
}

bool File::IsValid() const
{
	return _mIsValid;
}

size_t File::Length() const
{
	return _mLength;
}

size_t File::Read(std::string& out, size_t const len)
{
	char* tmpOut = new char[len + 1] {};

	if (!_mFile.is_open()) {
		delete[] tmpOut;
		return -1;
	}

	_mFile.read(tmpOut, len);
	out = std::string(tmpOut, len);

	delete[] tmpOut;

	return len;
}

size_t File::ReadLine(std::string& out)
{
	size_t begp = _mFile.tellg();
	std::getline(_mFile, out);
	size_t endp = _mFile.tellg();

	return endp - begp;
}

size_t File::Write(std::string const& toWrite)
{
	_mFile.write(toWrite.c_str(), toWrite.length());
	return 0;
}

void File::Close()
{
	_mFile.close();
}
