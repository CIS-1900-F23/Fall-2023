#  Class 10

##  Higher-Order Functions


Higher-order functions, as you've seen in CIS 1200, are really useful.

To pass around functions, we can use function pointers, but these are not ideal.

Combining function pointers with template arguments can be difficult, and function pointers only work with regular functions, defined at compile-time.

This means you cannot dynamically build a function at runtime and obtain a function pointer to it.

Function objects solve both of these problems in C++.

  
##  Function Objects


A function object, also called a *functor* in C++, is an object that can be called as a function.

This is done by overloading `operator()` in the class.

The parameter list and return type of `operator()` can be fully customized, and serves as the parameter list and return type of the functor as a whole.

Since a function object is still a normal object, you could give the class some member variables in order to change its behavior:


https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/functor1.cpp#L1-L25

  

The class of a function object can also be made into a class template, to accomodate different type arguments.

Then, a function template that expects a function argument can use a template argument to specify the type of the function, as shown in this example:
  

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/functor2.cpp#L1-L40
  

This code uses the `predicate<T>` concept, which constrains the type to be something that can be called with a `T` and returns something that can be converted to a `bool`.

This allows for both functors and function pointers to be passed in as arguments.
  

Other useful concepts involving "callable" types like functors and function pointers are `invocable` and `regular_invocable`, which specify argument types.

The difference between the two is purely semantic (i.e. their definitions are equal, but they are supposed to have different meanings when used).

`regular_invocable` requires that the function is equality-preserving (i.e. equal inputs always give equal outputs), which rules out functions like random number generators.

These concepts do not specify the return type, which can instead be specified using the `invoke_result` and `convertible_to` concepts together.
  

##  Lambdas


While function objects are a good solution for representing functions, it is very inconvenient to write a new class for each function.

One useful feature in C++ is that classes can be nested within other classes and functions, reducing the amount of classes polluting the overall context.

An even more useful feature for shortening your function object code is the *lambda*.
  

A lambda expression is basically shorthand for declaring a function object (not the class of that function object!):


https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/lambda.cpp#L1-L19
  

The syntax is fairly straightforward, allowing you to write the parameter list and body of the function as usual.

The `[]` part is the capture list, discussed below.
  

The type of a lambda expression is *unspecified*, meaning you cannot write down the "real" type of the lambda, forcing you to use `auto`.

If you do need to write down the type of the lambda (e.g. if you want to use a lambda as the comparison type for `map`), then you can use `decltype` to get the type of the lambda.

For example, if you have an `auto lam = [](...){...};`, then you can make a copy using `decltype(lam) copy {lam};`.

`decltype` stands for "declared type", and gives the type of an expression.

  

The `[]` is the *capture list* of the lambda expression.

The lambda "captures" variables listed inside the brackets, allowing the body of the lambda to use them.

`[var]` captures `var` by value, copying the value of `var` when the lambda is created.

On the other hand, `[&var]` captures `var` by reference, taking a reference to the original variable, allowing for the lambda to see and make changes to the variable.

Multiple variables can be captured by separating them by commands in the capture list.

Finally, `[=]` and `[&]` capture *everything* by value and reference respectively.

  

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/capture.cpp#L1-L24

  

Lambda expressions can also be *generic*, by using `auto` for its arguments.

This is basically shorthand for declaring a template class for a function object:

  

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/generic_lambda.cpp#L1-L14

  

To write the return type of a lambda, you can use the syntax `-> T` after the parameter list to specify that the lambda has return type `T`.

Typically this can be inferred and it can be omitted, but sometimes it must be manually specified.

When dealing with generic lambdas, `decltype` is often useful here, since you do not know the types in advance.

  

##  Functional Programming in C++

  
C++ is a multi-paradigm language.

We've seen object-oriented programming, generic programming, and procedural programming so far, as well as some rudimentary functional programming features.

More sophisticated functional programming is also possible.

  

`bind` in `<functional>` partially applies a function, a useful operation for functional programming:
  
https://github.com/CIS-1900-F23/Fall-2023/blob/4e908cb2be74b7632f3555d247aabb59c75a41ec/10/bind.cpp#L21-L31

To specify that an argument should be left unbound, use `placeholders::_1`, `placeholders::_2`, etc . (in order).

In this example, we use `bind` to set the  last two arguments of `f` to `40` and `50` respectively. We leave the first three arguments unbound with placeholders (in a different order).

We can also bind to lambdas, member functions and data members and nest bindings as you can see through the rest of the examples.

https://github.com/CIS-1900-F23/Fall-2023/blob/4e908cb2be74b7632f3555d247aabb59c75a41ec/10/bind.cpp#L33-L44

###  Function Types

  

So far we have used templates to let us take arguments of "function type", which could be the type of a function object or a function pointer type.

In the case when the functor is a lambda, the type is some *unspecified* unique type generated by the compiler, so you cannot write its type without `auto`.

An alternative is `std::function<T>`, which is a class that can hold any callable function of a type `T`.

This is useful for writing recursive lambdas, since `auto` needs to know the type of what it's capturing, but that type depends on itself:

  
https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/recursion.cpp#L1-L17

  
In this example, we need to capture by reference since capturing by value gets an uninitialized `function`.

Note that you can't return this lambda since it depends on a reference to a local variable (itself).

There are also other methods to create recursive lambdas, but this is the most straightforward.


`function` is also useful for storing a heterogeneous collction of functions with the same function signature, but not necessarily the same type:

  
https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/functions.cpp#L1-L29

  

This cannot be done using templates, just like how templates cannot store a collection of objects of different types in the same container, and something like object-oriented programming must be used instead.

Just like that use case, you should prefer templates to using `function` if possible, since `function` has some runtime overhead, due to indirection from having to follow a pointer to the actual function, as well as possibly having to copy the function object.


**We will talk more about how we can use these functional programming techniques after we introduce the standard template library since it where many applications can be found.**


#  The Standard Template Library (STL)

Throughout this class, we have evaded some of the most commonly used c++ classes: `vector`, `map`, etc which are part of what is called the standatd template library. The reason is these classes use a lot of advanced C++ features from templating (hence the name) to functnional programming that are helpful to know beforehand.

The standard template library was developed independly by Alexander Stepanov and provides four components called algorithms, containers, functions, and iterators. 

The STL provides a set of common classes for C++, such as containers and associative arrays, that can be used with any built-in type and with any user-defined type that supports some elementary operations (such as copying and assignment). STL algorithms are independent of containers, which significantly reduces the complexity of the library. 

The STL achieves its results through the use of templates. This approach provides compile-time polymorphism that is often more efficient than traditional run-time polymorphism. Modern C++ compilers are tuned to minimize abstraction penalties arising from heavy use of the STL.

We will start by introducing containers, and talk about alorithm and iterator in the next class



## Sequence Containers

First we'll cover *sequence containers*, containers where the ordering of elements is determined by the user, for example in an array.

### `vector`

A `vector` (in the `<vector>` header) provides a resizable sequence of contiguous elements, like an array.
This means that accessing elements in a `vector` is just as fast as it is with an array.
Because of this, `vector` is by far the most useful and commonly-seen container class.
This class can be thought of as similar to an `ArrayList` in Java.

All standard library containers provide the `.size()` and `.empty()` member functions for checking the number of elements and whether that number is 0, respectively.
`vector` also allows users to access elements using `[...]` as with an array.
It further provides `.push_back(i)` to add a new element `i` to the back of the `vector` (by copy/move, just as with other function arguments), increasing the size of the `vector` by one.

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/vector.cpp#L1-L18

As seen in this example, just as with normal arrays, `vector`s (and all other standard library containers) support the range-based for loop.

`vector` is typically implemented as doubling the size when its internal "buffer" array is full.
This leads to many copies/moves being made, when the buffer is resized and previous objects need to moved over to the new buffer.
`vector` chooses whether to do a copy or a move in this order of preference:
1. `noexcept` move constructor
2. copy constructor
3. (non-`noexcept`) move constructor

This is because a move constructor that throws is more dangerous than a copy constructor that throws, since the move constructor can modify the object being moved, whereas the copy constructor leaves the other object unchanged.
For more details, see [this SO question](https://stackoverflow.com/questions/28627348/noexcept-and-copy-move-constructors).

To slightly improve performance, there is an alternative to `push_back` to adding new elements to a `vector` called `emplace_back`.
`emplace_back` takes the arguments of an object constructor and initializes the object *in* the `vector`, avoiding the copy or move necessary when creating the object then using `.push_back`:

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/emplace.cpp#L1-L25

This example also shows what happens when the internal array needs to be resized.

Using `[i]` to access elements of the `vector` directly accesses the underlying array, without checking if the index is out of range.
`vector`s also provide `.at(i)` which accesses the element after checking that `i` is in range, and throws `out_of_range` if it is not.
There is, of course, some overhead to doing this.
Tests show that it is around 10%, but this will differ on different machines and architectures.

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/range.cpp#L1-L24

On my machine, the undefined behavior from the out of bounds access using `v[10]` runs without issue, which is very scary.
This can easily lead to bugs occuring later on, if the programmer never notices this and the undefined behavior starts behaving differently on a different machine or on a different run.

Note that in these examples we have sometimes initialized `vector`s with their contents inside `{}`, like an array.
This differs from how we've been initializing objects.
`vector<int> v {1, 2, 3}` does not call a constructor of `vector` with arguments 1, 2, and 3, but it rather calls a different argument which takes the whole `{1, 2, 3}` as one argument of `initializer_list`, which initializes the contents of the `vector`.
This [std::initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list) should not be confused with the [member initializer list](https://en.cppreference.com/w/cpp/language/constructor) in the constructor that we've seen before.
This approach is common for container classes.
If you do want to call a constructor of a container class with arguments 1, 2, and 3, you need to use `()`: `vector<int> v(1, 2, 3)` (though no constructor of that type exists for `vector`).

### `array`

An `array` (in the `<array>` header) serves as a replacement for built-in fixed-length arrays ("C arrays").
Benefits to using `array` are that it stores its size (using `.size()`, like other containers), doesn't implicitly convert to a pointer like a C array, and has other convenience functions like bounds-checked access using `.at(i)`.
There is *no* runtime overhead at all to using `array` over C arrays, since the `array` operations can be translated to operations over a C array at compile time.
If you are using plain C fixed-length arrays on the stack, then you should be using `array` instead!

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/array.cpp#L1-L34

This code is also an example of template argument deduction, where some type arguments (in `<>`) can be omitted if the compiler can deduce them.
Here, the number of elements and their type can be deduced.

### `list`

`list` (in the `<list>` header) provides a doubly-linked list.
In addition to the common operations that other containers provide, `list`s also allow you to push or emplace to the front:

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/list.cpp#L1-L16

As usual with a linked list, direct access to elements via `[...]` is not available.

There is also a class for singly-linked lists, `forward_list`, but this class is meant for extremely resource-constrained environments that cannot handle the overhead of the second pointer at each node in `list`.
For this reason, this class also does not provide `.size()`, saving memory by not storing the size of the list.

### `deque`

A double-ended queue, or deque (pronounced "deck"), has two usual implementations: as a doubly linked list and as an array that can be extended in either direction.
Since `list` already provides a doubly-linked list, `deque` (in the `<deque>` header) provides the other implementation.
This class is similar to `vector`, providing fast access to any element and fast insertion and deletion to both the front and the back.
The underlying implementation typically uses multiple arrays, so it is not strictly better than `vector`.
Elements are not guaranteed to be contiguous, and require multiple pointer dereferences to access the array it is in, as opposed to just one dereference for a `vector`.
It also typically needs more memory than a `vector` to store the additional implementation details.

### Container Adapters

C++ also provides some containers which are an alternative interface on top of one of the above containers.
These are called container *adapters*.
`stack` and `queue` (in `<stack>` and `<queue>` respectively) are fairly simple and use a `deque` by default, though `list` can also work for either, and a `vector` is suitable for `stack`.
`priority_queue` (which is also in `<queue>`) implements a priority queue, providing constant time access for the largest element, and logarithmic time for deleting the largest and inserting other values.
This uses a `vector` by default, though `deque` is also acceptable.
Implementation-wise, a `priority_queue` uses the underlying container to define a heap.

The underlying implementation can be specified as the second template argument.
For example, `stack<int, list<int>> s` would create a stack of `int`s that uses a `list<int>` as the underlying container.

### Comparison of Sequence Containers

| Container | Insertion | Access | Deletion |
| --- | --- | --- | --- |
| `vector` | O(1) at back<br>O(n) otherwise | O(1) | O(1) at back<br>O(n) otherwise |
| `deque` | O(1) at front and back<br>O(n) otherwise | O(1) | O(1) at front and back<br>O(n) otherwise |
| `list`/`forward_list` | O(1) after reaching the element | O(1) at front and back (back only for `list`)<br>O(n) otherwise | O(1) after reaching the element |
| `priority_queue` | O(log n) | O(1) for the max | O(log n) for the max |



## Associative Containers

Associative containers are those that store a collection of key-value pairs (or just keys without a value) which are not ordered by the user.
The simplest implementation of an associative container is just a sequence container of `pair`s of keys and values.
However, this is clearly not very efficient, and the C++-provided associative containers will be much better.

### `map`

A `map` (in the `<map>` header) is an associative container that is typically implemented as a balanced binary search tree, like a red-black tree.
`[]` returns a reference to the value corresponding to a key if the key exists in the map.
If it doesn't yet exist, it will be added with a corresponding default-initialized value.

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/map1.cpp#L1-L21

Other familiar member functions are also available, like a checked access using `.at(i)`, `.size()`, `.empty()`, and `.erase()`.
`.insert(...)` and `.emplace(...)` are also available, though they do not take an iterator to the location to insert at, since `map`s maintain their own order.
Iterators are also available for `map`s, and iterating through a `map` will always go in sorted order.

To use a custom class for the key type, this class should be comparable using `operator<`.
`map` will *only* use this single operation.
If neither element is less than the other (i.e. `!(a < b) && !(b < a)`) then the `map` will consider `a` and `b` equivalent.
In the following example a wrong implementation of `operator<` results in two objects not being considered equal when they are in fact equal:

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/map2.cpp#L1-L45

Other ordering operations (like `>` instead of `<`) can also be specified for a `map`, which we'll see when we talk about function objects in a few classes.

### `set`

A `set` (in the `<set>` header) is essentially the same as `map`, but with only keys and no values.
Rather than `[]` or `.at(i)`, the `.count()` or the `.contains()` member functions are more useful here.

### `unordered_map`

`map` uses a BST, which has logarithmic complexity for its operations.
Hash tables can do better than this, with amortized constant-time operations.
This is what `unordered_map` (in the `<unordered_map>` header) implements.
The public interface is basically identical to `map`, though an ordering operation is not required.
Rather, a hashing function is used for the key type, which is already provided for all the built-in and standard library types.
Unlike other languages where the hashing function is a member function, C++ does not have a common base class for all classes.
This approach requires virtual calls for common functionality like calling the hash function, which is additional performance overhead.
As with custom comparison operations, we'll cover custom hash functions when we talk about function objects in a few classes.

https://github.com/CIS-1900-F23/Fall-2023/blob/214c85d4339503e830606f2374af7f9289fe602c/10/unordered_map.cpp#L1-L21

Like its name suggests, `unordered_map` is not ordered, so iterating through one using iterators does not always go in sorted order.
Prefer `unordered_map` to `map` for the improved performance, unless the sorted order iteration is useful for your use case or writing a good hash function for your key type is too difficult.

### `unordered_set`

Similar to `set`, `unordered_set` (in `<unordered_set>`) is the key-only version of `unordered_map`.

### `multi`-versions

In addition to these four associative containers, each of them have a version which supports multiple instances of keys: `multimap`, `multiset`, `unordered_multimap`, and `unordered_multiset`.
These classes are found in the corresponding non-`multi` headers, not in their own headers.

For the `multimap` and `unordered_multimap`, instead of using `[]` or `.at(i)` (which are not available), member functions like `.equal_range(key)` are more useful.
This function returns a pair of iterators representing the range of elements that match `key`.
