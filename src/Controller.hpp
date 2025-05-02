#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vector>
#include <memory>

class FileReader;
class WordCounter;
class ThreadManager;

class Controller final {
public:
   explicit Controller(const char*);
   ~Controller();

   void process();

private:
   std::unique_ptr<FileReader> reader;
   std::vector<std::unique_ptr<WordCounter>> counters;
   std::unique_ptr<ThreadManager> threadManager;
};

#endif //CONTROLLER_HPP
