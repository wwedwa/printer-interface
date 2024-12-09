#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include <string>
#include <map>

namespace printconfig {

std::string RemoveWhitespace(std::string str);

std::map<std::string, std::string> ReadProperties();

}
#endif  // CONFIG_READER_H_