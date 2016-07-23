#ifndef JsonParser_H_
#define JsonParser_H_

#include <string>

#include "../DataTables.h"

data::TextureData parseJsonFile(const std::string& name, const std::string& extra_identifier);
#endif