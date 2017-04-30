Brian Henson
4/30/2017
CS 362


First off, I'm not sure why you want this in a .c file rather than a .txt file, but whatever. I can work with it.

Second, I'm not sure what you mean by "how it finds the error message". I will assume by "error message" you mean [({ ax})] followed by 'reset'. And I didn't write that part of the code, it was already there, so I'm not sure why I'm expected to explain it. But I totally can, so whatever.

Developing the random input generators was very simple. All I needed to do for generating a random character was look up an ASCII chart to determine what range of ASCII values have "normal" & printable characters (32-126), and use an offset modulus operator to generate a random number within that range. Then that decimal number is automatically converted to the character it corresponds to when assigned to a 'char' variable.
The random string generator is a bit more complicated. Firstly, I decided that I wanted to output strings of variable length, from 0-10 human-readable characters. To do this, I generate a random integer 0-10 and put the null-terminating character in that position. I then iterate up to that value, and use the inputChar() function described above to put a random char in each string position before the null-terminator. 
The trick is that, to avoid memory leaks (?) and to ensure that the randomly-generated string is still accessible from outside the function, the string is declared as a static variable. This means that the same exact memory address is used/overwritten for the random string on every iteration, and it also means that the same pointer is returned every time.
In order to "find the error message", what this function being tested looks for is the result of the inputChar() function to be these characters "[({ ax})]" in this exact sequence, with other characters possily interspersed between them. Each of these characters are possible for the inputChar() function to generate, so this requirement can be met. After that has been satisfied, it then searches for/waits for the result of inputString() to be exactly "reset\0". This is also a valid output for the inputString function. Then, once this requirement is satisfied, the program will print simply "error" and exit the loop.

