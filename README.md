# Basic Calculator

<img width="618" alt="Screen Shot 2023-03-12 at 7 11 15 PM" src="https://user-images.githubusercontent.com/73136662/224579751-a75426f4-c855-404f-8ce4-c52051da9c2f.png">

## Overview

Creating a program that performs simple calculations is complex not due to the calculations themselves. Two plus two is simple enough, but 
what about *2+2\*3*? Us devout followers of [PEMDAS](https://en.wikipedia.org/wiki/Order_of_operations) can easily see that it is 6, but what about when things get a little more complex like the expression: *2+2\*(3-(4/2))*. Still,
this may be relatively simple for us to figure out, but how do you get a computer to solve a problem like this when it is given as text? The challenge quickly becomes one of parsing user input, and presenting the expression to the basic arithmetic [operations](https://en.wikipedia.org/wiki/Operation_(mathematics)) available in most, if not all, programming languages (*+*,*-*,*\**,*/*,*%*,etc.). However, before we can utilize these operators, we need to make sure that the order of operations is conserved. It is not immediately obvious how to do this with the format that the above expressions currently hold. Fortunately, there is another way that we can express these expressions (pun intended). 

The notation that we usually see mathematical expressions in is called [infix notation](https://en.wikipedia.org//wiki/Infix_notation). This notation is characterized by placing the operators between the [operands](https://en.wikipedia.org/wiki/Operand) that they act on. For example, in the expression *2+1*, the addition *operator* is *infixed* between the two *operands* (2 and 3). Another valid way to write this expression is in the form *22+*. This notation is called *postfix*, more famously known as [*reverse Polish notation*](https://en.wikipedia.org/wiki/Reverse_Polish_notation) (RPN). In this notation, the operator is placed after the operands that it acts on. The example we posed above (2+2\*(3-(4/2))) would be written as *2 2 3 4 2 / - \* +* (separated by spaces for clarity). This notation is very useful when we learn how to evaluate an expression in this form. As we will see, the order of operations is embedded in this notation, which is why there is no need for parenthesis. Before we learn how to evaluate an expression in this form, we will look at the algorithm used to convert an expression from infix to postfix, known as the [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm). But first, we will briefly talk about regular expressions.

## Regular Expressions
To convert an expression from infix to postfix (RPN) notation, we must first verify that the expression is actually valid. Chiefly, it involves a concept in parsing known as [regular expressions](https://en.wikipedia.org/wiki/Regular_expression). A regular expression, or regex, is a set of finite characters or tokens that follow defined rules and/or patterns that can be used to encode and extract useful information. For example, imagine a regex used to search the body of text below for a phone numbers.

```
Need assistance creating regular expressions? Call (123) 456-7890, or visit us at 46819 E 56th between the hours of 9AM and 5PM M-F. 
```


The regular expression for phone numbers would need recognize *(123)456-7890* as a phone number and disregard all of the other numbers in the sentence. Moreover, a well written regular expression should just as easily recognize *1234567890* or *123-456-7890* as the same phone number. So in this example, the regex would define a finite set of characters *(0,1,2,3,4,5,6,7,8,9,(,),-)*, and also have a set of rules like: a phone number is 7 numbers, may or may not have parenthesis, and may or may not have hyphens. Similarly, for determining if a mathematical expression in infix notation is valid, the set of characters would be the digits *0-9*, parenthesis, and the set of operators (*+*,*-*,*\**, etc.). A few rules that given these tokens would be: there must be an equal number of opening and closing parenthesis, an operator cannot immediately follow another operator (*4+/2*) unless the second operator is the minus (*-*) operator immediately followed by a number (*5\*-2*). In this second rule, the expression *5\*-2* should be interpreted as "5 multiplied by negative 2" and considered valid, while *4+/2*, or "4 plus divided by 2" does not make sense. Furthermore, the regex for an expression in RPN would require a different set of rules. 

If you want to see how I validated the users input, check the *is_valid* function in the *parser.c* folder. Another thing to note is that our implementation creates a linked list of the tokens found on the expression in the order that that are presented (*2+3\*4* => *2-> + -> 3 -> \* -> 4*) and passed the list to the shunting yard algorithm. This is important to do to eliminate non-essential characters like spaces that are in the users input. We will now move into shunting yard. 


## The Shunting Yard Algorithm

My implementation of the shunting yard algorithm involves 2 data abstract data structures, a [queue](https://en.wikipedia.org/wiki/Queue_(abstract_data_type)) and a [stack](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)) (similar to, but not to be confused with the [stack memory construct] (https://en.wikipedia.org/wiki/Stack-based_memory_allocation)). The queue and stack are both implemented with arrays, but the way they function is precisely opposite. The queue operates in a FIFO (first-in-first-out) fashion, and the stack operates on a LIFO (last-in-first-out) basis. The stack has 2 main functions that act on it: *push* and *pop*. The push adds an element to the "top" of the array, and *pop* removes the element that is currently at the "top" of the stack. The two main operations for a queue are *enqueue* and *dequeue*. *Enqueue* adds an element to the "head" of an array, and *dequeue* removes the element as the "tail" of the queue. These will be clarified later on. For now, you need to know that the queue will be the output of the shunting yard algorithm, to be used in calculating the result, and the stack is essential in the transformation from infix to postfix. As mentioned before, the shunting yard algorithm is passed a linked list of the tokens involved in the infix expression, and evaluates them one by one. Below is the outline of the algorithm, followed by an example.


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
                    return to (a.) if there remain operators on the stack, else, go to (b.). 
        
            b. Push the current operator to the operator stack.

    Rule 3: If the current token is a left parenthesis, push it to the operator stack.

    Rule 4: If the current token is a right parenthesis:

            a. Pop operators from the operator stack and push them to the output queue until a left parenthesis is encountered.

            b. Pop the left parenthesis from the operator stack (but don't push it to the output queue).

    Rule 5: If there are no more tokens, pop remaining operators from the operator stack and push them to the output queue.

------------------------------------------
    Example: 3 + 2 * 4 - 2

    Step 1
------------------------------------------  
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

    Step 2
------------------------------------------  
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

    Step 3
------------------------------------------  
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

    Step 4
------------------------------------------  
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

    Step 5
------------------------------------------  
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

    Step 6
------------------------------------------  
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

    Step 7
------------------------------------------  
    Rule 2.a.ii
                                    
    |   |                   |   |
    |   |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 4 |                   |   |
    | 2 |                   |   |
    | 3 |                   | - |
   -------                 ------- 
    output queue            operator stack

    Step 8
------------------------------------------  
    Rule 1:

    |   |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 4 |                   |   |
    | 2 |                   |   |
    | 3 |                   | - |
   -------                 ------- 
    output queue            operator stack

    Step 9
------------------------------------------  
    Rule 5:

    | - |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 4 |                   |   |
    | 2 |                   |   |
    | 3 |                   |   |
   -------                 ------- 
    output queue            operator stack
    
                    postfix
    3 + 2 * 4 - 2   =======>  3 2 4 * + 2 -

```

Looking at the example above, we can see a visual representation of how the stack data structure works. The element at the top of the stack, which is the most recently added element, is the first to leave the stack. Try and write out a few equations, and convert them to postfix to get an intuition for this algorithm. If you want to look through my implementation of this, look for the *shunting_yard* function in the *shunting_yard.c* file, though it may be hard to follow if you are not familiar with object-oriented programming (OOP) techniques in C. For the sake of brevity, I will not get into the code, but I will try and leave some helpful comments throughout. 

Once we have converted the user’s expression to postfix, solving it is comparably simple. Again, we will use a queue and a stack, but this time, the stack will keep track of the intermediate calculations and final answer. 

## Solving expressions in postfix

The rules for solving an expression in postfix are simple:

```
    Rule 1: Dequeue the element at the front of the queue (first element in the array)
            
            a. If it is a number (operand), push it to the stack

            b. If it is an operator, pop the two topmost elements of the stack, perform
               the specified operation, and push the solution back to the stack.
    
    Rule 2: After all elements have been dequeued, the remaining operand on the stack is the final answer.
```

Thats it! In a valid operation, for every operator there are two operands. So, if you apply this algorithm to a valid
expression, you will always end up with a single operand, the answer, on the stack. Continuing with our previous example, we will show how the postfix expression is evaluated, step-by-step. Just remember with the queue data structure, we dequeue elements in the order that they were added.


```
    starting queue: [3, 2, 4, *, +, 2, -]

    Step 1
------------------------------------------    
    Rule 1.a:
    
    | - |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 4 |                   |   |
    | 2 |                   |   |
    |   |                   | 3 |
   -------                 ------- 
    input queue            solution stack

    Step 2
------------------------------------------  
    Rule 1.a:

    | - |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    | * |                   |   |
    | 4 |                   |   |
    |   |                   | 2 |
    |   |                   | 3 |
   -------                 ------- 
    input queue            solution stack

    Step 3
------------------------------------------  
    Rule 1.a:

    | - |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    | * |                   |   |
    |   |                   | 4 |
    |   |                   | 2 |
    |   |                   | 3 |
   -------                 ------- 
    input queue            solution stack

    Step 4
------------------------------------------  
    Rule 1.b:
              2 * 4 = 8                   
    | - |                   |   |
    | 2 |                   |   |
    | + |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   | 8 |
    |   |                   | 3 |
   -------                 ------- 
    input queue            solution stack

    Step 5
------------------------------------------  
    Rule 1.b:
              3 + 8 = 11                      
    | - |                   |   |
    | 2 |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   | 11|
   -------                 ------- 
    input queue            solution stack

    Step 6
------------------------------------------  
    Rule 1.a
                                    
    | - |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   | 2 |
    |   |                   | 11|
   -------                 ------- 
    input queue            solution stack

    Step 7
------------------------------------------  
    Rule 1.b
               11 - 2 = 9                     
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   |   |
    |   |                   | 9 |
   -------                 ------- 
    input queue            solution stack
 
    Final answer: 9  

```

Look over these examples, and use them as a guide to do more complex problems, such as those with multiple sets of parentheses. Take a note of step 4 and 5. Notice that when I evaluate, the first operand that is popped from the stack is used as the second operand (to the right of the operator) in the resulting expression. This does not make a difference in these examples, since addition and multiplication are [commutative](https://en.wikipedia.org/wiki/Commutative_property), but this will not always be the case. Since in this implementation we are moving from a queue to a stack, the operand at the top of the stack is the one that is the rightmost in a mathematical expression. Just to prove this answer is correct, let’s look at the original infix expression, since solving problems in this way is more intuitive for us.

```
Starting expression: 3 + 2 * 4 - 2

    Step 1
    PE[M]DAS
------------------------------------------ 

3 + (2*4) - 2 => 3 + 8 - 2

    Step 2
    PEMD[A]S
------------------------------------------ 

(3 + 8) - 2 => 11 - 2

    Step 3
    PEMDA[S]
------------------------------------------ 

11 - 2 = 9

``` 
Compare this to the steps we just used to solve the expression in postfix, and you will see that the order of operations is conserved. 

## Summary

This project is an excellent introduction into the use of stacks and queues, as well as the power of regular expressions. Regular expressions are used universally in parsing applications such as language processing where the grammar of a sentence and spelling of words needs to be validated and/or tokenized instead of mathematical objects. Stacks are widely used in algorithms such as depth first search (DFS) where backtracking is necessary, and is the main technique used by operating systems to allocate memory and keep track of memory used by a program. The queue is used in algorithms like breadth first search (BFS) and applications that run processes/threads in parallel to keep track of the order in which processes should be executed (see [parallel task scheduling](https://en.wikipedia.org/wiki/Parallel_task_scheduling)). 


