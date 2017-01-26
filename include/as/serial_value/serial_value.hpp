#ifndef ASMITH_SERIAL_VALUE_HPP
#define ASMITH_SERIAL_VALUE_HPP

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

#include <vector>
#include <map>
#include <string>
#include <cstdint>

namespace as {

	class serial_value {
	public:
		typedef void null_t;
		typedef char char_t;
		typedef bool bool_t;
		typedef uint64_t unsigned_t;
		typedef int64_t signed_t;
		typedef double float_t;
		typedef void* pointer_t;
		typedef std::string string_t;
		typedef std::vector<serial_value> array_t;
		typedef std::map<string_t, serial_value> object_t;
	
		enum type : uint8_t {
			NULL_T,
			CHAR_T,
			BOOL_T,
			UNSIGNED_T,
			SIGNED_T,
			FLOAT_T,
			POINTER_T,
			STRING_T,
			ARRAY_T,
			OBJECT_T
		};
	private:
		union {
			char_t mChar;
			bool_t mBool;
			unsigned_t mUnsigned;
			signed_t mSigned;
			float_t mFloat;
			pointer_t mPointer;
			string_t* mString;
			array_t* mArray;
			object_t* mObject;
		};
		type mType;
	public:
		serial_value() throw();
		serial_value(serial_value&&) throw();
		serial_value(const serial_value&) throw();
		serial_value(const type) throw();
		serial_value(const char_t) throw();
		serial_value(const bool_t) throw();
		serial_value(const uint8_t) throw();
		serial_value(const uint16_t) throw();
		serial_value(const uint32_t) throw();
		serial_value(const uint64_t) throw();
		serial_value(const int8_t) throw();
		serial_value(const int16_t) throw();
		serial_value(const int32_t) throw();
		serial_value(const int64_t) throw();
		serial_value(const float) throw();
		serial_value(const double) throw();
		serial_value(const pointer_t) throw();
		serial_value(const string_t&) throw();
		serial_value(const char*) throw();
		~serial_value() throw();

		serial_value& operator=(serial_value&&) throw();
		serial_value& operator=(const serial_value&) throw();

		size_t size() const throw();
		serial_value& operator[](size_t);
		const serial_value& operator[](size_t) const;
		serial_value& operator[](const char*);
		const serial_value& operator[](const char*) const;
		serial_value& operator[](const string_t&);
		const serial_value& operator[](const string_t&) const;
	
		void set_null() throw();
		char_t& set_char() throw();
		bool_t& set_bool() throw();
		unsigned_t& set_unsigned() throw();
		signed_t& set_signed() throw();
		float_t& set_float() throw();
		pointer_t& set_pointer() throw();
		string_t& set_string() throw();
		array_t& set_array() throw();
		object_t& set_object() throw();
	
		char_t& get_char();
		bool_t& get_bool();
		unsigned_t& get_unsigned();
		signed_t& get_signed();
		float_t& get_float();
		pointer_t& get_pointer();
		string_t& get_string();
		array_t& get_array() ;
		object_t& get_object();
		
		char_t get_char() const;
		bool_t get_bool() const;
		unsigned_t get_unsigned() const;
		signed_t get_signed() const;
		float_t get_float() const;
		pointer_t get_pointer() const;
		const string_t get_string() const;
		const array_t& get_array() const;
		const object_t& get_object() const;
		
		bool is_null() const throw();
		bool is_char() const throw();
		bool is_bool() const throw();
		bool is_unsigned() const throw();
		bool is_signed() const throw();
		bool is_float() const throw();
		bool is_pointer() const throw();
		bool is_string() const;
		bool is_array() const;
		bool is_object() const;

		type get_type() const throw();
	};
}

#endif