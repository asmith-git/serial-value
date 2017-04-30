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

#include "asmith/serial/json.hpp"
#include "asmith/serial/string_tools.hpp"
	
namespace asmith { namespace serial {

	value::type json_determine_type(std::istream& aStream) {
		skip_whitespace(aStream);
		char c = aStream.peek();
		switch(c) {
		case 'n':
			return value::NULL_T;
		case 't':
		case 'f':
			return value::BOOl_T;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
		case '+':
			return value::NUMBER_T;
		case '"':
			return value::STRING_T;
		case '[':
			return value::ARRAY_T;
		case '{':
			return value::OBJECT_T;
		default:
			throw std::runtime_error("asmith::json_format::read_serial : Could not determin JSON type");
		}
	}

	value json_read_value(std::istream&);

	value json_read_null(std::istream& aStream) {
		skip_whitespace(aStream);

		char buf[4];
		aStream.read(buf, 4);
		if(memcmp(buf, "null", 4) != 0) throw std::runtime_error("asmith::json_format::read_serial : Expected 'null'");
		return value();
	}

	value json_read_bool(std::istream& aStream) {
		skip_whitespace(aStream);

		char buf[5];
		aStream.read(buf, 4);
		if(memcmp(buf, "true", 4) == 0) {
			return value(true);
		}else if(memcmp(buf, "fals", 4) == 0) {
			aStream.read(buf, 1);
			if(buf[0] == 'e') return value(false);
		}
		throw std::runtime_error("asmith::json_format::read_serial : Expected 'true' or 'false'");
	}

	value json_read_number(std::istream& aStream) {
		skip_whitespace(aStream);

		static const auto is_number = [](const char c)->bool {
			return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.' || c == 'e' || c == 'E';
		};

		char buf[32];
		size_t s = 0;
		char c = aStream.peek();
		while(is_number(c)) {
			buf[s++] = c;
			aStream.read(&c, 1);
			c = aStream.peek();
		}
		buf[s] = '\0';
		return value(atof(buf));
	}

	value json_read_string(std::istream& aStream) {
		skip_whitespace(aStream);

		char c = aStream.peek();
		if(c != '"') throw std::runtime_error("asmith::json_format::read_serial : Expected string to begin with '\"'");
		aStream.read(&c, 1);

		value tmp;
		value::string_t& str = tmp.set_string();

		c = aStream.peek();
		while(c != '"') {
			str += c;
			aStream.read(&c, 1);
			if(aStream.eof()) throw std::runtime_error("asmith::json_format::read_serial : Expected string to end with '\"'");
			c = aStream.peek();
		}
		aStream.read(&c, 1);

		return tmp;
	}

	value json_read_array(std::istream& aStream) {
		skip_whitespace(aStream);

		char c = aStream.peek();
		if(c != '[') throw std::runtime_error("asmith::json_format::read_serial : Expected array to begin with '['");
		aStream.read(&c, 1);
		skip_whitespace(aStream);

		value tmp;
		value::array_t& array_ = tmp.set_array();

		c = aStream.peek();
		while(c != ']') {
			if(aStream.eof()) throw std::runtime_error("asmith::json_format::read_serial : Expected array to end with ']'");
			array_.push_back(json_read_value(aStream));
			skip_whitespace(aStream);
			c = aStream.peek();
			if(c == ']') break;
			else if(c != ',') throw std::runtime_error("asmith::json_format::read_serial : Expected array elements to be seperated with ','");
			aStream.read(&c, 1);
			skip_whitespace(aStream);
		}
		aStream.read(&c, 1);

		return tmp;
	}

	value json_read_object(std::istream& aStream) {
		skip_whitespace(aStream);

		char c = aStream.peek();
		if(c != '{') throw std::runtime_error("asmith::json_format::read_serial : Expected object to begin with '{'");
		aStream.read(&c, 1);
		skip_whitespace(aStream);

		value tmp;
		value::object_t& object = tmp.set_object();

		c = aStream.peek();
		while(c != '}') {
			if(aStream.eof()) throw std::runtime_error("asmith::json_format::read_serial : Expected object to end with '}'");
			const value::string_t name = json_read_string(aStream).get_string();
			skip_whitespace(aStream);
			aStream.read(&c, 1);
			if(c != ':') throw std::runtime_error("asmith::json_format::read_serial : Expected object name to be end with ':'");
			skip_whitespace(aStream);
			object.emplace(name, json_read_value(aStream));
			skip_whitespace(aStream);
			c = aStream.peek();
			if(c == '}') break;
			else if(c != ',') throw std::runtime_error("asmith::json_format::read_serial : Expected object elements to be seperated with ','");
			aStream.read(&c, 1);
			skip_whitespace(aStream);
		}
		aStream.read(&c, 1);

		return tmp;
	}

	value json_read_value(std::istream& aStream) {
		switch(json_determine_type(aStream)) {
		case value::NULL_T:
			return json_read_null(aStream);
		case value::BOOl_T:
			return json_read_bool(aStream);
		case value::NUMBER_T:
			return json_read_number(aStream);
		case value::STRING_T:
			return json_read_string(aStream);
		case value::ARRAY_T:
			return json_read_array(aStream);
		case value::OBJECT_T:
			return json_read_object(aStream);
		default:
			throw std::runtime_error("asmith::json_format::read_serial : Could not determin JSON type");
		}
	}

	// json_format


	json_format::json_format() :
		mFancy(false)
	{}

	json_format& json_format::set_fancy_writing(const bool aOption) {
		mFancy = aOption;
		return *this;
	}

	void json_format::write_serial_internal(size_t aDepth, const value& aType, std::ostream& aStream) {
		const auto indent = [&]()->void {
			for(size_t i = 0; i < aDepth; ++i) aStream << '\t';
		};
		const value::type tBuf = aType.get_type();

		switch(tBuf) {
		case value::NULL_T:
			aStream << "null";
			break;
		case value::BOOl_T:
			aStream << (aType.get_bool() ? "true" : "false");
			break;
		case value::CHAR_T:
			aStream << '"' << aType.get_char() << '"';
			break;
		case value::NUMBER_T:
			aStream << aType.get_number();
			break;
		case value::STRING_T:
			aStream << '"' << aType.get_string() << '"';
			break;
		case value::ARRAY_T:
			{
				aStream << '[';
				if(mFancy) aStream << '\n';
				++aDepth;
				const value::array_t tmp = aType.get_array();
				const size_t s = tmp.size();
				for(size_t i = 0; i < s; ++i) {
					if(mFancy) indent();
					write_serial_internal(aDepth, tmp[i], aStream);
					if(i + 1 < s) aStream << ',';
					if(mFancy) aStream << '\n';
				}
				--aDepth;
				if(mFancy) indent();
				aStream << ']';
			}
			break;
		case value::OBJECT_T:
		{
			aStream << '{';
			if(mFancy) aStream << '\n';
			++aDepth;
			const value::object_t tmp = aType.get_object();
			const size_t s = tmp.size();
			size_t i = 0;
			for(const auto& v : tmp) {
				if(mFancy) indent();
				aStream << '"' << v.first<< '"' << ':';
				write_serial_internal(aDepth, v.second, aStream);
				if(i + 1 < s) aStream << ',';
				if(mFancy) aStream << '\n';
				++i;
			}
			--aDepth;
			if(mFancy) indent();
			aStream << '}';
		}
		break;
		default:
			throw std::runtime_error("json_format : Invalid serial type");
			break;
		}
	}

	void json_format::write_serial(const value& aType, std::ostream& aStream) {
		write_serial_internal(0, aType, aStream);
	}

	value json_format::read_serial(std::istream& aStream) {
		return json_read_value(aStream);
	}
}}