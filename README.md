
# MMReflection
C++ time Reflection Library

## How to Use
### Manual registration
```cpp
#include <registration.h>
using namespace MM::Reflection;

struct MyStruct { MyStruct() {}; void Func(double) {}; int data_; };

MM_REGISTER {
  Class<MyStruct>{"MyStruct"}
    .Constructor<>("Init")
    .Method("Func", &MyStruct::Func)
    .Property("data_", &MyStruct::data_);
}
```
### Iterate over members
```cpp
const Meta* meta_data = GetMetaDatabase().at(typeid(MyStruct).hash_code());
for (const Property* prop : meta_data->GetAllProperty()) {
  std::cout << "name: " << prop->GetPropertyName();
}

for (const Method* method : meta_data->GetAllMethod()) {
  std::cout << "name: " << method->GetMethodName();
}
```

### Constructing types
```cpp
const Meta* meta_data = GetMetaDatabase().at(typeid(MyStruct).hash_code());
Variable var = meta_data->CreateInstance("Init");    // will invoke the previously registered ctor

const Constructor* ctor = meta_data->GetConstructor("Init");  // 2nd way with the constructor class
var = ctor->Invoke();
std::cout << var.GetType()->GetTypeName(); // prints 'MyStruct'
```

### Set/get properties
```cpp
const Meta* meta_data = GetMetaDatabase().at(typeid(MyStruct).hash_code());
Variant var = meta_data->CreateInstance("init");

Variable data_variable = var.GetPropertyVariable("data_");
assert(data_variable.IsRefrenceVariable() == true);
assert(data_variable.IsPropertyVariable() == true);

data_variable = Variable::CreateVariable(23); // set property
std::cout << data_variable.GetValueCast<int>() << std::endl; // get property
```

### Invoke Methods:
```cpp
const Meta* meta_data = GetMetaDatabase().at(typeid(MyStruct).hash_code());
Variant var = meta_data->CreateInstance("init");

const Method* method = meta_data->GetMethod("Func");
Variable arg_var = Variable::CreateVariable(42.0);
method->Invoke(var, arg_var); // 1st way

var.Invoke("Func", arg_var); // 2nd way
```

Features
---------
- reflect constructors, methods, data member
- classes; with *single*-, *multiple*- and *virtual*-inheritance
- constructors (arbitrary argument count)
- methods (*virtual*, arbitrary argument count)
- ability to invoke properties and methods of classes from any arbitrary class level
- no header pollution; the reflection information is created in the cpp file to minimize compile time when modifying the data
- possibility to add additional *metadata* to all reflection objects
- minimal macro usage
- **no** additional 3rd party dependencies are needed
- **no** exceptions
- **no** external compiler or tool needed, only standard ISO C++17

[//]: # (Portability)

[//]: # (-----------)

[//]: # (Tested and compiled with:)

[//]: # (- Microsoft Visual Studio 2015 & 2017 &#40;2013 support till version 0.9.6&#41;)

[//]: # (- GCC 4.8.1)

[//]: # (- Clang 3.7)

[//]: # (- MinGW 4.8.2)

License
-------
MMReflection is released under the terms of the [MIT](LICENSE.txt) license,
so it is free to use in your free or commercial projects.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


[//]: # (Installation)

[//]: # (------------)

[//]: # (The installation guide can be found [here][rttr.install].)


[//]: # (Get Started:)

[//]: # (------------)

[//]: # (Take a look at the [documentation][rttr.docs] or start with the [tutorial][rttr.tutorial].)