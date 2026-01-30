# Check runner (aka Test_runner CPP edition)

## What is it?:

New version of previous super very popular tool for QA, previously called Test_runner.
Fully rewritten from 'Go' to 'C++', improve functionality and stability. Now supports directives in your test suit.
Has become harder to understand what is going on in your test suits.

## Utility flags:

Utility can take various flags, that modify program behaviour
Example of utility with flags usage: ./Check_runner --suit="some-test-suit.txt" --colored=true --comments=true

### Available utility flags:

Just write flag in command line interface and provide parameter:

1) **strategy** - using for determining execution strategy, can take one of the following values:
    1) _high_prior_ - runs only test cases with priority that equal to High;
    2) _random_ - pseudo random strategy, shuffle test cases and execute them;
    3) _parallel_ - parallel test case search;
    4) _everything_now_ - print all tests cases in console for fully manual mode;
    5) _usual_ - usually test case execution (do nothing with ts);
    6) _choose<>_ - you can enter strat name, by default it will be all tests (if you do not write triangular
       brackets) or
       you may enter brackets and utility give test cases only with chosen priority, other will be ignored.
2) **devices** - provide text file with devices to test on,
3) **time_record** - bool value, if set to true then each test case will count of execution,
4) **colored** - bool value, if set to true then utility output text will be colored in colors,
5) **comments** - bool value, if set to true then enables comments in test cases.
6) **interface** - bool value, if set to true starts graphical user interface

## Symbols meaning:

These symbols are using in simple text file (just .txt),

### Available special symbols:

1. '*' - for comments in your suit
2. '#' - for directive start
3. '|' - test case separator, separate name, priority and check comment
4. ':' - for group directive start, also using in other directives

## Allowed directives:

1) _Group_start_ - start group of test cases
2) _Group_end_ - end directive for group start directive
3) _If_ - same as 'if' operator in programming languages
4) _Elif_ - another (alternative) way of 'if' operator
5) _Else_ - default way of 'if' operator execute
6) _End_if_ - end directive for 'if' operators
7) _Import_ - import another suit into suit
8) _Steps_start_ - add steps to test case
9) _Steps_end_ - end directive for steps
10) _Ignore_ - ignore test case from execution (not add test to suit)
11) _Parameters_ - special directive for adding parameters into suit, can be in several files, if one suit include into
    another, then parameters accumulate; parse parameters till the line end

## Rules of writing:

1. Parameters - parameters should contain equal sign, that separates name and value of parameter, ex. first_param=true,
   second_param=false, third_param=havValue (third parameter contain string value 'havValue')
2. Test case - test case should contain '|' symbol, that separates name, priority and comment, ex. Наличие версии игры
   на главном экране|Высокий|

## Examples of usage:

### Usage of group directive:

```
#Group_start Общие проверки:
   Наличие версии игры на главном экране|Высокий|
#Group_end
```

*Where "#Group_start" starts group directive and add test cases above this line until reach "#Group_end" directive

### Usage of branching operators:

#### If operator

```
#Parameters: a=1, b=2

#If a < b:
   test1||
   test2||
   test3||
#End_if
```

*If "if" statement executes to true then next tests are added to suit

#### Elif operator

```
#Parameters: a=1, b=2

#If a > b:
   test1||
   test2||
   test3||
#Elif a < b:
   test4||
   test5||
   test6||
#End_if
```

*If "elif" statement executes to true then next tests are added to suit

#### Else operator

```
#Parameters: a=1, b=2

#If a > b:
   test1||
   test2||
   test3||
#Elif a == b:
   test4||
   test5||
   test6||
#Else:
   test7||
   test8||
   test9||
#End_if
```

*Last tests

### Other directives:

_#Ignore_ - will not add test to suit,
_#Parameters_ - add variables to suit,
_#Import_ - add suit to another suit

## Config file in utility:

### What is it?

Utility allowed to have settings in .ini files, it means that you can not provide arguments in command line interface,
but provide by .ini file. Parameter might not be provided in .ini file, ex. comments might not be provided and utility
starts without comments displaying.

### Example of usage:

Here is example of usage .ini file in setting utility parameters.
**It is important to use same names in your .ini config file that in "Available utility flags" section of README.** You
can not provide any of these parameters.

```
[LoadParameters]
colored=true
comments=true
time_record=true ; or it can be false if you do not want to record time

[FilePointers] ; files names, might be with .txt or not
suit=one-time-test.txt
devices=devices.txt

[Strategy]
interface=true ; runs graphical user interface or console - runs console app
strategy=usual
```