#include <stack>

template <typename T>
class Queue
{
public:
    void Enqueue(T& x);
    void Enqueue(T&& x);
    T Dequeue();
private:
    std::stack<T> s_a_;
    std::stack<T> s_b_;
};

template <typename T>
void Queue<T>::Enqueue(T& x)
{
    s_a_.push(x);
}

template <typename T>
void Queue<T>::Enqueue(T&& x)
{
    s_a_.emplace(std::move(x));
}

template <typename T>
T Queue<T>::Dequeue()
{
    if (s_b_.empty())
    {
        while (s_a_.empty() == false)
        {
            s_b_.emplace(std::move(s_a_.top()));
            s_a_.pop();
        }
    }
    T top = std::move(s_b_.top());
    s_b_.pop();
    return std::move(top);
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("int")
{
    int i, j, h;
    std::cout << "-------------------\n";
    clock_t start = clock();
    Queue<int> q;
    q.Enqueue(1);
    q.Enqueue(2);
    q.Enqueue(3);
    REQUIRE(q.Dequeue() == 1);
    REQUIRE(q.Dequeue() == 2);
    REQUIRE(q.Dequeue() == 3);
    q.Enqueue(4);
    q.Enqueue(5);
    q.Enqueue(6);
    REQUIRE(q.Dequeue() == 4);
    REQUIRE(q.Dequeue() == 5);
    q.Enqueue(7);
    q.Enqueue(8);
    REQUIRE(q.Dequeue() == 6);
    REQUIRE(q.Dequeue() == 7);
    REQUIRE(q.Dequeue() == 8);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

#include <string>

TEST_CASE("string")
{
    int i, j, h;
    std::cout << "-------------------\n";
    clock_t start = clock();
    Queue<std::string> q;
    q.Enqueue(std::string("1"));
    q.Enqueue("2");
    std::string three = "3";
    q.Enqueue(three);
    REQUIRE(three == "3");
    REQUIRE(q.Dequeue() == "1");
    REQUIRE(q.Dequeue() == "2");
    REQUIRE(q.Dequeue() == "3");
    std::string four = "4";
    q.Enqueue(std::move(four));
    REQUIRE(four == "");
    q.Enqueue("5");
    q.Enqueue("6");
    REQUIRE(q.Dequeue() == "4");
    REQUIRE(q.Dequeue() == "5");
    q.Enqueue("7");
    q.Enqueue("8");
    REQUIRE(q.Dequeue() == "6");
    REQUIRE(q.Dequeue() == "7");
    REQUIRE(q.Dequeue() == "8");
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}
