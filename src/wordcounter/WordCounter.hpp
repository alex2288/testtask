#ifndef WORDCOUNTER_HPP
#define WORDCOUNTER_HPP

#include <mutex>
#include <unordered_set>
#include <string>

#include "../utils/Types.hpp"

class WordCounter final {
public:
   explicit WordCounter(Types::TStreamData&);
   ~WordCounter() = default;

   void process();
   static unsigned int getWordCount();

private:
   std::unordered_set<std::string> localWords;
   static std::unordered_set<std::string> globalWords;

   static std::mutex wordsMutex;

   std::ifstream& fileStream;
   const std::pair<std::uintmax_t, std::uintmax_t> offsets;

   void processBuffer(const std::vector<char>&, std::size_t);
   void mergeWords();
};

#endif //WORDCOUNTER_HPP
