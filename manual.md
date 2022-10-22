# Kettle programming language v1.0.0
Kettle is a simple embedded programming language with dynamic duck type system,
garbage collector and interpreter written in pure C for better compatibility
with other projects and minimal size.

## 1 – Basics
### 1.1 – Types and values
The language has dynamic typing, that means that any variable can hold value of
any type. All values are 1st-class values, which means that they can be stored
in variables, passed as arguments to function and be return values. There are 9
different base types: `nil`, `boolean`, `int`, `float`, `string`, `table`,
`object`, `function` and `userdata`.

#### 1.1.1 – `nil`
`nil` is similar to `null` in many other languages. The only value that has
type `nil` is the `nil` itself. This type is stored and passed directly.

#### 1.1.2 – `boolean`
Only 2 values have the boolean type: `false` and `true`. Those values are
stored and passed directly.

#### 1.1.3 – `int` and `float`
`int` and `float` work the same way as in C. By default the interpreter uses `long`
and `double` for those types. Underlying types may be changed in configuration
header file before building the interpreter. Those values are stored and passed
directly.

`int` and `float` may be mixed together in most arithmetic operations. If you
use 2 `int`s on both sides, the result will also be `int`, except for division
(`/`), result of which is always `float`. If there's `float` on either side,
the result is `float`. Integer division(`//`) is an exception and must have
integers on both sides.

#### 1.1.4 – Strings
Strings are a immutable sequences of bytes. Any 8-bit value can be stored in a
string, including zero(`'\0'`). You can interact with them using special
functions provided by the interpreter. Strings are stored by references to special
structures that are managed by a garbage collector.

#### 1.1.5 – Tables
Tables are associative arrays that can use any type for both key and value.

#### 1.1.6 – Objects
Objects behave like maps with strings as keys and any types as values, usually
function. A function in an object is called a method. Unlike in many other
languages, methods don’t get a reference to the object, so all fields must be
captured local variables from the constructor. Objects are stored and passed by
references and managed by garbage collector.

#### 1.1.7 – Functions
Functions can store either a list of actions to execute or a reference to a
C-function. They also store captured variables that are inserted into local
scope before execution of the function. Functions are stored and passed by
references and managed by garbage collector.

#### 1.1.8 – Userdata
Userdata objects are pieces of memory that the interpreter can’t work with,
only C code can use it. The kettle code is not supposed to operate with raw
userdata values, they should only be captured by C-functions that use the
value. Those values are also managed by garbage collector.
           
### 1.2 – Modules
Each file is a module. When the module is imported for the first time, code in
the file is executed and all variables and functions defined in the module are
exposed to the module that imported current module as a single object.
           
### 1.3 – Scopes
Each function and module has it’s own scope where all local variables are
stored. When a variable or a function is created, it’s assigned to the scope of
current function or module if the variable is outside all function. There is
also a global scope, only C code can add values to it and all code can access
it at any time. If a variable doesn’t exist in local scope, it’s looked up in
global scope, if it isn’t found there, it is considered to be `nil`.

#### 1.3.1 – Captures
A function may capture a variable from a higher scope with capture keyword. If
done so, the variable will hold the value even when the function returned. A
variable may be captured by multiple functions, changes done to it’s value will
be visible from other functions.

### 1.4 – Errors
Some operations may case an error that interrupts the program and immediately
passes the control to error handler. After the error handler returns, the
interpreter will crash the program with abort function. If that behavior is
unwanted, the handler must never return. For example, it may call longjmp to
return control back and restore the state.

Memory errors are handled with separate handler that doesn’t require additional
memory allocations.

## 2 – The language
### 2.1 – Language tokens
Kettle ignores spaces and comments between tokens.

Names of variables and function can be any string of Latin letters, numbers and
underscores and must not start with a number and must not be a reserved keyword.

Following keywords are reserved:
```
break   capture else    false   fn      for
if      in      nil     return  true    while
```

Kettle is a case-sensitive language. `break` is a keyword but `Break` and
`BREAK` are 2 different valid names.

Following strings denote other valid tokens:
```
=   +   -   *   /   //  %   ^   &   &&  |
||  !   <   <<  >   >>  ==  !=  >= <=   ~
(   )   {   }   [   ]   ;   ,   .
```

As in many other scripting programming languages, comments in Kettle start
with `#` and ends with new line.

#### 2.1.1 - Integers
Integers can be written in decimal format with no prefix or in binary, octal
or hexadecimal format using `0b`, `0o` or `0x` prefixes respectively

Examples:
```
123 0b1010  0o70    0xff
```

#### 2.1.2 - Floats
Floats are written in decimal format with optional exponent separated by `e` or `E`

Examples:
```
1.0 0.123   6.674e-11
```

#### 2.1.3 - Strings
String literals are enclosed in single or double quotes (`'` or `"`) and can
contain following esxape sequences: `'\a'`(bell), `'\b'`(backspace),
`'\f'`(form feed), `'\n'`(new line), `'\r'`(carriage return), `'\t'`(horizontal
tab), `'\v'`(vertical tab), `'\\'`(backslash), `'\''`(single quote), 
`'\"'`(double quote) and `'\xXX'`(byte 0xXX). Strings can also contain any
special characters but it's not recommended because the interpreter opens files
in text mode that may distort those characters. Use `\xXX` instead.

## 3 - The complete syntax of Kettle
This section contains the complete syntax of Kettle programmming language in
BNF notation. `{ stmt }` means any number of `stmt`; `[ smth ]` means 0 or 1
`stmt`; `one|another` meant either `one` or `another`.

```
chunk ::= { stmt }
block ::= `{` chunk `}`

stmt ::= `;`
    | expr
    | ident `=` expr
    | atom `.` ident `=` expr
    | atom `[` expr `]` = expr
    | `if` expr block { `elseif` expr block } [ `else` block ]
    | [ ident `:` ] `while` expr block
    | [ ident `:` ] `for` ident `in` expr block
    | `fn` ident `(` arglist `)` [ `capture` `(` arglist `)` ] block
    | `break` [ ident ]
    | `return` expr

expr ::= expr1 { `||` expr1 }
expr1 ::= expr2 { `&&` expr2 }
expr2 ::= expr3 { (`==`|`!=`|`<`|`<=`|`>`|`>=`) expr3 }
expr3 ::= expr4 { `|` expr4 }
expr4 ::= expr5 { `~` expr5 }
expr5 ::= expr6 { `&` expr6 }
expr6 ::= expr7 { (`<<`|`>>`) expr7 }
expr7 ::= expr8 { (`+`|`-`) expr8 }
expr8 ::= expr9 { (`*`|`/`|`%`) expr9 }
expr9 ::= [ `+`|`-`|`!` ] expr10
expr10 ::= atom { `^` atom }
atom ::= ident | atom `.` ident | atom `[` expr `]` | atom `(` arglist `)`
    | literal | `(` expr `)`

arglist ::= [ expr { `,` expr } ]
```

## 4 - API
This section describes the C interface of the language.

### 4.1 - Stack
Internally, Kettle uses a virtual stack for all operations. This allows garbage
collector to see all values at all times. Some operations use *indices* to
select values from stack. Indices may be *absolute*, going from `0` upwards,
`0` is the bottom of the stack(value that was pushed the first); or *relative*,
goint from `-1` downwards, `-1` is the top of the stack. If index doesn't exist
in the stack, the interpreter will raise an error with code `stack_corruption`.

By default the stack has capacity for 32 values, this value may be adjusted by
changing `KTL_VSTACK_CAPACITY` constant in the configuration header file.
