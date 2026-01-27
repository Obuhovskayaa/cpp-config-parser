#ifndef CONFIG_VALUE_HPP
#define CONFIG_VALUE_HPP

#include <string>
#include <vector>
#include <optional>
#include <variant>

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

		ConfigValue(const ConfigValue&) = default;

		ConfigValue& operator=(const ConfigValue&) = default;

		ConfigValue(ConfigValue&&) noexcept = default;


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
		
		/*ConfigValue(int value);

		ConfigValue(double value);

		ConfigValue(bool value);

		ConfigValue(const std::string& value);

		ConfigValue(const std::vector<ConfigValue>& values);*/


		


		template<typename T>
		T get() const {

			try {
				return std::get<T>(m_value); //trying to get T type from variant of current obj
			}
			catch (const std::bad_variant_access&) {
				throw std::runtime_error("type mismatch for current obj");
			}
		}

		template<typename T>
		std::optional<T> tryGet() const noexcept {

			if (const T* ptr = std::get_if<T>(&m_value)) { //get_if for noexcept
				return *ptr;
			}
			return std::nullopt;

		}

		template<typename T>
		bool is() const noexcept {
			return std::holds_alternative<T>(m_value);
		}


		ValueType type() const noexcept;



		/*bool isNull() const;

		bool isInt() const;

		bool isDouble() const;

		bool isBool() const;

		bool isString() const;

		bool isVector() const;
		*/


		std::string toString() const;

		~ConfigValue() = default;

	private:
		//ValueType m_type;

		VariantType m_value;

		//void cleanUp();



	};

}








#endif
