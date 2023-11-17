#  Class 11

We have given an introduction into functional programming and the C++ standard library containers. We will now show how to manipulate the standard library using iterators and functional programming.

##  Iterators
  
How do we actually use a `list`?

We know how to insert or delete from the front and back, but linked lists should support fast inserting anywhere in the middle of it, as long as you have already moved to that spot in the list.

Similarly, if you've looked at the documentation, you might have seen that other containers like `vector` support insertion in the middle of it, but this interface does not take an index to the position to insert at.

For these sorts of operations, C++ uses *iterators*.

  

Iterators are objects that represent an element from a sequence.

Each container has a type member `::iterator` (e.g. `vector<int>::iterator`) for the type of its iterator (remember type members? Throughout this, you will hopefully see how incredibly useful this in allowing us to write generic code without knowing much about the container using it is defined type members.)

Iterators have a similar interface to pointers, they overlading operators allowing for "dereferencing" via `*` and `->` to access the element they represent.

To make an iterator, every container has the `.begin()` member function which returns an iterator to the first element of the container.

To check whether you've arrived at the end of the container, every container also has the `.end()` member function which returns an iterator to the element following the last element in the container---a nonexistent element which represents the end.

See the image on [this page](https://en.cppreference.com/w/cpp/container/vector/begin) for a visual representation.

Finally, one can move to the next element in the sequence using `++`:

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/iterator1.cpp#L1-L16

As a side note, the range-based for loop (e.g. `for (auto i : l)`) uses iterators and thus works for any container that provides them.  

There can also be `const` iterators, which disallow using the iterator to change the element it refers to.

The type of the const iterator is given by the `::const_iterator` member and these iterators can be created using `.cbegin()` and `.cend()`.

Further, remember that iterators represent elements from a *sequence*, not a *container*.

This means that iterators do not always have to follow the container's order.

Containers also provide reverse iterators, which allow one to iterate through the container from back to front, where `++` goes "backwards" in the original container order.

Iterators to these can be created using `.rbegin()` and `.rend()` (and `.crbegin()` and `.crend()` for the `const` versions).

The image on [this page](https://en.cppreference.com/w/cpp/container/vector/rbegin) shows a visual representation.

 
###  Types of Iterators

  

Iterators of different containers have different capabilities. There are 3 main types, forward, bidrectional and random access.

Iterators of a `vector`, for example, are random access should be able to move forward an arbitrary number of elements, since `vectors` are contiguous blocks of memory.

On the other hand, iterators of a `list` would not be able to do this in constant time, and would rather require using `++` repeatedly.

These capabilities form a hierarchy, though not a class hierarchy.

While subtyping would be a good fit here, there would be a performance hit due to the many virtual calls.

Rather, each iterator type is a completely independent class.

To write functions that can take any kind of iterator as an argument, we'll have to use template arguments, which we'll see next class.

The same is true of the container classes, which also do not form any class hierarchy.

Here is a simplified description of the iterator hierarchy:

- All iterators can be incremented (`++`) to go to the next element of its sequence, by compared for equality (`==`, `!=`), and access the element it represents (`*`, `->`).

The iterators that can *only* do these things are called *forward iterators*.

- If the iterator has the additional capability of being decremented (`--`) to go backwards in the sequence, they are called *bidirectional iterators*.

- The most powerful kind of iterators are *random-access iterators*. These can go forwards or backwards in the sequence for any number of steps in constant time (using `+`, `-`, `+=`, `-=`), and can be compared for inequality (`<`, `<=`, `>`, `>=`) to check which iterator comes before another in the same sequence.

For example, `forward_list` provides forward iterators, `list` provides bidirectional iterators, and `vector` and `deque` provide random-access iterators.

The kind of iterator that each container provides should make sense, since they reflect how each container is implemented.  

While some of these operations are not available for all kinds of iterators, some of them can be simulated.

The `<iterator>` header provides functions like `advance(it, n)`, which advances the `it` iterator forward `n` times.

For a random access iterator, it will choose to use `+=` and run quickly, but for a forward iterator it will use `++`  `n` times and run much slower.

As another example, `distance(first, last)` returns the distance between two elements of a sequence.

It just uses `-` for random access iterators, but for other kinds of iterators it will count the number of `++` necessary to get from `first` to `last` (using `==` to check when to stop).

This is undefined behavior if `last` is not actually reachable from `first`.


https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/iterator2.cpp#L1-L14
  
##  Member Functions

In Addition to the functions we have seen in the last class, STL containers usually provide some important functions that are defined based on iterators.

`insert(it)` and `erase(it)` will add an element after the element pointed to by it or remove that element respectively.  This allows you to remove or insert arbitrarly inside a container. These are very useful in conjuctions with some functions we are going to see soon. These functions are defined for the majority of containers. Examples are `vector`, `map`, `list`, etc.

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/insert_erase.cpp#L1-L26

  
##  `<algorithm>`

  
The standard library provides a long list of algorithms in `<algorithm>`.

The algorithms library aprovided generic algorithms that can work on any type of container that meets its condition. It uses iterator, the common interface between these contaienrs to acheive that. It also allows for customaizablity through higher order functions where you will be able to pass predicates and other custom functions using HOFs. This a good example of how useful HOFs are.

Most of these are function templates that take iterator arguments.


`for_each(first, last, f)` calls a higher order function `f` on each element of the range defined by iterators `first` and `last`.

A range-based for loop is often simpler for an entire container, but `for_each` allows the exact range to be specified.

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/for_each.cpp#L1-L18

 
`find(first, last, v)` returns an iterator to the first place where `v` is found in the range, or the end iterator `last` if no match is found.

A very common use of find is to use with the earlier mentioned `insert` or `erase` to first find some element, and once we have an iterator for it, add or remove at that location.

There are also `find_if` and `find_if_not` variants that take a predicate rather than just a value.

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/find.cpp#L1-L27

This example gives a taste of the variety of functions available in `<algorithm>`:

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/algorithms.cpp#L1-L48

  
`iota` fills a range with an incrementing sequence, starting at some value.

It is named after the ι function that did the same thing in APL, a programming language.

The standard library also provide classes (in `<random>`) that generate pseudorandom as well as *truly random* numbers in much more sophisticated ways than C `rand` and `srand`.

See the [documentation](https://en.cppreference.com/w/cpp/numeric/random) for more details.

`shuffle` reorders the range such that any permutation has equal probability to be chosen.

It requires a source of random numbers.

`replace` and `replace_if` replace some elements of a range.

`sort` sorts a range defined by *random access* iterators.

Other containers like `list` have a member `sort` function, since the implementation has to differ from the random-access one.

`lower_bound` and `upper_bound` find a lower or upper bound for a value in an ordered sequence. The sequence must be sorted for this to work properly. The way this works is as follows:

-   If the iterators are random access, it is the slightly modified binary search to get the lower/upper bound as specified. Therefore the complexity is logarithmic in N.
-   If the iterators are just bidirectional, the complexity is linear in  `N`.

Note that while this works for set, it is harder to do so with map, since `std::map::value_type` is defined as `std::pair<const  Key, T>` which will not work with the `lower_bound` function. Therefore, `std::map` (and `std::set`) provide their own member function `.lower_bound` and `.upper_bound` that will only compre keys.


###  Modifying Containers

Algorithms that take iterator arguments operate on *ranges* defined by those iterators, so they cannot access the container directly.

This means that these functions cannot directly affect the size of the container, for example by calling `insert` or `erase`.

For example, `remove` shifts the elements in the range to remove some elements, and returns a new end of range iterator. The underlying container's size is unchanged:  

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/remove1.cpp#L1-L22

One option to modify the container is by directly referring to it.

For example, after calling `remove`, we can call `erase` on the container using the iterator returned by `remove`:

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/remove2.cpp#L1-L16

As another example, `copy` assumes there is enough space in the destination container to copy the source range:

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/copy1.cpp#L1-L14

 
One solution is to ensure there is enough space by resizing before calling `copy`:

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/copy2.cpp#L1-L16

Another solution is to use an iterator that knows more about the container.

For example `back_inserter`, `inserter`, and `front_inserter` (in `<iterator>`) create special iterators that call `push_back`, `insert`, and `push_front` respectively, so the container grows when something is written via the iterator:

 
https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/copy3.cpp#L1-L17


###  Iterator Invalidation

All sequence containers provide the `.insert(...)` and `.erase(...)` member functions, which take iterator arguments to determine where to insert into or erase from the container.

These operations must be used with care, however, since they can *invalidate* existing iterators into the container.

For example, erasing an element in the middle of a `vector` requires shifting everything after it down to take up the erased room, so any iterators that represent those elements would become invalid, and using them would be undefined behavior.

In this following example, we erase the element represented by an iterator from a `list`, which obviously would invalidate that iterator.

To deal with this, we use the return value of `erase`, which returns an iterator to the element after the erased element.

To prevent the element following the erased element from being skipped in the loop, the loop only increments `it` if nothing was erased.

 
https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/iterator3.cpp#L1-L27

 
See [this chart](https://en.cppreference.com/w/cpp/container#Iterator_invalidation) for a summary of the invalidation rules.

When using operations that modify the container, check the documentation to see if the return value provides a way to deal with potential iterator invalidation, like with `erase` above.

###  Parallel Algorithms

  Most of the standard library algorithms also accept an *execution policy* as their first argument.

These execution policies specify how the function should be run.

`execution::seq`, for sequential execution, is the default when no execution policy is specified.

Passing `execution::par` allows the algorithm to be parallelized, where execution is split up and run on parallel threads.

If this code can result in a race condition when parallelized, then you must add synchronization such as `mutex`s yourself.

Additionally, there is also `execution::par_unseq` and `execution::unseq`, which permits the code to be *vectorized*.

This means that code can run out of sequence on a single thread, using [SIMD](https://en.wikipedia.org/wiki/SIMD) instructions, which are instructions that run on multiple values at once, and are available on some processors.

For example, if your code is trying to sum up an array, vectorization might allow for the first four numbers to all be added at once, instead of one by one.

This may also break some synchronization methods, so many operations like using shared memory or `mutex`s are not safe within vectorized code.

These execution policies are actually just *guidelines* to the compiler.

It is not guaranteed that using them will actually result in parallel execution, and the compiler can always fall back to sequential execution.

  Execution policies are not supported by Clang and require installing additional libraries for GCC, so I won't be going in depth into any examples.

However, if they are available for your platform, then they are a very lightweight and easy way to improve performance.


##  Customizing Containers
 
###  Hash

unordered maps and sets use the `hash` functor for hasing keys. `hash` is implemented using templates, to avoid the overhead from using virtual functions.

`unordered_map` uses `std::hash` by default to hash values (This is implemented as template argument where the template has a default argument.)

One way to provide a hash function for a custom key class is to write your own class for a functor, and pass that to the `unordered_map` to use instead of `std::hash`:

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/hash1.cpp#L1-L32

This hash function uses bitwise xor (`^`) and left bitshift (`<<`) to combine the hash values of the first and last name (obtained using `std::hash`) in a "random-looking" way.

`==` is required to be defined for our key class, and is used when there are hash collisions.

An alternative method is to make `std::hash` work with your class, rather than bypassing its use entirely.

`std::hash<T>` is a class template, which, when instantiated into an object, is a function object representing the hash function for the type `T`.

Of course, this class cannot provide hash functions for all possible types.

The standard library provides specializations of `std::hash` for the built-in and standard library types.

For all other types without a specialization, trying to use `std::hash` will result in a compiler error.

So for your own class, you can write a specialization of `std::hash` for it:

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/hash2.cpp#L1-L35  

Since `hash` is inside the `std` namespace, we first need to "open" the `std` namespace.

Then, to specialize the template, we just need to provide the type argument we are writing the specialization for.

The `template<>` with the empty `<>` is required to tell the compiler that this is still a template, just that there are no more template parameters.


Since Hash is a functor, we can also use lambdas for defining the hash.

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/hash1.cpp#L33-L40
 
###  Ordered Associative Containers

As discussed in a previous class, `map` and `set` uses `<` by default to compare keys.

We can customize this using the template argument following the key and value types of these class templates, which defaults to `std::less<Key>`.

Objects of this type are function objects that take two `Key`s and returns the result of comparing them using `operator<`.

The standard library also provides `greater<Key>`, `less_equal<Key>`, and `greater_equal<Key>`, though the ones involving equality are likely not suitable, as shown by the [example](../09/map2.cpp) from class 9.

  
https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/map.cpp#L1-L37

Remember that just as with `std::hash`, the template argument is a *type*, where every object of this type is a function object, and each of these function objects behaves identically.

Since this is also a functor, we can lambda syntax to define custom equality.

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/map.cpp#L38-L39


### Others

Additionaly, other contaienrs like priority_queue allow you to customize comparision using custom structs or lambdas. 
If you have been looking at cppreference, you might have also seen allocator template arguments. In general, you can allocate memory for any type using `new` which is the default, but in some cases where resources are constrianed, a different allocation scheme can be helpful such as [arenas](https://stackoverflow.com/questions/12825148/what-is-the-meaning-of-the-term-arena-in-relation-to-memory).  We won't go in depth here, but is useful to know.


The standard template library relies heavily on iterators and HOFs to perform generic algorithms. It is generally best to use the STL if you need any container since most of them are optimized and flexible enough. However, STL is not perfect and there is a lot of opportunities to improve on it. For example, unordered maps and sets use buckets implemented as linked lists which is bad for cache locality and are know to be inefficient. In general, if you are working on a large project with many collaborators, it may be helpful to redefine some of these containers to fit your needs, but otherwise, the STL should be good enough.

## Ranges

A few classes ago we talked about concepts and how they can make templates code safer, less error prone and cleaner to debug. However, concepts were added in C++20 while the STL has been around  since 1994. Since the STL is a huge library, it has not adapted to more modern c++ and is very unsafe. std::ranges is a new library (some people call it STL 2.0) that improves upon the STL and uses concepts to provide more safety and better debugging experience. It is defined in the header `<ranges>`. `std::ranges` build on top of iterators and formalizes a lot of implicit concepts in the STL such as the concept of a range, that is defined by a begin and end iterators.



For example, std::sort should work with containers that provides random access iterators. Therefore, you should expect it to work on vectors but not sets. If you uncomment the call to sort on set, you will see that it fails to compiling and prints (on my device) 312 line of errors.

https://github.com/CIS-1900-F23/Fall-2023/blob/d49fe6779961c1277631fc31a017b933e92cb355/11/ranges.cpp#L1-L31

Now, uncomment the same example using ranges and you will see that there is less (23 lines on my device) lines of error printed and a very clear messages about the requirement for random access iterator not being met.

Notice also that ranges has a different syntax where you are able to pass the container directly instead of passing a begin and end iterators. This is one of the main improvement where ranges hides the iterators and provides a much more readable snytax.

`std::ranges` provide additional features such as range adaptors that can be composied into pipelines. It is very use and can lead into a much more readable and easy to debug syntax. You can learn more about it [here](https://en.cppreference.com/w/cpp/ranges) and through this [talk](https://www.youtube.com/watch?v=SYLgG7Q5Zws&ab_channel=CppCon)

