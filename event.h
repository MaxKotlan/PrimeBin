#pragma once
#include <chrono>

class Event{
    public:
        Event(std::string message);
        Event(std::string message, bool displayoncomplete);
        void DisplayOnComplete();
        void start(std::string message);
        void stop();
        void PreMessage();
        void PostMessage();
    private:
        static bool displayevents;
        static uint32_t max_message_length;
        bool _displayoncomplete;
        std::string _message;
        std::chrono::high_resolution_clock::time_point _clock_start;
        std::chrono::high_resolution_clock::time_point _clock_stop;
};

bool     Event::displayevents = true;
uint32_t Event::max_message_length = 30;

Event::Event(std::string message) : _message(message), _displayoncomplete(false){
    start(message);
}

Event::Event(std::string message, bool displayoncomplete) : _message(message), _displayoncomplete(displayoncomplete){
    start(message);
}

void Event::DisplayOnComplete(){
    _displayoncomplete = true;
}

void Event::PreMessage(){
    if (displayevents){
        if (_message.length() > max_message_length)
            max_message_length = _message.length();
        std::cout << _message << "...";
        for (uint32_t i = 0; i < max_message_length - _message.length(); i++)
            std::cout << " ";
        std::cout << "\t" << std::flush;
    }   
}

void Event::PostMessage(){
    if (displayevents)
        std::cout << "Done! " << std::chrono::duration_cast<std::chrono::milliseconds>(_clock_stop - _clock_start).count() << " milliseconds" << std::endl;
}

void Event::start(std::string message){
    if (!_displayoncomplete)
        PreMessage();
    _clock_start = std::chrono::high_resolution_clock::now(); 
}

void Event::stop(){
    _clock_stop = std::chrono::high_resolution_clock::now();
    if (_displayoncomplete)
        PreMessage();
    PostMessage();
}