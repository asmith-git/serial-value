#ifndef ASMITH_SERIALISER_HPP
#define ASMITH_SERIALISER_HPP

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

#include <deque>
#include <list>
#include "serial_value.hpp"

namespace as {

	template<class T, class ENABLE = void>
	struct serialiser {
		typedef T serialise_t;
		typedef T deserialise_t;

		static serial_value serialise(serialise_t);
		static deserialise_t deserialise(const serial_value&);
	};

	// -- Specialisations

	template<>
	struct serialiser<char, void> {
		typedef char serialise_t;
		typedef char deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			return serial_value(aValue);
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			return aValue.get_char();
		}
	};

	template<>
	struct serialiser<bool, void> {
		typedef bool serialise_t;
		typedef bool deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			return serial_value(aValue);
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			return aValue.get_bool();
		}
	};

	template<class T>
	struct serialiser<T, typename std::enable_if<
		std::is_same<T, uint8_t>::value ||
		std::is_same<T, uint16_t>::value ||
		std::is_same<T, uint32_t>::value ||
		std::is_same<T, uint64_t>::value
	>::type> {
		typedef T serialise_t;
		typedef T deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			return serial_value(aValue);
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			return static_cast<deserialise_t>(aValue.get_unsigned());
		}
	};

	template<class T>
	struct serialiser<T, typename std::enable_if<
		std::is_same<T, int8_t>::value ||
		std::is_same<T, int16_t>::value ||
		std::is_same<T, int32_t>::value ||
		std::is_same<T, int64_t>::value
	>::type> {
		typedef T serialise_t;
		typedef T deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			return serial_value(aValue);
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			return static_cast<deserialise_t>(aValue.get_signed());
		}
	};

	template<class T>
	struct serialiser<T, typename std::enable_if<
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value
	>::type> {
		typedef T serialise_t;
		typedef T deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			return serial_value(aValue);
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			return static_cast<deserialise_t>(aValue.get_float());
		}
	};

	template<>
	struct serialiser<std::string, void> {
		typedef const std::string& serialise_t;
		typedef std::string deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			return serial_value(aValue);
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			return aValue.get_string();
		}
	};

	// -- Container specialisations

	template<class T>
	struct serialiser<std::vector<T>, void> {
		typedef const std::vector<T>& serialise_t;
		typedef std::vector<T> deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			serial_value tmp;
			std::vector<serial_value>& values = tmp.set_array();
			for(const T& i : aValue) values.push_back(serialiser<T>::serialise(i));
			return tmp;
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			const std::vector<serial_value>& values = aValue.get_array();
			deserialise_t tmp;
			for(const serial_value& i : values) tmp.push_back(serialiser<T>::deserialise(i));
			return tmp;
		}
	};

	template<class T>
	struct serialiser<std::deque<T>, void> {
		typedef const std::deque<T>& serialise_t;
		typedef std::vector<T> deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			serial_value tmp;
			std::vector<serial_value>& values = tmp.set_array();
			for(const T& i : aValue) values.push_back(serialiser<T>::serialise(i));
			return tmp;
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			const std::vector<serial_value>& values = aValue.get_array();
			deserialise_t tmp;
			for(const serial_value& i : values) tmp.push_back(serialiser<T>::deserialise(i));
			return tmp;
		}
	};

	template<class T>
	struct serialiser<std::list<T>, void> {
		typedef const std::list<T>& serialise_t;
		typedef std::list<T> deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			serial_value tmp;
			std::vector<serial_value>& values = tmp.set_array();
			for(const T& i : aValue) values.push_back(serialiser<T>::serialise(i));
			return tmp;
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			const std::vector<serial_value>& values = aValue.get_array();
			deserialise_t tmp;
			for(const serial_value& i : values) tmp.push_back(serialiser<T>::deserialise(i));
			return tmp;
		}
	};

	template<class T>
	struct serialiser<std::map<std::string,T>, void> {
		typedef const std::map<std::string,T>& serialise_t;
		typedef std::map<std::string,T> deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			serial_value tmp;
			std::map<serial_value::string_t,serial_value>& values = tmp.set_object();
			for(const auto& i : aValue) values.emplace(i.first, serialiser<T>::serialise(i.second));
			return tmp;
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			const std::map<serial_value::string_t, serial_value>& values = aValue.get_object();
			deserialise_t tmp;
			for(const auto& i : values) tmp.emplace(i.first, serialiser<T>::deserialise(i.second));
			return tmp;
		}
	};

	template<class K, class T>
	struct serialiser<std::map<K,T>, void> {
		typedef const std::map<K,T>& serialise_t;
		typedef std::map<K,T> deserialise_t;

		static serial_value serialise(serialise_t aValue) {
			serial_value tmp;
			std::map<serial_value::string_t, serial_value>& parent = tmp.set_object();
			parent.emplace("keys", serial_value());
			parent.emplace("values", serial_value());
			std::vector<serial_value>& keys = parent.find("keys")->second.set_array();
			std::vector<serial_value>& values = parent.find("values")->second.set_array();

			for(const auto& i : aValue) {
				keys.push_back(serialiser<K>::serialise(i.first));
				values.push_back(serialiser<T>::serialise(i.second));
			}
			return tmp;
		}

		static deserialise_t deserialise(const serial_value& aValue) {
			const std::vector<serial_value>& keys = aValue["keys"].get_array();
			const std::vector<serial_value>& values = aValue["values"].get_array();
			const size_t size = keys.size();
			deserialise_t tmp;

			for(size_t i = 0; i < size; ++i) {
				tmp.emplace(
					serialiser<K>::deserialise(keys[i]),
					serialiser<T>::deserialise(values[i])
				);
			}

			return tmp;
		}
	};
}

#endif