#include "config/ConfigValue.hpp"
#include <sstream>



namespace config {

	ConfigValue::ConfigValue() : m_type(ValueType::Null) { 

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
				const auto& other_vec = *other.m_vectorValue; //excessively, but wrote it because i could
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

	ValueType ConfigValue::type() const {

		return m_type;
	}

	bool ConfigValue::isNull() const {

		return m_type == ValueType::Null;
	}

	bool ConfigValue::isInt() const {

		return m_type == ValueType::Int;
	}

	bool ConfigValue::isDouble() const {

		return m_type == ValueType::Double;
	}

	bool ConfigValue::isBool() const {

		return m_type == ValueType::Bool;
	}

	bool ConfigValue::isString() const {

		return m_type == ValueType::String;
	}

	bool ConfigValue::isVector() const {

		return m_type == ValueType::Vector;
	}

	int ConfigValue::asInt() const {

		if (!isInt()) {
			throw std::runtime_error{ "Type is not int!" };
		}
		return m_intValue;
	}

	double ConfigValue::asDouble() const {

		if (!isDouble()) {
			throw std::runtime_error{ "Type is not double!" };
		}
		return m_doubleValue;
	}

	bool ConfigValue::asBool() const {

		if (!isBool()) {
			throw std::runtime_error{ "Type is not bool!" };
		}
		return m_boolValue;

	}

	const std::string& ConfigValue::asString() const {

		if (!isString()) {
			throw std::runtime_error{ "Type is not string!" };
		}
		return m_stringValue;

	}
	
	const std::vector<ConfigValue>& ConfigValue::asVector() const { 

		if (!isVector()) {
			throw std::runtime_error{ "Type is not vector!" };
		}
		return *m_vectorValue;
	}

	std::string ConfigValue::toString() const {

		std::stringstream ss;

		switch (m_type) {

		case ValueType::Null: 
			ss << "null";
			break;
		case ValueType::Int:
			ss << m_intValue;
			break;
		case ValueType::Double:
			ss << m_doubleValue;
			break;
		case ValueType::Bool:
			ss << (m_boolValue ? "true" : "false");
			break;
		case ValueType::String:
			ss << "\"" << m_stringValue << "\"";
			break;
		case ValueType::Vector:
			for (const auto& value : *m_vectorValue) {
				ss << value.toString() << ", ";
			}
			break;
		}

		return ss.str();
	}

	void ConfigValue::cleanUp() {
		if (m_type == ValueType::String) {
			m_stringValue.~basic_string();
		}
		if (m_type == ValueType::Vector) {
			delete m_vectorValue;
		}
		m_type = ValueType::Null;
	}

	ConfigValue::~ConfigValue() {
		cleanUp();
	}





}