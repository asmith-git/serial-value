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

#include "asmith/serial/binary.hpp"
	
namespace asmith { namespace serial {
	void binary_format::write_serial(const value& aType, std::ostream& aStream) {
		const value::type tBuf = aType.get_type();
		aStream.write(reinterpret_cast<const char*>(&tBuf), sizeof(tBuf));
	
		switch(tBuf) {
		case value::NULL_T:
			break;
		case value::BOOl_T:
			{
				const value::bool_t buf = aType.get_bool();
				aStream.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
			}
			break;
		case value::CHAR_T:
			{
				const value::char_t buf = aType.get_char();
				aStream.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
			}
			break;
		case value::NUMBER_T:
			{
				const value::number_t buf = aType.get_number();
				aStream.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
			}
			break;
		case value::STRING_T:
			{
			const value::string_t tmp = aType.get_string();
			const uint16_t buf = static_cast<uint16_t>(tmp.size());
				aStream.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
				aStream.write(tmp.c_str(), buf);
			}
			break;
		case value::ARRAY_T:
			{
				const value::array_t tmp = aType.get_array();
				const uint16_t buf = static_cast<uint16_t>(tmp.size());
				aStream.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
				for(uint16_t i = 0; i < buf; ++i) write_serial(tmp[i], aStream);
			}
			break;
		case value::OBJECT_T:
			{
				const value::object_t tmp = aType.get_object();
				const uint16_t buf = static_cast<uint16_t>(tmp.size());
				aStream.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
				for(const auto& i : tmp) {
					const uint16_t buf2 = static_cast<uint16_t>(i.first.size());
					aStream.write(reinterpret_cast<const char*>(&buf2), sizeof(buf2));
					aStream.write(i.first.c_str(), buf2);
					write_serial(i.second, aStream);
				}
			}
			break;
		default:
			throw std::runtime_error("binary_format : Invalid serial type");
			break;
		}
	}
	
	value binary_format::read_serial(std::istream& aStream) {
		value value;
		value::type tBuf;
		aStream.read(reinterpret_cast<char*>(&tBuf), sizeof(tBuf));
	
		switch(tBuf) {
		case value::NULL_T:
			value.set_null();
			break;
		case value::BOOl_T:
			{
				value::bool_t buf;
				aStream.read(reinterpret_cast<char*>(&buf), sizeof(buf));
				value.set_bool() = buf;
			}
			break;
		case value::CHAR_T:
			{
				value::char_t buf;
				aStream.read(reinterpret_cast<char*>(&buf), sizeof(buf));
				value.set_char() = buf;
			}
			break;
		case value::NUMBER_T:
			{
				value::number_t buf;
				aStream.read(reinterpret_cast<char*>(&buf), sizeof(buf));
				value.set_number() = buf;
			}
			break;
		case value::STRING_T:
			{
				value::string_t& tmp = value.set_string();
				uint16_t buf;
				aStream.read(reinterpret_cast<char*>(&buf), sizeof(buf));
				char c;
				for(size_t i = 0; i < buf; ++i) {
					aStream.read(&c, 1);
					tmp += c;
				}
			}
			break;
		case value::ARRAY_T:
			{
				value::array_t& tmp = value.set_array();
				uint16_t buf;
				aStream.read(reinterpret_cast<char*>(&buf), sizeof(buf));
				for(uint16_t i = 0; i < buf; ++i) tmp.push_back(read_serial(aStream));
			}
			break;
		case value::OBJECT_T:
			{
				value::object_t& tmp = value.set_object();
				uint16_t buf ;
				aStream.read(reinterpret_cast<char*>(&buf), sizeof(buf));
				for(uint16_t i = 0; i < buf; ++i) {
					std::string key;
					uint16_t kSize; 
					aStream.read(reinterpret_cast<char*>(&kSize), sizeof(kSize));
					char c;
					for(uint16_t j = 0; j < kSize; ++j) {
						aStream.read(&c, 1);
						key += c;
					}
					tmp.emplace(key, read_serial(aStream));
				}
			}
			break;
		default:
			throw std::runtime_error("binary_format : Invalid serial type");
			break;
		}
		return value;
	}
}}