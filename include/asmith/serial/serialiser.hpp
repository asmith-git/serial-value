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

		static value serialise(input_t) throw() = delete;
		static output_t deserialise(const value&) throw() = delete;
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
				val.emplace(i.first, ::serialise<T>(i.second));
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
				keys.push_back(::serialise<K>(i.first));
				values.push_back(::serialise<T>(i.second));
			}
			return tmp;
		}

		template<class K2 = K>
		static typename std::enable_if<std::is_same<K2, std::string>::value, output_t>::type deserialise(const value& aValue) {
			output_t tmp;
			const value::object_t& val = aValue.get_object();
			for(const auto& i : val) {
				tmp.emplace(i.first, ::deserialise<T>(i.second));
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
				tmp.emplace(::deserialise<K>(keys[i]), ::deserialise<T>(values[i]));
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
				val.push_back(::serialise<T>(i));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = tmp.get_array();
			for(const value& i : val) {
				tmp.push_back(::deserialise<T>(i));
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
				val.push_back(::serialise<T>(i));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = tmp.get_array();
			for(const value& i : val) {
				tmp.push_back(::deserialise<T>(i));
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
				val.push_back(::serialise<T>(i));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = tmp.get_array();
			for(const value& i : val) {
				tmp.push_back(::deserialise<V>(i));
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
				val.push_back(::serialise<T>(tmp[i]));
			}
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::array_t& val = tmp.get_array();
			for(size_t i = 0; i < S; ++i) {
				tmp[i] = ::deserialise<T>(val[i]);
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
			val.emplace("first", ::serialise<A>(aValue.first));
			val.emplace("second", ::serialise<B>(aValue.second));
			return tmp;
		}

		static output_t deserialise(const value& aValue) {
			output_t tmp;
			const value::object_t& val = tmp.get_object();
			tmp.first = ::deserialise<A>(val.find("first")->second);
			tmp.second = ::deserialise<B>(val.find("second")->second);
			return tmp;
		}
	};

}}

#endif