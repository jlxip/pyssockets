# pyssockets
This is a Python wrapper for [SSockets](https://github.com/jlxip/ssockets).

## Introduction
Hope you're familiar with SSockets already. If not, [go have a look](https://github.com/jlxip/ssockets).

Coding in C is cool and all, but we all want to mess around and hack things together sometimes. Python is great for that. This is for keeping the scalability of SSockets, I/O multiplexing and all, even if degrading throughput due to the language of choice.

This module is exclusively distributed as sources, so SSockets is expected to be there when doing:

```
pip install pyssockets
```.

For this very reason, pyssockets versions are not correlated to SSockets versions.

## How to use
You might want to [see an example](https://github.com/jlxip/pyssockets/tree/master/echo/echo.py).

The following functions are available:
- `pyssockets.addState`, which receives a function.
- `pyssockets.setHangupCallback`, `pyssockets.setTimeoutCallback`, and `pyssockets.setDestroyCallback` receive a function too.
- `pyssockets.run(ip : str, port : int, nthreads : int)`

These constants are defined too:
- `pyssockets.RET_OK`
- `pyssockets.RET_READ`
- `pyssockets.RET_WRITE`
- `pyssockets.RET_ERROR`
- `pyssockets.RET_FINISHED`
