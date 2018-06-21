#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <Windows.h>

class CameraConfigurationFile
{

	std::string path;
	std::unordered_map<std::string, std::string> content{};

	std::string getValue(const std::string& key)
	{
		const auto v = content.find(key);
		if (v != std::end(content))
			return v->second;

		throw std::runtime_error("Value " + key + " doesn't exist in loaded config.");
	}

	double get_double(const std::string& key)
	{
		return strtod(getValue(key).c_str(), nullptr);
	}

	size_t get_size_t(const std::string& key)
	{
		return static_cast<size_t>(strtoul(getValue(key).c_str(), nullptr, 10));
	}

	void loadFile()
	{
		std::ifstream file(path);
		if (!file)
			throw std::runtime_error("Could not open file " + path);

		static std::string buffer, key, val;
		static const std::string whitespace{ " \t\r\n\v\f" };

		while (std::getline(file, buffer))
		{
			if (buffer.empty()) continue; //Dont evaluate empty lines

			const auto firstNotWhitespace = buffer.find_first_not_of(whitespace);
			if (firstNotWhitespace == std::string::npos) continue; //don't evaluate lines that contains only whitespace

			if (buffer[firstNotWhitespace] == '#') continue; //comments starts with #

			//Load the key=value in the map
			std::stringstream loadStream(buffer);
			std::getline(loadStream, key, '=');
			std::getline(loadStream, val);
			content[key] = val;
		}

	}

public:

	static std::string getMyDocumentPath()
	{
		//Get Win32 string path to "My Documents"
		TCHAR tchar_string[MAX_PATH];
		const auto result = SHGetFolderPath(nullptr, CSIDL_MYDOCUMENTS, nullptr, SHGFP_TYPE_CURRENT, tchar_string);
		if (result != S_OK) return "";

		//Handle the bytestring convertion if we are using wide characters
#ifdef UNICODE
		//Create an empty string with the right size reserved
		auto string_string = std::string(static_cast<size_t>(WideCharToMultiByte(CP_UTF8, 0, tchar_string, lstrlen(tchar_string), nullptr, 0, nullptr,
																		nullptr)), 0);
		//Fill in the string 
		WideCharToMultiByte(CP_UTF8, 0, tchar_string, lstrlen(tchar_string), &string_string[0], static_cast<int>(string_string.size()), nullptr, nullptr);
		return string_string;
#else
return { tchar_string };
#endif

	}

	size_t width, height;
	double fps;

	CameraConfigurationFile(const std::string& filePath) : path{filePath}
	{
		loadFile();
	}

	size_t getWidth()
	{
		return get_size_t("width");
	}

	size_t getHeight()
	{
		return get_size_t("height");
	}

	double getFrameRate()
	{
		return get_double("FPS");
	}

	void readConfig()
	{
		width = getWidth();
		height = getHeight();
		fps = getFrameRate();
	}
};


