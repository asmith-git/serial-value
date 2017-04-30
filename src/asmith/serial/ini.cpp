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

#include "asmith/serial/ini.hpp"
#include <sstream>
#include "asmith/serial/string_tools.hpp"
	
namespace asmith { namespace serial {

	// ini_format

	void ini_format::write_ini(const std::string& aParentName, const std::string& aName, const value& aValue, std::ostream& aStream) {
		switch(aValue.get_type()) {
		case value::NULL_T:
			aStream << aName << mNameSeperator << "null" << std::endl;
			break;
		case value::BOOL_T:
			aStream << aName << mNameSeperator << (aValue.get_bool() ? "true" : "false") << std::endl;
			break;
		case value::NUMBER_T:
			aStream << aName << mNameSeperator << aValue.get_number() << std::endl;
			break;
		case value::STRING_T:
			aStream << aName << mNameSeperator << aValue.get_string() << std::endl;
			break;
		case value::ARRAY_T:
			{
				const value::array_t& array_ = aValue.get_array();
				const std::string parentName = aParentName == "" ? aName : aParentName + mHierarchySeperator + aName;
				if(parentName != "") aStream << '[' << parentName << ']' << std::endl;
				size_t j = 0;
				for(const auto& i : array_) {
					const value::type t = i.get_type();
					if(t != value::ARRAY_T && t != value::OBJECT_T) write_ini(parentName, std::to_string(j), i, aStream);
					++j;
				}
				j = 0;
				for(const auto& i : array_) {
					const value::type t = i.get_type();
					if(t == value::ARRAY_T || t == value::OBJECT_T) write_ini(parentName, std::to_string(j), i, aStream);
					++j;
				}
			}
			break;
		case value::OBJECT_T:
			{
				const value::object_t& object = aValue.get_object();
				const std::string parentName = aParentName == "" ? aName : aParentName + mHierarchySeperator + aName;
				if(parentName != "") aStream << '[' << parentName << ']' << std::endl;
				for(const auto& i : object) {
					const value::type t = i.second.get_type();
					if(t != value::ARRAY_T && t != value::OBJECT_T) write_ini(parentName, i.first, i.second, aStream);
				}
				for(const auto& i : object) {
					const value::type t = i.second.get_type();
					if(t == value::ARRAY_T || t == value::OBJECT_T) write_ini(parentName, i.first, i.second, aStream);
				}
			}
			break;
		default:
			throw 0;
		}
	}

	ini_format::ini_format() :
		mHierarchySeperator('.'),
		mNameSeperator('='),
		mCommentSymbol(';')
	{}

	ini_format& ini_format::set_hierarchy_seperator(char aSeperator) {
		mHierarchySeperator = aSeperator;
		return *this;
	}

	ini_format& ini_format::set_name_seperator(char aSeperator) {
		mNameSeperator = aSeperator;
		return *this;
	}

	ini_format& ini_format::set_comment_symbol(char aSymbol) {
		mCommentSymbol = aSymbol;
		return *this;
	}

	void ini_format::write_serial(const value& aValue, std::ostream& aStream) {
		write_ini("", "", aValue, aStream);
	}
	
	value ini_format::read_serial(std::istream& aStream) {
		////! \todo Handle comments
		////! \todo Handle escape characters
		////! \todo Option for quoted strings

		value root;
		value::object_t& rootObject = root.set_object();

		//std::vector<std::pair<std::string, std::string>> values;
		std::string section = "";
		std::string buf;
		char c;

		const auto convert_to_serial = [&](std::string aKey, const std::string aValue) {
			value* head = &root;

			while(true) {
				auto j = aKey.find(mHierarchySeperator);
				if(j == 0) {
					if(head->get_type() != value::OBJECT_T) head->set_object();
					value::object_t& object = head->get_object();
					aKey.erase(aKey.begin());
					object.emplace(aKey, value(aValue));
					break;
				}else if (j == std::string::npos) {
					if(head->get_type() != value::OBJECT_T) head->set_object();
					value::object_t& object = head->get_object();
					object.emplace(aKey, value(aValue));
					break;
				}else {
					const std::string next = aKey.substr(0, j);
					value::object_t& object = head->get_object();
					auto k = object.find(next);
					if(k == object.end()) {
						head = &object.emplace(next, value()).first->second;
					}else {
						head = &k->second;
					}
					aKey.erase(0, j + 1);
				}
			}
		};

		// Read sections
		while(!aStream.eof()) {
			std::getline(aStream, buf);
			if (!aStream) break;
			std::stringstream line;
			line << buf;

			skip_whitespace(line);
			c = line.peek();
			if(c == '[') {
				line.read(&c, 1);
				c = line.peek();

				section = "";
				while(c != ']') {
					section += c;
					line.read(&c, 1);
					c = line.peek();
				}
			}else {
				std::string name;
				std::string value;
				bool comment = false;
				while(c != '=') {
					if(c == mCommentSymbol) {
						comment = true;
						break;
					}
					name += c;
					line.read(&c, 1);
					c = line.peek();
				}

				if(comment) continue;
				line.read(&c, 1);
				c = line.peek();
				while(! line.eof()) {
					if(c == mCommentSymbol) {
						comment = true;
						break;
					}
					value += c;
					line.read(&c, 1);
					c = line.peek();
				}

				if(comment) continue;
				convert_to_serial(section + mHierarchySeperator + name, value);

			}
		}

		return root;
	}
}}