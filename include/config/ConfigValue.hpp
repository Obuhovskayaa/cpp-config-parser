#ifndef CONFIG_VALUE
#define CONFIG_VALUE

#include <string>
#include <vector>

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

		ConfigValue();

		ConfigValue(int value);

		ConfigValue(double value);

		ConfigValue(bool value);

		ConfigValue(const std::string& value);

		ConfigValue(const std::vector<ConfigValue>& values);

		ConfigValue(const ConfigValue& other);

		ConfigValue& operator=(const ConfigValue& other);



		ValueType type() const;

		bool isNull() const;

		bool isInt() const;

		bool isDouble() const;

		bool isBool() const;

		bool isString() const;

		bool isVector() const;


		int asInt() const;

		double asDouble() const;

		bool asBool() const;

		const std::string& asString() const;

		const std::vector<ConfigValue>& asVector() const;

		std::string toString() const;

		~ConfigValue();

	private:
		ValueType m_type;

		union {
			int m_intValue;
			double m_doubleValue;
			bool m_boolValue;
			std::string m_stringValue;
			std::vector<ConfigValue>* m_vectorValue;
		};

		void cleanUp();



	};

}








#endif
