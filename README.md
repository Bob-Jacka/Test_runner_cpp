# Check runner (aka Test_runner)

## What is it?:

New version of previous super very popular tool for QA, previously called Test_runner.
Fully rewritten from 'Go' to 'C++', improve functionality and stability. Now supports directives in your test suit.
Has become harder to understand what is going on in your test suits.

## Utility flags:

Utility can take various flags, that modify program behaviour
Example of utility with flags usage: ./Check_runner --suit="some-test-suit.txt" --colored=true --comments=true

### Available utility flags:

Just write flag in command line interface and provide parameter

1) strategy - using for determining execution strategy, can take one of the following values:
   1.1) high_prior - runs only test cases with priority that equal to High
   1.2) random - pseudo random strategy, shuffle test cases and execute them
   1.3) parallel
2) devices - provide text file with devices to test on
3) time_record - bool value, if set to true then each test case will count of execution
4) colored - bool value, if set to true then utility output text will be colored in colors
5) comments - bool value, if set to true then enables comments in test cases

## Symbols meaning:

These symbols are using in simple text file,

### Available special symbols:

1. '*' - for comments in your suit
2. '#' - for directive start
3. '|' - test case separator, separate name, priority and check comment
4. ':' - for group directive start

## Allowed directives:

1) Group_start - start group of test cases
2) Group_end - end directive for group start directive
3) If - same as 'if' operator in programming languages
4) Elif - another (alternative) way of 'if' operator
5) Else - default way of 'if' operator execute
6) End_if - end directive for 'if' operators
7) Import - import another suit into suit
8) Steps_start - add steps to test case
9) Steps_end - end directive for steps
10) Ignore - ignore test case from execution (not add test to suit)
11) Parameters - special directive for adding parameters into suit, can be in several files, if one suit include into
    another, then parameters accumulate

## Rules of writing:

1. Parameters - parameters should contain equal sign, that separates name and value of parameter, ex. first_param=true,
   second_param=false, third_param=havValue (third parameter contain string value 'havValue')
2. Test case - test case should contain '|' symbol, that separates name, priority and comment, ex. Наличие версии игры
   на главном экране|Высокий|

## Examples of usage:

### Usage of group directive:

#Group_start Общие проверки:
Наличие версии игры на главном экране|Высокий|
#Group_end

*Where "#Group_start" starts group directive and add test cases above this line until reach "#Group_end" directive

### Usage of branching operators:

#### If operator

```
#Parameters: a=1, b=2

#If a < b:
   test1||
   test2||
   test3||
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
```

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
```

*If "elif" statement executes to true then next tests are added to suit

### Other directives:

#Ignore - will not add test to suit,
#Parameters - add variables to suit,
#Import - add suit to another suit