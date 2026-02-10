#include "config/JsonParser.hpp"
#include <stdexcept>


namespace config {


	JsonParser::JsonParser(std::string_view jsonText)
		: m_input(jsonText) {}

	ConfigNode JsonParser::parse() {
		return parseNode();
	}

	ConfigNode JsonParser::parseNode() {
		
		skipSpaces();
		ConfigNode node;
		if (current() == '{') {
			advance();
			throughNode("", node);
		}
		else {
			throw std::logic_error(
				"Line: " +
				std::to_string(m_line) +
				"Expected '{'"
			);
		}
		return node;
	}

	void JsonParser::throughNode(std::string path, ConfigNode& node) {

		parsePath(path, node);

		if (current() != '}') {
			throw std::logic_error(
				"Line: " +
				std::to_string(m_line) +
				": Expected '}'"
			);
		}

		advance();
	}

	void JsonParser::parsePath(std::string path, ConfigNode& node) {

		skipSpaces();

		if (current() != '\"') {
			throw std::logic_error(
				"Line: " +
				std::to_string(m_line) +
				": expected '\"' as a start of a word"		
			);
		}

		advance();
		std::string word;

		while (current() != '\"') {
			if (current() == '\n') {
				throw std::logic_error(
					"Line: " +
					std::to_string(m_line) +
					": expected '\"' as an end of the word"
				);
			}
			word += current();
			advance();
		}

		advance();
		skipSpaces();

		if (current() != ':') {
			throw std::logic_error(
				"Line: " +
				std::to_string(m_line) +
				": expected ':' "	
			);
		} 

		advance();
		skipSpaces();

		if (current() == '\"') {
			advance();
			node[path + "." + word] = parseString();
		}
		else if (isNumber(current())) {
			node[path + "." + word] = parseNumber();
		}
		else if (current() == 'n') {
			node[path + "." + word] = parseNull();
		}
		else if (current() == 't' || current() == 'f') {
			node[path + "." + word] = parseBool();
		}
		else if (current() == '[') {
			advance();
			node[path + "." + word] = parseVector();
		}
		else if (current() == '{') {
			advance();
			throughNode(path + "." + word, node);
		}
		else {
			throw std::logic_error(
				"Line " +
				std::to_string(m_line) +
				": Unexpected symbol '" +
				current() +
				"'"
			);
		}
		skipSpaces();
		if (current() == ',') {
			advance();
			parsePath(path, node);
		}
		else {
			return;
		}

	}

	bool JsonParser::isNumber(char c) {

		return c == '0' || c == '1' || c == '2' ||
			c == '3' || c == '4' || c == '5' || c == '6' ||
			c == '7' || c == '8' || c == '9';
		
	}

	ConfigValue JsonParser::parseNull() {

		if (m_input.substr(m_position, 4) == "null") {

			m_position += 4;
			skipSpaces();
			return ConfigValue();		
		}
		else {
			std::string_view nextChars = m_input.substr(m_position, 4);

			throw std::logic_error(
				"Line " +
				std::to_string(m_line) +
				": Expected 'null' but found '" +
				std::string(nextChars) +
				"'"
			);
		}
	}

	ConfigValue JsonParser::parseNumber() {

		std::string str;
		bool isDouble = false;
		while (current() != ',' && m_input[m_position] != ' ' &&
			m_input[m_position] != '\n' && m_input[m_position] != '}' && 
			m_input[m_position] != ']')
		{
			if (isDouble && m_input[m_position] == '.') {
				throw std::logic_error(
					"Line " +
					std::to_string(m_line) +
					": incorrect number '" +
					str +
					"'"
				);
			} 

			str += m_input[m_position];
			advance();

			if (m_input[m_position - 1] == '.') {
				isDouble = true;
			}
				
		}
		if (isDouble) {
			return ConfigValue(std::stod(str));
		}
		return ConfigValue(std::stoi(str));

	}

	ConfigValue JsonParser::parseBool() {
		
		bool value;

		if (m_input.substr(m_position, 4) == "true") {
			m_position += 4;
			value = true;
		}
		else if (m_input.substr(m_position, 5) == "false") {
			m_position += 5;
			value = false;
		}
		else {
			std::string_view nextChars = m_input.substr(m_position, 5);

			throw std::logic_error(
				"Line " +
				std::to_string(m_line) +
				": Expected true/false, but found " +
				std::string(nextChars)
			);
		}

		skipSpaces();
		return ConfigValue(value);
	}

	ConfigValue JsonParser::parseString() {
		std::string str;
		while (current() != '"') {
			if (current() == '\n') {
				throw std::logic_error(
					"Line " +
					std::to_string(m_line) +
					": Expected '\"'"
				);
			}
			str += current();
			++m_position;
		}
		advance();
		return ConfigValue(str);
	}

	ConfigValue JsonParser::parseVector() {
		
		skipSpaces();
		std::vector<ConfigValue>values;

		while (true) {
			if (isNumber(current())) {
				values.push_back(parseNumber());
			}
			else {
				switch (m_input[m_position]) {
				case '\"': {
					advance();
					values.push_back(parseString());
					break;
				}
				case 'n':
					values.push_back(parseNull());
					break;
				case '{': {
					auto node = std::make_shared<ConfigNode>(std::move(parseNode()));
					values.push_back(ConfigValue(node));
					break;
				}
				case '[': {
					advance();
					values.push_back(parseVector());
					break;
				}
				case 't':
					values.push_back(parseBool());
					break;
				case 'f':
					values.push_back(parseBool());
					break;
				default:
					throw std::logic_error(
						"Line " +
						std::to_string(m_line) +
						": Cannot handle symbol '" +
						m_input[m_position] +
						"'"
					);
				}
				
			}
			skipSpaces();
			if (current() != ',') {
				if (m_input[m_position] == ']') {
					advance();
					break;
				}
				else {
					throw std::logic_error(
						"Line " +
						std::to_string(m_line) +
						": Expected ',' or ']'. Cannot handle symbol '" +
						m_input[m_position] +
						"'"
					);
				}
			}
			advance();
			skipSpaces();
		}
		return ConfigValue(values);

	}

	void JsonParser::skipSpaces() {

		while (m_position < m_input.size()) {
			char c = current();

			if (c == ' ' || c == '\t' || c == '\r') {
				++m_position;
			}
			else if (c == '\n') {
				++m_position;
				++m_line;
			}
			else {
				break;
			}
		}
	}

	void JsonParser::advance() {

		if (m_position < m_input.size()) {
			++m_position;
		}
	}

	char JsonParser::current() {

		return (m_position < m_input.size()) ? m_input[m_position] : '\0';
	}

}
