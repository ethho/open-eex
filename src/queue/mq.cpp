#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <functional>
#include <thread>

class Event {
public:
    virtual ~Event() {}
    virtual void handle() = 0;
};

class MessageBroker {
public:
    MessageBroker(size_t numThreads) : running_(true) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads_.emplace_back(&MessageBroker::worker, this);
        }
    }

    ~MessageBroker() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            running_ = false;
            condition_.notify_all();
        }
        for (std::jthread& thread : threads_) {
            thread.join();
        }
    }

    void addEvent(std::unique_ptr<Event> event) {
        std::unique_lock<std::mutex> lock(mutex_);
        events_.emplace(std::move(event));
        condition_.notify_one();
    }

private:
    void worker() {
        while (running_) {
            std::unique_ptr<Event> event;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                while (events_.empty() && running_) {
                    condition_.wait(lock);
                }
                if (!events_.empty()) {
                    event = std::move(events_.front());
                    events_.pop();
                }
            }
            if (event) {
                event->handle();
            }
        }
    }

    std::vector<std::jthread> threads_;
    std::queue<std::unique_ptr<Event>> events_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool running_;
};

class MyEvent : public Event {
public:
    void handle() override {
        // Handle the event...
        std::cout << "Handling event..." << std::endl;
    }
};

/*
int main() {
    MessageBroker broker(4);

    // Trigger the event...
    std::unique_ptr<MyEvent> event = std::make_unique<MyEvent>();
    broker.addEvent(std::move(event));

    // Wait for the event to be handled...
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
*/