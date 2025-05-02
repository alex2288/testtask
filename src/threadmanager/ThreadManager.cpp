#include "ThreadManager.hpp"

#include <cassert>

#include "../utils/Consts.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Types.hpp"

ThreadManager::ThreadManager() {
    threadCount = getThreadCount();
    threads.reserve(threadCount);
}

ThreadManager::~ThreadManager() {

}

unsigned int ThreadManager::getThreadCount() const {
    if (threadCount)
        return threadCount;
    unsigned int hardwareCount = std::thread::hardware_concurrency();
    if (hardwareCount > 1)
        return std::max(hardwareCount - 1, consts::DEFAULT_THREAD_COUNT);
    return consts::DEFAULT_THREAD_COUNT;
}

void ThreadManager::process(const std::vector<std::function<void()>>& workers) {
    assert(workers.size() == threadCount);
    LOG("Creating " << workers.size() << " threads!");
    for(const auto& current : workers) {
        threads.emplace_back(std::thread{current});
    }
    for(auto& current : threads)
        current.join();
}
