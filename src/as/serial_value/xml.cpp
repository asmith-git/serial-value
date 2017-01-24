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

#include <cctype>
#include <sstream>
#include "as/serial_value/xml.hpp"

#define ASMITH_SERIAL_PTR "as::serial_value::pointer_t="

bool write_attribute(std::ostream& aStream, const std::string& aName, const as::serial_value& aValue) {
	//! \todo Remove chars illegal in XML
	switch (aValue.get_type()) {
	case as::serial_value::NULL_T:
		std::cout << aName << '=' << '"' << "null" << '"';
		return true;
	case as::serial_value::CHAR_T:
		std::cout << aName << '=' << '"' << aValue.get_char() << '"';
		return true;
	case as::serial_value::BOOL_T:
		std::cout << aName << '=' << '"' << (aValue.get_bool() ? "true" : "false") << '"';
		return true;
	case as::serial_value::UNSIGNED_T:
		std::cout << aName << '=' << '"' << aValue.get_unsigned() << '"';
		return true;
	case as::serial_value::SIGNED_T:
		std::cout << aName << '=' << '"' << aValue.get_signed() << '"';
		return true;
	case as::serial_value::FLOAT_T:
		std::cout << aName << '=' << '"' << aValue.get_float() << '"';
		return true;
	case as::serial_value::POINTER_T:
		std::cout << aName << '=' << '"' << ASMITH_SERIAL_PTR  << aValue.get_pointer() << '"';
		return true;
	case as::serial_value::STRING_T:
		std::cout << aName << '=' << '"' << aValue.get_string() << '"';
		return true;
	default:
		return false;
	}
}

void write_value_x(std::ostream& aStream, const as::serial_value& aValue) {
	//! \todo Implement
	switch (aValue.get_type()) {
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

namespace as {
	void serialise_xml(std::ostream& aStream, const serial_value& aValue) {
		write_value_x(aStream, aValue);
	}

	serial_value deserialise_xml(std::istream& aStream) {
		serial_value tmp;
		//! \todo Implement
		return tmp;
	}

}