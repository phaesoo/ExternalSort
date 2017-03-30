#pragma once



#include <vector>

#include <string>




	class TextUtil

	{

	public:

		TextUtil() = delete;



		static std::vector<std::string> SeperateString(const std::string& text, char token);

	};

