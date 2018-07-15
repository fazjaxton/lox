#include "error.h"

#include <iostream>

void report(unsigned line, std::string where, std::string message) {
	std::cerr << "[line " << line <<
		"] Error" << where << ": " << message << std::endl;
}

void error(unsigned line, std::string message) {
    report(line, "", message);
}

