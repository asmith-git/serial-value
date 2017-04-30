## Features
1. Semi-automated serialisation for C++ classes
2. JSON Support
3. XML Support

## Serialization of C++ Classes
```C++
using namespace asmith;

// This is the class we want to serialise
struct objective_function {
	std::string name;
	std::vector<float> lower_bounds;
	std::vector<float> upper_bounds;
	uint32_t dimensions;
	bool minimise;
};

// This is the class that handles conversion to and from an intermediary serialisation format 
template<>
struct asmith::serial::serialiser<objective_function> {
	typedef const objective_function& input_t;  // This is the type that is converted
	typedef objective_function output_t;        // This is the type that is produced from deserialisation

  	// This function serialises the class
	static inline value serialise(input_t aValue) throw() { 
		value tmp;
		value::object_t& object = tmp.set_object();
		#define auto_serialise(output, input) object.emplace(output, serial::serialise<decltype(input)>(input));
			auto_serialise("name",		  aValue.name)
			auto_serialise("lower_bounds",	  aValue.lower_bounds)
			auto_serialise("upper_bounds",	  aValue.upper_bounds)
			auto_serialise("dimensions",	  aValue.dimensions)
			auto_serialise("minimise",	  aValue.minimise)
		#undef auto_serialise
		return tmp;
	}

  	// This function deserialises the class
	static inline output_t deserialise(const value& aValue) throw() { 
		output_t tmp;
		const value::object_t& object = aValue.get_object();
		#define auto_deserialise(input, output) output = serial::deserialise<decltype(output)>(object.find(input)->second);
			auto_deserialise("name",          tmp.name)
			auto_deserialise("lower_bounds",  tmp.lower_bounds)
			auto_deserialise("upper_bounds",  tmp.upper_bounds)
			auto_deserialise("dimensions",    tmp.dimensions)
			auto_deserialise("minimise",      tmp.minimise)
		#undef auto_deserialise
		return tmp;
	}
};
```

## Serialization of C++ Classes
```C++
using namespace asmith;

// Create an object that should be serialised
objective_function myObject;

// Write the object into a binary file
binary_format().write<objective_function>(myObject, std::ofstream("myObject.bin"));

// Write the object into a JSON file
json_format().write<objective_function>(myObject, std::ofstream("myObject.json"));

// Write the object into a XML file
xml_format().write<objective_function>(myObject, std::ofstream("myObject.xml"));

// Read the object from a binary file
objective_function binaryObject = binary_format().read<objective_function>(std::ifstream("myObject.bin"));

// Read the object from a JSON file
objective_function jsonObject = json_format().read<objective_function>(std::ifstream("myObject.json"));

// Read the object from a XML file
objective_function xmlObject = xml_format().read<objective_function>(std::ifstream("myObject.xml"));

```
