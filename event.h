#pragma once
#include <chrono>

class Event{
    public:
        Event(std::string message);
        void start(std::string message);
        void stop();
    private:
        static uint32_t max_message_length;
        std::string _message;
        std::chrono::high_resolution_clock::time_point _clock_start;
        std::chrono::high_resolution_clock::time_point _clock_stop;
};

uint32_t Event::max_message_length = 30;

Event::Event(std::string message) : _message(message){
    start(message);
}

void Event::start(std::string message){
    if (message.length() > max_message_length)
        max_message_length = message.length();
    std::cout << message << "...";
    for (uint32_t i = 0; i < max_message_length - message.length(); i++)
        std::cout << " ";
    std::cout << "\t" << std::flush;
    _clock_start = std::chrono::high_resolution_clock::now(); 
}

void Event::stop(){
    _clock_stop = std::chrono::high_resolution_clock::now();
    std::cout << "Done! " << std::chrono::duration_cast<std::chrono::milliseconds>(_clock_stop - _clock_start).count() << " milliseconds" << std::endl; 
}