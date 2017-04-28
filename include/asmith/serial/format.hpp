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

#include "serialiser.hpp"

#ifndef ASMITH_SERIAL_FORMAT_HPP
#define ASMITH_SERIAL_FORMAT_HP
	
namespace asmith { namespace serial {
	class format {
	public:
		virtual ~format() {}

		virtual void write_serial(const value&, std::ostream&) = 0;
		virtual value read_serial(std::istream&) = 0;
	
		template<class T>
		void write(const T& aValue, std::ostream& aStream) {
			write_serial(serialise<T>(aValue), aStream);
		}
	
		template<class T>
		T read(std::istream& aStream) {
			return deserialise<T>(read_serial(aStream));
		}
	};
}}

#endif
