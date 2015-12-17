*phases*: swanson's version of compile-time vs runtime in a compiled language,
or a clearer distinction between the import and run steps (or something like
that) for an interpreted/scripting language

We'll have a *value model* (or something like that) that is super simple,
swanson's "assembly language".  This is probably the bulk of what defines S0.
This has types, and lets you call operations on values, but doesn't have
anything like macros.  This would live at "phase 0".  You can write a Swanson
module directly in phase 0, but it would be similar to writing code directly in
assembly -- it's painful, so you only do it in extreme circumstances.

"Phase 1" would be the compiler.  It would provide a bunch of operations that
you could call, which would build up the phase 0 output module as part of their
execution.  This is where "macros" would live.  They wouldn't have any special
rules, though, like they do in a Lisp; they'd be just like any other phase-1
function that produces a phase-0 object as an output.

(You'd also want some function that you could call in phase 0 that would bring
you up into phase 1; it would call some functions in phase 1, which would need
to produce some phase-0 object as a result, and that would be returned.  This
would be the swanson equivalent of `eval`, but would be type-safe!)

[So, if a phase-1 function "returns a phase-0 object", does that mean it
actually returns the object, or is it return the phase-0 swanson code that would
construct that object?  I'm thinking the latter, to keep the boundaries nice and
clean, but I don't know yet how to describe that better.]

[I'm sure all of this is answered in the literature on reflection towers, so I
guess I should read that more closely!]
