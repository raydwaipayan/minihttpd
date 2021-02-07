<div align="center">

<img src="https://socialify.git.ci/raydwaipayan/minihttpd/image?description=1&language=1&owner=1&pattern=Circuit%20Board&theme=Dark" />

<b>minihttpd</b>
<br/>
</div>

## What is it?

A tiny http web server to serve static content and execute php scripts (php-cgi).

The server optionally supports php-fpm through an optional fastcgi configuration that needs to be enabled
during compilation.

## Usage

```bash
./minihttpd [Directory] [PORT (default = 8000)]
```

* On some systems the server is needed to be run as root.

## Build

The binary can be built by simply running:
```bash
make
```

The minihttpd binary is deployed to `./build`

## Enable FAST CGI

The web server can use fast cgi just by setting
a macro in `srd/httpd.c`:

```c
#define FAST_CGI 1
```