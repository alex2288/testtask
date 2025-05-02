#include "Controller.hpp"

#include <cassert>

#include "filereader/FileReader.hpp"
#include "threadmanager/ThreadManager.hpp"
#include "wordcounter/WordCounter.hpp"
#include "utils/Logger.hpp"
#include "utils/Consts.hpp"

#include <functional>

Controller::Controller(const char* filename) :
   reader(std::make_unique<FileReader>(filename)) {
}

Controller::~Controller() {}

void Controller::process() {
   assert(reader->isValid());
   const auto start = std::chrono::system_clock::now();
   if(reader->getFileSize() < consts::FILESIZE_THRESHOLD) {
      LOG("File size is less than " << consts::FILESIZE_THRESHOLD << " - processing in a single thread!");
      counters.emplace_back(std::make_unique<WordCounter>(reader->getStreams(1).front()));
      counters.front()->process();
   }
   else {
      LOG("File size is more than " << consts::FILESIZE_THRESHOLD << " - processing in multiple threads!");
      threadManager = std::make_unique<ThreadManager>();
      LOG("Thread count: " << threadManager->getThreadCount());
      auto& streams = reader->getStreams(threadManager->getThreadCount());
      LOG("Streams count: " << streams.size());
      std::vector<std::function<void()>> workers;
      for(auto& current : streams) {
         LOG("Current stream: " << current.startOffset << " - " << current.endOffset);
         counters.emplace_back(std::make_unique<WordCounter>(current));
         workers.emplace_back([counter = counters.back().get()](){ counter->process(); });
      }
      threadManager->process(workers);
      LOG("Result of processing: " << WordCounter::getWordCount() << " unique words!");
   }
   const auto end = std::chrono::system_clock::now();
   const auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
   LOG("Processing time: " << duration.count() << " s");
}