#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

class Pool
{

private:
    std::queue<std::function<void()>> m_function_queue;
    std::mutex m_lock;
    std::condition_variable m_data_condition;
    std::atomic<bool> m_accept_functions;

public:

    Pool();
    ~Pool();
    void push(std::function<void()> func);
    void done();
    void infinite_loop_func();
};

Pool::Pool() 
    : m_function_queue(), m_lock(), m_data_condition(), m_accept_functions(true)
{}

Pool::~Pool() {}

void Pool::push(std::function<void()> func)
{
    std::unique_lock<std::mutex> lock(m_lock);
    m_function_queue.push(func);
    lock.unlock();
    m_data_condition.notify_one();
}

void Pool::done()
{
    std::unique_lock<std::mutex> lock(m_lock);
    m_accept_functions = false;
    lock.unlock();
    m_data_condition.notify_all();
}

void Pool::infinite_loop_func()
{
    std::function<void()> func;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_lock);
            m_data_condition.wait(lock, [this]() {return !m_function_queue.empty() || !m_accept_functions; });
            if (!m_accept_functions && m_function_queue.empty())
            {
                return;
            }
            func = m_function_queue.front();
            m_function_queue.pop();
        }
        func();
    }
}

Pool func_pool;

class quit_worker_exception : public std::exception {};

void example_function()
{
    int sum = 0;
    for(int i = 1; i <= 1000; ++i)
    {
        sum += i;
    }
    std::cout << sum << "\n";
}

int main()
{
    std::cout << "Stating operation" << std::endl;
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> thread_pool;
    for (int i = 0; i < num_threads; i++)
    {
        thread_pool.push_back(std::thread(&Pool::infinite_loop_func, &func_pool));
    }

    for (int i = 0; i < 3; i++)
    {
        func_pool.push(example_function);
    }
    func_pool.done();
    for (unsigned int i = 0; i < thread_pool.size(); i++)
    {
        thread_pool.at(i).join();
    }
}