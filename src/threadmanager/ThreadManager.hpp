#ifndef THREADMANAGER_HPP
#define THREADMANAGER_HPP

#include <functional>
#include <thread>
#include <vector>

namespace Types {
   struct TStreamData;
}

class ThreadManager final {
public:
   ThreadManager();
   ~ThreadManager();

   unsigned int getThreadCount() const;
   void process(const std::vector<std::function<void()>>&);

private:
   unsigned int threadCount = 0;

   std::vector<std::thread> threads;
};

#endif //THREADMANAGER_HPP
