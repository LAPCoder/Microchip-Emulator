/**
 * @brief Provide some memory functions, classes...
 * @author LAPCoder
 * @license MIT
 */

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <type_traits>
#include <functional>
#include <numeric>

typedef unsigned char byte;

template <int ARRAY_LEN, class T> class Array_RAM
{
	protected:
		T reg[ARRAY_LEN] = {};
	public:
		explicit Array_RAM()  {}
		~Array_RAM() {}
		template<
			typename Y, //real type
			typename = typename std::enable_if<std::is_arithmetic<Y>::value, Y>::type
		> T *operator[](Y &index)
		{
			if (ARRAY_LEN > index)
				return &this->reg[index];
			else return nullptr;
		}
		template<
			typename Y, //real type
			typename = typename std::enable_if<std::is_arithmetic<Y>::value, Y>::type
		> T *operator[](Y index)
		{
			if (ARRAY_LEN > index)
				return &this->reg[index];
			else return nullptr;
		}
		auto operator()()
		{
			return std::accumulate(
					std::begin(this->reg),
					std::end(this->reg),
					0,
					std::plus<T>());
		}
		void operator=(T *pointer)
		{
			if (ARRAY_LEN == 1)
				reg[0] = *pointer;
			else throw "Cannot copy that";
		}
		void operator=(T elem)
		{
			if (ARRAY_LEN == 1)
				reg[0] = elem;
			else throw "Cannot copy that";
		}
};

#endif // MEMORY_HPP
