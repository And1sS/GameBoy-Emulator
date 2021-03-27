#pragma once

#ifndef IN_RANGE
#define IN_RANGE(VAL, START, INCLUSIVE_END) (((VAL) >= (START)) && ((VAL) <= (INCLUSIVE_END)))
#endif // !IN_RANGE

#ifndef GET_BIT
#define GET_BIT(VAL, IDX) (((VAL) >> (IDX)) & 1)
#endif // !GET_BIT

using ull = unsigned long long;
ull operator "" _KB(ull val);
