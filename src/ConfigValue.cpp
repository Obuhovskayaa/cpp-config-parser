#include "config/ConfigValue.hpp"




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
		return ValueType::Null;

	}

	std::string ConfigValue::toString() const noexcept {
		return
			std::visit([](auto&& value) -> std::string {

			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, int>) {
				return std::to_string(value);
			}
			else if constexpr (std::is_same_v<T, double>) {
				return std::to_string(value);
			}
			else if constexpr (std::is_same_v<T, bool>) {
				return value ? "true" : "false";
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				return value;
			}
			else if constexpr (std::is_same_v<T, std::vector<ConfigValue>>) {

				std::stringstream ss;
				ss << "[";

				for (size_t i = 0; i < value.size(); i++) {
					ss << value[i].toString();

					if (i != value.size() - 1) {
						ss << ", ";
					}
				}
				ss << "]";
				return ss.str();
			}
			else {
				return "unknown type";
			}

				}, m_value);
	}



}