module;

#include <print>

export module tds;

export void print_hello(uint32_t num)
{
	std::println("hello {}", num);
}
