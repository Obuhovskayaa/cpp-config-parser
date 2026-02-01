#ifndef CONFIG_NODE_HPP
#define CONFIG_NODE_HPP


#include "ConfigValue.hpp"
#include <map>
#include <vector>
#include <string>

namespace config {

	class ConfigNode {

	public:
		ConfigNode() = default;

		const ConfigValue* findValue(const std::string& path) const noexcept;

		ConfigValue& operator[](const std::string& path);

		const ConfigValue& at(const std::string& path) const;

		ConfigValue& at(const std::string& path);

		template<typename T>
		T get(const std::string& path) const {
			return at(path).get<T>();
		}

		bool has(const std::string& path) const noexcept;

	private:

		std::map<std::string, ConfigValue> m_values;
		std::map<std::string, ConfigNode> m_children;

		static std::vector<std::string> splitPath(const std::string& path);
	};
}

#endif