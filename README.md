# kjc
Kotlin and Java compile abstraction.

So basically, I don't like either the Kotlin or Java compiler command line tools. So I wanted to created a tool that abstracts their compilation in to one tool and allows easier creation of 2 things, raw class files and a runnable jar file in a destination directory.

It's very basic and the code is in need of serious clean up.

Here what it does currently: take in options -s, -d and -lib each with 1 argument

-s is the source directory, where the source files are.
-d is the output directory, where everything will be compiled.
-lib is a directory with all the libraries in it.

Now, it needs some work but the lin option essentially ignores directories and copies every file it finds with the .jar, .so, .dll or .dylib extension the the output directory marked with the -d option.

Then all the source files are copied and compiled to class files in their correct packages/directories and are compiled in place. 

Then to run, you would use `java com.whatever.Main` in the output directory.

Currently the source files do not get deleted which I want to do.
I also need to add runnable jar support.