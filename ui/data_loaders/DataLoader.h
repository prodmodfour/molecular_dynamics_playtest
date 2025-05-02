#pragma once

namespace ui
{
class DataLoader 
{
public:
    virtual ~DataLoader() = default;

    virtual bool load() = 0;
};
} // namespace ui