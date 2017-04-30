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

#include "asmith/serial/value.hpp"
#include <algorithm>
	
namespace asmith { namespace serial {

	bool sv_strcmp(const char* aFirst, const char* aSecond) {
		char a = *aFirst;
		char b = *aSecond;
		while (a != '\0' && b != '\0') {
			if (a >= 'A' && a <= 'Z') a -= 'A' - 'a';
			if (b >= 'A' && b <= 'Z') b -= 'A' - 'a';
			if (a != b) return false;
			++aFirst;
			++aSecond;
			a = *aFirst;
			b = *aSecond;
		}
		return a == b;
	}

	// value

	value::value() throw() :
		mType(NULL_T)
	{}

	value::value(const type aType) throw() :
		mType(NULL_T)
	{
		switch(aType) {
		case NULL_T:
			set_null();
			break;
		case BOOl_T:
			set_bool();
			break;
		case CHAR_T:
			set_char() ;
			break;
		case NUMBER_T:
			set_number();
			break;
		case STRING_T:
			set_string();
			break;
		case ARRAY_T:
			set_array();
			break;
		case OBJECT_T:
			set_object();
			break;
		default:
			break;
		}
	}

	value::value(const value& aOther) throw() :
		mType(NULL_T)
	{
		operator=(aOther);
	}

	value::value(value&& aOther) throw() :
		mType(NULL_T)
	{
		operator=(std::move(aOther));
	}

	value::~value() throw() {
		set_null();
	}

	value& value::operator=(const value& aOther) throw() {
		switch(aOther.mType) {
		case NULL_T:
			set_null();
			break;
		case BOOl_T:
			set_bool() = aOther.mBool;
			break;
		case CHAR_T:
			set_char() = aOther.mChar;
			break;
		case NUMBER_T:
			set_number() = aOther.mNumber;
			break;
		case STRING_T:
			set_string() = *aOther.mString;
			break;
		case ARRAY_T:
			set_array() = *aOther.mArray;
			break;
		case OBJECT_T:
			set_object() = *aOther.mObject;
			break;
		default:
			break;
		}
		return *this;
	}

	value& value::operator=(value&& aOther) throw() {
		uint8_t buf[sizeof(value)];
		memcpy(buf, this, sizeof(value));
		memcpy(this, &aOther, sizeof(value));
		memcpy(&aOther, buf, sizeof(value));
		return *this;
	}

	bool value::operator==(const value& aOther) const throw() {
		//! \todo Conversions
		if(mType != aOther.mType) return false;

		switch (mType) {
		case NULL_T:
			return true;
		case BOOl_T:
			return mBool == aOther.mBool;
		case CHAR_T:
			return mChar == aOther.mChar;
		case NUMBER_T:
			return mNumber == aOther.mNumber;
		case STRING_T:
			return *mString == *aOther.mString;
		case ARRAY_T:
			return *mArray == *aOther.mArray;
		case OBJECT_T:
			return *mObject == *aOther.mObject;
		default:
			return false;
		}
	}

	bool value::operator!=(const value& aOther) const throw() {
		return ! operator==(aOther);
	}

	value& value::operator[](const size_t aIndex) {
		return const_cast<value&>(const_cast<const value*>(this)->operator[](aIndex));
	}

	value& value::operator[](const std::string& aIndex) {
		return const_cast<value&>(const_cast<const value*>(this)->operator[](aIndex));
	}

	const value&value:: operator[](const size_t aIndex) const {
		switch(mType) {
		case ARRAY_T:
			return mArray->operator[](aIndex);
		case OBJECT_T:
		{
			auto i = mObject->begin();
			for(size_t j = 0; j < aIndex; ++j) ++i;
			return i->second;
		}
		default:
			throw std::runtime_error("value : Value is not an array or object");
		}
	}

	const value& value::operator[](const std::string& aIndex) const {
		if(mType != OBJECT_T) throw std::runtime_error("value : Value is not an object");
		const auto i = mObject->find(aIndex);
		if(i == mObject->end()) throw std::runtime_error("value : Object does not contain object with given name");
		return i->second;
	}

	size_t value::size() const throw() {
		return mType == ARRAY_T ? mArray->size() : mType == OBJECT_T ? mObject->size() : 0;
	}

	value::type value::get_type() const throw() {
		return mType;
	}

	value::bool_t value::get_bool() const {
		switch(mType) {
		case BOOl_T:
			return mBool;
			break;
		case CHAR_T:
			switch (mChar)
			{
			case '0':
			case 'n':
			case 'f':
			case 'N':
			case 'F':
				return false;
			case '1':
			case 'y':
			case 't':
			case 'Y':
			case 'T':
				return true;
			default:
				break;
			}
			break;
		case NUMBER_T:
			return mNumber >= 0.5;
			break;
		case STRING_T:
			if(
				sv_strcmp(mString->c_str(), "0") ||
				sv_strcmp(mString->c_str(), "n") ||
				sv_strcmp(mString->c_str(), "f") ||
				sv_strcmp(mString->c_str(), "zero") ||
				sv_strcmp(mString->c_str(), "no") ||
				sv_strcmp(mString->c_str(), "false") 
			) return false;
			if(
				sv_strcmp(mString->c_str(), "1") ||
				sv_strcmp(mString->c_str(), "y") ||
				sv_strcmp(mString->c_str(), "t") ||
				sv_strcmp(mString->c_str(), "one") ||
				sv_strcmp(mString->c_str(), "yes") ||
				sv_strcmp(mString->c_str(), "true") 
			) return false;
			break;
		default:
			break;
		}

		throw std::runtime_error("value : Value is not convertable to bool");
	}

	value::char_t value::get_char() const {
		switch(mType) {
		case BOOl_T:
			return mBool ? '0' : '1';
			break;
		case CHAR_T:
			return mChar;
			break;
		case NUMBER_T:
		{
			static uint8_t tmp = static_cast<uint8_t>(mNumber);
			if(tmp == mNumber && tmp >= 0 && tmp <= 9) return '0' + tmp;
		}
		break;
		case STRING_T:
			if(! mString->empty()) return mString->front();
			break;
		default:
			break;
		}

		throw std::runtime_error("value : Value is not convertable to char");
	}

	value::number_t value::get_number() const {
		switch(mType) {
		case BOOl_T:
			return mBool ? 0.0 : 1.0;
			break;
		case CHAR_T:
			if(mChar >= '0' && mChar <= '9') return mChar - '0';
			break;
		case NUMBER_T:
			return mNumber;
			break;
		case STRING_T:
			try {
				return std::stold(mString->c_str());
			}catch (...) {

			}
			break;
		default:
			break;
		}

		throw std::runtime_error("value : Value is not convertable to number");
	}

	value::string_t value::get_string() const {
		switch(mType) {
		case BOOl_T:
			return mBool ? "true" : "false";
			break;
		case CHAR_T:
			{
				const char buf[2] = {mChar, '\0'};
				return buf;
			}
			break;
		case NUMBER_T:
			return std::to_string(mNumber);
			break;
		case STRING_T:
			return *mString;
			break;
		default:
			break;
		}

		throw std::runtime_error("value : Value is not convertable to string");
	}

	value::array_t value::get_array() const {
		switch(mType) {
		case ARRAY_T:
			return *mArray;
		case OBJECT_T:
		{
			value tmp;
			const object_t& object = *mObject;
			array_t& array_ = tmp.set_array();
			const size_t s = object.size();

			// Check if object keys are contiguous indices
			auto j = object.begin();
			for(size_t i = 0; i < s; ++i, ++j) {
				array_.push_back(atof(j->first.c_str()));
			}

			std::sort(array_.begin(), array_.end(), [](const value& a, const value& b)->bool {
				return a.mNumber < b.mNumber;
			});

			j = object.begin();
			for(size_t i = 0; i < s; ++i, ++j) {
				if(array_[i].mNumber != static_cast<number_t>(i)) throw std::runtime_error("value : Value is not convertable to array");
			}

			// Copy values to arrayj = object.begin();
			j = object.begin();
			for(size_t i = 0; i < s; ++i, ++j) {
				array_[atoi(j->first.c_str())] = j->second;
			}

			return array_;
		}
		default:
			break;
		}

		throw std::runtime_error("value : Value is not convertable to array");
	}

	value::object_t value::get_object() const {
		switch(mType) {
		case ARRAY_T:
			{
				value tmp;
				object_t& object = tmp.get_object();
				const array_t& array_ = *mArray;
				const size_t s = array_.size();
				for(size_t i = 0; i < s; ++i) {
					object.emplace(
						std::to_string(i),
						array_[i]
					);
				}
				return object;
			}
		case OBJECT_T:
			return *mObject;
		default:
			break;
		}

		throw std::runtime_error("value : Value is not convertable to object");
	}

	#define SV_JOIN2(a, b) a##b
	#define SV_JOIN(a, b) SV_JOIN2(a , b)

	#define SV_CONSTRUCTOR(VAL) \
	value::value(VAL aValue) throw() :\
		SV_VALUE(static_cast<SV_TYPE>(aValue)),\
		mType(SV_ENUM)\
	{}

	#define SV_CONSTRUCTOR_NEW(VAL) \
	value::value(VAL aValue) throw() :\
		SV_VALUE(new SV_TYPE(aValue)),\
		mType(SV_ENUM)\
	{}

	#define SV_SET \
	value::SV_TYPE& value::SV_JOIN(set_,SV_NAME)() throw() {\
		if(mType != SV_ENUM) {\
			set_null();\
			mType = SV_ENUM;\
		}\
		return SV_VALUE;\
	}

	#define SV_SET_NEW \
	value::SV_TYPE& value::SV_JOIN(set_,SV_NAME)() throw() {\
		if(mType != SV_ENUM) {\
			set_null();\
			SV_VALUE = new SV_TYPE();\
			mType = SV_ENUM;\
		}else{\
			SV_VALUE->clear();\
		}\
		return *SV_VALUE;\
	}

	#define SV_GET \
	value::SV_TYPE& value::SV_JOIN(get_,SV_NAME)() throw() {\
		if(mType != SV_ENUM) {\
			const SV_TYPE tmp = static_cast<const value*>(this)->SV_JOIN(get_,SV_NAME)();\
			SV_JOIN(set_,SV_NAME)() = tmp;\
		}\
		return SV_VALUE;\
	}

	#define SV_GET_NEW \
	value::SV_TYPE& value::SV_JOIN(get_,SV_NAME)() throw() {\
		if(mType != SV_ENUM) {\
			const SV_TYPE tmp = static_cast<const value*>(this)->SV_JOIN(get_,SV_NAME)();\
			SV_JOIN(set_,SV_NAME)() = tmp;\
		}\
		return *SV_VALUE;\
	}

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#define SV_NAME null
	#define SV_TYPE null_t
	#define SV_ENUM NULL_T
	#define SV_VALUE

	value::SV_TYPE value::SV_JOIN(set_, SV_NAME)() throw() {
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
		default:
			break;
		}
		mString = nullptr;
		mType = SV_ENUM;
	}

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#define SV_NAME bool
	#define SV_TYPE bool_t
	#define SV_ENUM BOOl_T
	#define SV_VALUE mBool

	SV_CONSTRUCTOR(bool)
	SV_SET
	SV_GET

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#define SV_NAME char
	#define SV_TYPE char_t
	#define SV_ENUM CHAR_T
	#define SV_VALUE mChar

	SV_CONSTRUCTOR(char)
	SV_SET
	SV_GET

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#define SV_NAME number
	#define SV_TYPE number_t
	#define SV_ENUM NUMBER_T
	#define SV_VALUE mNumber

	SV_CONSTRUCTOR(uint8_t)
	SV_CONSTRUCTOR(uint16_t)
	SV_CONSTRUCTOR(uint32_t)
	SV_CONSTRUCTOR(uint64_t)
	SV_CONSTRUCTOR(int8_t)
	SV_CONSTRUCTOR(int16_t)
	SV_CONSTRUCTOR(int32_t)
	SV_CONSTRUCTOR(int64_t)
	SV_CONSTRUCTOR(float)
	SV_CONSTRUCTOR(double)
	SV_SET
	SV_GET

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#define SV_NAME string
	#define SV_TYPE string_t
	#define SV_ENUM STRING_T
	#define SV_VALUE mString

	SV_CONSTRUCTOR_NEW(const char*)
	SV_CONSTRUCTOR_NEW(const std::string&)
	SV_SET_NEW
	SV_GET_NEW

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#define SV_NAME array
	#define SV_TYPE array_t
	#define SV_ENUM ARRAY_T
	#define SV_VALUE mArray

	SV_SET_NEW
	SV_GET_NEW

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#define SV_NAME object
	#define SV_TYPE object_t
	#define SV_ENUM OBJECT_T
	#define SV_VALUE mObject

	SV_SET_NEW
	SV_GET_NEW

	#undef SV_NAME
	#undef SV_TYPE
	#undef SV_ENUM
	#undef SV_VALUE
	#undef SV_CONSTRUCTOR
	#undef SV_CONSTRUCTOR_NEW
	#undef SV_SET
	#undef SV_SET_NEW
	#undef SV_GET
	#undef SV_GET_NEW
	#undef SV_JOIN
	#undef SV_JOIN2
}}