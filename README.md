# Check runner (aka Test_runner)

## What is it?:

New version of previous very popular QA tool - Test_runner.
Fully rewritten from 'Go' to 'C++' and improve functionality and stability. Now supports directives in your test suit.
Has become harder to understand what is going on in your test suits.

## Symbols meaning:

1. '*' - for comments in your suit
2. '#' - for directive start
3. '|' - test case separator, separate name, priority and check comment
4. ':' - for group directive start

## Allowed directives:

1. Group_start - start group of test cases
2. Group_end - end directive for group start directive
3. If - same as 'if' operator in programming languages
4. Elif - another way of 'if' operator
5. Else - default way of 'if' operator execute
6. End_if - end directive for 'if' operators
7. Import - import another suit into suit
8. Steps_start - add steps to test case
9. Steps_end - end directive for steps
10. Ignore - ignore test case
11. Parameters - special directive for adding parameters into suit

## Rules of writing:

1. Parameters - parameters should contain equal sign, that separates name and value of parameter, ex. first_param=true,
   second_para=false
2. Test case - test case should contain '|' symbol, that separates name, priority and comment, ex. Наличие версии игры
   на главном экране|Высокий|

## Examples of usage:

#Group_start Общие проверки:
Наличие версии игры на главном экране|Высокий|
#Group_end