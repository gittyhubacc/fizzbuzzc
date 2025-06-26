[wikipedia]: https://en.wikipedia.org/wiki/Fizz_buzz

# fzzbzzc
This is a compiler for "fzzbzz", a dsl for creating programs that behave [fizzbuzzishly][wikipedia]. With fzzbzz, writing fizzbuzzing programs has never been eaiser(?). Fzzbzz's grammar can be seen in the form used to generate it's parser in `parser.c`. Here's my informal description of fzzbzz's grammar:
```
# lhs is a terminal symbol,
# produced during lexical analysis by the rhs regular expression
open => '<'
close => '>'
equals => '='
slash => '/'
comma => ','
int => [0-9]*
string => [a-zA-Z].[0-9a-zA-Z]*

# lhs is nonterminal symbol, rhs is terminal-nonterminal symbol string
lower -> open, int
upper -> int, close
rules -> int, equals, string 
rules -> rules, comma, int, equals, string
fizzbuzz -> lower, slash, rules, slash, upper
```
## Examples
```
$ ./bin/fizzbuzzc '<1/3=fizz,5=buzz/15>' > vanilla.c
$ clang vanilla.c -o vanilla
$ ./vanilla
1
2
fizz
4
buzz
fizz
7
8
fizz
buzz
11
fizz
13
14
fizzbuzz
```
```
$ ./bin/fizzbuzzc '<99/3=holy,5=cow,7=batman/111>' > step_forward.c
$ clang step_forward.c -o step_forward
$ ./step_forward
holy
cow
101
holy
103
104
holycowbatman
106
107
holy
109
cow
holy
```
