#pragma once

namespace ui
{
class DataLoader 
{
public:
    // Virtual destructor for proper cleanup of derived classes
    virtual ~DataLoader() = default;

    // Pure virtual function: must be implemented by derived classes
    virtual bool load() = 0;
};
} // namespace ui