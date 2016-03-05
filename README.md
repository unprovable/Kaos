**To make this explicit** - this is **experimental** software. It has been released in this state as no one has yet come forward offering to actually help with the project. As such, this is released as a PoC **only**, as I'm busy rewriting NIST testing code to fit the various scenarios that affect this algorithm. (Also, why are there no C implementations of NIST tests? Bah...)

Best case scenario - you can play with some unusual crypto. Worst case scenario, full compromise of your local system and significant data loss. YMMV.

Hope this makes it clear for people... When this is ready for release, you'll have proper comments, and less latin.

# Kaos

Proof of Concept Encryption/Decryption and Hashing algorithms. Provided with full source code and example binaries.

__NB__ - Major Bug - windows and linux binaries give different outputs. No idea why - I just write proprietary strong, practically unbreakable crypto algorithms in my spare time. This stuff I'd have to google for some time around.

## Benefits

It's fast, light on memory and resources, and thanks to the use of a chaotic iterator, very hard to break. This is an exercise in minimal code size, maximal speed, with an attempt to create crypto that is resistant to standard techniques of cryptanalysis, and possibly even Quantum Computational attack.

This is the not the first of its kind - there is a lit review that I'm working on finishing, but here are the benefits that have been missed before:

1. Speed - it's fast.
1. Security - see below for why chaos is good for this
1. 3-parameter hashes - the following can be varied, without compromising the algorithm:
 * Input string
 * Iteration count
 * Output length
1. Backdoor-free PRNG - an easy extraction from the kaoscrypt code.

## How does it work?

Very well. I'll put a full algo disclosure here, but there is an academic paper in the offing.

TL;DR - the code implements a chaotic function, in the form of the known-chaotic Rule30 Elementary Cellular Automata (ECA). The use of a chaotic function is handy for the following reason - there are things you are guaranteed to have in chaotic functions. What these are and why there are important:

1. Transitivity - 
 * A topology term, but in this case would imply that from any starting string you can (eventually) get to any other starting string. As such, the entropy of the system is as maximal as possible.
1. The Periodic Points are dense - 
 * there are conditions in which this algorithm is 'periodic', and of arbitrary length/size. Google for an image of the 'Rule30 ECA' and you'll see a triangle - on the left is the periodic part, and on the right is the chaotic part. Although these don't relate to this condition exactly, the code shows that you need to iterate left-to-right to make use of the chaotic stuff.
1. Sensitive dependence on Initial Conditions - 
 * The other main reason to use these functions - want a hashing algorithm that naturally will completely change by just changing 1 bit or several billion? Use a chaotic iterator, and let this condition do the work for you.

At the moment, I just iterate over ULL's of data. Some of the core code was found on RosettaCode, but it didn't work in the way I needed it to, as the boundary conditions were actually wrong for what I needed.

__NB__ - I have no idea who wrote that code as they're uncredited, but if they get in touch, I'll happily buy them a beer. Very, very nice way of doing things! Thank you!!! You read my mind for how to do it without referencing memory all the time!

There are some quirks in the code - I'm happy to explain them, at length, to anyone who's interested. :)


## FAQ

Q: What is 'kaos'? 
A: It's a mis-spelling of 'Chaos' so people don't think this comes from CCC or something...

Q: Why didn't you release the source first?
A: Because I wanted to be sure there was nothing I'd n00bed out on.

Q: The windows and Linux binaries give different answers between them?
A: Yeah, no idea. Linux dev, here, so Windows magic like that I'm asking those in the know about. Know how to fix it? Please, go ahead.

Q: But there's so little code? And why is the kaos_iter() so... weird?
A: Because that is Unix nature...

## Links

Primer on Chaos Theory:
https://web.math.pmf.unizg.hr/~sonja/T+PP=Chaos.pdf

RosettaCode that read and then blew my mind:
http://rosettacode.org/wiki/Elementary_cellular_automaton#C
