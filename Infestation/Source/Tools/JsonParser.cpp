#include "JsonParser.h"

#include <fstream>
#include <iostream>
#include <functional>

data::TextureData parseJsonFile(const std::string& name, const std::string& extra_identifier)
{
	data::TextureData data;

	std::ifstream fin("Assets/TextureData/" + name + "_Data.json", std::ios::in);
	if (fin.is_open()) {
		unsigned short index = 1;

		while (!fin.eof()) {
			fin.seekg(0);

			std::string strNeeded = name + "_";
			if (!extra_identifier.empty())
				strNeeded += extra_identifier + "_";
			strNeeded += std::to_string(index++) + ".png";

			std::string strRead;

			// Find Filename
			do {
				std::getline(fin, strRead);
			} while (strRead.find(strNeeded) == std::string::npos && !fin.eof());
			if (fin.eof())
				break;

			// Find Frame
			std::getline(fin, strRead);
			fin.seekg((size_t)fin.tellg() - (strRead.length() + 1));

				// Find Rect Start
			sf::FloatRect rect;
			char ch;
			do {
				fin.get(ch);
			} while (ch != '{');

				// Find Values
			std::function<std::string()> findNextValue([&fin]() -> std::string {
				std::string value = "";
				char ch;

				do {
					fin.get(ch);
				} while (ch != ':');

				while (true) {
					fin.get(ch);
					if (ch != ',' && ch != '}')
						value += ch;
					else
						break;
				}

				return value;
			});
			
			rect.left   = std::stof(findNextValue());
			rect.top    = std::stof(findNextValue());
			rect.width  = std::stof(findNextValue());
			rect.height = std::stof(findNextValue());

			// Find Pivot
			for (unsigned i = 0; i < 6; i++) {
				std::getline(fin, strRead);
			}
			fin.seekg((size_t)fin.tellg() - (strRead.length() + 1));

			// Find Origin Start
			sf::Vector2f origin;
			do {
				fin.get(ch);
			} while (ch != '{');

			// Find Values
			origin.x = std::stof(findNextValue());
			origin.y = std::stof(findNextValue());

			data.push_back(std::make_pair(rect, origin));
		}

		fin.close();
	}
	else
		std::cerr << "Unable to open file: " << name << "_Data.json\n";

	return std::move(data);
}