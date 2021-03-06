**INCOMPLETE**

# 1 - Introduction
Eris is a general-purpose, lightweight, dynamic programming language with class-based object-oriented programming support.

Eris is designed to be simple enough that many programmers can achieve fluency in the language.
Eris in spirit is related to Python & C++ but is organized rather differently, with a number of aspects omitted and a few ideas from other languages included.

# 2 - Grammar

This section defines the formal grammar used to define the lexical and syntactic structure of an Eris program.

## 2.1 - Grammar Notation

Both grammars are defined in standard Backus-Naur form, with some slight modifications:

* The following escape sequences are used for control characters throughout the grammar definitions:
    * Horizontal tab (ASCII char code `0x09`) - `\t`
    * Line feed, newline (ASCII char code `0x0A`) - `\n`
    * Vertical tab (ASCII char code `0x0B`) - `\v`
    * Form feed, new page (ASCII char code `0x0C`) - `\f`
    * Carriage return (ASCII char code `0x0D`) - `\r`
    * Double quote - `\"`
* In some rare cases, it may be impractical to list all productions for a given grammar rule, so the rule will be instead be defined using a descriptive English text. For example:
    ```
    <comment-char> ::= (any unicode character, but not a newline)
    ```

## 2.2 - The Lexical Grammar

The `<skip>` token is ignored in parsing.

```bnf
<any-char> ::= (any unicode character)
<any-char-seq> ::= <any-char> | <any-char-seq> <any-char>
<single-line-char> ::= (any unicode character, but not a newline)
<single-line-char-seq> ::= <inline-char> | <inline-char-seq> <inline-char>
<string-char> ::= (any unicode character, but not a double quote or newline)
<string-char-seq> ::= <string-char> | <string-char-seq> <string-char>
<char-literal-char> ::= (any unicode character, but not a single quote or newline)
<whitespace-char> ::= " " | "\t" | "\n" | "\v" | "\f" | "\r"
<letter> ::= "A" | "B" | "C" | "D" | "E" | "F" | "G"
       | "H" | "I" | "J" | "K" | "L" | "M" | "N"
       | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
       | "V" | "W" | "X" | "Y" | "Z" | "a" | "b"
       | "c" | "d" | "e" | "f" | "g" | "h" | "i"
       | "j" | "k" | "l" | "m" | "n" | "o" | "p"
       | "q" | "r" | "s" | "t" | "u" | "v" | "w"
       | "x" | "y" | "z"
<digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
<identifier-start-char> ::= <letter> | "_"
<identifier-char> ::= <letter> | <digit> | "_"
<whitespace> ::= <whitespace-char> | <whitespace> <whitespace-char> 
<single-line-comment> ::= "//" <opt-=single-line-char-seq>
<multi-line-comment> ::= "/*" <opt-any-char-seq> "*/"
<skip> ::= <whitespace> | <single-line-comment> | <multi-line-comment>
<double-literal> ::= <int-literal> "." <opt-int-literal>
<int-literal> ::= <digit> | <int-literal> digit
<char-literal> ::= "'" <char-literal-char> "'"
<identifier> ::= <identifier-start-char> | <identifier> <identifier-char>
<equality-operator> ::= "==" | "!="
<simple-assign> ::= "="
<complex-assign> ::= "*=" | "-=" | "+="
<additive-operator> ::= "+" | "-"
<relational-operator> ::= ">" | "<" | ">=" | "<="
<string-literal> ::= "\"" <opt-string-char-seq> "\""
```

## 2.3 - The Syntactic Grammar
```
<program> ::= <statement-list>
<statement-list> ::= <statement> | <statement-list> <statement>
<statement> ::= <expression-statement> | <empty-statement> | <block-statement> | <variable-statement> | <iteration-statement> | <return-statement> | <require-statement> | <function-declaration> | <class-declaration> | <namespace-declaration>
<namespace-declaration> ::= "namespace" <identifier> <block-statement>
<class-declaration> ::= "class" <identifier> <opt-class-extends> <block-statement>
<class-extends> ::= "extends" <expression>
<function-declaration> ::= "def" <identifier> "(" <opt-formal-parameter list> ")" BlockStatement
<formal-parameter-list> ::= <identifier> | <formal-parameter-list> "," <identifier>
<return-statement> ::= "return" <expression> ";"
<require-statement> ::= "require" <string> ";"
<iteration-statement> ::= <while-statement> | <do-while-statement> | <for-statement>
<while-statement> ::= "while" "(" <expression> ")" <statement>
<do-while-statement> ::= "do" <statement> "while" <expression> ";"
<for-statement> ::= "for" "(" <for-statement-init> ";" <expression> ";" <expression> ")" 
<for-statement-init> ::= <variable-statement-init> | <expression>
<if-statement> ::= "if" "(" <expression> ")" <statement> | "if" "(" <expression> ")" <statement> "else" <statement>
<variable-statement> ::= <variable-statement-init> ";"
<variable-statement-init> ::= "let" <identifier> <opt-variable-declaration>
<block-statement> ::= "{" <opt-statement-list> "}"
<empty-statement> ::= ";"
<expression-statement> ::= <expression> ";"
<expression> ::= <assignment-expression>
<assignment-expression> ::= <logical-or-expression> | <lhs-expression> <assignment-operator> <assignment-expression>
<lhs-expression> ::= <identifier>
<assignment-operator> ::= <simple-assign> | <complex-assign>
<logical-or-expression> ::= <logical-and-expression> | <logical-or-expression> <logical-or> <logical-and-expression>
<logical-and-expression> ::= <equality-expression> | <logical-and-expression> <logical-and> <equality-expression>
<equality-expression> ::= <relational-expression> | <equality-expression> <equality-operator> <relational-expression>
<relational-expression> ::= <additive-expression> | <relational-expression> <relational-operator> <additive-expression>
<additive-expression> ::= <multiplicative-expression> | <additive-expression> <additive-operator> <multiplicative-expression>
<multiplicative-expression> ::= <unary-expression> | <multiplicative-expression> <multiplicative-operator> <unary-expression> 
<unary-expression> ::= <call-member-expression> | <additive-operator> <call-member-expression> | <logical-not> <call-member expression>
<call-member-expression> ::= <member-expression> | <call-expression>
<call-expression> ::= <callee> <arguments>
<callee> ::= <member-expression> | <call-expression>
<arguments> ::= "(" <opt-argument-list> ")"
<member-expression> ::= <primary-expression> | <member-expression> "." <identifier> | <member-expression> "::" <identifier>
<primary-expression> ::= <literal> | <paranthesized-expression> | <lhs-expression>
<literal> ::= <int-literal> | <double-literal> | <string-literal> | "true" | "false" | "null" | <list-literal>
<list-literal> ::= "[" <opt-argument-list> "]"
```

# 3 - The Language
## 3.1 - Values and Types

All data in Eris, including functions, classes, and namespaces, is modeled by values. All values can be stored in variables, passed as arguments to other functions, and returned as results.

All values belong to one of the seven basic types: *null*, *boolean*, *int*, *double*, *char*, *object*, *class*, and *namespace*. The type *object* has the subtypes *string*, *list*, and *function* that serve more specialized uses. 

### 3.1.1 - The *null* Type and Values
The type *null* has exactly one value, `null`, that represents the absence of a useful value. 

### 3.1.2 - The *boolean* Type and Values
The type *boolean* has exactly two values, `true` and `false`. They are created using boolean literals, which are outlined in the [grammar rule](#2---Grammar) `<boolean-literal>`. *boolean* values support the following operators:

* The equality operators, which result in a value of type `boolean`:
    * The equality operator `==`
    * The inequality operator `!=`
* The logical operators, which result in a value of type `boolean`:
    * The logical NOT operator `!`
    * The logical AND operator `&&`
    * The logical OR operator `||`

Boolean expressions determine the control flow in several types of statements. 

Both `null` and `false` make a condition false. Any other value makes a condition true. 

### 3.1.3 - The *int* Type and Values
The type *int* represents a signed 32-bit integer that supports standard mathematical operations. 2's complement notation is used for negative numbers. They may be created using integer literals, which are outlined in the [grammar rule](#2---Grammar) `<int-literal>`. *int* values support the following operators:

* The equality operators, which result in a value of type `boolean`:
    * The equality operator `==`
    * The inequality operator `!=`
* The relational operators, which result in a value of type `boolean`:
    * The less-than operator, `<`
    * The greater-than operator, `>`
    * The less-than or equal operator, `<=`
    * The greater-than or equal operator, `>=`    
* The numerical operators, which result in a value of type `int` or `double`:
    * The addition operator `+`
    * The subtraction operator `-`
    * The multiplication operator `*`
    * The division operator `/`
    * The unary plus operator `+`
    * The unary minus operator `-`
    
### 3.1.4 - The *double* Type and Values
The type *double* represents a double-precision floating-number that follows the IEEE-754 `binary64` format. They may be created using double literals, which are outlined in the [grammar rule](#2---Grammar) `<double-literal>`. *double* values support the following operators:

* The equality operators, which result in a value of type `boolean`:
    * The equality operator `==`
    * The inequality operator `!=`
* The relational operators, which result in a value of type `boolean`:
    * The less-than operator, `<`
    * The greater-than operator, `>`
    * The less-than or equal operator, `<=`
    * The greater-than or equal operator, `>=`    
* The numerical operators, which result in a value of type `int` or `double`:
    * The addition operator `+`
    * The subtraction operator `-`
    * The multiplication operator `*`
    * The division operator `/`
    * The unary plus operator `+`
    * The unary minus operator `-`
    
## 3.2 - Environments

## 3.3 - Variables
## 3.4 - Expressions
## 3.5 - Statements
## 4 - Standard Library
### 4.1 - Global Functions
### 4.2 - The Class Object
### 4.3 - The Class String
### 4.4 - The Class List
### 4.5 - The Class Function