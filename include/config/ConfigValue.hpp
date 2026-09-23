#ifndef CONFIG_VALUE_HPP
#define CONFIG_VALUE_HPP

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <sstream>

namespace config {

	class ConfigNode; 
	enum class ValueType {
		Null,
		Int,
		Double,
		Bool,
		String,
		Vector,
		Node
	};

	class ConfigValue {
	public:
		ConfigValue() : m_value(std::monostate{}) {}

		template<typename T, typename =
		std::enable_if_t< ! std::is_same_v<std::decay_t<T>, ConfigValue>>>
		
		ConfigValue(T&& value) { 
			using Decayed = std::decay_t<T>;

			static_assert(
				std::is_same_v<Decayed, int> ||
				std::is_same_v<Decayed, double> ||
				std::is_same_v<Decayed, bool> ||
				std::is_same_v<Decayed, std::string> ||
				std::is_convertible_v<Decayed, std::string> ||
				std::is_same_v<Decayed, std::shared_ptr<ConfigNode>> ||
				std::is_same_v<Decayed, ConfigNode*> ||
				std::is_same_v<Decayed, std::vector<ConfigValue>>,
				"ConfigValue only supports int, double, bool, std::string, std::vector<ConfigValue>, ConfigNode*, std::shared_ptr<ConfigNode>"
				);
			if constexpr (std::is_convertible_v<Decayed, std::string>) {
				m_value.emplace<std::string>(std::forward<T>(value));
			}
			else if constexpr (std::is_same_v<Decayed, ConfigNode*>) {
				if (value) {
					m_value = std::make_shared<ConfigNode>(value);
				}
				else {
					m_value = nullptr;
				}
			}
			else if constexpr (std::is_same_v <Decayed, std::shared_ptr<ConfigNode>>) {
				m_value = std::forward<T>(value);
			}
			else {
				m_value.emplace<Decayed>(std::forward<T>(value));
			}
		}

		template<typename T>
		T get() const {
			if (!isInitialized()) {
				throw std::runtime_error("Value is not initialized");
			}
			try {
				return std::get<T>(m_value);
			}
			catch (const std::bad_variant_access&) {
				throw std::runtime_error("Type mismatch");
			}
		}

		template<typename T>
		std::optional<T> tryGet() const noexcept {
			if (const T* ptr = std::get_if<T>(&m_value)) {
				return *ptr;
			}
			return std::nullopt;
		}

		ValueType type() const noexcept;
		std::string toString(const std::string& tabs, bool forVec) const noexcept;

		template<typename T>
		bool is() const noexcept {
			return std::holds_alternative<T>(m_value);
		}

		~ConfigValue() = default;

	private:
		bool isInitialized() const noexcept {
			return !(is<std::monostate>());
		}
		using VariantType = std::variant<
			std::monostate,
			int,
			double,
			bool,
			std::string,
			std::vector<ConfigValue>, 
			std::shared_ptr<ConfigNode>
		>;
		VariantType m_value;
	};
}

#endif
