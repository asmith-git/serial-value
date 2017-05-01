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

#ifndef ASMITH_SERIAL_VALUE_HPP
#define ASMITH_SERIAL_VALUE_HPP

#include <string>
#include <vector>
#include <map>
	
namespace asmith { namespace serial {

	class value {
	public:
		typedef void null_t;
		typedef bool bool_t;
		typedef char char_t;
		typedef double number_t;
		typedef std::string string_t;
		typedef std::vector<value> array_t;
		typedef std::map<std::string, value> object_t;
		
		enum type : uint8_t {
			NULL_T,
			BOOL_T,
			CHAR_T,
			NUMBER_T,
			STRING_T,
			ARRAY_T,
			OBJECT_T
		};
	private:
		union {
			bool_t mBool;
			char_t mChar;
			number_t mNumber;
			string_t* mString;
			array_t* mArray;
			object_t* mObject;
		};
		type mType;
	public:
		value() throw();
		value(const type) throw();
		value(const value&) throw();
		value(value&&) throw();
		value(bool) throw();
		value(char) throw();
		value(uint8_t) throw();
		value(uint16_t) throw();
		value(uint32_t) throw();
		value(uint64_t) throw();
		value(int8_t) throw();
		value(int16_t) throw();
		value(int32_t) throw();
		value(int64_t) throw();
		value(float) throw();
		value(double) throw();
		value(const char*) throw();
		value(const std::string&) throw();
		~value() throw();
		
		value& operator=(const value&) throw();
		value& operator=(value&&) throw();
		
		bool operator==(const value&) const throw();
		bool operator!=(const value&) const throw();

		value& operator[](const size_t);
		value& operator[](const std::string&);
		const value& operator[](const size_t) const;
		const value& operator[](const std::string&) const;

		size_t size() const throw();
		type get_type() const throw();
		
		void set_null() throw();
		bool_t& set_bool() throw();
		char_t& set_char() throw();
		number_t& set_number() throw();
		string_t& set_string() throw();
		array_t& set_array() throw();
		object_t& set_object() throw();
		
		bool_t& get_bool() throw();
		char_t& get_char() throw();
		number_t& get_number() throw();
		string_t& get_string() throw();
		array_t& get_array() throw();
		object_t& get_object() throw();
		
		bool_t get_bool() const;
		char_t get_char() const;
		number_t get_number() const;
		const string_t& get_string() const;
		const array_t& get_array() const;
		const object_t& get_object() const;
	};
}}

#endif