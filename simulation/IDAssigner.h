#pragma once

class IDAssigner
{
    public:
        IDAssigner()
        {
        };

        int highest_id = 0;

        int get_new_id()
        {
            highest_id++;
            return highest_id - 1;
        }
};
