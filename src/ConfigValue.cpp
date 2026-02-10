#include "config/ConfigValue.hpp"
#include "config/ConfigNode.hpp"



namespace config {

	ValueType ConfigValue::type() const noexcept {

		if (std::holds_alternative<std::monostate>(m_value)) {
			return ValueType::Null;
		}
		if (std::holds_alternative<int>(m_value)) {
			return ValueType::Int;
		}
		if (std::holds_alternative<double>(m_value)) {
			return ValueType::Double;
		}
		if (std::holds_alternative<bool>(m_value)) {
			return ValueType::Bool;
		}
		if (std::holds_alternative<std::string>(m_value)) {
			return ValueType::String;
		}
		if (std::holds_alternative<std::vector<ConfigValue>>(m_value)) {
			return ValueType::Vector;
		}
		if (std::holds_alternative<std::shared_ptr<ConfigNode>>(m_value)) {
			return ValueType::Node;
		}
		return ValueType::Null;

	}

	std::string ConfigValue::toString(const std::string& tabs, bool forVec) const noexcept {
		return
			std::visit([&tabs, forVec](auto&& value) -> std::string {

			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, int>) {
				return (forVec ? tabs : "") + std::to_string(value);
			}
			else if constexpr (std::is_same_v<T, double>) {
				return (forVec ? tabs : "") + std::to_string(value);
			}
			else if constexpr (std::is_same_v<T, bool>) {
				return (forVec ? tabs : "") + (value ? "true" : "false");
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				return (forVec ? tabs : "") + "\"" + value + "\"";
			}
			else if constexpr (std::is_same_v<T, std::vector<ConfigValue>>) {

				std::string addTab("  ");
				addTab += tabs;

				std::stringstream ss;
				forVec ? ss << tabs : ss << "";
				ss << "[\n";
			
				for (size_t i = 0; i < value.size(); i++) {
					ss << value[i].toString(addTab, true);

					if (i != value.size() - 1) {
						ss << ", \n";
					}
				}
				
				ss << "\n" << tabs << "]";
				return ss.str();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<ConfigNode>>) {
				if (value) {
					return value->toString(tabs, forVec);
				}
				return "";
			}
			else {
				return "unknown type";
			}

				}, m_value);
	}



}