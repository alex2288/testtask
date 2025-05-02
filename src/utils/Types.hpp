#ifndef TYPES_HPP
#define TYPES_HPP

#include <fstream>

namespace Types {
   struct TStreamData final {
      std::ifstream stream;
      std::uintmax_t startOffset;
      std::uintmax_t endOffset;
   };
}

#endif //TYPES_HPP
