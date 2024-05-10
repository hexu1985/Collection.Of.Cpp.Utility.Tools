#pragma once

class Visitor;

class Element {
public:
    virtual ~Element();

    virtual void accept(Visitor* v) = 0;
};
