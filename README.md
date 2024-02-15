# MathVM
![MathVM](https://github.com/rdeioris/MathVM/assets/2234592/0be2bda5-6b79-4979-9e87-3c848d5a4811)

Unreal Engine plugin for Math Expressions evaluation

![image](https://github.com/rdeioris/MathVM/assets/2234592/929f682e-7ec7-4caa-b70f-781d0fb16f01)

![image](https://github.com/rdeioris/MathVM/assets/2234592/5c891201-d0bd-4127-bc08-80752761ac60)

![image](https://github.com/rdeioris/MathVM/assets/2234592/c91bdfa0-76fc-4cef-8029-afafee432bf0)


## What is it?
This plugins exposes a simple framework (both C++ and Blueprint) for running user-defined math expressions:

```
y = sin(x) * 2
```

This expression will compute the sin of x and multiply it by 2. The result will be put in y.

The two symbols (x and y), can be `local` variable or `globals` (more on this below) and, in the case of x, it can be a `constant`.

There is a very important difference between `local` and `global`: MathVM supports running expressions in parallel (read: on multiple threads) but while accessing local variables is lock-free (each
thread works on a different copy of them), global variables are shared between parallel evaluations, so a locking is required to avoid race conditions (see below). Constants are shared and lock-free (but obviously you cannot change them)

## Parrallel evaluation (A.K.A. critical sections)

If there are parts of your expressions that works over global variables, and you want to avoid race conditions you can "surround" critical sections with curly brackets (braces):

Note: the compiler will automatically detect deadlocks


## Resources

## Plotting

## Builtin Functions

### abs(n)

returns the absolute value of n.

### acos(n)

returns the arccosine (inverse cosine) of n (in radians).

### all(...)

returns 1 if all of the arguments are non 0. Otherwise returns 0.
