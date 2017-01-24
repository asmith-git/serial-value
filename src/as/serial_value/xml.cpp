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
#define ASMITH_SERIAL_XML "__as_serial_xml_element"

bool write_attribute(std::ostream& aStream, const std::string& aName, const as::serial_value& aValue) {
	//! \todo Remove chars illegal in XML
	switch (aValue.get_type()) {
	case as::serial_value::NULL_T:
		aStream << aName << '=' << '"' << "null" << '"';
		return true;
	case as::serial_value::CHAR_T:
		aStream << aName << '=' << '"' << aValue.get_char() << '"';
		return true;
	case as::serial_value::BOOL_T:
		aStream << aName << '=' << '"' << (aValue.get_bool() ? "true" : "false") << '"';
		return true;
	case as::serial_value::UNSIGNED_T:
		aStream << aName << '=' << '"' << aValue.get_unsigned() << '"';
		return true;
	case as::serial_value::SIGNED_T:
		aStream << aName << '=' << '"' << aValue.get_signed() << '"';
		return true;
	case as::serial_value::FLOAT_T:
		aStream << aName << '=' << '"' << aValue.get_float() << '"';
		return true;
	case as::serial_value::POINTER_T:
		aStream << aName << '=' << '"' << ASMITH_SERIAL_PTR  << aValue.get_pointer() << '"';
		return true;
	case as::serial_value::STRING_T:
		aStream << aName << '=' << '"' << aValue.get_string() << '"';
		return true;
	default:
		return false;
	}
}

void write_element(std::ostream& aStream, const std::string& aName, const as::serial_value& aValue) {
	//! \todo Remove chars illegal in XML
	switch (aValue.get_type()) {
	case as::serial_value::NULL_T:
		aStream << '<' << aName << '/' << '>';
		break;
	case as::serial_value::CHAR_T:
		aStream << '<' << aName << '>' << aValue.get_char() << '<' << '/' << aName << '>';
		break;
	case as::serial_value::BOOL_T:
		aStream << '<' << aName << '>' << (aValue.get_bool() ? "true" : "false") << '<' << '/' << aName << '>';
		break;
	case as::serial_value::UNSIGNED_T:
		aStream << '<' << aName << '>' << aValue.get_unsigned() << '<' << '/' << aName << '>';
		break;
	case as::serial_value::SIGNED_T:
		aStream << '<' << aName << '>' << aValue.get_signed() << '<' << '/' << aName << '>';
		break;
	case as::serial_value::FLOAT_T:
		aStream << '<' << aName << '>' << aValue.get_float() << '<' << '/' << aName << '>';
		break;
	case as::serial_value::POINTER_T:
		aStream << '<' << aName << '>' << ASMITH_SERIAL_PTR << aValue.get_pointer() << '<' << '/' << aName << '>';
		break;
	case as::serial_value::STRING_T:
		aStream << '<' << aName << '>' << aValue.get_string() << '<' << '/' << aName << '>';
		break;
	case as::serial_value::ARRAY_T:
		{
			aStream << '<' << aName << '>';
			const as::serial_value::array_t& array_ = aValue.get_array();
			const size_t s = array_.size();
			for(size_t i = 0; i < s; ++i) write_element(aStream, std::to_string(i), array_[i]);
			aStream << '<' << '/' << aName << '>';
		}
		break;
	case as::serial_value::OBJECT_T:
		{
			const as::serial_value::object_t& object = aValue.get_object();
			aStream << '<' << aName;
			size_t attributes = 0;
			for(const auto& i : object) {
				const as::serial_value::type t = i.second.get_type();
				if(t != as::serial_value::ARRAY_T && t != as::serial_value::OBJECT_T) {
					write_attribute(aStream, i.first, i.second);
					++attributes;
				}
			}

			if(attributes == object.size()) {
				aStream << '/' << '>';
			}else {
				aStream << '>';
				for(const auto& i : object) {
					const as::serial_value::type t = i.second.get_type();
					if(t == as::serial_value::ARRAY_T || t == as::serial_value::OBJECT_T) {
						write_element(aStream, i.first, i.second);
						++attributes;
					}
				}
				aStream << '<' << '/' << aName << '>';
			}
		}
		break;
	}
}

namespace as {
	void serialise_xml(std::ostream& aStream, const serial_value& aValue) {
		write_element(aStream, ASMITH_SERIAL_XML, aValue);
	}

	serial_value deserialise_xml(std::istream& aStream) {
		serial_value tmp;
		//! \todo Implement
		return tmp;
	}

}