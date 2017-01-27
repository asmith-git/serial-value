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
#include "as/serial_value/json.hpp"

#define ASMITH_SERIAL_PTR "as::serial_value::pointer_t="

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
		aStream << '"' << ASMITH_SERIAL_PTR << aValue.get_pointer() << '"';
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

void ignore_whitespace(std::istream& aStream) {
	char c = aStream.peek();
	while(std::isspace(c)) {
		aStream.read(&c, 1);
		c = aStream.peek();
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
	//! \todo Handle control character '\'
	auto pos = aStream.tellg();
	char c;
	aStream >> c;
	if(c != '"') {
		aStream.seekg(pos);
		throw std::runtime_error("as::deserialise_json : Expected string to begin with '\"'");
	}
	as::serial_value tmp;
	as::serial_value::string_t& str = tmp.set_string();
	aStream.read(&c, 1);
	while(c != '"') {
		if(aStream.eof()) {
			aStream.seekg(pos);
			throw std::runtime_error("as::deserialise_json : Expected string to end with'\"'");
		}
		str += c;
		aStream.read(&c, 1);
	}
	// The string is a char
	if(str.size() == 1) return as::serial_value(str[0]);

	// If the string is a pointer
	if(str.find(ASMITH_SERIAL_PTR) == 0) {
		std::stringstream ss;
		ss << str.substr(strlen(ASMITH_SERIAL_PTR));
		as::serial_value::pointer_t ptr;
		ss >> ptr;
		return as::serial_value(ptr);
	}
	return tmp;
}

as::serial_value read_array(std::istream& aStream) {
	auto pos = aStream.tellg();
	char c;
	aStream >> c;
	if (c != '[') {
		aStream.seekg(pos);
		throw std::runtime_error("as::deserialise_json : Expected array to begin with '['");
	}
	as::serial_value tmp;
	as::serial_value::array_t& values = tmp.set_array();
	c = aStream.peek();
	while(c != ']') {
		ignore_whitespace(aStream);
		c = aStream.peek();
		if(c == ',') aStream >> c;

		try {
			values.push_back(read_unknown(aStream));
		}catch (std::exception& e) {
			aStream.seekg(pos);
			throw e;
		}

		if(aStream.eof()) {
			aStream.seekg(pos);
			throw std::runtime_error("as::deserialise_json : Expected array to end with ']'");
		}
		c = aStream.peek();
	}
	aStream >> c;
	return tmp;
}

as::serial_value read_object(std::istream& aStream) {
	auto pos = aStream.tellg();
	char c;
	aStream >> c;
	if (c != '{') {
		aStream.seekg(pos);
		throw std::runtime_error("as::deserialise_json : Expected object to begin with '{'");
	}
	as::serial_value tmp;
	as::serial_value::object_t& values = tmp.set_object();
	c = aStream.peek();
	while(c != '}') {
		ignore_whitespace(aStream);
		c = aStream.peek();
		if(c == ',') aStream >> c;

		try {
			as::serial_value::string_t name = read_string(aStream).get_string();
			ignore_whitespace(aStream);
			aStream >> c;
			if(c != ':') throw std::runtime_error("as::deserialise_json : Expected object members to be seperated with ':'");
			values.emplace(name, read_unknown(aStream));
		}catch (std::exception& e) {
			aStream.seekg(pos);
			throw e;
		}

		if(aStream.eof()) {
			aStream.seekg(pos);
			throw std::runtime_error("as::deserialise_json : Expected object to end with '}'");
		}
		c = aStream.peek();
	}
	aStream >> c;
	return tmp;
}

as::serial_value read_unknown(std::istream& aStream) {
	ignore_whitespace(aStream);

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
		//! \todo Fix bug with whitespace
		std::stringstream tmp;
		char c;
		while(!aStream.eof()) {
			aStream >> c;
			tmp << c;
		}
		return read_unknown(tmp);
	}

}