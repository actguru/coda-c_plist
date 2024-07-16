
# Coda-C_PList

## The Coda-C Property List Library

This library provides a classed system of objects that model the Apple Propery List or 'plist'.
A plist is any one of the supported objects, but is most often a Dictionary.
Plists are easy to create and modify programmatically, and even easier to save and load from disk.

## Features:

* Supports XML serialization in 'plist' or 'codalist' formats
* Native C object types.
* Provides container manipulation for Array and Dictionary
* Retain counted objects simplify changes to plists
* Plugin support for Binary Property List serialization with a compatable library
* A pure C source and API for increased portability

## Building

You need a modern C compiler.  Testing has been done for 64-bit Apple, Linux, and Windows systems with gcc/clang.  A Makefile is provided but not required.  The resulting library
and headers should be installed by you, normally in /usr/local.

## Links

* Homepage:  [Coda-C](https://coda-c.com/)

> [!TIP]
> Release notes:
> 1. Empty \<data\> is not allowed.
> 2. "\<key/\>" is not allowed for blank dictionary keys.
> 3. XML attributes like aaa in ( \<true aaa="x" /\> ) are not allowed.
> 4. Set is serialized to Array for Apple XML.



> [!CAUTION]
> UNDER CONSTRUCTION

a

> [!NOTE]
> Useful information that users should know, even when skimming content.

> [!TIP]
> Helpful advice for doing things better or more easily.

> [!IMPORTANT]
> Key information users need to know to achieve their goal.

> [!WARNING]
> Urgent info that needs immediate user attention to avoid problems.

> [!CAUTION]
> Advises about risks or negative outcomes of certain actions.
