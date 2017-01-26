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

	/*!
		\class serial_value
		\brief A class that can represent any C++ primative value or class.
		\date 23rd January 2017
		\author Adam Smith
	*/
	
	/*!
		\brief Create a null serial_value.
	*/
	serial_value::serial_value() throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(NULL_T)
	{}

	/*!
		\brief Move a serial_value.
		\param aOther The value to move.
	*/
	serial_value::serial_value(serial_value&& aOther) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(NULL_T)
	{
		operator=(std::move(aOther));
	}

	/*!
		\brief Copy a serial_value.
		\param aOther The value to copy.
	*/
	serial_value::serial_value(const serial_value& aOther) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(NULL_T)
	{
		operator=(aOther);
	}

	/*!
		\brief Create a serial_value with a given type.
		\param aType The type to create.
	*/
	serial_value::serial_value(const type aType) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(aType)
	{
		switch(mType) {
		case STRING_T:
			mString = new string_t();
			break;
		case ARRAY_T:
			mArray = new array_t();
			break;
		case OBJECT_T:
			mObject = new object_t();
			break;
		}
	}

	/*!
		\brief Create a CHAR_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const char_t aValue) throw() :
		mChar(aValue),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(CHAR_T)
	{}

	/*!
		\brief Create a BOOL_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const bool_t aValue) throw() :
		mChar(),
		mBool(aValue),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(BOOL_T)
	{}

	/*!
		\brief Create a UNSIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const uint8_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(aValue),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(UNSIGNED_T)
	{}

	/*!
		\brief Create a UNSIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const uint16_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(aValue),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(UNSIGNED_T)
	{}
	
	/*!
		\brief Create a UNSIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const uint32_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(aValue),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(UNSIGNED_T)
	{}
	
	/*!
		\brief Create a UNSIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const uint64_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(aValue),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(UNSIGNED_T)
	{}

	/*!
		\brief Create a SIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const int8_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(aValue),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(SIGNED_T)
	{}

	/*!
		\brief Create a SIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const int16_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(aValue),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(SIGNED_T)
	{}
	
	/*!
		\brief Create a SIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const int32_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(aValue),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(SIGNED_T)
	{}
	
	/*!
		\brief Create a SIGNED_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const int64_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(aValue),
		mFloat(),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(SIGNED_T)
	{}
	
	/*!
		\brief Create a FLOAT_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const float aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(aValue),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(FLOAT_T)
	{}
		
	/*!
		\brief Create a FLOAT_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const double aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(aValue),
		mPointer(),
		mString(),
		mArray(),
		mObject(),
		mType(FLOAT_T)
	{}
		
	/*!
		\brief Create a POINTER_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const pointer_t aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(aValue),
		mString(),
		mArray(),
		mObject(),
		mType(POINTER_T)
	{}
			
	/*!
		\brief Create a STRING_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const string_t& aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(new string_t(aValue)),
		mArray(),
		mObject(),
		mType(STRING_T)
	{
		//! \bug mString becomes null
	}
			
	/*!
		\brief Create a STRING_T serial_value.
		\param aValue The value to set.
	*/
	serial_value::serial_value(const char* aValue) throw() :
		mChar(),
		mBool(),
		mUnsigned(),
		mSigned(),
		mFloat(),
		mPointer(),
		mString(new string_t(aValue)),
		mArray(),
		mObject(),
		mType(STRING_T)
	{
		//! \bug mString becomes null
	}
			
	/*!
		\brief Drestroy the serial_value and any value it contains.
	*/
	serial_value::~serial_value() throw() {
		set_null();
	}
			
	/*!
		\brief Move a serial_value.
		\param aOther The value to move.
		\return A reference to this value.
	*/
	serial_value& serial_value::operator=(serial_value&& aOther) throw() {
		set_null();
		switch(aOther.mType) {
		case CHAR_T:
			mChar = aOther.mChar;
			break;
		case BOOL_T:
			mBool = aOther.mBool;
			break;
		case UNSIGNED_T:
			mUnsigned = aOther.mUnsigned;
			break;
		case SIGNED_T:
			mSigned = aOther.mSigned;
			break;
		case FLOAT_T:
			mFloat = aOther.mFloat;
			break;
		case POINTER_T:
			mPointer = aOther.mPointer;
			break;
		case STRING_T:
			mString = aOther.mString;
			break;
		case ARRAY_T:
			mArray = aOther.mArray;
			break;
		case OBJECT_T:
			mObject = aOther.mObject;
			break;
		}
		mType = aOther.mType;
		aOther.mType = NULL_T;
		return *this;
	}
	
			
	/*!
		\brief Copy a serial_value.
		\param aOther The value to copy.
		\return A reference to this value.
	*/
	serial_value& serial_value::operator=(const serial_value& aOther) throw() {
		if(mType != aOther.mType) set_null();
		switch(aOther.mType) {
		case CHAR_T:
			mChar = aOther.mChar;
			break;
		case BOOL_T:
			mBool = aOther.mBool;
			break;
		case UNSIGNED_T:
			mUnsigned = aOther.mUnsigned;
			break;
		case SIGNED_T:
			mSigned = aOther.mSigned;
			break;
		case FLOAT_T:
			mFloat = aOther.mFloat;
			break;
		case POINTER_T:
			mPointer = aOther.mPointer;
			break;
		case STRING_T:
			if(mType == STRING_T) *mString = *aOther.mString;
			else mString = new string_t(*aOther.mString);
			break;
		case ARRAY_T:
			if(mType == ARRAY_T) *mArray = *aOther.mArray;
			else mArray = new array_t(*aOther.mArray);
			break;
		case OBJECT_T:
			if(mType == OBJECT_T) *mObject = *aOther.mObject;
			else mObject = new object_t(*aOther.mObject);
			break;
		}
		mType = aOther.mType;
		return *this;
	}

	/*!
		\brief Get the number of values contained within this value.
		\param For any values that are not either ARRAY_T or OBJECT_T this function will return 0.
		\return The number of values.
	*/
	size_t serial_value::size() const throw() {
		return
			mType == ARRAY_T ? mArray->size() :
			mType == OBJECT_T ? mObject->size() : 
			0;
	}

	/*!
		\brief Access a value by index.
		\detail Will throw exception if type is not either ARRAY_T or OBJECT_T.
		\param aIndex The index to access.
		\return The value at the index.
	*/
	serial_value& serial_value::operator[](const size_t aIndex) {
		return const_cast<serial_value&>(const_cast<const serial_value*>(this)->operator[](aIndex));
	}
	
	/*!
		\brief Access a value by index.
		\detail Will throw exception if type is not either ARRAY_T or OBJECT_T.
		\param aIndex The index to access.
		\return The value at the index.
	*/
	const serial_value& serial_value::operator[](const size_t aIndex) const {
		switch (mType) {
		case ARRAY_T:
			return mArray->operator[](aIndex);
			break;
		case OBJECT_T:
			{
				const auto end = mObject->end();
				auto i = mObject->begin();
				for(size_t j = 0; j < aIndex; ++j) {
					++i;
					if(i == end) throw std::runtime_error("as::serial_value::operator[] : Index is out of bounds");
				}
				return i->second;
			}
			break;
		}
		throw std::runtime_error("as::serial_value::operator[] : Type is not ARRAY_T or OBJECT_T");
	}
	
	/*!
		\brief Access a value by index.
		\detail Will throw exception if type is not OBJECT_T.
		\param aIndex The index to access.
		\return The value at the index.
	*/
	serial_value& serial_value::operator[](const char* aIndex) {
		return operator[](string_t(aIndex));
	}
		
	/*!
		\brief Access a value by index.
		\detail Will throw exception if type is not OBJECT_T.
		\param aIndex The index to access.
		\return The value at the index.
	*/
	const serial_value& serial_value::operator[](const char* aIndex) const {
		return operator[](string_t(aIndex));
	}
		
	/*!
		\brief Access a value by index.
		\detail Will throw exception if type is not OBJECT_T.
		\param aIndex The index to access.
		\return The value at the index.
	*/
	serial_value& serial_value::operator[](const string_t& aIndex) {
		return const_cast<serial_value&>(const_cast<const serial_value*>(this)->operator[](aIndex));
	}
		
	/*!
		\brief Access a value by index.
		\detail Will throw exception if type is not OBJECT_T.
		\param aIndex The index to access.
		\return The value at the index.
	*/
	const serial_value& serial_value::operator[](const string_t& aIndex) const {
		if(mType != OBJECT_T) throw std::runtime_error("as::serial_value::operator[] : Type is not OBJECT_T");
		const auto i = mObject->find(aIndex);
		if(i == mObject->end()) throw std::runtime_error("as::serial_value::operator[] : Member with given key not found");
		return i->second;
	}
		
	/*!
		\brief The value will become NULL_T.
		\detail Any previous value will be deleted.
	*/
	void serial_value::set_null() throw() {
		switch(mType) {
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

	/*!
		\brief The value will become CHAR_T.
		\detail Any previous value will be deleted.
	*/
	serial_value::char_t& serial_value::set_char() throw() {
		if(mType != CHAR_T) {
			set_null();
			mType = CHAR_T;
		}
		mChar = 0;
		return mChar;
	}
	
	/*!
		\brief The value will become BOOL_T.
		\detail Any previous value will be deleted.
	*/
	serial_value::bool_t& serial_value::set_bool() throw() {
		if(mType != BOOL_T) {
			set_null();
			mType = BOOL_T;
		}
		mBool = false;
		return mBool;
	}

	/*!
		\brief The value will become UNSIGNED_T.
		\detail Any previous value will be deleted.
	*/
	serial_value::unsigned_t& serial_value::set_unsigned() throw() {
		if(mType != UNSIGNED_T) {
			set_null();
			mType = UNSIGNED_T;
		}
		mUnsigned = 0;
		return mUnsigned;
	}
	
	/*!
		\brief The value will become SIGNED_T.
		\detail Any previous value will be deleted.
	*/
	serial_value::signed_t& serial_value::set_signed() throw() {
		if(mType != SIGNED_T) {
			set_null();
			mType = SIGNED_T;
		}
		mSigned = 0;
		return mSigned;
	}

	/*!
		\brief The value will become FLOAT_T.
		\detail Any previous value will be deleted.
	*/
	serial_value::float_t& serial_value::set_float() throw() {
		if(mType != FLOAT_T) {
			set_null();
			mType = FLOAT_T;
		}
		mFloat = 0.0;
		return mFloat;
	}
	
	/*!
		\brief The value will become POINTER_T.
		\detail Any previous value will be deleted.
	*/
	serial_value::pointer_t& serial_value::set_pointer() throw() {
		if(mType != POINTER_T) {
			set_null();
			mType = POINTER_T;
		}
		mPointer = nullptr;
		return mPointer;
	}

	/*!
		\brief The value will become STRING_T.
		\detail Any previous value will be deleted.
	*/
	serial_value::string_t& serial_value::set_string() throw() {
		if(mType != STRING_T) {
			set_null();
			mType = STRING_T;
			mString = new string_t();
		}else {
			mString->clear();
		}
	
		return *mString;
	}

	/*!
		\brief The value will become ARRAY_T.
		\detail Any previous value will be deleted.
	*/
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

	/*!
		\brief The value will become OBJECT_T.
		\detail Any previous value will be deleted.
	*/
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

	/*!
		\brief Return a CHAR_T representation of this value.
		\detail If the value is not CHAR_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the CHAR_T value.
	*/
	serial_value::char_t& serial_value::get_char() {
		if(mType == CHAR_T) return mChar;
		set_char() = const_cast<const serial_value*>(this)->get_char();
		return mChar;
	}

	/*!
		\brief Return a BOOL_T representation of this value.
		\detail If the value is not BOOL_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the BOOL_T value.
	*/
	serial_value::bool_t& serial_value::get_bool() {
		if(mType == BOOL_T) return mBool;
		set_bool() = const_cast<const serial_value*>(this)->get_bool();
		return mBool;
	}
	
	/*!
		\brief Return a UNSIGNED_T representation of this value.
		\detail If the value is not UNSIGNED_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the UNSIGNED_T value.
	*/
	serial_value::unsigned_t& serial_value::get_unsigned() {
		if(mType == UNSIGNED_T) return mUnsigned;
		set_unsigned() = const_cast<const serial_value*>(this)->get_unsigned();
		return mUnsigned;
	}
		
	/*!
		\brief Return a SIGNED_T representation of this value.
		\detail If the value is not SIGNED_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the UNSIGNED_T value.
	*/
	serial_value::signed_t& serial_value::get_signed() {
		if(mType == SIGNED_T) return mSigned;
		set_signed() = const_cast<const serial_value*>(this)->get_signed();
		return mSigned;
	}
			
	/*!
		\brief Return a FLOAT_T representation of this value.
		\detail If the value is not FLOAT_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the FLOAT_T value.
	*/
	serial_value::float_t& serial_value::get_float() {
		if(mType == FLOAT_T) return mFloat;
		set_float() = const_cast<const serial_value*>(this)->get_float();
		return mFloat;
	}

	/*!
		\brief Return a POINTER_T representation of this value.
		\detail If the value is not POINTER_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the POINTER_T value.
	*/
	serial_value::pointer_t& serial_value::get_pointer() {
		if(mType == POINTER_T) return mPointer;
		set_pointer() = const_cast<const serial_value*>(this)->get_pointer();
		return mPointer;
	}
	
	/*!
		\brief Return a STRING_T representation of this value.
		\detail If the value is not STRING_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the STRING_T value.
	*/
	serial_value::string_t& serial_value::get_string() {
		if(mType == STRING_T) return *mString;
		const string_t tmp = const_cast<const serial_value*>(this)->get_string();
		set_string() = tmp;
		return *mString;
	}
		
	/*!
		\brief Return a ARRAY_T representation of this value.
		\detail If the value is not ARRAY_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the ARRAY_T value.
	*/
	serial_value::array_t& serial_value::get_array() {
		if(mType == ARRAY_T) return *mArray;
		const array_t tmp = const_cast<const serial_value*>(this)->get_array();
		set_array() = tmp;
		return *mArray;
	}
			
	/*!
		\brief Return a OBJECT_T representation of this value.
		\detail If the value is not OBJECT_T then it will be converted and overwritten or an exception will be thrown.
		\return A reference to the OBJECT_T value.
	*/
	serial_value::object_t& serial_value::get_object() {
		if(mType == OBJECT_T) return *mObject;
		const object_t tmp = const_cast<const serial_value*>(this)->get_object();
		set_object() = tmp;
		return *mObject;
	}
			
	/*!
		\brief Return a CHAR_T representation of this value.
		\detail If the value is not CHAR_T then it will be converted or an exception will be thrown.
		\return The CHAR_T value.
	*/
	serial_value::char_t serial_value::get_char() const {
		switch(mType) {
		case CHAR_T:
			return mChar;
			break;
		case BOOL_T:
			return mBool ? '1' : '0';
			break;
		case UNSIGNED_T:
			if(mUnsigned >= 0 && mUnsigned <= 9) return static_cast<char_t>('0' + mUnsigned);
			break;
		case SIGNED_T:
			if(mSigned >= 0 && mSigned <= 9) return static_cast<char_t>('0' + mSigned);
			break;
		case FLOAT_T:
			{
				const int64_t tmp = static_cast<int64_t>(mFloat);
				if(static_cast<float_t>(tmp) == mFloat && tmp >= 0 && tmp <= 9) 
					return static_cast<char_t>('0' + tmp);
			}
			break;
		case STRING_T:
			if(mString->size() == 1) mString->front();
			break;
		}
		throw std::runtime_error("as::serial_value::get_char : Type is not convertable to char");
	}
			
	/*!
		\brief Return a BOOL_T representation of this value.
		\detail If the value is not BOOL_T then it will be converted or an exception will be thrown.
		\return The BOOL_T value.
	*/
	serial_value::bool_t serial_value::get_bool() const {
		switch(mType) {
		case CHAR_T:
			if(mChar == '1' || mChar == 'y' || mChar == 'Y') return true;
			else if(mChar == '0' || mChar == 'n' || mChar == 'N') return false;
			break;
		case BOOL_T:
			return mBool;
		case UNSIGNED_T:
			return mUnsigned > 0;
		case SIGNED_T:
			return mSigned > 0;
		case FLOAT_T:
			return mFloat >= 1.0;
		case POINTER_T:
			return mPointer != nullptr;
		case STRING_T:
			if(
				*mString == "1" || *mString == "y" || *mString == "Y" || *mString == "true" ||
				*mString == "TRUE" || *mString == "True" || *mString == "yes" || *mString == "YES" ||
				*mString == "Yes"
			) return true;
			else if(
				*mString == "0" || *mString == "n" || *mString == "N" || *mString == "false" ||
				*mString == "FALSE" || *mString == "False" || *mString == "no" || *mString == "NO" ||
				*mString == "N0"
			) return true;
			break;
		}
		throw std::runtime_error("as::serial_value::get_bool : Type is not convertable to bool");
	}
			
	/*!
		\brief Return a UNSIGNED_T representation of this value.
		\detail If the value is not UNSIGNED_T then it will be converted or an exception will be thrown.
		\return The UNSIGNED_T value.
	*/
	serial_value::unsigned_t serial_value::get_unsigned() const {
		switch(mType) {
		case CHAR_T:
			if(mChar >= '0' && mChar <= '9') return mChar - '0';
		case BOOL_T:
			return mBool ? 1 : 0;
		case UNSIGNED_T:
			return mUnsigned;
		case SIGNED_T:
			return static_cast<unsigned_t>(mSigned);
		case FLOAT_T:
			return static_cast<unsigned_t>(mFloat);
		case STRING_T:
			try{
				const int tmp = std::stoi(*mString);
				return static_cast<unsigned_t>(tmp);
			}catch(...) {}

			break;
		}
		throw std::runtime_error("as::serial_value::get_unsigned : Type is not convertable to unsigned");
	}
				
	/*!
		\brief Return a SIGNED_T representation of this value.
		\detail If the value is not SIGNED_T then it will be converted or an exception will be thrown.
		\return The SIGNED_T value.
	*/
	serial_value::signed_t serial_value::get_signed() const {
		switch(mType) {
		case CHAR_T:
			if(mChar >= '0' && mChar <= '9') return mChar - '0';
			break;
		case BOOL_T:
			return mBool ? 1 : 0;
		case UNSIGNED_T:
			return static_cast<signed_t>(mUnsigned);
		case SIGNED_T:
			return mSigned;
		case FLOAT_T:
			return static_cast<signed_t>(mFloat);
		case STRING_T:
			try{
				const int tmp = std::stoi(*mString);
				return static_cast<signed_t>(tmp);
			}catch(...) {}

			break;
		}
		throw std::runtime_error("as::serial_value::get_signed : Type is not convertable to signed");
	}
				
	/*!
		\brief Return a FLOAT_T representation of this value.
		\detail If the value is not FLOAT_T then it will be converted or an exception will be thrown.
		\return The FLOAT_T value.
	*/
	serial_value::float_t serial_value::get_float() const {
		switch(mType) {
		case CHAR_T:
			if(mChar >= '0' && mChar <= '9') return mChar - '0';
			break;
		case BOOL_T:
			return mBool ? 1.0 : 0.0;
		case UNSIGNED_T:
			return static_cast<float_t>(mUnsigned);
		case SIGNED_T:
			return static_cast<float_t>(mSigned);
		case FLOAT_T:
			return static_cast<float_t>(mFloat);
		case STRING_T:
			try{
				const double tmp = std::stod(*mString);
				return static_cast<float_t>(tmp);
			}catch(...) {}

			break;
		}
		throw std::runtime_error("as::serial_value::get_float : Type is not convertable to float");
	}
				
	/*!
		\brief Return a POINTER_T representation of this value.
		\detail If the value is not POINTER_T then it will be converted or an exception will be thrown.
		\return The POINTER_T value.
	*/
	serial_value::pointer_t serial_value::get_pointer() const {
		switch(mType) {
		case POINTER_T:
			return mPointer;
		}
		throw std::runtime_error("as::serial_value::get_pointer : Type is not convertable to pointer");
	}
					
	/*!
		\brief Return a STRING_T representation of this value.
		\detail If the value is not STRING_T then it will be converted or an exception will be thrown.
		\return The STRING_T value.
	*/
	const serial_value::string_t serial_value::get_string() const {
		switch(mType) {
		case NULL_T:
			return "null";
			break;
		case CHAR_T:
			return string_t() + mChar;
			break;
		case BOOL_T:
			return mBool ? "true" : "false";
			break;
		case UNSIGNED_T:
			return std::to_string(mUnsigned);
			break;
		case SIGNED_T:
			return std::to_string(mSigned);
			break;
		case FLOAT_T:
			return std::to_string(mFloat);
			break;
		case STRING_T:
			return *mString;
			break;
		}
		throw std::runtime_error("as::serial_value::get_string : Type is not convertable to string");
	}
						
	/*!
		\brief Return a ARRAY_T representation of this value.
		\detail If the value is not ARRAY_T then an exception will be thrown.
		\return The ARRAY_T value.
	*/
	const serial_value::array_t& serial_value::get_array() const {
		switch(mType) {
		case ARRAY_T:
			return *mArray;
			break;
		}
		throw std::runtime_error("as::serial_value::get_array : Type is not convertable to array");
	}
							
	/*!
		\brief Return a OBJECT_T representation of this value.
		\detail If the value is not OBJECT_T then an exception will be thrown.
		\return The OBJECT_T value.
	*/
	const serial_value::object_t& serial_value::get_object() const {
		switch(mType) {
		case OBJECT_T:
			return *mObject;
			break;
		}
		throw std::runtime_error("as::serial_value::get_object : Type is not convertable to object");
	}
								
	/*!
		\brief Check if the value is convertable to NULL_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_null
		\see get_type
	*/
	bool serial_value::is_null() const throw() {
		return mType == NULL_T;
	}
									
	/*!
		\brief Check if the value is convertable to CHAR_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_char
		\see get_char
		\see get_type
	*/
	bool serial_value::is_char() const throw() {
		if(mType == CHAR_T) return true;
		try{
			get_char();
			return true;
		}catch(...) {
			return false;
		}
	}
									
	/*!
		\brief Check if the value is convertable to BOOL_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_bool
		\see get_bool
		\see get_type
	*/
	bool serial_value::is_bool() const throw() {
		if(mType == BOOL_T) return true;
		try{
			get_bool();
			return true;
		}catch(...) {
			return false;
		}
	}
									
	/*!
		\brief Check if the value is convertable to UNSIGNED_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_unsigned
		\see get_unsigned
		\see get_type
	*/
	bool serial_value::is_unsigned() const throw() {
		if(mType == UNSIGNED_T) return true;
		try{
			get_unsigned();
			return true;
		}catch(...) {
			return false;
		}
	}
									
	/*!
		\brief Check if the value is convertable to SIGNED_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_signed
		\see get_signed
		\see get_type
	*/
	bool serial_value::is_signed() const throw() {
		if(mType == SIGNED_T) return true;
		try{
			get_signed();
			return true;
		}catch(...) {
			return false;
		}
	}
									
	/*!
		\brief Check if the value is convertable to FLOAT_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_float
		\see get_float
		\see get_type
	*/
	bool serial_value::is_float() const throw() {
		if(mType == FLOAT_T) return true;
		try{
			get_float();
			return true;
		}catch(...) {
			return false;
		}
	}
									
	/*!
		\brief Check if the value is convertable to POINTER_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_pointer
		\see get_pointer
		\see get_type
	*/
	bool serial_value::is_pointer() const throw() {
		if(mType == POINTER_T) return true;
		try{
			get_pointer();
			return true;
		}catch(...) {
			return false;
		}
	}
										
	/*!
		\brief Check if the value is convertable to STRING_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_string
		\see get_string
		\see get_type
	*/
	bool serial_value::is_string() const {
		if(mType == STRING_T) return true;
		try{
			get_string();
			return true;
		}catch(...) {
			return false;
		}
	}
										
	/*!
		\brief Check if the value is convertable to ARRAY_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_array
		\see get_array
		\see get_type
	*/
	bool serial_value::is_array() const {
		if(mType == ARRAY_T) return true;
		try{
			get_array();
			return true;
		}catch(...) {
			return false;
		}
	}
										
	/*!
		\brief Check if the value is convertable to OBJECT_T.
		\detail Use get_type to get the real type of the value.
		\return True if the object is convertable otherwise false.
		\see set_object
		\see get_object
		\see get_type
	*/
	bool serial_value::is_object() const {
		if(mType == OBJECT_T) return true;
		try{
			get_object();
			return true;
		}catch(...) {
			return false;
		}
	}
											
	/*!
		\brief Get the type of the value.
		\return The type of the value.
	*/
	serial_value::type serial_value::get_type() const throw() {
		return mType;
	}

}