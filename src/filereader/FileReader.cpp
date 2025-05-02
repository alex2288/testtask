#include "FileReader.hpp"

#include <cassert>

#include "../utils/Logger.hpp"
#include "../utils/Types.hpp"

FileReader::FileReader(const char* iFilename) {
   openFile(iFilename);
}

FileReader::~FileReader() {
   for(auto& current : streams)
      current.stream.close();
}

void FileReader::openFile(const char* input) {
   path = std::filesystem::path(input);
   LOG("Reading file:" << path);
   if(!std::filesystem::exists(path)) {
      LOG("File does not exist!");
      fileValid = false;
      return;
   }
   if(!std::filesystem::is_regular_file(path)) {
      LOG("File is not a regular file!");
      fileValid = false;
      return;
   }
   if(std::filesystem::is_empty(path)) {
      LOG("File is empty!");
      fileValid = false;
      return;
   }
   fileValid = true;
}

bool FileReader::isValid() const {
   return fileValid;
}

std::uintmax_t FileReader::getFileSize() const {
   return std::filesystem::file_size(path);
}

std::vector<Types::TStreamData>& FileReader::getStreams(const unsigned int count) {
   LOG("Creating " << count << " file streams!");
   streams.reserve(count);
   if(count == 1) {
      streams.emplace_back(std::ifstream{path, std::ios::in}, 0, getFileSize());
      return streams;
   }
   const std::uintmax_t chunkSize = getFileSize() / count;
   std::uintmax_t previousEndOffset = 0;
   for(int i = 0; i < count; ++i) {
      const auto currentStart = previousEndOffset;
      streams.emplace_back(std::ifstream{path, std::ios::in}, currentStart, previousEndOffset += chunkSize);
      assert(streams.back().stream.is_open() and streams.back().stream.good());
   }
   streams.back().endOffset = getFileSize();
   LOG("Successfully created " << streams.size() << "streams!");
   return streams;
}




