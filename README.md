# Big Decimal Calculator
* This program can implement big decimal compute `+`, `-`, `*`, `/`, `!`, `^`, short compute also included, such as `1+1`=>2 then `*9`=>18, you can ignore the first value, the system will replace it with the last result.
## Main Function
* `+` Add
* `-` Substrct
* `*` Multiply
* `/` Divide
* `^` Exponential(exp should be integer)
* `!` Factorial(should be integer).
## Features
### Incremental Operation
* If you got result in last round, you can enter a expression **with one parameter loss**, like `1/`, `*9`, `!`, `2^`, `^2` , it will replace result to that blank parameter.
### IO shortcut
* You can use the following shortcut to by enter the specific keyword.
    * **cpy** to copy r into clipboard
    * **exp** to represent result in scientific notation.
    * **fpml** to change fpml(float point max length) in runtime(too high fpml can lead to crash, due to memory insufficient)
    * **clear** to clean the screen
