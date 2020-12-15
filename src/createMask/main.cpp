#include <iostream>
#include <bitset>

int main()
{
    constexpr size_t uint_bitsize = sizeof(unsigned int)*8;
    for(int shift = 0; shift < uint_bitsize; ++shift)
    {
        unsigned int bitmask = 0;

        bitmask |= (1<<shift);
        std::cout << std::bitset<uint_bitsize>(bitmask) << std::endl;
    }
}