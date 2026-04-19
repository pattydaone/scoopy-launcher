#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class ConfigError : std::exception {
	std::string value;
public:
	ConfigError(const std::string& val) : value { val } {}

	const char* what() const noexcept override {
		return "Unrecognized config key";
	}

	const std::string& get_value() {
		return value;
	}
};

#endif
