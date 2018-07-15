#ifndef _ERROR_H
#define _ERROR_H

#include <string>

void report(unsigned line, std::string where, std::string message);
void error(unsigned line, std::string message);

#endif
