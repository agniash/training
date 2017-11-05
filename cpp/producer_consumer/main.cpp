#include <queue>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <cstdlib>

const size_t SLEEP_TIME = 1000;
const size_t MAX_QUEUE_SIZE = 5;
const size_t PRODUCERS_NUM = 10;
const size_t CONSUMERS_NUM = 2;

void SleepAwhile()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % SLEEP_TIME));
}

void Log(const std::string& message)
{
    static std::mutex mutex;
    std::lock_guard<decltype(mutex)> lock(mutex);
    std::cout << message << std::endl;
}

class TTask {
public:
    TTask()
        : ID(TTask::GenerateID())
    {}

    void Run()
    {
        // do something
    }

private:
    static size_t GenerateID()
    {
        static std::atomic<decltype(ID)> staticID{0};
        return ++staticID;
    }

private:
    size_t ID;
};

class TTaskQueue {
public:
    using Ptr = std::shared_ptr<TTaskQueue>;

    friend TTaskQueue::Ptr MakeTaskQueuePtr();

    void Push(TTask task)
    {
        std::unique_lock<decltype(Mutex)> lock(Mutex);
        NotFullCV.wait(lock, [this] {
            return !this->IsFull();
        });
        Queue.push(std::move(task));
        NotEmptyCV.notify_all();
    }

    TTask Pop()
    {
        std::unique_lock<decltype(Mutex)> lock(Mutex);
        NotEmptyCV.wait(lock, [this] {
            return !this->IsEmpty();
        });
        auto task = std::move(Queue.front());
        Queue.pop();
        NotFullCV.notify_all();
        return task;
    }

    bool IsEmpty() const
    {
        return Queue.empty();
    }

    bool IsFull() const
    {
        return Queue.size() >= MAX_QUEUE_SIZE;
    }

private:
    TTaskQueue() = default;

private:
    std::queue<TTask> Queue;
    std::mutex Mutex;
    std::condition_variable NotFullCV;
    std::condition_variable NotEmptyCV;
};

TTaskQueue::Ptr MakeTaskQueuePtr()
{
    return TTaskQueue::Ptr(new TTaskQueue());
}

template <class TDerived>
class TBase {
public:
    TBase(const TTaskQueue::Ptr& queue, size_t index)
        : Thread([queue, index] {
            TDerived::Run(queue, index);
        })
    {}

    TBase(TBase&&) = default;

    ~TBase()
    {
        if (Thread.joinable()) {
            Thread.join();
        }
    }

private:
    std::thread Thread;
};

class TProducer : public TBase<TProducer> {
public:
    using TBase<TProducer>::TBase;

private:
    friend class TBase<TProducer>;
    static void Run(const TTaskQueue::Ptr& queue, size_t index)
    {
        while (true) {
            SleepAwhile();
            queue->Push(TTask{});
            Log("producer: " + std::to_string(index));
        }
    }
};

class TConsumer : public TBase<TConsumer> {
public:
    using TBase<TConsumer>::TBase;

private:
    friend class TBase<TConsumer>;
    static void Run(const TTaskQueue::Ptr& queue, size_t index)
    {
        while (true) {
            SleepAwhile();
            auto task = queue->Pop();
            Log("consumer: " + std::to_string(index));
            task.Run();
        }
    }
};

int main(int argc, const char** argv)
{
    auto queuePtr = MakeTaskQueuePtr();
    
    std::vector<TProducer> producers;
    for (size_t i = 0; i < PRODUCERS_NUM; ++i) {
        producers.emplace_back(queuePtr, i + 1);
    }

    std::vector<TConsumer> consumers;
    for (size_t i = 0; i < CONSUMERS_NUM; ++i) {
        consumers.emplace_back(queuePtr, i + 1);
    }

    return 0;
}
