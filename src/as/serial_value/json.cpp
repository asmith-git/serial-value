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

as::serial_value read_unknown(std::istream&);

as::serial_value read_null(std::istream& aStream) {
	const auto pos = aStream.tellg();
	char tmp[4];
	aStream.read(tmp, 4);
	if(memcmp(tmp, "null", 4) != 0) {
		aStream.seekg(pos);
		throw std::runtime_error("as::deserialise_json : Expected 'null'");
	}
	return as::serial_value();
}

as::serial_value read_bool(std::istream& aStream) {
	const auto pos = aStream.tellg();
	char tmp[4];
	aStream.read(tmp, 4);
	const bool t = memcmp(tmp, "true", 4) == 0;
	const bool f = memcmp(tmp, "fals", 4) == 0;
	if(t) {
		return as::serial_value(true);
	}else if(f) {
		aStream >> tmp[0];
		if(tmp[0] == 'e') return as::serial_value(false);
	}
	aStream.seekg(pos);
	throw std::runtime_error("as::deserialise_json : Expected 'true' or 'false'");
}

as::serial_value read_number(std::istream& aStream) {
	as::serial_value::float_t tmp;
	aStream >> tmp;
	return as::serial_value(tmp);
}

as::serial_value read_string(std::istream& aStream) {
	as::serial_value tmp;
	//! \todo Implement
	return tmp;
}

as::serial_value read_array(std::istream& aStream) {
	as::serial_value tmp;
	//! \todo Implement
	return tmp;
}

as::serial_value read_object(std::istream& aStream) {
	as::serial_value tmp;
	//! \todo Implement
	return tmp;
}

as::serial_value read_unknown(std::istream& aStream) {
	//! \todo Ignore whitespace
	switch(aStream.peek())
	{
	case 'n':
		return read_null(aStream);
	case 't':
	case 'f':
		return read_bool(aStream);
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return read_number(aStream);
	case '"':
		return read_string(aStream);
	case '[':
		return read_array(aStream);
	case '{':
		return read_object(aStream);
	default:
		throw std::runtime_error("as::deserialise_json : Could not determine type of JSON value");
		break;
	}
}

namespace as {
	void serialise_json(std::ostream& aStream, const serial_value& aValue) {
		write_value(aStream, aValue);
	}

	serial_value deserialise_json(std::istream& aStream) {
		return read_unknown(aStream);
	}

}