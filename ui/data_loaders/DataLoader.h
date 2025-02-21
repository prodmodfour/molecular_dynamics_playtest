#pragma once

class DataLoader 
{
public:
    // Virtual destructor for proper cleanup of derived classes
    virtual ~DataLoader() = default;

    // Pure virtual function: must be implemented by derived classes
    virtual void load() = 0;
};