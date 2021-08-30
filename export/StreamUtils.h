#pragma once

// Fixed width integer types are defined in the C++11 header <cstdint>
#include <cstdint>

#include <ostream>
#include <algorithm>

namespace stream
{

/**
 * greebo: Writes the given number value to the given output stream in Big Endian byte order,
 * regardless of the calling platform's endianness.
 */
template<typename ValueType>
void writeBigEndian(std::ostream& stream, ValueType value)
{
	ValueType output = value;

#ifndef __BIG_ENDIAN__
	std::reverse(reinterpret_cast<char*>(&output), reinterpret_cast<char*>(&output) + sizeof(ValueType));
#endif

	stream.write(reinterpret_cast<const char*>(&output), sizeof(ValueType));
}

/**
* greebo: Writes the given number value to the given output stream in Little Endian byte order,
* regardless of the calling platform's endianness.
*/
template<typename ValueType>
void writeLittleEndian(std::ostream& stream, ValueType value)
{
	ValueType output = value;

#ifdef __BIG_ENDIAN__
	std::reverse(reinterpret_cast<char*>(&output), reinterpret_cast<char*>(&output) + sizeof(ValueType));
#endif

	stream.write(reinterpret_cast<const char*>(&output), sizeof(ValueType));
}

}
