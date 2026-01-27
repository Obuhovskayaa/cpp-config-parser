#include "config/ConfigValue.hpp"
#include <sstream>



namespace config {



	/*ConfigValue::ConfigValue() : m_type(ValueType::Null) {

	}

	ConfigValue::ConfigValue(int value) : m_type(ValueType::Int), m_intValue(value) {

	}

	ConfigValue::ConfigValue(double value) : m_type(ValueType::Double), m_doubleValue(value) {

	}

	ConfigValue::ConfigValue(bool value) : m_type(ValueType::Bool), m_boolValue(value) {

	}

	ConfigValue::ConfigValue(const std::string& value) : m_type(ValueType::String) {
		
		new (&m_stringValue) std::string(value);
	}

	ConfigValue::ConfigValue(const std::vector<ConfigValue>& values) : m_type(ValueType::Vector) {

		m_vectorValue = new std::vector<ConfigValue>(values);
	}

	ConfigValue& ConfigValue::operator=(const ConfigValue& other) {

		if (this != &other) {
			cleanUp();

			m_type = other.m_type;

			switch (m_type) {
			case ValueType::Int:
				m_intValue = other.m_intValue;
				break;
			case ValueType::Double:
				m_doubleValue = other.m_doubleValue;
				break;
			case ValueType::Bool:
				m_boolValue = other.m_boolValue;
				break;
			case ValueType::String:
				new (&m_stringValue) std::string(other.m_stringValue);
				break;
			case ValueType::Vector:
			{
				const auto& other_vec = *other.m_vectorValue;
				m_vectorValue = new std::vector<ConfigValue>(other_vec);
				break;
			}
			case ValueType::Null:
				break;

			}
			return *this;
		}

	}

	ConfigValue::ConfigValue(const ConfigValue& other) : m_type(other.m_type) {

		switch (m_type) {
		case ValueType::Int:
			m_intValue = other.m_intValue;
			break;
		case ValueType::Double:
			m_doubleValue = other.m_doubleValue;
			break;
		case ValueType::Bool:
			m_boolValue = other.m_boolValue;
			break;
		case ValueType::String:
			new(&m_stringValue) std::string(other.m_stringValue);
			break;
		case ValueType::Vector:
		{
			const auto& other_vec = *other.m_vectorValue;
			m_vectorValue = new std::vector<ConfigValue>(other_vec);
			break;
		}
		case ValueType::Null:
			break;

		}
	}

	ConfigValue::ConfigValue(ConfigValue&& other) noexcept : m_type(other.m_type) {

		switch (m_type) {
		case ValueType::Int:
			m_intValue = other.m_intValue;
			break;
		case ValueType::Double:
			m_doubleValue = other.m_doubleValue;
			break;
		case ValueType::Bool:
			m_boolValue = other.m_boolValue;
			break;
		case ValueType::String:
		{
			new(&m_stringValue) std::string(std::move(other.m_stringValue));
			other.m_stringValue.~basic_string();
			break;
		}
		case ValueType::Vector:
		{
			m_vectorValue = other.m_vectorValue;
			other.m_vectorValue = nullptr;
			break;
		}
		}
		other.m_type = ValueType::Null;

	}
	*/



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


	std::string ConfigValue::toString() const {

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


	/*void ConfigValue::cleanUp() {
		if (m_type == ValueType::String) {
			m_stringValue.~basic_string();
		}
		if (m_type == ValueType::Vector) {
			delete m_vectorValue;
		}
		m_type = ValueType::Null;
	}
	*/






}