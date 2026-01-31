#ifndef CONFIG_VALUE_HPP
#define CONFIG_VALUE_HPP

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <sstream>

namespace config {

	enum class ValueType {

		Null,
		Int,
		Double,
		Bool,
		String,
		Vector

	};

	class ConfigValue {
	public:

		using VariantType = std::variant<
			std::monostate,
			int,
			double,
			bool,
			std::string,
			std::vector<ConfigValue>
		>;

		ConfigValue() : m_value(std::monostate{}) {}

		/*
		ConfigValue(const ConfigValue&) = default;

		ConfigValue& operator=(const ConfigValue&) = default;

		ConfigValue(ConfigValue&&) noexcept = default;
		*/


		template<typename T, typename =
		std::enable_if_t< ! std::is_same_v<std::decay_t<T>, ConfigValue>>
		> //exclude this constr for ConfigValue type
		ConfigValue(T&& value) { //we use forward because we want to preserve original type 
		//so either call copy constr or move constr. r-value says "steal me sempaiii". l-value means "copy me"

			using Decayed = std::decay_t<T>;

			static_assert(
				std::is_same_v<Decayed, int> ||
				std::is_same_v<Decayed, double> ||
				std::is_same_v<Decayed, bool> ||
				std::is_same_v<Decayed, std::string> ||
				std::is_convertible_v<Decayed, std::string> ||
				std::is_same_v<Decayed, std::vector<ConfigValue>>,
				"ConfigValue only supports int, double, bool, std::string, std::vector<ConfigValue>"

				);
			if constexpr (std::is_convertible_v<Decayed, std::string>) {
				m_value = std::string(std::forward<T>(value));
			}
			else {
				m_value = std::forward<T>(value);
			}

		}

		template<typename T>
		T get() const {

			if (!isInitialized()) {
				throw std::runtime_error("Value is not initialized");
			}

			try {
				return std::get<T>(m_value); //trying to get T type from variant of current obj
			}
			catch (const std::bad_variant_access&) {
				throw std::runtime_error("Type mismatch");
			}
		}

		template<typename T>
		std::optional<T> tryGet() const noexcept {

			if (const T* ptr = std::get_if<T>(&m_value)) { //get_if for noexcept
				return *ptr;
			}
			return std::nullopt;

		}


		ValueType type() const noexcept {

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

		std::string toString() const {
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

		template<typename T>
		bool is() const noexcept {
			return std::holds_alternative<T>(m_value);
		}

		~ConfigValue() = default;

	private:

		bool isInitialized() const {
			return !(is<std::monostate>());
		}

		VariantType m_value;

		



	};

}








#endif
