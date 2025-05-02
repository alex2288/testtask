#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <filesystem>
#include <vector>

#include "../utils/Types.hpp"

class FileReader final {
public:
   explicit FileReader(const char*);
   ~FileReader();

   bool isValid() const;
   std::uintmax_t getFileSize() const;
   std::vector<Types::TStreamData>& getStreams(unsigned int);

private:
   void openFile(const char*);

   bool fileValid = false;
   std::filesystem::path path;
   std::vector<Types::TStreamData> streams;
};

#endif //FILEREADER_HPP
