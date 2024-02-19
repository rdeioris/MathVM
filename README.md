# MathVM
![MathVM](https://github.com/rdeioris/MathVM/assets/2234592/0be2bda5-6b79-4979-9e87-3c848d5a4811)

Unreal Engine plugin for (parallel) Math Expressions evaluation and plotting

![image](https://github.com/rdeioris/MathVM/assets/2234592/929f682e-7ec7-4caa-b70f-781d0fb16f01)

![image](https://github.com/rdeioris/MathVM/assets/2234592/5c891201-d0bd-4127-bc08-80752761ac60)

![image](https://github.com/rdeioris/MathVM/assets/2234592/c91bdfa0-76fc-4cef-8029-afafee432bf0)

Join the Discord Server: https://discord.gg/WJkRY2UVhn

Support the project with Patreon: https://www.patreon.com/rdeioris

Commercial support is provided by Blitter S.r.l. (Italy) [contact the Discord Server Admin for infos]

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

### MathVMRunSimple()

```cpp
static bool MathVMRunSimple(const FString& Code, UPARAM(ref) TMap<FString, double>& LocalVariables, const TArray<UMathVMResourceObject*>& Resources, double& Result, FString& Error);
```

This is the simplest node with support for local variables, resources (see below) and a single return value

![image](https://github.com/rdeioris/MathVM/assets/2234592/8523d66f-12af-4fa5-bb6c-00025ed431e1)

### MathVMRunSimpleMulti()

```cpp
bool MathVMRunSimpleMulti(const FString& Code, UPARAM(ref) TMap<FString, double>& LocalVariables, const TArray<UMathVMResourceObject*>& Resources, const int32 PopResults, TArray<double>& Results, FString& Error)
````

This is a variant of the simple node supporting multiple return values (by specifying the amoutn of results to pop)

![image](https://github.com/rdeioris/MathVM/assets/2234592/892ea32a-d131-402f-ae7b-167f910a061a)

### MathVMRun()

```cpp
static void MathVMRun(const FString& Code, const TMap<FString, double>& GlobalVariables, const TMap<FString, double>& Constants, const TArray<UMathVMResourceObject*>& Resources, const FMathVMEvaluatedWithResult& OnEvaluated, const int32 NumSamples = 1, const FString& SampleLocalVariable = "i");
```
This is the full-featured function supporting parallel execution (by specifying the number of 'samples'). The 'SampleLocalVariable' specifies the name of the local variable that will get the current SampleId (so you can recognize each iteration by that value). Internally, ParallelFor is used, that means the tasks
will be distributed among various threads (generally based on the number of available cpu cores)

![image](https://github.com/rdeioris/MathVM/assets/2234592/f643dabc-c061-4c7d-877c-eac12ad95505)


Note: The braces in the code are used for locking (see the parallel execution section below)

### MathVMPlotter()

```cpp
static void MathVMPlotter(UObject* WorldContextObject, const FString& Code, const int32 NumSamples, const TMap<FString, FMathVMPlot>& VariablesToPlot, const TArray<FMathVMText>& TextsToPlot, const TMap<FString, double>& Constants, const TMap<FString, double>& GlobalVariables, const TArray<UMathVMResourceObject*>& Resources, const FMathVMPlotGenerated& OnPlotGenerated, const FMathVMPlotterConfig& PlotterConfig, const double DomainMin = 0, const double DomainMax = 1, const FString& SampleLocalVariable = "i");
```

This follows the same logic of MathVMRun() but plots lines and points in a texture, based on the expressions results. 

![image](https://github.com/rdeioris/MathVM/assets/2234592/dbc23085-6d1b-4735-b997-f18f7e45ea89)

The VariablesToPlot map allows you to define how to draw each sampled value (currently you can draw lines, points and a combination of two with custom thickness).

By default a 1024x1024 texture (a RenderTarget) will be returned, but you can pass an already existent render target using the PlotterConfig structure:

![image](https://github.com/rdeioris/MathVM/assets/2234592/fb8feceb-f022-4f23-842a-f73a3da7ef6b)

The structure allows to define background, borders and grid properties of the plot too.

This is an example result for 8 samples, two variables (x and y), domain (-1, 1) and a red grid:

```
y = 0;
x = 1 / (i + 1);
```

![image](https://github.com/rdeioris/MathVM/assets/2234592/481e39e3-96a3-46e5-aebf-3a6a6ca86cd3)

You can add text to the plot by adding items to the TextsToPlot array:

![image](https://github.com/rdeioris/MathVM/assets/2234592/43ff359a-8226-4e4e-99ad-74cfd62c410d)

You can put global variables in the text by surrounding them in braces. 

## The C++ API

The ```FMathVM``` class implements a full-featured VM for executing basic math and trigonometry operations. Once you have an instance you can assign Globals, Consts or Resources (see below):

```cpp
#include "MathVM.h" // Remember to add MathVM module in your Build.cs file!
...

FMathVM MathVM;
MathVM.RegisterGlobalVariable("hello", 100);
MathVM.RegisterConst("XYZ", 200);
```

You can then tokenize and compile the code:

```cpp
const bool bSuccess = MathVM.TokenizeAndCompile("y = sin(x)");
if (!bSuccess)
{
    // you can access the tokenization and compilation error FString with MathVM.GetError()
}
```

Running is accomplished with the ```Execute``` family of methods:

```cpp
// execute the compiled code passing the specified local variables and popping the specified number of results. In case of error, the method will return false and will put the message in the Error reference
bool Execute(TMap<FString, double>& LocalVariables, const int32 PopResults, TArray<double>& Results, FString& Error, void* LocalContext = nullptr);

// like Execute(), but discards the return values
bool ExecuteAndDiscard(TMap<FString, double>& LocalVariables, FString& Error, void* LocalContext = nullptr);

// like Execute() but pops a single return value
bool ExecuteOne(TMap<FString, double>& LocalVariables, double& Result, FString& Error, void* LocalContext = nullptr);

// like Execute() but ignores return values and Error message. (Useful for testing)
bool ExecuteStealth(TMap<FString, double>& LocalVariables, void* LocalContext = nullptr);
```

## Parrallel evaluation (A.K.A. critical sections)

If there are parts of your expressions that works over global variables, and you want to avoid race conditions you can "surround" critical sections with curly brackets (braces):

```
y = sin(z); {x = x + 1;}
```

Here the x increment (assuming x is a global variable) will be under lock.

Note: the compiler will automatically detect deadlocks

From C++ you can make use of the ParallelFor() function:

```cpp
ParallelFor(100, [&](const int32 ThreadId)
{
    TMap<FString, double> LocalVariables;
    LocalVariables.Add("i", ThreadId);
    MathVM->ExecuteStealth(LocalVariables);
});
```

This will try to distribute 100 evalutations by setting the "i" local variable for each one.

Generally every Execute() method is thread safe so you can use it safely in Async tasks.

## Resources

Resources are blocks of data that can be read and written by a MathVM instance. Currently Textures, Curves, DataTables and Array of doubles are supported, but you can implement your own in C++ by implementing the ```IMathVMResource``` interface.

To access those data from your expressions you can use the read() and write() functions:

```
value = read(id, ...);
write(id, ...);
```

In addition to `id` (the index of the provided resource), they have variable number of arguments as each Resource type has a different way of indexing data:

### Texture2D (readonly)

```
value = read(id, channel, u, v)
```

`channel` is the color channel (0 for r, 1 for g, 2 for b, 3 for a) u and v are the UV coordinates of the texture sample you want to read (no filtering is applied).

Currently only RGBA and BGRA no mips textures are supported. Every other type of texture will return a 0.

You can create a Texture2D Resource with 

```cpp
static UMathVMResourceObject* MathVMResourceObjectFromTexture2D(UTexture2D* Texture);
MakeShared<FMathVMTexture2DResource>(Texture); // C++ only version
```

### Curve (readonly)

```
value = read(id, channel, t)
```

```channel``` is the index of the RichCurve in the Curve asset. (0 for a simple float Curve, between 0 and 3 for color curve).

```t``` is the 'time' at which the curve is sampled.

You can create a Curve Resource with 

```cpp
static UMathVMResourceObject* MathVMResourceObjectFromCurveBase(UCurveBase* Curve);
MakeShared<FMathVMCurveBaseResource>(Curve); // C++ only version
```

### DataTable (reaonly)

```
value = read(id, row, column)
```

Get a double value from the specified row and column of the DataTable.

Only numeric columns are supported. You need to specify which columns you want to import when creating the Resource:

```cpp
static UMathVMResourceObject* MathVMResourceObjectFromDataTable(UDataTable* DataTable, const TArray<FString>& FieldNames);
MakeShared<FMathVMDataTableResource>(DataTable, FieldNames); // C++ only version
```

### Array of doubles (read/write)

```
value = read(id, index);
write(id, index, value);
```

The simples of all the provided Resources. You just specify the `index` of the array you are interested in.

You can create Array of doubles Resources with:

```cpp
static UMathVMResourceObject* MathVMResourceObjectAsDoubleArray(const int32 ArraySize);
MakeShared<FMathVMDoubleArrayResource>(ArraySize); // C++ only version
```

## Plotting

Plotting is currently sopported only via the blueprint function ```MathVMPlotter()```. While you can obviously use it from C++, a more advanced api (with SVG support) is in development.

Points, lines and texts are the only plottable objects. More shapes and graph types (like bars and pies) are in development.

## Adding functions to the VM

You can extend an FMathVM instance using the method 

```cpp
bool RegisterFunction(const FString& Name, FMathVMFunction Callable, const int32 NumArgs)
```

The ```FMathVMFunction``` represents the signature of the function:

```cpp
TFunction<bool(FMathVMCallContext& CallContext, const TArray<double>& Args)>
```

A bunch of macros are available for quick function definitions.

You can define a new function using a lambda:

```cpp
MathVM.RegisterFunction("sin2", MATHVM_LAMBDA
{
    MATHVM_RETURN(FMath::Sin(Args[0]));
}, 1);
```

This corresponds to:

```cpp
MathVM.RegisterFunction("sin2", [](FMathVMCallContext& CallContext, const TArray<double>& Args) -> bool
{
    return CallContext.PushResult(FMath::Sin(Args[0])));
}, 1);
```

The FMathVMCallContext object contains the Stack of the current execution as well as local variables and a LocalContext (it is a void pointer that can be passed by the various Execute() functions).

By passing -1 to the NumberOfArgs argument in RegisterFunction(), you can support variable number of arguments.

This is the implementation of the `all(...)` function:

```cpp
bool All(MATHVM_ARGS)
{
	for (const double Value : Args)
	{
		if (Value == 0.0)
		{
			MATHVM_RETURN(0);
		}
	}
	MATHVM_RETURN(1);
}
```

While this one (with error check and reporting) is for `equal(...)`:

```cpp
bool Equal(MATHVM_ARGS)
{
	if (Args.Num() < 2)
	{
		MATHVM_ERROR("equal expects at least 2 arguments");
	}

	for (int32 ArgIndex = 1; ArgIndex < Args.Num(); ArgIndex++)
	{
		if (Args[ArgIndex] != Args[0])
		{
			MATHVM_RETURN(0);
		}
	}

	MATHVM_RETURN(1);
}
```

## Unit Tests

The plugin has extensive code coverage available into ```Source/MathVM/Private/Tests```.

You can run from the Automation tool in the Unreal Engine Editor, or via commandline:

```UnrealEditor-Cmd.exe 'path to the .uproject file' -NoSound -nullrhi -nosplash -log -Unattended -Nopause -TestExit="Automation Test Queue Empty" -ExecCmds="Automation RunTests MathVM"```

## TODO

 * Investigate Templated version of FMathVM for supporting other types in addition to doubles (like a float one, an integer one or a vector based one)
 * Improve the Texture2D Resource
 * Investigate an error api for Resources
 * MetaSounds support (generate sounds from expressions)
 * Integration with Compushady (https://github.com/rdeioris/CompushadyUnreal) for GPU execution
 * Procedural mesh generation from expressions

## Builtin Functions in FMathVM

### abs(n)

returns the absolute value of n.

### acos(n)

returns the arccosine (inverse cosine) of n (in radians).

### all(...)

returns 1 if all of the arguments are non 0. Otherwise returns 0.

### any(...)

returns 1 if any of the arguments is non 0. Otherwise returns 0.

### asin(n)

returns the arcsine of n.

### atan(n)

returns teh arc tangent of n.

### ceil(n)

### clamp(n, x, y)

### cos(n)

returns the cosine of n.

### degrees(n)

returns the value of n radians in degrees.

### distance(...)

### dot(...)

### equal(...)

returns 1 if all of the arguments contain the same value, otherwise 0.

### exp(n)

### exp2(n)

### floor(n)

### fract(n)

returns the fractional part of n.

### gradient(...)

### greater(n, m)

returns 1 if n is greater than m. Otherwise 0.

### greater_equal(n, m)

returns 1 if n is greater or equal than m. Otherwise 0.

### hue2b(n)

### hue2g(n)

### hue2r(n)

### length(...)

### lerp(x, y, n)

returns linear interpolation of x to y with gradient n.

### less(n, m)

returns 1 if n is less than m. Otherwise 0.

### less_equal(n, m)

returns 1 if n is less or equal than m. Otherwise 0.

### log(n)

returns the base E logarithm of n.

### log10(n)

returns the base 10 logarithm of n.

### log2(n)

returns the base 2 logarithm of n.

### logx(b, n)

returns the base b logarithm of n.

### map(n, x0, x1, y0, y1)

### max(...)

returns the maximum value among the passed arguments.

### min(...)

returns the minimum value among the passed arguments.

### mod(n, m)

### not(n)

returns 0 if n not equal to 0.

### pow(n, m)

### radians(n)

returns n degrees in radians.

### rand(n, m)

returns a random value between n and m (inclusive).

### round(n)

### round_even(n)

### sign(n)

returns 1 if n > 0, -1 if n < 0, 0 if n is equal to 0.

### sin(n)

returns the sine of n.

### sqrt(n)

returns the square root of n.

### tan(n)

returns the tangent of n.

### trunc(n)

returns the integer part of n.

### read(id, ...)

read from Resource id (check the Resources section)

### write(id, ...)

write to Resource id (check the Resources section)
