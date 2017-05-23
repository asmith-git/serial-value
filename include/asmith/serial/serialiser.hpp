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

#include <list>
#include <deque>
#include <array>
#include "value.hpp"

#ifndef ASMITH_SERIAL_SERIALISER_HPP
#define ASMITH_SERIAL_SERIALISER_HPP

namespace asmith { namespace serial {

	template<class T, class ENABLE = void>
	struct serialiser {
		typedef const T& input_t;
		typedef T output_t;

#ifdef ASMITH_REFLECTION_CLASS_HPP
		static value serialise(input_t aValue) throw() {
			return reflection_serialise(reflect<T>(), &aValue);
		}

		static value deserialise(const value& aValue) throw() {
			output_t tmp;
			return reflection_deserialise(aValue, reflect<T>(), &tmp);
			return tmp;
		}
#else
		static value serialise(input_t) throw() = delete;
		static output_t deserialise(const value&) throw() = delete;
#endif
	};

	template<class T>
	value serialise(typename serialiser<T>::input_t aValue) throw() {
		try {
			return serialiser<T>::serialise(aValue);
		}catch (...) {
			return value();
		}
	}

	template<class T>
	typename serialiser<T>::output_t deserialise(const value& aValue){
		return serialiser<T>::deserialise(aValue);
	}




#ifdef ASMITH_REFLECTION_CLASS_HPP
	static value reflection_serialise(const reflection_class& aCls, const void* aValue) {
		const char* name = aCls.get_name();
		const size_t nameLen = strlen(name);

		switch (name[nameLen]) {
		case '*':
			throw std::runtime_error("asmith::serial::reflection_serialise : Automatic serialisation not implemented for pointers");
		case '&':
			//! \todo Implement for references
			throw std::runtime_error("asmith::serial::reflection_serialise : Automatic serialisation not implemented for references");
		case ']':
			//! \todo Implement for arrays
			throw std::runtime_error("asmith::serial::reflection_serialise : Automatic serialisation not implemented for fixed size arrays");
		}

		if(strcmp(name, reflect<bool>().get_name()) == 0)			return value(*reinterpret_cast<const bool*>(aValue));
		else if(strcmp(name, reflect<char>().get_name()) == 0)		return value(*reinterpret_cast<const char*>(aValue));
		else if(strcmp(name, reflect<uint8_t>().get_name()) == 0)	return value(*reinterpret_cast<const uint8_t*>(aValue));
		else if(strcmp(name, reflect<uint16_t>().get_name()) == 0)	return value(*reinterpret_cast<const uint16_t*>(aValue));
		else if(strcmp(name, reflect<uint32_t>().get_name()) == 0)	return value(*reinterpret_cast<const uint32_t*>(aValue));
		else if(strcmp(name, reflect<uint64_t>().get_name()) == 0)	return value(*reinterpret_cast<const uint64_t*>(aValue));
		else if(strcmp(name, reflect<int8_t>().get_name()) == 0)	return value(*reinterpret_cast<const int8_t*>(aValue));
		else if(strcmp(name, reflect<int16_t>().get_name()) == 0)	return value(*reinterpret_cast<const int16_t*>(aValue));
		else if(strcmp(name, reflect<int32_t>().get_name()) == 0)	return value(*reinterpret_cast<const int32_t*>(aValue));
		else if(strcmp(name, reflect<int64_t>().get_name()) == 0)	return value(*reinterpret_cast<const int64_t*>(aValue));
		else if(strcmp(name, reflect<float>().get_name()) == 0)		return value(*reinterpret_cast<const float*>(aValue));
		else if(strcmp(name, reflect<double>().get_name()) == 0)	return value(*reinterpret_cast<const double*>(aValue));
		//! \todo Handle std::string, std::vector, ect

		value val;
		const size_t varCount = aCls.get_variable_count();
		if(varCount == 0) return val;

		size_t bufSize = 256;
		uint8_t* buffer = new uint8_t[bufSize];
		value::object_t& object = val.set_object();
		for(size_t i = 0; i < varCount; ++i) {
			const reflection_variable& var = aCls.get_variable(i);
			const reflection_class& vCls = var.get_class();
			const size_t s = vCls.get_size();
			if(s > bufSize) {
				delete[] buffer;
				buffer = new uint8_t[bufSize];
			}

			vCls.get_trivial_constructor().call(buffer);
			try {
				var.get_unsafe(aValue, buffer);
				object.emplace(var.get_name(), reflection_serialise(vCls, buffer));
			}catch (std::exception& e) {
				vCls.get_destructor().call(buffer);
				throw e;
			}
			vCls.get_destructor().call(buffer);
		}
		delete[] buffer;

		return val;
	}


	void reflection_deserialise(const value& aValue, const reflection_class& aCls, void* aReturn) {
		const char* name = aCls.get_name();
		const size_t nameLen = strlen(name);

		switch (name[nameLen]) {
		case '*':
			throw std::runtime_error("asmith::serial::reflection_deserialise : Automatic deserialisation not implemented for pointers");
		case '&':
			//! \todo Implement for references
			throw std::runtime_error("asmith::serial::reflection_deserialise : Automatic deserialisation not implemented for references");
		case ']':
			//! \todo Implement for arrays
			throw std::runtime_error("asmith::serial::reflection_deserialise : Automatic deserialisation not implemented for fixed size arrays");
		}

		if(strcmp(name, reflect<bool>().get_name()) == 0)			{ *reinterpret_cast<bool*>(aReturn) = aValue.get_bool(); return; }
		else if(strcmp(name, reflect<char>().get_name()) == 0)		{ *reinterpret_cast<char*>(aReturn) = aValue.get_char(); return; }
		else if(strcmp(name, reflect<uint8_t>().get_name()) == 0)	{ *reinterpret_cast<uint8_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<uint16_t>().get_name()) == 0)	{ *reinterpret_cast<uint16_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<uint32_t>().get_name()) == 0)	{ *reinterpret_cast<uint32_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<uint64_t>().get_name()) == 0)	{ *reinterpret_cast<uint64_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<int8_t>().get_name()) == 0)	{ *reinterpret_cast<int8_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<int16_t>().get_name()) == 0)	{ *reinterpret_cast<int16_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<int32_t>().get_name()) == 0)	{ *reinterpret_cast<int32_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<int64_t>().get_name()) == 0)	{ *reinterpret_cast<int64_t*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<float>().get_name()) == 0)		{ *reinterpret_cast<float*>(aReturn) = aValue.get_number(); return; }
		else if(strcmp(name, reflect<double>().get_name()) == 0)	{ *reinterpret_cast<double*>(aReturn) = aValue.get_number(); return; }
		//! \todo Handle std::string, std::vector, ect

		const size_t varCount = aCls.get_variable_count();
		if(varCount == 0) return;

		size_t bufSize = 256;
		uint8_t* buffer = new uint8_t[bufSize];
		const value::object_t& object = aValue.get_object();
		for(size_t i = 0; i < varCount; ++i) {
			const reflection_variable& var = aCls.get_variable(i);
			const reflection_class& vCls = var.get_class();
			const size_t s = vCls.get_size();
			if(s > bufSize) {
				delete[] buffer;
				buffer = new uint8_t[bufSize];
			}

			vCls.get_trivial_constructor().call(buffer);
			try {
				reflection_deserialise(object.find(var.get_name())->second, vCls, buffer);
				var.set_unsafe(aReturn, buffer);
			}catch (std::exception& e) {
				vCls.get_destructor().call(buffer);
				throw e;
			}
			vCls.get_destructor().call(buffer);
		}
		delete[] buffer;
	}
#endif

	// -- Specialisations --

	template<>
	struct serialiser<value> {
		typedef value input_t;
		typedef value output_t;

		static inline value serialise(input_t aValue) throw() { return aValue; }
		static inline output_t deserialise(const value& aValue) throw() { return aValue; }
	};

	template<>
	struct serialiser<bool>{
		typedef bool input_t;
		typedef bool output_t;

		static inline value serialise(input_t aValue) throw() { return value(aValue); }
		static inline output_t deserialise(const value& aValue) throw() { return aValue.get_bool(); }
	};

	template<>
	struct serialiser<char> {
		typedef char input_t;
		typedef char output_t;

		static inline value serialise(input_t aValue) throw() { return value(aValue); }
		static inline output_t deserialise(const value& aValue) throw() { return aValue.get_bool(); }
	};

	template<class T>
	struct serialiser<T, typename std::enable_if<
		std::is_same<T, uint8_t>::value ||
		std::is_same<T, uint16_t>::value ||
		std::is_same<T, uint32_t>::value ||
		std::is_same<T, uint64_t>::value ||
		std::is_same<T, int8_t>::value ||
		std::is_same<T, int16_t>::value ||
		std::is_same<T, int32_t>::value ||
		std::is_same<T, int64_t>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value
	>::type> {
		typedef T input_t;
		typedef T output_t;

		static inline value serialise(input_t aValue) throw() { return value(aValue); }
		static inline output_t deserialise(const value& aValue) throw() { return static_cast<T>(aValue.get_number()); }
	};

	template<>
	struct serialiser<std::string> {
		typedef const std::string& input_t;
		typedef std::string output_t;

		static inline value serialise(input_t aValue) throw() { return value(aValue.c_str()); }
		static inline output_t deserialise(const value& aValue) throw() { return aValue.get_string(); }
	};

	template<class K, class T>
	struct serialiser<std::map<K,T>> {
		typedef const std::map<K,T>& input_t;
		typedef std::map<K,T> output_t;

		template<class K2 = K>
		static typename std::enable_if<std::is_same<K2, std::string>::value, value>::type serialise(input_t aValue) throw() {
			value tmp;
			value::object_t& val = tmp.set_object();
			for(const auto& i : aValue) {
				val.emplace(i.first, serial::serialise<T>(i.second));
			}
			return tmp;
		}

		template<class K2 = K>
		static typename std::enable_if<! std::is_same<K2, std::string>::value, value>::type serialise(input_t aValue) throw() {
			value tmp;
			value::object_t& val = tmp.set_object();
			val.emplace("keys", value());
			val.emplace("values", value());
			value::array_t& keys = val.find("keys")->second.set_array();
			value::array_t& values = val.find("values")->second.set_array();
			for(const auto& i : aValue) {
				keys.push_back(serial::serialise<K>(i.first));
				values.push_back(serial::serialise<T>(i.second));
			}
			return tmp;
		}

		template<class K2 = K>
		static typename std::enable_if<std::is_same<K2, std::string>::value, output_t>::type deserialise(const value& aValue) {
			output_t tmp;
			const value::object_t& val = aValue.get_object();
			for(const auto& i : val) {
				tmp.emplace(i.first, serial::deserialise<T>(i.second));
			}
			return tmp;
		}

		template<class K2 = K>
		static typename std::enable_if<!std::is_same<K2, std::string>::value, output_t>::type deserialise(const value& aValue) {
			output_t tmp;
			const value::object_t& val = aValue.get_object();
			const value::array_t& keys = val.find("keys")->second.get_array();
			const value::array_t& values = val.find("values")->second.get_array();
			const size_t s = keys.size();
			for(size_t i = 0; i < s; ++i) {
				tmp.emplace(serial::deserialise<K>(keys[i]), serial::deserialise<T>(values[i]));
			}
			return tmp;
		}
	};

	template<class T>
	struct serialiser<std::vector<T>> {
		typedef const std::vector<T>& input_t;
		typedef std::vector<T> output_t;

		static value serialise(input_t aValue) throw() {
			value tmp;
			value::array_t& val = tmp.set_array();
			for(const T& i : aValue) {
				val.push_back(serial::serialise<T>(i));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = aValue.get_array();
			for(const value& i : val) {
				tmp.push_back(serial::deserialise<T>(i));
			}
			return tmp;
		}
	};

	template<class T>
	struct serialiser<std::list<T>> {
		typedef const std::list<T>& input_t;
		typedef std::list<T> output_t;

		static value serialise(input_t aValue) throw() {
			value tmp;
			value::array_t& val = tmp.set_array();
			for(const T& i : aValue) {
				val.push_back(serial::serialise<T>(i));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = aValue.get_array();
			for(const value& i : val) {
				tmp.push_back(serial::deserialise<T>(i));
			}
			return tmp;
		}
	};

	template<class T>
	struct serialiser<std::deque<T>> {
		typedef const std::deque<T>& input_t;
		typedef std::deque<T> output_t;

		static value serialise(input_t aValue) throw() {
			value tmp;
			value::array_t& val = tmp.set_array();
			for(const T& i : aValue) {
				val.push_back(serial::serialise<T>(i));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = aValue.get_array();
			for(const value& i : val) {
				tmp.push_back(serial::deserialise<V>(i));
			}
			return tmp;
		}
	};

	template<class T, size_t S>
	struct serialiser<std::array<T,S>> {
		typedef const std::array<T,S>& input_t;
		typedef std::array<T,S> output_t;

		static value serialise(input_t aValue) throw() {
			value tmp;
			value::array_t& val = tmp.set_array();
			for(size_t i = 0; i < S; ++i) {
				val.push_back(serial::serialise<T>(tmp[i]));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = aValue.get_array();
			for(size_t i = 0; i < S; ++i) {
				tmp[i] = serial::deserialise<T>(val[i]);
			}
			return tmp;
		}
	};

	template<class A, class B>
	struct serialiser<std::pair<A,B>> {
		typedef const std::pair<A,B>& input_t;
		typedef std::pair<A,B> output_t;

		static value serialise(input_t aValue) throw() {
			value tmp;
			value::object_t& val = tmp.set_object();
			val.emplace("first", serial::serialise<A>(aValue.first));
			val.emplace("second", serial::serialise<B>(aValue.second));
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::object_t& val = aValue.get_object();
			tmp.first = serial::deserialise<A>(val.find("first")->second);
			tmp.second = serial::deserialise<B>(val.find("second")->second);
			return tmp;
		}
	};

}}

#endif
