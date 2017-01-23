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
		typedef bool serialise_t;
		typedef bool deserialise_t;

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
}

#endif