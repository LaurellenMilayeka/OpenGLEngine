#include "File.h"
#include <filesystem>

using namespace Engine::Filesystem;

File::File(std::string const& fileName)
{
	_mFile = std::fstream(fileName);

	_mIsValid = _mFile.good();
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

size_t File::Read(char **out, size_t const len)
{
	_mFile.read(*out, len);
	if (_mFile)
		return len;
	return -1;
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
