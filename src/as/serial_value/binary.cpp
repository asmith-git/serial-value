// Copyright 2017 Adam Smith
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "as/serial_value/binary.hpp"

void write_value_b(std::ostream& aStream, const as::serial_value& aValue) {
	const uint8_t type = aValue.get_type();
	aStream.write(reinterpret_cast<const char*>(&type), 1);

	switch(aValue.get_type()) {
	case as::serial_value::NULL_T:
		break;
	case as::serial_value::CHAR_T:
		{
			const as::serial_value::char_t value = aValue.get_char();
			aStream.write(&value, sizeof(as::serial_value::char_t));
		}
		break;
	case as::serial_value::BOOL_T:
		{
			const as::serial_value::bool_t value = aValue.get_bool();
			aStream.write(reinterpret_cast<const char*>(&value), sizeof(as::serial_value::bool_t));
		}
		break;
	case as::serial_value::UNSIGNED_T:
		{
			const as::serial_value::unsigned_t value = aValue.get_unsigned();
			aStream.write(reinterpret_cast<const char*>(&value), sizeof(as::serial_value::unsigned_t));
		}
		break;
	case as::serial_value::SIGNED_T:
		{
			const as::serial_value::signed_t value = aValue.get_signed();
			aStream.write(reinterpret_cast<const char*>(&value), sizeof(as::serial_value::signed_t));
		}
		break;
	case as::serial_value::FLOAT_T:
		{
			const as::serial_value::float_t value = aValue.get_float();
			aStream.write(reinterpret_cast<const char*>(&value), sizeof(as::serial_value::float_t));
		}
		break;
	case as::serial_value::POINTER_T:
		{
			const as::serial_value::pointer_t value = aValue.get_pointer();
			aStream.write(reinterpret_cast<const char*>(&value), sizeof(as::serial_value::pointer_t));
		}
		break;
	case as::serial_value::STRING_T:
		{
			const as::serial_value::string_t& value = aValue.get_string();
			const uint16_t size = static_cast<uint16_t>(value.size());
			aStream.write(reinterpret_cast<const char*>(&size), 2);
			aStream.write(value.c_str(), size);
		}
		break;
	case as::serial_value::ARRAY_T:
		{
			const as::serial_value::array_t& value = aValue.get_array();
			const uint16_t size = static_cast<uint16_t>(value.size());
			aStream.write(reinterpret_cast<const char*>(&size), 2);
			for(size_t i = 0; i < size; ++i) write_value_b(aStream, value[i]);
		}
		break;
	case as::serial_value::OBJECT_T:
		{
			const as::serial_value::object_t& value = aValue.get_object();
			uint16_t size = static_cast<uint16_t>(value.size());
			aStream.write(reinterpret_cast<const char*>(&size), 2);
			const auto end = value.end();
			for(auto i = value.begin(); i != end; ++i) {
				size = static_cast<uint16_t>(i->first.size());
				aStream.write(reinterpret_cast<const char*>(&size), 2);
				aStream.write(i->first.c_str(), size);
				write_value_b(aStream, i->second);
			}
		}
		break;
	}
}

as::serial_value read_unknown_b(std::istream&);

as::serial_value read_null_b(std::istream& aStream) {
	return as::serial_value();
}

as::serial_value read_bool_b(std::istream& aStream) {
	as::serial_value::bool_t value;
	aStream.read(reinterpret_cast<char*>(&value), sizeof(as::serial_value::bool_t));
	return as::serial_value(value);
}

as::serial_value read_unsigned_b(std::istream& aStream) {
	as::serial_value::unsigned_t value;
	aStream.read(reinterpret_cast<char*>(&value), sizeof(as::serial_value::unsigned_t));
	return as::serial_value(value);
}

as::serial_value read_signed_b(std::istream& aStream) {
	as::serial_value::signed_t value;
	aStream.read(reinterpret_cast<char*>(&value), sizeof(as::serial_value::signed_t));
	return as::serial_value(value);
}

as::serial_value read_float_b(std::istream& aStream) {
	as::serial_value::float_t value;
	aStream.read(reinterpret_cast<char*>(&value), sizeof(as::serial_value::float_t));
	return as::serial_value(value);
}

as::serial_value read_string_b(std::istream& aStream) {
	uint16_t size;
	aStream.read(reinterpret_cast<char*>(&size), 2);
	as::serial_value value;
	as::serial_value::string_t& str = value.set_string();
	for(uint16_t i = 0; i < size; ++i) str += ' ';
	aStream.read(const_cast<char*>(str.c_str()), size);
	return value;
}

as::serial_value read_array_b(std::istream& aStream) {
	//! \todo Implement
	return as::serial_value();
}

as::serial_value read_object_b(std::istream& aStream) {
	//! \todo Implement
	return as::serial_value();
}

as::serial_value read_unknown_b(std::istream& aStream) {
	//! \todo Implement
	return as::serial_value();
}

namespace as {
	void serialise_binary(std::ostream& aStream, const serial_value& aValue) {
		write_value_b(aStream, aValue);
	}

	serial_value deserialise_binary(std::istream& aStream) {
		return read_unknown_b(aStream);
	}

}