# CS 300 Portfolio: Data Structures and Algorithms

# What was the problem you were solving in the projects for this course?

The problem was to build course advising software for ABCU University that loads the Computer Science course data from a file, stores it in memory, and lets an advisor look up a course, see its prerequisites, and print the full list of courses in alphanumeric order. Before writing the program, I had to analyze which data structure would handle that job best. Project One was my run-time and memory analysis of the options, and Project Two was the working code that sorts and prints the course list.

# How did you approach the problem? Why are data structures important to understand?

I started by studying how a vector, a hash table, and a binary search tree each store and retrieve data, and I wrote pseudocode for loading, searching, and printing with each one. Then I compared them on run time and memory. A vector is simple and compact, but searching it is slow unless it is sorted, and a hash table gives very fast lookups but has no natural order, so printing a sorted list requires extra work. A binary search tree keeps the data ordered as it is inserted, so an in-order traversal prints the courses in alphanumeric order with no separate sorting step. That made the tree the best fit for this problem, since the advisor needs both quick lookups and a sorted list. Data structures matter because the same program can be fast and efficient or slow and wasteful depending entirely on this choice, and the right choice only becomes clear when you understand the tradeoffs.

# How did you overcome any roadblocks you encountered?

My biggest roadblocks were getting the tree insertion and recursive traversal logic right and handling the course file reliably, including parsing each line and checking that every prerequisite refers to a course that exists. I worked through them by tracing the logic on paper with small examples, testing the code in small pieces, and using the debugger to watch how nodes were being added and visited. When I got stuck, I went back to the course readings and my pseudocode to check that the code matched my plan. Breaking a big problem into small testable steps was what got me through.

# How has your work on this project expanded your approach to designing software and developing programs?

I now plan before I code. Writing pseudocode and analyzing run time and memory first showed me that design decisions have real consequences, and that it is much cheaper to find a flawed approach on paper than after the program is built. I also think more about the people using the software, since an advisor needs fast lookups and clean, sorted output. I now ask what operations a program needs to perform most often and choose the structure and algorithms around them.

# How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

I write smaller functions that each do one job, such as loading the file, inserting a course, searching, and printing, which makes the code easier to read and test. I use clear names, consistent formatting, and comments that explain why something is done instead of restating what the code does. I also keep the data structure separate from the user menu, so the tree could be swapped for another structure or extended with new features, like searching by department, without rewriting the whole program. These habits make my code easier for someone else, or for me months later, to understand and change.
