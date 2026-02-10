#include <iostream>
#include <fstream>
#include <sstream>
#include "config/ConfigValue.hpp"
#include "config/ConfigNode.hpp"
#include "config/JsonParser.hpp"



int main() {

	std::ifstream file("../../../data/test_config.json");

	std::stringstream buffer;
	if (!file.is_open()) {
		std::cerr << "Cannot open the file " << std::endl;
	}
	else {
		buffer << file.rdbuf();
	}

	std::string jsonContent = buffer.str();
	
	config::JsonParser Parser(jsonContent);

	config::ConfigNode node;
	try {
		node = Parser.parse();
	}
	catch (const std::logic_error& e) {
		std::cerr << e.what() << std::endl;
	}
	

	std::cout << jsonContent << "\n\n";

	std::cout << "node: " << std::endl;
	node.printNode();

	return 0;

}

