## DEPP

DEPP is a functional language in a lisp-like syntax that features automatic asynchrony.
All function calls are cooperatively scheduled according the amount of available threads
on the machine.

Concurrency is managed using the actor model found in several modern libraries and languages.
The language follows the form of: `(function dependencies dependencies)` only when all dependencies
have been resolved to literals will the function execute.

DEPP features three primary container types that help further define code execution and structure:

* __Vectors__ are defined using the syntax `[]`. Each dependency in the container will be executed
in a serial manner. E.g.
```
[(print 1) (print 2) (print 3)]
```
will always print in the order `1 2 3`.

* __Lists__ are defined using traditional lisp parens `()`. Each dependency will spawn in a new actor
and act independently of any other action occuring. E.g.
```
((print 1) (print 2) (print 3))
```
has no defined output and can occur in any order.

* __Maps__ are the sole method of storing object structure beyond lists or vectors. These are simple
associative arrays between an identifier and either 1) another map container 2) a literal container
or 3) an atom. The illustrative syntax is as follows:

```
(def example {id: 1, colors: '(green blue red)})
(# id example) ; Return the integer 1
```
