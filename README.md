# MathVM

Unreal Engine plugin for Math Expressions evalutation

![image](https://github.com/rdeioris/MathVM/assets/2234592/929f682e-7ec7-4caa-b70f-781d0fb16f01)

![image](https://github.com/rdeioris/MathVM/assets/2234592/5c891201-d0bd-4127-bc08-80752761ac60)

## What is it?
This plugins exposes a simple framework (both C++ and Blueprint) for running user-defined math expressions:

```
y = sin(x) * 2
```

This expression will compute the sin of x and multiply it by 2. The result will be put in y.

The two symbols (x and y), can be `local` variable or `globals` (more on this below) and, in the case of x, it can be a `constant`.

There is a very important difference between `local` and `global`: MathVM is thread safe (so you can evaluaate the same expressions in parallel with different data) but while accessing local variables is lock-free (each
thread works on a different copy of them), global variables are shared between parallel evaluations, so a read-write lock is required. Constants are global and lock-free (but obviously you cannot change them)


