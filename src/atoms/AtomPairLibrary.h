#pragma once
#include <string>
#include <vector>
#include "AtomPairData.h"
#include <stdexcept>
#include "../debug/logging.h"

namespace atoms {


class AtomPairLibrary
{
    public:
        AtomPairLibrary();

        AtomPairData get_atom_pair_data(std::string atom_type_1, std::string atom_type_2);
        void add_atom_pair_data(AtomPairData atom_pair_data);
        
        private:
            std::vector<AtomPairData> atom_pair_data;
};

}   