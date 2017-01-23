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

#include "as/serial_value/serial_value.hpp"

namespace as {

	// serial_value

	serial_value::serial_value() throw() {
		//! \todo Implement
	}

	serial_value::serial_value(type) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(bool_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(uint8_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(uint16_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(uint32_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(uint64_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(int8_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(int16_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(int32_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(int64_t) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(float) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(const string_t&) throw() {
		//! \todo Implement
	}

	serial_value::serial_value(const char*) throw() {
		//! \todo Implement
	}

	serial_value::~serial_value() throw() {
		set_null();
	}

	void serial_value::set_null() throw() {
		switch(mType) {
		case NULL_T :
			break;
		case CHAR_T:
			break;
		case BOOL_T:
			break;
		case UNSIGNED_T:
			break;
		case SIGNED_T:
			break;
		case FLOAT_T:
			break;
		case POINTER_T:
			break;
		case STRING_T:
			delete mString;
			break;
		case ARRAY_T:
			delete mArray;
			break;
		case OBJECT_T:
			delete mObject;
			break;
		}
		mType = NULL_T;
	}

	serial_value::char_t& serial_value::set_char() throw() {
		if(mType != CHAR_T) {
			set_null();
			mType = CHAR_T;
		}
		return mChar;
	}

	serial_value::bool_t& serial_value::set_bool() throw() {
		if(mType != BOOL_T) {
			set_null();
			mType = BOOL_T;
		}
		return mBool;
	}

	serial_value::unsigned_t& serial_value::set_unsigned() throw() {
		if(mType != UNSIGNED_T) {
			set_null();
			mType = UNSIGNED_T;
		}
		return mUnsigned;
	}

	serial_value::signed_t& serial_value::set_signed() throw() {
		if(mType != SIGNED_T) {
			set_null();
			mType = SIGNED_T;
		}
		return mSigned;
	}

	serial_value::float_t& serial_value::set_float() throw() {
		if(mType != FLOAT_T) {
			set_null();
			mType = FLOAT_T;
		}
		return mFloat;
	}

	serial_value::pointer_t& serial_value::set_pointer() throw() {
		if(mType != POINTER_T) {
			set_null();
			mType = POINTER_T;
		}
		return mPointer;
	}

	serial_value::string_t& serial_value::set_string() throw() {
		if(mType != STRING_T) {
			set_null();
			mType = STRING_T;
			mString = new string_t();
		}else {
			mString->clear();
		}
	
	return			*mString;
	}

	serial_value::array_t& serial_value::set_array() throw() {
		if(mType != ARRAY_T) {
			set_null();
			mType = ARRAY_T;
			mArray = new array_t();
		}else {
			mArray->clear();
		}
		return *mArray;
	}

	serial_value::object_t& serial_value::set_object() throw() {
		if(mType != OBJECT_T) {
			set_null();
			mType = OBJECT_T;
			mObject = new object_t();
		}else {
			mObject->clear();
		}
		return *mObject;
	}

	serial_value::char_t& serial_value::get_char() {
		//! \todo Implement
		return mChar;
	}

	serial_value::bool_t& serial_value::get_bool() {
		//! \todo Implement
		return mBool;
	}

	serial_value::unsigned_t& serial_value::get_unsigned() {
		//! \todo Implement
		return mUnsigned;
	}

	serial_value::signed_t& serial_value::get_signed() {
		//! \todo Implement
		return mSigned;
	}

	serial_value::float_t& serial_value::get_float() {
		//! \todo Implement
		return mFloat;
	}

	serial_value::pointer_t& serial_value::get_pointer() {
		//! \todo Implement
		return mPointer;
	}

	serial_value::string_t& serial_value::get_string() {
		//! \todo Implement
		return *mString;
	}

	serial_value::array_t& serial_value::get_array() {
		//! \todo Implement
		return *mArray;
	}

	serial_value::object_t& serial_value::get_object() {
		//! \todo Implement
		return *mObject;
	}

	serial_value::char_t serial_value::get_char() const throw() {
		//! \todo Implement
		return mChar;
	}

	serial_value::bool_t serial_value::get_bool() const throw() {
		//! \todo Implement
		return mBool;
	}

	serial_value::unsigned_t serial_value::get_unsigned() const throw() {
		//! \todo Implement
		return mUnsigned;
	}

	serial_value::signed_t serial_value::get_signed() const throw() {
		//! \todo Implement
		return mSigned;
	}

	serial_value::float_t serial_value::get_float() const throw() {
		//! \todo Implement
		return mFloat;
	}

	serial_value::pointer_t serial_value::get_pointer() const throw() {
		//! \todo Implement
		return mPointer;
	}

	const serial_value::string_t& serial_value::get_string() const {
		//! \todo Implement
		return *mString;
	}

	const serial_value::array_t& serial_value::get_array() const {
		//! \todo Implement
		return *mArray;
	}

	const serial_value::object_t& serial_value::get_object() const {
		//! \todo Implement
		return *mObject;
	}

	bool serial_value::is_null() const throw() {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_char() const throw() {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_bool() const throw() {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_unsigned() const throw() {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_signed() const throw() {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_float() const throw() {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_pointer() const throw() {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_string() const {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_array() const {
		//! \todo Implement
		return false;
	}

	bool serial_value::is_object() const {
		//! \todo Implement
		return false;
	}

}