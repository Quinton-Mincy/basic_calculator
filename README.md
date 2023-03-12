# Basic Calculator

![Screen Shot 2023-03-12 at 1 46 33 PM](https://user-images.githubusercontent.com/73136662/224562730-95758fe5-f341-4059-987a-e47af1bb698f.png)

## Overview

Creating a program that performs simple calculations is complex not due to the calculations themselves. Two plus two is simple enough, but 
what about *2+2\*3*? Us devout followers of [PEMDAS](https://en.wikipedia.org/wiki/Order_of_operations) can easily see that it is 6, but what about when things get a little more complex like the expression: *2+2\*(3-(4/2))*. Still,
this may be relatively simple for us to figure out, but how do you get a computer to solve a problem like this when it is given as text? The challenge quickly becomes one of parsing user input, and presenting the expression to the basic arithmetic [operations](https://en.wikipedia.org/wiki/Operation_(mathematics)) available in most, if not all, programming languages (*+*,*-*,*\**,*/*,*%*,etc.). However, before we can utilize these operators, we need to make sure that the order of operations is conserved. It is not immediatley obvious how to do this with the format that the above expressions currently hold. Fortunately, there is another way that we can express these expressions (pun intended). 

The notation that we usually see mathematical expressions in is called [infix notation](https://en.wikipedia.org//wiki/Infix_notation). This notation is characterized by placing the operators between the [operands](https://en.wikipedia.org/wiki/Operand) that they act on. For example, in the expression *2+1*, the addition *operator* is *infixed* between the two *operands* (2 and 3). Another valid way to write this expression is in the form *22+*. This notation is called *postfix*, more famously known as [*reverse Polish notation*](https://en.wikipedia.org/wiki/Reverse_Polish_notation) (RPN). In this notation, the operator is placed after the operands that it acts on. The example we posed above (2+2\*(3-(4/2))) would be written as *2 2 3 4 2 / - \* +* (separated by spaces for clarity). This notation is very useful when we learn how to evaluate an expression in this form. As we will see, the order of operations is embedded in this notation, which is why there is no need for parenthesis. Before we learn how to evaluate an expression in this form, we will look at the algorithm used to convert an expression from infix to postfix, known as the [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm). But first, we will briefly talk about regular expressions.

## Regular Expressions
To convert an expression from infix to postfix (RPN) notation, we must first verify that the expression is actually valid. Chiefly, it involves a concept in parsing known as [regular expressions](https://en.wikipedia.org/wiki/Regular_expression). A regular expression, or regex, is a set of finite characters or tokens that follow defined rules and/or patterns that can be used to encode and extract useful information. For example, imagine a regex used to search the body of text below for a phone numbers.

```
Need assistance creating regular expressions? Call (123) 456-7890, or visit us at 46819 E 56th between the hours of 9AM and 5PM M-F. 
```

The regular expression for phone numbers would need recognize *(123)456-7890* as a phone number and disregard all of the other numbers in the sentence. Moreover, a well written regular expression should just as easily recognize *1234567890* or *123-456-7890* as the same phone number. So in this example, the regex would define a finite set of characters *(0,1,2,3,4,5,6,7,8,9,(,),-)*, and also have a set of rules like: a phone number is 7 numbers, may or may not have parenthesis, and may or may not have hyphens. Similarly, for determining if a mathematical expression in infix notation is valid, the set of characters would be the digits *0-9*, parenthesis, and the set of operators (*+*,*-*,*\**, etc.). A few rules that goven these tokens would be: there must be an equal number of opening and closing parenthesis, an operator cannot immediatley follow another operator (*4+/2*) unless the second operator is the minus (*-*) operator immediatly followed by a number (*5\*-2*). In this second rule, the expression *5\*-2* should be interpreted as "5 multiplied by negative 2" and considered valid, while *4+/2*, or "4 plus divided by 2" does not make sense. Furthermore, the regex for an expression in RPN would require a different set of rules. 

If you want to see how I validated the users input, check the *is_valid* function in the *parser.c* folder. Another thing to note is that our implementation creates a linked list of the tokens found on the expression in the order that that are presented (*2+3\*4* => *2-> + -> 3 -> \* -> 4*) and passed the list to the shunting yard algorith. This is important to do to eliminate non-essentail characters like spaces that are in the users input. We will now move into shunting yard. 

## The Shunting Yard Algorithm

My implementation of the shunting yard algorithm invloves 2 data abstract data structures, a [queue](https://en.wikipedia.org/wiki/Queue_(abstract_data_type)) and a [stack](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)) (similar to, but not to be confused with the [stack memory construct](https://en.wikipedia.org/wiki/Stack-based_memory_allocation)). The queue and stack are both implemented with arrays, but the way they function is precisely opposite. The queue operates in a FIFO (first-in-first-out) fashion, and the stack operates on a LIFO (last-in-first-out) basis. The stack has 2 main functions that act on it: *push* and *pop*. The push adds an element to the "top" of the array, and *pop* removes the element that is currently at the "top" of the stack. The two main operations for a queue are *enqueue* and *dequeue*. *Enqueue* adds an element to the "head" of an array, and *dequeue* removes the element as the "tail" of the queue. These will be calrified lter on. For now, you need to know that the queue will be the output of the shunting yard algorithm, to be used in calculating the result, and the stack is essential in the transformation from infix to postfix. As mentioned before, the shunting yard algorithm is passed a linked list of the tokens involved in the infix expression, and evaluates them one by one. Below is the outline of the algorithm, followed by an example.

```
    Initialize a queue and a stack: for the output and operators, respectively.
    Iterating through the linked list:

    Rule 1: If the current token is a number (operand), push it to the output queue.
    
    Rule 2: If the current token is an operator:

            a. There is an operator on the top of the operator stack:

                i. If the current operator has higher precedence (referring to the order of operations) than
                   the operator at the top of the stack => push the current operator to the stack.

                ii. The current operator has the same or lesser precedence as the operator at the top of the
                    stack => pop, the operator at the top of the stack, enqueue it in the output queue, and
                    return to (a.) if there remains operators on the stack, else, go to (b.). 
        
            b. Push the current operator to the operator stack.

    Rule 3: If the current token is a left parenthesis, push it to the operator stack.

    Rule 4: If the current token is a right parenthesis:

            a. Pop operators from the operator stack and push them to the output queue until a left parenthesis is encountered.

            b. Pop the left parenthesis from the operator stack (but don't push it to the output queue).

    Rule 5: If there are no more tokens, pop remaining operators from the operator stack and push them to the output queue.


    Example: 3 + 2 * 4 - 2

    Rule 1:
    
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    | 3 |                   |   |
   -------                 ------- 
    output queue            operator stack

    Rule 2.b:

    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    | 3 |                   | + |
   -------                 ------- 
    output queue            operator stack

    Rule 1:

    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    | 2 |                   |   |
    | 3 |                   | + |
   -------                 ------- 
    output queue            operator stack

    Rule 2.a.i:
                                    
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    | 2 |                   | * |
    | 3 |                   | + |
   -------                 ------- 
    output queue            operator stack

    Rule 1:
                                    
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    | 4 |                   |   |
    | 2 |                   | * |
    | 3 |                   | + |
   -------                 ------- 
    output queue            operator stack

    Rule 2.a.ii
                                    -
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    | * |                   |   |
    | 4 |                   |   |
    | 2 |                   |   |
    | 3 |                   | + |
   -------                 ------- 
    output queue            operator stack

    Rule 2.a.ii
                                    
    |   |                   |   |
    |   |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 3 |                   |   |
    | 2 |                   |   |
    | 4 |                   | - |
   -------                 ------- 
    output queue            operator stack

    Rule 1:

    |   |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 3 |                   |   |
    | 2 |                   |   |
    | 4 |                   | - |
   -------                 ------- 
    output queue            operator stack

    Rule 5:

    | - |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 3 |                   |   |
    | 2 |                   |   |
    | 4 |                   |   |
   -------                 ------- 
    output queue            operator stack

```

