#ifndef CONFIG_NODE_HPP
#define CONFIG_NODE_HPP


#include "ConfigValue.hpp"
#include <map>
#include <vector>
#include <string>

namespace config {

	class ConfigNode {

	private:
		std::map<std::string, ConfigValue> m_values;
		std::map<std::string, ConfigNode> m_children;

		static std::vector<std::string> splitPath(const std::string& path) {

			std::vector<std::string> splited;
			std::string word;

			for (char c : path) {

				if (c == '.') {
					if (!word.empty()) {
						splited.push_back(std::move(word));
						word.clear();
					}

				}
				else {
					word += c;
				}
			}

			if (!word.empty()) {
				splited.push_back(std::move(word));
			}

			return splited;

		}

		

		

	public:
		ConfigNode() = default;

		const ConfigValue* findValue(const std::string& path) const {

			auto parts = splitPath(path);

			if (parts.empty()) {
				return nullptr;
			}

			const ConfigNode* currentNode = this;

			for (size_t i = 0; i < parts.size() - 1; ++i) {

				auto it = currentNode->m_children.find(parts[i]); //points at the key if it's found

				if (it == currentNode->m_children.end()) {
					return nullptr;
				}

				//currentNode = &currentNode->m_children.at(parts[i]); //[] is not allowed because it can modify the object
				currentNode = &(it->second);

			}

			auto it = currentNode->m_values.find(parts.back());

			if (it == currentNode->m_values.end()) {
				return nullptr;
			}

			return &(it->second);
		}


		ConfigValue& operator[](const std::string& path) {

			auto parts = splitPath(path);

			if (parts.empty()) {
				throw std::invalid_argument(
					"Empty path is not allowed"
				);
			}

			if (parts.size() == 1) {
				//if key exists then return ConfigValue& of this key, otherwise create new pair with new key and default ConfigValue
				return m_values[parts[0]];
			}

			ConfigNode* currentNode = this;

			for (size_t i = 0; i < parts.size() - 1; ++i) {
				//create the key or apply to it and return its value(&ConfigNode)
				ConfigNode& child = currentNode->m_children[parts[i]]; 

				currentNode = &child;
				
			}
			//now currentNode refers to the second last node
			//and we return the last part of "parts" to its m_values

			return currentNode->m_values[parts.back()];

		}


		const ConfigValue& at(const std::string& path) const { //for constant objects only for read

			if (const ConfigValue* val = findValue(path)) {
				return *val;
			}
			throw std::runtime_error("Path is not found: " + path);

		}

		ConfigValue& at(const std::string& path) {

			return const_cast<ConfigValue&>(std::as_const(*this).at(path));
		}

		template<typename T>
		T get(const std::string& path) const {
			return at(path).get<T>();
		}

		bool has(const std::string& path) const {
			return findValue(path) != nullptr;
		}

		




	};




}







#endif