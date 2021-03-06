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

#include <string>
#include <iostream>

#ifndef ASMITH_SERIAL_STRING_TOOLS_HPP
#define ASMITH_SERIAL_STRING_TOOLS_HPP
	
namespace asmith { namespace serial {
	void string_replacement(std::string&, const std::string&, const std::string&);
	void skip_whitespace(std::istream&);
}}

#endif