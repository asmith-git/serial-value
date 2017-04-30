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

#include "asmith/serial/xml.hpp"
#include <cctype>
#include "asmith/serial/string_tools.hpp"
	
namespace asmith { namespace serial {
	
	static const std::string XML_ILLEGAL[5][2] = {
		{"&",   "&amp;"},
		{"<",   "&lt;"},
		{">",   "&gt;"},
		{"\"",  "&quot;"},
		{"'",   "&apos;"}
	};

	void xml_encode_string(std::string& aStr) {
	    for(const std::string* i : XML_ILLEGAL) {
			string_replacement(aStr, i[0], i[1]);
	    }
	}

	void xml_decode_string(std::string& aStr) {
	    for(const std::string* i : XML_ILLEGAL) {
			string_replacement(aStr, i[1], i[0]);
	    }
	}

	void read_xml(xml_parser& aParser, std::istream& aStream) {
		char c;
		char nameBuf[256];
		size_t nameLength = 0;

		// Open tag
		skip_whitespace(aStream);
		c = aStream.peek();
		if(c != '<') throw std::runtime_error("asmith::serial::read_xml : Expected tag to begin with '<'");
		aStream.read(&c, 1);

		// Read name
		skip_whitespace(aStream);
		c = aStream.peek();
		while(! (std::isspace(c) || c == '/' || c == '>')) {
			if(nameLength >= 256) throw std::runtime_error("asmith::serial::read_xml : Element name too long");
			aStream.read(&c, 1);
			nameBuf[nameLength++] = c;
			c = aStream.peek();
		}
		nameBuf[nameLength] = '\0';
		aParser.begin_element(nameBuf);

		// Read attributes
		char attribName[256];
		char attribValue[2048];
		size_t attribNameLength = 0;
		size_t attibValueLength = 0;

		skip_whitespace(aStream);
		c = aStream.peek();
		while(c != '/' && c != '>') {
			// Read name
			while(c != '=') {
				if(attribNameLength >= 256) throw std::runtime_error("asmith::serial::read_xml : Attribute name too long");

				attribName[attribNameLength++] = c;
				aStream.read(&c, 1);
				c = aStream.peek();
			}
			attribName[attribNameLength] = '\0';
			aStream.read(&c, 1);

			// Read value
			skip_whitespace(aStream);
			c = aStream.peek();
			if(c != '"') throw std::runtime_error("asmith::serial::read_xml : Expected attribute to begin with '\"'");
			aStream.read(&c, 1);

			while (c != '>') {
				if(attibValueLength >= 2048) throw std::runtime_error("asmith::serial::read_xml : Attribute value too long");
				attribValue[attibValueLength++] = c;
				aStream.read(&c, 1);
				c = aStream.peek();
			}
			attribValue[attibValueLength] = '\0';
			if (c != '"') throw std::runtime_error("asmith::serial::read_xml : Expected attribute to end with '\"'");
			aStream.read(&c, 1);

			// Parse attribute
			aParser.add_attribute(attribName, attribValue);
			attribNameLength = 0;
			attibValueLength = 0;
		}

		// Close tag
		skip_whitespace(aStream);
		c = aStream.peek();
		if(c == '/') {
			aStream.read(&c, 1);
			c = aStream.peek();
			aStream.read(&c, 1);
			if(c != '>') throw std::runtime_error("asmith::serial::read_xml : Expected tag to end with '>'");
			aParser.end_element(nameBuf);
			return;
		}else if (c == '>') {
			aStream.read(&c, 1);
		}

		skip_whitespace(aStream);
		c = aStream.peek();
		if(c == '<') {
			// Read child elements
			while(true){
				aStream.read(&c, 1);
				c = aStream.peek();
				aStream.putback('<');
				if(c == '/') {
					break;
				}else {
					read_xml(aParser, aStream);
				}
			}
		}else {
			// Read body
			std::string body;

			while (c != '>') {
				body += c;
				aStream.read(&c, 1);
				c = aStream.peek();
			}

			aParser.add_body(body.c_str());
		}

		// Open tag

		// Read name

		// Close tag
		aParser.end_element(nameBuf);
	}

	static void xml_write_internal(const value& aType, std::ostream& aStream) {
		const value::type tBuf = aType.get_type();

		switch(tBuf) {
		case value::NULL_T:
			aStream << "null";
			break;
		case value::BOOl_T:
			aStream << (aType.get_bool() ? "true" : "false");
			break;
		case value::CHAR_T:
			aStream << aType.get_char() ;
			break;
		case value::NUMBER_T:
			aStream << aType.get_number();
			break;
		case value::STRING_T:
			{
				std::string str = aType.get_string();
				xml_encode_string(str);
				aStream << str;
			}
			break;
		case value::ARRAY_T:
			throw std::runtime_error("xml_format : Cannot write array as internal value");
			break;
		case value::OBJECT_T:
			throw std::runtime_error("xml_format : Cannot write object as internal value");
			break;
		default:
			throw std::runtime_error("xml_format : Invalid serial type");
			break;
		}
	}

	static void xml_write_element(const char* const aName, const value& aType, std::ostream& aStream) {
		bool closeTag = true;
		aStream << "<" << aName;
		const value::type tBuf = aType.get_type();

		switch(tBuf) {
		case value::NULL_T:
			aStream << "/>";
			closeTag = false;
			break;
		case value::BOOl_T:
			aStream << ">";
			aStream << (aType.get_bool() ? "true" : "false");
			break;
		case value::CHAR_T:
			aStream << ">";
			aStream << aType.get_char();
			break;
		case value::NUMBER_T:
			aStream << ">";
			aStream << aType.get_number();
			break;
		case value::STRING_T:
			aStream << ">";			
			{
				std::string str = aType.get_string();
				xml_encode_string(str);
				aStream << str;
			}
			break;
		case value::ARRAY_T:
			aStream << ">";
			{
				const value::array_t tmp = aType.get_array();
				const size_t s = tmp.size();
				for(size_t i = 0; i < s; ++i) {
					xml_write_element(std::to_string(i).c_str(), tmp[i], aStream);
				}
			}
			break;
		case value::OBJECT_T:
			{
				const value::object_t tmp = aType.get_object();
				const size_t s = tmp.size();

				for(const auto& v : tmp) {
					const value::type t = v.second.get_type();
					if(t != value::ARRAY_T && t != value::OBJECT_T) {
						aStream << ' ' << v.first << '=' << '"';
						xml_write_internal(v.second, aStream);
						aStream << '"';
					}
				}

				aStream << ">";

				for (const auto& v : tmp) {
					const value::type t = v.second.get_type();
					if(t == value::ARRAY_T || t == value::OBJECT_T) {
						xml_write_element(v.first.c_str(), v.second, aStream);
					}
				}
			}
			break;
		default:
			throw std::runtime_error("xml_format : Invalid serial type");
			break;
		}
		if(closeTag) aStream << "</" << aName << ">";
	}

	void xml_format::write_serial(const value& aType, std::ostream& aStream) {
		xml_write_element("xml", aType, aStream);
	}

	value xml_format::read_serial(std::istream& aStream) {
		class serial_xml_parser : public xml_parser {
		private:
			std::vector<value*> mValueStack;
			std::vector<std::string> mElementName;
		public:
			value root;
			
			serial_xml_parser() {
				mValueStack.push_back(&root);
			}
			
			// Inherited from xml_parser

			void begin_element(const char* aName) override {
				mElementName.push_back(aName);
			}
			
			void end_element(const char*) override {
				mElementName.pop_back();
			}
			
			void begin_comment() override {
				
			}
			
			void end_comment() override {
				
			}
			
			void add_attribute(const char* aName, const char* aValue) override {
				value::object_t& object = mValueStack.back()->get_object();
				object.emplace(aName, value(aValue));
			}
			
			void add_body(const char*) override {
				
			}
		};

		serial_xml_parser parser;
		read_xml(parser, aStream);
		return parser.root;
	}
}}
