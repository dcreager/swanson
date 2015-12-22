# Reflection tower

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

# Linear logic

J Berdine, P O'Hearn, U Reddy, and H Thielecke.  Linear continuation passing.

Great stuff, this will be key.  The variables themselves will follow something
similar to linear logic, though you won't be limited to a single use; just that
you're obligated to unreference (or throw away or whatever we call it) every
variable that you "own" or have responsibility for.  But, to make that work,
we'll need something like these linear continuation.  Example: you'll transfer
ownership of one of your variables to a continuation that you've just created.
But, that means that the continuation MUST be executed at some point, otherwise
that variable that you were responsible for will leak.  When thinking this
through this morning, I was confused with how that would work in the presence of
an `if` statement, where you have a variable that you want to be able to access
in either of the two branches.  You'd create a continuation for each branch, but
you wouldn't be able to pass ownership of the variable to both of them.  The
answer (which comes from linear logic itself, but I found it first in the above
paper) is the "additive pair", k&m.  That would represent two continuations that
are linked together.  You can't pass control to a continuation pair, but you can
project out one of the two continuations.  But the key is that doing that
"destroys" the element that you didn't project out.  So in the `if` example,
you'd create a continuation pair for the two branches, and it's the *pair* that
you pass control of the variable to, not to either of the continuations
individually.  Inside the implementation of `if`, we'll project out one or the
other continuation, depending on the value of the predicate, and pass control to
it.

In S0 itself, we can generalize this to a continuation "tuple", which consists
of one or more continuations.  A "closure" instruction would let you pass
control of some of your variables to the tuple.  The tuple would also have some
free variables that you have to provide values for at invocation time.  An
"invoke" instruction would let you choose one of the continuations in the tuple.
Any variables you have left in scope must line up with the free variables in the
tuple (since you have to deref everything, and the only way to deref some during
"invoke" is to pass that responsibility on to the continuation that you're
passing control to).  Those variables that you pass in during "invoke" will get
merged with the variables that were passed in during "closure".  And then S0
will pass control to whichever continuation you chose.  All of the other
continuations, and the tuple itself, are destroyed in the process.

Next I wondered whether "closure" and "invoke" needed to be separate
instructions, or could you combine them together into one.  But I think we do
want them separate.  Back to the `if` example, both branches will need to "meet"
back together with another merge continuation.  For that to work we'll have to
pass that merge continuation into both branches of the `if` (as a free variable
just like anything else), which in turn means that we have to be able to create
the continuation (tuple) via "closure" without having to immediately invoke it.

# Functions and the stack frame

With the CPS system described above, the "return pointer" of a function would be
passed in as a parameter.  You'd have to thread that through all of the
continuations that define the structure of the function itself, so that the last
basic block in the function can "return" by passing control to the return
pointer.  This is similar to the PowerPC calling convention, where the return
pointer is stored in a register (and the callee has to save it on the stack if
necessary), and unlike the Intel convention, where the caller stores the return
pointer on the stack before calling the function.

It seems like an alternative (more in keeping with the Berdine paper) is that a
function is modeled by a continuation transformer — a function in the semantics
that takes in a continuation and returns another.  I won't be surprised if I run
into a problem down the road that makes me switch over to this style, but for
now I'm going to stick with the explicit "return continuation is another
parameter" style.  That seems more in line with what we want to do with control
structures, anyway.

# YAML

In the spirit of supporting lots of different hosts easily, the file format for
almost everything will be YAML, with well-defined schemas that have to be
followed.  We can throw some simple compression on top of that, and we should
have something that's easy to read in just about any host language.
