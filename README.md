
# Coda-C_PList

## ⭐️ The Coda-C Property List Library ⭐️

This library provides a classed system of objects that model the Apple Propery List or 'plist'.
A plist is any one of the supported objects where containers reference
supported objects, but is most often a Dictionary.
Plists are easy to create and modify programmatically, and even easier to save and load from disk.

## 🔵 Features:

* Supports XML serialization in 'plist' or 'codalist' formats
* Native C object types.
* Provides container manipulation for Array and Dictionary
* Retain counted objects simplify changes to plists
* Plugin support for Binary Property List serialization with a compatable library
* A pure C source and API for increased portability

## 🟤 Requirements

* gcc/clang

## 🟢 Building

You need a modern C compiler.  Testing has been done for 64-bit Apple and Linux systems with gcc/clang.  A Makefile is provided with the default to install in /usr/local.
Note:  For Linux, "sudo ldconfig" should be run after "make install".

## 🟣 Links

* Documentation:  [Docs](https://github.com/actguru/coda-c_plist-docs)
* Homepage:  [Coda-C](https://coda-c.com/)

## 🟠 Release notes:
1. Empty \<data\> is not allowed.
2. "\<key/\>" is not allowed for blank dictionary keys.
3. XML attributes like aaa in ( \<true aaa="x" /\> ) are not allowed.
4. Set is serialized to Array for Apple XML.
