#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class ConfigKeyError : std::exception {
	std::string value;
public:
	ConfigKeyError(const std::string& val) : value { val } {}

	const char* what() const noexcept override {
		return "Unrecognized config key";
	}

	const std::string& get_value() {
		return value;
	}
};

class ConfigValueError : std::exception {
	std::string value;
public:
	ConfigValueError(const std::string& val) : value { val } {}

	const char* what() const noexcept override {
		return "Unrecognized config value";
	}

	const std::string& get_value() {
		return value;
	}
};

#endif
