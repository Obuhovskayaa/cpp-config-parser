#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <string>
#include <string_view>
#include "config/ConfigValue.hpp"
#include "config/ConfigNode.hpp"


namespace config {

	class JsonParser {
		
	public:
		JsonParser(std::string_view jsonText);
		ConfigNode parse();

	private:
		
		std::string_view m_input;
		size_t m_position = 0;
		int m_line = 1;

		void skipComments(); //todo
		void skipSpaces();
		void advance();
		char current();
		static bool isNumber(char);

		void parsePath(std::string name, ConfigNode& node);
		void throughNode(std::string path, ConfigNode& node);
		//every parseValue method stops at the next non-space element after the parsed value
		ConfigNode parseNode();
		ConfigValue parseNull();
		ConfigValue parseNumber();
		ConfigValue parseBool();
		ConfigValue parseString();
		ConfigValue parseVector();
		
		
		
		
		
	};
}





#endif JSON_PARSER_HPP