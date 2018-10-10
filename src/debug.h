// debug.h
// some debugging tools to better understand
// the project

#ifdef DEBUG
#define DBG_SPLIT(miss)	miss_split((miss))

void miss_split(long long int miss);

#else
#define DBG_SPLIT(miss)

#endif