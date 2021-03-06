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

#ifndef ASMITH_SERIAL_XML_HPP
#define ASMITH_SERIAL_XML_HPP
	
namespace asmith { namespace serial {
	class xml_format : public format {
		void write_serial(const value&, std::ostream&) override;
		value read_serial(std::istream&) override;
	};

	class xml_parser {
	public:
		virtual ~xml_parser() {}

		virtual void begin_element(const char*) = 0;
		virtual void end_element(const char*) = 0;

		virtual void begin_comment() = 0;
		virtual void end_comment() = 0;

		virtual void add_attribute(const char*, const char*) = 0;
		virtual void add_body(const char*) = 0;
	};

	void read_xml(xml_parser&, std::istream&);
}}

#endif