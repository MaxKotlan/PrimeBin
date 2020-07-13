#pragma once
#include <chrono>

#define MAX_NUMBEROF_SPACES 30

class Event{
    public:
        Event(std::string message);
        void start(std::string message);
        void stop();
    private:
        std::string _message;
        std::chrono::high_resolution_clock::time_point _clock_start;
        std::chrono::high_resolution_clock::time_point _clock_stop;
};

Event::Event(std::string message) : _message(message){
    start(message);
}

void Event::start(std::string message){
    std::cout << message << "...";
    for (uint8_t i = 0; i < MAX_NUMBEROF_SPACES - message.length(); i++)
        std::cout << " ";
    std::cout << "\t" << std::flush;
    _clock_start = std::chrono::high_resolution_clock::now(); 
}

void Event::stop(){
    _clock_stop = std::chrono::high_resolution_clock::now();
    std::cout << "Done! " << std::chrono::duration_cast<std::chrono::milliseconds>(_clock_stop - _clock_start).count() << " milliseconds" << std::endl; 
}