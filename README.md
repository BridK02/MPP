# MPP
Multiparadigm processing tasks

1. Add a doc string to each function in the procedural program, make sure to document each parameter
and the return value. You can see this has done for get average in Listing 3.

2. Move the functions around, you should find that this has no effect on the program execution as they
are just blocks of code which are called by the “main method” at the bottom. This won’t be true in
older languages like C, so be aware of that.

3. Move bubblesort into another .py file and use the import keyword to import it into your file for use in
the median calculation. This will look something like:
1 from sorting_algorithms import bubble_sort
2
This can be very useful when programs grow large.

4. Add in two new functions which extract some statistics from the data. No imports allowed here.

5. Write a new function to bring in data from a separate column in the CSV file, can you do this by
modifying the existing method to be more generalised? Consider default parameters (this is a language
feature of Python).

6. Replace 4 functions in the procedural program with imports or the use of the standard Python library.
For example you could replace get minimum value with the min() function for lists which is built
into Python
