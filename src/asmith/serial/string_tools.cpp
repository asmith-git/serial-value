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

#include "asmith/serial/string_tools.hpp"
#include <cctype>
	
namespace asmith { namespace serial {

	void string_replacement(std::string& aStr, const std::string& aTarget, const std::string& aReplacement) {
	    size_t i = aStr.find(aTarget);
	    while(i != std::string::npos) {
			aStr.replace(i, aTarget.size(), aReplacement);
			i = aStr.find(aTarget, i+aReplacement.size());
	    }
	}

	void skip_whitespace(std::istream& aStream) {
		char c = aStream.peek();
		while (std::isspace(c)) {
			aStream.read(&c, 1);
			c = aStream.peek();
		}
	}

}}
