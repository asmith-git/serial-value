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

#include "asmith/serial/ini.hpp"
	
namespace asmith { namespace serial {

	static void write_ini(const std::string& aParentName, const std::string& aName, const value& aValue, std::ostream& aStream) {
		switch(aValue.get_type()) {
		case value::NULL_T:
			aStream << aName << '=' << "null" << std::endl;
			break;
		case value::BOOL_T:
			aStream << aName << '=' << (aValue.get_bool() ? "true" : "false") << std::endl;
			break;
		case value::NUMBER_T:
			aStream << aName << '=' << aValue.get_number() << std::endl;
			break;
		case value::STRING_T:
			aStream << aName << '=' << aValue.get_string() << std::endl;
			break;
		case value::ARRAY_T:
			{
				const value::array_t& array_ = aValue.get_array();
				const std::string parentName = aParentName == "" ? aName : aParentName + "." + aName;
				if(parentName != "") aStream << '[' << parentName << ']' << std::endl;
				size_t j = 0;
				for(const auto& i : array_) {
					const value::type t = i.get_type();
					if(t != value::ARRAY_T && t != value::OBJECT_T) write_ini(parentName, std::to_string(j), i, aStream);
					++j;
				}
				j = 0;
				for(const auto& i : array_) {
					const value::type t = i.get_type();
					if(t == value::ARRAY_T || t == value::OBJECT_T) write_ini(parentName, std::to_string(j), i, aStream);
					++j;
				}
			}
			break;
		case value::OBJECT_T:
			{
				const value::object_t& object = aValue.get_object();
				const std::string parentName = aParentName == "" ? aName : aParentName + "." + aName;
				if(parentName != "") aStream << '[' << parentName << ']' << std::endl;
				for(const auto& i : object) {
					const value::type t = i.second.get_type();
					if(t != value::ARRAY_T && t != value::OBJECT_T) write_ini(parentName, i.first, i.second, aStream);
				}
				for(const auto& i : object) {
					const value::type t = i.second.get_type();
					if(t == value::ARRAY_T || t == value::OBJECT_T) write_ini(parentName, i.first, i.second, aStream);
				}
			}
			break;
		default:
			throw 0;
		}
	}

	void ini_format::write_serial(const value& aValue, std::ostream& aStream) {
		write_ini("", "", aValue, aStream);
	}
	
	value ini_format::read_serial(std::istream& aStream) {
		//! \todo Implement
		return value();
	}
}}