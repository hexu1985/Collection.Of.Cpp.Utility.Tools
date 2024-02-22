#pragma once

#include <memory>
#include <vector>
#include "limitedsize_queue.hpp"

template <typename T>
using Pipe = std::shared_ptr<limitedsize_queue<T>>;

template <typename T>
Pipe<T> make_pipe()
{
    return Pipe<T>(new limitedsize_queue<T>{});
}

template <typename T>
Pipe<T> make_pipe(size_t max_size)
{
    return Pipe<T>(new limitedsize_queue<T>(max_size));
}

class ProcessNode {
public:
    ProcessNode();
    virtual ~ProcessNode();

    virtual void start() = 0;
    virtual void stop() = 0;
};

template <typename T>
class DataSource: public ProcessNode {
public:
    DataSource(Pipe<T> pipe_): pipe(pipe_) {}
    ~DataSource() override {}

protected:
    void put(T value)
    {
        pipe->push(std::move(value));
    }

private:
    Pipe<T> pipe;
};

template <typename IT, typename OT>
class DataFilter: public ProcessNode {
public:
    DataFilter(Pipe<IT> in_pipe_, Pipe<OT> out_pipe_):
        in_pipe(in_pipe_), out_pipe(out_pipe_)
    {}

    ~DataFilter() override {}

protected:
    void get(IT& value)
    {
        in_pipe->pop(value);
    }

    void put(OT value)
    {
        out_pipe->push(std::move(value));
    }

private:
    Pipe<IT> in_pipe;
    Pipe<OT> out_pipe;
};

template <typename T>
class DataSink: public ProcessNode {
public:
    DataSink(Pipe<T> pipe_): pipe(pipe_) {}
    ~DataSink() override {}

protected:
    void get(T& value)
    {
        pipe->pop(value);
    }

private:
    Pipe<T> pipe;
};

class Pipeline {
public:
    Pipeline();
    virtual ~Pipeline();

    void start();
    void stop();

    void add_process_node(std::shared_ptr<ProcessNode> process_node);
    void clear();

protected:
    std::vector<std::shared_ptr<ProcessNode>> process_nodes;
};


