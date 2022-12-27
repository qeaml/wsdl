# Contributing

*For basic contribution information, refer to the
[main "Contributing" document][main].*

## Before pushing

Ensure the example compiles, e.g. with MSVC:

```console
> .\build_example.bat
example.cpp

> #(no errors = good)
```

Ensure the tests pass, e.g. with MSVC:

```console
> cd test
> .\run_all_tests.bat
---- Test: fs
fs.cpp
==== PASSED
---- Test: rwops
rwops.cpp
==== PASSED

... and so on
>
```

[main]: https://github.com/qeaml/qeaml/blob/main/CONTRIBUTING.md
