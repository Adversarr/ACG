#pragma once
#ifndef NDEBUG
#include <iostream>

#define SHOW_THIS(e) std::cout<< __FILE__ ": " << __LINE__  << ": " #e << (e) << std::endl
#endif