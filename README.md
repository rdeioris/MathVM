# MathVM
![MathVM](https://github.com/rdeioris/MathVM/assets/2234592/0be2bda5-6b79-4979-9e87-3c848d5a4811)

Unreal Engine plugin for (parallel) Math Expressions evaluation and plotting

![image](https://github.com/rdeioris/MathVM/assets/2234592/929f682e-7ec7-4caa-b70f-781d0fb16f01)

![image](https://github.com/rdeioris/MathVM/assets/2234592/5c891201-d0bd-4127-bc08-80752761ac60)

![image](https://github.com/rdeioris/MathVM/assets/2234592/c91bdfa0-76fc-4cef-8029-afafee432bf0)


## What is it?
This plugins exposes a simple framework (both C++ and Blueprint) for running user-defined math expressions:

```
y = sin(x) * 2
```

This expression will compute the sin of x and multiply it by 2. The result will be put in y.

The two symbols (x and y), can be `local` variables or `globals` (more on this below) and, in the case of x, it can be a `constant`.

There is a very important difference between `local` and `global`: MathVM supports running expressions in parallel (read: on multiple threads) but while accessing local variables is fully thread-safe and lock-free (each
thread works on a different copy of them), global variables are shared between parallel evaluations. For global variables a locking is required to avoid race conditions (see below). Constants are shared and lock-free (but obviously you cannot change them).

The native data type is double and multiple statements can be specified by using the ```;``` separator:

```
y = tan(x * 2); y = y + (3 * sin(z)); final = y + x;
```

The language has no support for loops or conditionals, this increases safety for external provided code and keeps the VM very simple and efficient (technically it is an extended Shunting Yard https://en.wikipedia.org/wiki/Shunting_yard_algorithm).

## Comments

You can add comments in your expressions using the # symbol. 

Comments can be per-line:

```
y = sin(x);
# this is a comment
z = cos(w);
# this is another comment
```

or in-line

```
y = sin(x); # compute the sin of x # z = cos(w); # this is another comment # xyz = 100;
```

## The Blueprint API

## The C++ API

## Parrallel evaluation (A.K.A. critical sections)

If there are parts of your expressions that works over global variables, and you want to avoid race conditions you can "surround" critical sections with curly brackets (braces):

```
y = sin(z); {x = x + 1;}
```

Here the x increment (assuming x is a global variable) will be under lock.

Note: the compiler will automatically detect deadlocks


## Resources

Resources are blocks of data that can be read and written by a MathVM instance. Currently Textures, Curves and Array of doubles are supported, but you can implement your own in C++ by implementing the ```IMathVMResource``` interface.

To access those data from your expressions you can use the read() and write() functions:

```
```

## Plotting

## Builtin Functions

### abs(n)

returns the absolute value of n.

### acos(n)

returns the arccosine (inverse cosine) of n (in radians).

### all(...)

returns 1 if all of the arguments are non 0. Otherwise returns 0.
