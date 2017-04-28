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
	
namespace asmith { namespace serial {
	
	class xml_parser {
	public:
		virtual ~xml_parser(){}

		virtual void begin_element(const char*) = 0;
		virtual void end_element(const char*) = 0;

		virtual void begin_comment() = 0;
		virtual void end_comment() = 0;

		virtual void add_attribute(const char*, const char*) = 0;
		virtual void add_body(const char*) = 0;
	};
	
	static const std::string XML_ILLEGAL[5][2] = {
		{"&",   "&amp;"},
		{"<",   "&lt;"},
		{">",   "&gt;"},
		{"\"",  "&quot;"},
		{"'",   "&apos;"}
	};

	static void xml_string_replace(std::string& aStr, const std::string& aTarget, const std::string& aReplacement) {
	    size_t i = aStr.find(aTarget);
	    while(i != std::string::npos) {
		aStr.replace(i, aTarget.size(), aReplacement);
		i = aStr.find(aTarget, i+aReplacement.size());
	    }
	}

	static void xml_encode_string(std::string& aStr) {
	    for(const std::string i[2] : XML_ILLEGAL) {
		xml_string_replace(aStr, i[0], i[1]);
	    }
	}

	static void xml_decode_string(std::string& aStr) {
	    for(const std::string i[2] : XML_ILLEGAL) {
		xml_string_replace(aStr, i[1], i[0]);
	    }
	}

	//! \todo Remove illegal characters in xml

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
			aStream << aType.get_string();
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
			aStream << aType.get_string();
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
		//! \todo Implement
		return value();
	}
}}
