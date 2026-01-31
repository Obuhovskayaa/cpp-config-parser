#include <iostream>
#include "config/ConfigValue.hpp"
#include "config/ConfigNode.hpp"



int main() {

	config::ConfigNode node;

	node["database.name"] = "David";

	node["database.email"] = "DavidPobedit";

	node["Sergey.sportCategory"] = "TretiiUnosheskiy";


	const auto* name = node.findValue("database.name");

	const auto* email = node.findValue("database.mail");

	if (node.has("Sergey.sportCategory")) {
		std::cout << node.at("Sergey.sportCategory").get<std::string>();

		std::cout << "\n" << node.get<std::string>("Sergey.sportCategory") << "\n";
	}

	if (name) {
		std::cout << name->get<std::string>() << std::endl;
	}

	if (!email) {
		std::cout << "email is not found" << std::endl;
	}
	

	

	

	return 0;



}

