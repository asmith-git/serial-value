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
	//!< Todo Implement
	switch(aValue.get_type()) {
	case as::serial_value::NULL_T:
		break;
	case as::serial_value::CHAR_T:
		break;
	case as::serial_value::BOOL_T:
		break;
	case as::serial_value::UNSIGNED_T:
		break;
	case as::serial_value::SIGNED_T:
		break;
	case as::serial_value::FLOAT_T:
		break;
	case as::serial_value::POINTER_T:
		break;
	case as::serial_value::STRING_T:
		break;
	case as::serial_value::ARRAY_T:
		break;
	case as::serial_value::OBJECT_T:
		break;
	}
}

as::serial_value read_unknown_b(std::istream&);

as::serial_value read_null_b(std::istream& aStream) {
	//! \todo Implement
	return as::serial_value();
}

as::serial_value read_bool_b(std::istream& aStream) {
	//! \todo Implement
	return as::serial_value();
}

as::serial_value read_number_b(std::istream& aStream) {
	//! \todo Implement
	return as::serial_value();
}

as::serial_value read_string_b(std::istream& aStream) {
	//! \todo Implement
	return as::serial_value();
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