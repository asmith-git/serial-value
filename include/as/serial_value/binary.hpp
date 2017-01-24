#ifndef ASMITH_SERIAL_BINARY_HPP
#define ASMITH_SERIAL_BINARY_HPP

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

#include <iostream>
#include "serial_value.hpp"

namespace as {
	void serialise_binary(std::ostream&, const serial_value&);
	serial_value deserialise_binary(std::istream&);
}

#endif