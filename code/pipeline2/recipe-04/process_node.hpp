#pragma once

#include <memory>
#include <vector>

class ProcessNode {
public:
    ProcessNode();
    virtual ~ProcessNode();

    virtual void start() = 0;
    virtual void stop() = 0;
};


