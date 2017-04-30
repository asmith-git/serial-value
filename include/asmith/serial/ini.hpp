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

#include "format.hpp"

#ifndef ASMITH_SERIAL_INI_HPP
#define ASMITH_SERIAL_INI_HPP

namespace asmith {
	namespace serial {
		class ini_format : public format {
		private:
			char mHierarchySeperator;
			char mNameSeperator;
			char mCommentSymbol;

			void write_ini(const std::string&, const std::string&, const value&, std::ostream&);
		public:
			ini_format();

			ini_format& set_hierarchy_seperator(char);
			ini_format& set_name_seperator(char);
			ini_format& set_comment_symbol(char);

			// Inherited from format

			void write_serial(const value&, std::ostream&) override;
			value read_serial(std::istream&) override;
		};
	}
}

#endif