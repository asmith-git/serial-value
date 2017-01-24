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

#include "as/serial_value/json.hpp"

void write_value(std::ostream& aStream, const as::serial_value& aValue) {
	switch (aValue.get_type()) {
	case as::serial_value::NULL_T:
		aStream << "null";
		break;
	case as::serial_value::CHAR_T:
		aStream << '"' << aValue.get_char() << "'";
		break;
	case as::serial_value::BOOL_T:
		aStream << aValue.get_bool() ? "true" : "false";
		break;
	case as::serial_value::UNSIGNED_T:
		aStream << aValue.get_unsigned();
		break;
	case as::serial_value::SIGNED_T:
		aStream << aValue.get_signed();
		break;
	case as::serial_value::FLOAT_T:
		aStream << aValue.get_float();
		break;
	case as::serial_value::POINTER_T:
		//! \todo Implement
		aStream << "null";
		break;
	case as::serial_value::STRING_T:
		aStream << '"' << aValue.get_string() << '"';
		break;
	case as::serial_value::ARRAY_T:
		{
			aStream << '[';
			const as::serial_value::array_t& tmp = aValue.get_array();
			const size_t s = tmp.size();
			for(size_t i = 0; i < s; ++i) {
				write_value(aStream, tmp[i]);
				if(i + 1 != s) aStream << ',';
			}
			aStream << ']';
		}
		break;
	case as::serial_value::OBJECT_T:
		{
			aStream << '{';
			const as::serial_value::object_t& tmp = aValue.get_object();
			const auto end = tmp.end();
			for(auto i = tmp.begin(); i != end; ++i) {
				aStream << '"' << i->first << '"' << ':';
				write_value(aStream, i->second);
				auto j = i;
				++j;
				if(j != end) aStream << ',';
			}
			aStream << '}';
		}
		break;
	}
}

namespace as {
	void serialise_json(std::ostream& aStream, const serial_value& aValue) {
		write_value(aStream, aValue);
	}

	serial_value deserialise_json(std::istream& aStream) {
		serial_value tmp;
		//! \todo Implement
		return tmp;
	}

}