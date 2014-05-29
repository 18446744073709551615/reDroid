reDroid
=======

reverse-engineering tools for Android NDK/C++/Qt

Why?
----

Sometimes you have to use a library available only as a binary and either poorly documented or just buggy.
Once I even saw a design flaw, a global variable storing some vital information while the
rest of the library was documented as thread-safe (apparently, they were porting the library to Android and
showed their first success to the marketing,
and the marketing decided to start sales immediately).

So you know _it's not your_ bug, but you have to make your software work. You have to find a workaround (or maybe even a fix).
So you have to find out what's going on inside...

What it is
----------

It is a collection of tools that one may borrow.
You will have to read the source and pick up the necessary pieces.


What it's not
-------------

It's not a product.
It compiles, but the .so library is useless; the code worked in a _different_ project.


