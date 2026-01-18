#include <iostream>
#include "config/ConfigValue.hpp"

int main() {
	std::cout << "=== Testing ConfigValue ===\n\n";


	std::cout << "=== Creating values === \n";

	config::ConfigValue num(42);
	config::ConfigValue height(3.15);
	config::ConfigValue flag(true);
	config::ConfigValue text(std::string("Gel"));
	
	std::vector<config::ConfigValue>values = { 53, true, 3.18 };
	config::ConfigValue several(values);
	config::ConfigValue empty;

	std::cout << " num: " << num.toString() << std::endl;
	std::cout << " height: " << height.toString() << std::endl;
	std::cout << " flag: " << flag.toString() << std::endl;
	std::cout << " text: " << text.toString() << std::endl;
	std::cout << " empty: " << empty.toString() << std::endl;
	std::cout << " several: " << several.toString() << std::endl;

	std::cout << "\n=== Getting values === \n";

	auto tryGet = [](auto func, const std::string& msg) {
		try {
			func();
		}
		catch (const std::exception& e) {
			std::cerr << msg << "Error: " << e.what() << std::endl;
		}
		};

		tryGet([&num]() { std::cout << "num as int: " << num.asInt() << std::endl; }, "num ");
		tryGet([&height]() { std::cout << "height as int: " << height.asInt() << std::endl; }, "height ");

	/*std::cout << "\n=== DEBUG Vector memory ===\n";
	std::cout << "Adress of several: " << &several << std::endl;
	std::cout << "Type of several (must be vector): ";
	switch (several.type()) {
	case config::ValueType::Vector:
		std::cout << "Vector\n";
		break;
	case config::ValueType::String:
		std::cout << "String\n";
		break;
	default:
		std::cout << " Other\n";
		break;
	}
	*/

	std::cout << "\n\n=== Type checking: ===\n";
	std::cout << " is num int? " << (num.isInt() ? "YES" : "NO") << std::endl;
	std::cout << " is height double? " << (height.isDouble() ? "YES" : "NO") << std::endl;

	return 0;



}