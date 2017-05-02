## Features
1. Semi-automated serialisation for C++ classes
2. JSON Support
3. XML Support
4. INI Support

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

// Define macros that simplify the serialisation
#define MACRO_JOIN2(a, b) a##b
#define MACRO_JOIN(a, b) MACRO_JOIN2(a , b)

#define SERIALISE_BEGIN value tmp; value::object_t& object = tmp.set_object();
#define SERIALISE_END return tmp
#define SERIALISE_MEMBER(name) object.emplace(#name, serial::serialise<decltype(MACRO_JOIN(aValue.,name))>(MACRO_JOIN(aValue.,name)))

#define DESERIALISE_BEGIN output_t tmp; const value::object_t& object = aValue.get_object();
#define DESERIALISE_END return tmp
#define DESERIALISE_MEMBER(name) MACRO_JOIN(tmp.,name) = serial::deserialise<decltype(MACRO_JOIN(tmp.,name))>(object.find(#name)->second)

// This is the class that handles conversion to and from an intermediary serialisation format 
template<>
struct asmith::serial::serialiser<objective_function> {
	typedef const objective_function& input_t;  // This is the type that is converted
	typedef objective_function output_t;        // This is the type that is produced from deserialisation

  	// This function serialises the class
	static inline value serialise(input_t aValue) throw() { 
		SERIALISE_BEGIN;
		SERIALISE_MEMBER(name);
		SERIALISE_MEMBER(lower_bounds);
		SERIALISE_MEMBER(upper_bounds);
		SERIALISE_MEMBER(dimensions);
		SERIALISE_MEMBER(minimise);
		SERIALISE_END;
	}

  	// This function deserialises the class
	static inline output_t deserialise(const value& aValue) throw() { 
		DESERIALISE_BEGIN;
		DESERIALISE_MEMBER(name);
		DESERIALISE_MEMBER(lower_bounds);
		DESERIALISE_MEMBER(upper_bounds);
		DESERIALISE_MEMBER(dimensions);
		DESERIALISE_MEMBER(minimise);
		DESERIALISE_END;
	}
};
```

## Serialization of C++ Classes
```C++
using namespace asmith;

// Create an object that should be serialised
objective_function myObject;
myObject.name = "De Jong 1";
myObject.lower_bounds = { -5.12f, -5.12f };
myObject.upper_bounds = { 5.12f, 5.12f };
myObject.dimensions = 2;
myObject.minimise = true;

// Write the object into a binary file
binary_format().write<objective_function>(myObject, std::ofstream("myObject.bin"));

// Write the object into a JSON file
json_format().write<objective_function>(myObject, std::ofstream("myObject.json"));

// Write the object into a XML file
xml_format().write<objective_function>(myObject, std::ofstream("myObject.xml"));

// Write the object into a INI file
ini_format().write<objective_function>(myObject, std::ofstream("myObject.ini"));

// Read the object from a binary file
objective_function binaryObject = binary_format().read<objective_function>(std::ifstream("myObject.bin"));

// Read the object from a JSON file
objective_function jsonObject = json_format().read<objective_function>(std::ifstream("myObject.json"));

// Read the object from a XML file
objective_function xmlObject = xml_format().read<objective_function>(std::ifstream("myObject.xml"));

// Read the object from a INI file
objective_function iniObject = ini_format().read<objective_function>(std::ifstream("myObject.ini"));
```

## Sample JSON Output
```C++
{
	"dimensions":2,
	"lower_bounds":[
		-5.12,
		-5.12
	],
	"minimise":true,
	"name":"De Jong 1",
	"upper_bounds":[
		5.12,
		5.12
	]
}
```

## Sample XML Output
```C++
<xml dimensions="2" minimise="true" name="De Jong 1">
	<lower_bounds>
		<0>-5.12</0>
		<1>-5.12</1>
	</lower_bounds>
	<upper_bounds>
		<0>5.12</0>
		<1>5.12</1>
	</upper_bounds>
</xml>
```

## Sample INI Output
```C++
dimensions=2
minimise=true
name=De Jong 1
[lower_bounds]
0=-5.12
1=-5.12
[upper_bounds]
0=5.12
1=5.12
```
