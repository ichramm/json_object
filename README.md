# Json Object

Yet another JSON library. Parsing and printing is based on [libjson](https://github.com/vincenthz/libjson).

## Usage

### Basic

```cpp
#include <json.hpp>

Json data;
data["name"] = "John Doe";
data["age"] = 30;
data["role"] = "Developer";
data["address"]["number"] = 125;
data["address"]["street"] = "Maple Avenue";
data["address"]["city"] = "San Pedro";
data["address"]["state"] = "California";
data["address"]["zip_code"] = 90731;
data["skills"].push_back("C++");
data["skills"].push_back("Linux");
data["skills"].push_back("JSON");

std::cout << data.size() << std::endl; // Output: 5
std::cout << data["name"].to_string() << std::endl; // Output: John Doe
std::cout << data["address"]["street"] << std::endl; // Output: Maple Avenue
std::cout << data["skills"].size() << std::endl; // Output: 3
std::cout << data["skills"][0] << std::endl; // Output: C++

// Output: The whole object (with indent and line feed)
std::cout << json::pretty() << data << std::endl;
```

### Parsing

JSON Values can be parsed from multiple sources, using multiple methods:

1. Using the constructor
   ```cpp
   // Parse stream
   std::string json_spec = ...;
   Json json(Json::option_parse, json_spec);
   ```
   ```cpp
   // Parse stream
   std::ifstream iss("/path/to/file.json");
   Json json(iss);
   ```
2. Using the member function `parse`
   ```cpp
   // Parse string
   std::string json_spec = ...;
   Json json;
   json.parse(json_spec);
   ```
   ```cpp
   // Parse stream
   std::ifstream iss("/path/to/file.json");
   Json json;
   json.parse(iss);
   ```
3. Using the namespace-level function `parse`
   ```cpp
   // Parse string
   std::string json_spec = ...;
   Json json = json::parse(json_spec);
   ```
   ```cpp
   // Parse stream
   std::ifstream iss("/path/to/file.json");
   Json json = json::parse(iss);
   ```
2. Using the input stream extraction operator
   ```cpp
   std::istringstream iss("/path/to/file.json");
   Json json;
   iss >> json;
   ```
### Printing

1. Using the member method `stringify`:
   ```cpp
   // Stringify to string
   Json json = ...;
   std::string json_spec = json.stringify(true); // true = pretty (with indent an line feed)
   ```
   ```cpp
   // Stringify to stream
   Json json = ...;
   json.stringify(std::cout, true); // true = pretty (with indent an line feed)
   ```
 2. Using the output stream insertion operator:
   ```cpp
   // Ugly (default)
   Json json = ...;
   std::cout << json << std::endl;
   ```
   ```cpp
   // Pretty
   Json json = ...;
   std::cout << json::pretty() << json << std::endl;
   ```

## JSON Pointer

This library support JSON pointer standard as defined in [RFC-6901](https://tools.ietf.org/html/rfc6901)

Example:

```cpp

Json json;
json["root"]["hello"] = "world";

// Note: '~' is JSON pointer's escape character.
// The only characters that need escaping at this moment are '~' and '/'.

json["root"]["hello~world"].push_back("First");
json["root"]["hello~world"].push_back("Second");

json["root"]["hello/world"].push_back("Second First");
json["root"]["hello/world"].push_back("Second Second");


// empty string or single slash returns the same object
const Json& actual_root = json[json::Pointer("/")];
assert(&actual_root == &json);

// the nested object with key="root"
const Json& root = json[json::Pointer("/root")];
assert(&root == &json["root"]);

// Output: "world"
std::cout << json[json::Pointer("/root/hello")] << std::endl;
assert(json["root"]["hello"] == json[json::Pointer("/root/hello"));

// Note: Use '~0' to escape '~'

// Output: "First"
std::cout << json[json::Pointer("/root/hello~0world/0")] << std::endl;
assert(json["root"]["hello~world"][0] == json[json::Pointer("/root/hello~0world/0")]);

// Output: "Second"
std::cout << json[json::Pointer("/root/hello~0world/1")] << std::endl;
assert(json["root"]["hello~world"][1] == json[json::Pointer("/root/hello~0world/1")]);

// Note: Use '~1' to escape '/'

// Output: "Second First"
std::cout << json[json::Pointer("/root/hello~1world/0")] << std::endl;

// Output: "Second Second"
std::cout << json[json::Pointer("/root/hello~1world/1")] << std::endl;
```

