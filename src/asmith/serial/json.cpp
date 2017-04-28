//	Copyright 2017 Adam Smith
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
// 
//	http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.

#include "asmith/serial/json.hpp"
	
namespace asmith { namespace serial {
	void json_format::write_serial(const value& aType, std::ostream& aStream) {
				const value::type tBuf = aType.get_type();
		switch(tBuf) {
		case value::NULL_T:
			aStream << "null";
			break;
		case value::BOOl_T:
			aStream << (aType.get_bool() ? "true" : "false");
			break;
		case value::CHAR_T:
			aStream << '"' << aType.get_char() << '"';
			break;
		case value::NUMBER_T:
			aStream << aType.get_number();
			break;
		case value::STRING_T:
			aStream << '"' << aType.get_string() << '"';
			break;
		case value::ARRAY_T:
			{
				aStream << '[';
				const value::array_t tmp = aType.get_array();
				const size_t s = tmp.size();
				for(size_t i = 0; i < s; ++i) {
					write_serial(tmp[i], aStream);
					if(i + 1 < s) aStream << ',';
				}
				aStream << ']';
			}
			break;
		case value::OBJECT_T:
		{
			aStream << '{';
			const value::object_t tmp = aType.get_object();
			const size_t s = tmp.size();
			size_t i = 0;
			for(const auto& v : tmp) {
				aStream << '"' << v.first<< '"' << ':';
				write_serial(v.second, aStream);
				if(i + 1 < s) aStream << ',';
				++i;
			}
			aStream << '}';
		}
		break;
		default:
			throw std::runtime_error("json_format : Invalid serial type");
			break;
		}
	}

	value json_format::read_serial(std::istream& aStream) {
		//! \todo Implement
		return value();
	}
}}