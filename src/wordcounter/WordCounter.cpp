#include "WordCounter.hpp"

#include <cassert>

#include "../utils/Logger.hpp"
#include "../utils/Consts.hpp"

#include <vector>

std::unordered_set<std::string> WordCounter::globalWords;
std::mutex WordCounter::wordsMutex;

WordCounter::WordCounter(Types::TStreamData& input) :
   fileStream(input.stream)
 , offsets(std::make_pair(input.startOffset, input.endOffset)) {
   LOG("Creating word counter instance!");
}

void WordCounter::processBuffer(const std::vector<char>& buffer, const std::size_t bufferSize) {
    assert(bufferSize > 0 and buffer.size() >= bufferSize);
    bool frontProcessed = false;
    std::string currentWord;
    for (std::size_t i = 0; i < bufferSize; ++i) {
        const char symbol = buffer[i];
        if (std::isalpha(symbol) or symbol == '-' or symbol == '\'')
            currentWord += symbol;
        else if (!currentWord.empty()) {
            if(!frontProcessed) {
                currentWord.clear();
                // Covering the case when the first word could be partial
                frontProcessed = true;
                continue;
            }
            localWords.emplace(std::move(currentWord));
            currentWord.clear();
        }
        // If the buffer is not empty upon exiting the loop, the rest will be handled in the next buffer
    }
}

void WordCounter::process() {
    assert(fileStream.is_open() && fileStream.good());

    // Adding some space to prevent partial words reading
    std::uintmax_t currentPosition = offsets.first;
    if(offsets.first and offsets.first > consts::MAX_WORD_LENGTH)
        currentPosition = offsets.first - consts::MAX_WORD_LENGTH;

    LOG("Processing file chunk: " << currentPosition << " - " << offsets.second);
    fileStream.seekg(currentPosition, std::ios_base::beg);
    std::vector<char> buffer(consts::BUFFER_SIZE);

    while (currentPosition < offsets.second) {
        const std::size_t readingChunk = std::min<std::size_t>(consts::BUFFER_SIZE, offsets.second - currentPosition);
        fileStream.read(buffer.data(), readingChunk);
        const std::size_t bytesRead = fileStream.gcount();
        assert(fileStream.good());
        processBuffer(buffer, bytesRead);
        // Moving the current position to overlap with the previous buffer
        currentPosition += ((bytesRead > consts::MAX_WORD_LENGTH) ? (bytesRead - consts::MAX_WORD_LENGTH) : bytesRead);
        fileStream.seekg(currentPosition, std::ios_base::beg);
    }

    LOG("Current thread finished processing. Merging local " << localWords.size() << " unique words!");
    mergeWords();
}

void WordCounter::mergeWords() {
    std::unique_lock lock(wordsMutex);
    globalWords.merge(localWords);
    localWords.clear();
}

unsigned int WordCounter::getWordCount() {
    std::unique_lock lock(wordsMutex);
    return globalWords.size();
}