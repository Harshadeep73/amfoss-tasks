
Q1) The 67th OEIS question, I first got the obvious idea of a geometric progression. It was mathematically correct but it crosses the constraints. So then,
    I went back and tried to fix the LCM's and remove the extra factors. I quickly realised that it was not gonna work and instead of trying to generate numbers
    from consecutive gcd's, I made the gcd's consecutive primes. This worked out both mathematically and under constraints.

Q2) This one, I did with some dp in it. First, I figured out the part where I could remove digits and make the number beautiful. Then came the greedy part and I
    struggled for a little bit there and chose dp to do this.

Q3) This one, I figured out pretty quickly, it was definitely greedy so there was like a second check for a minimum. What u do is that there is no need for a second
    rearrangement, so there should only be 1 correct rearrangement. Either that, or both the arrays are already in the minimum diff per element so we just add up the
    diff for all the elements.

Q4) The difficulty was definitely getting to me and I did this one after a long time. I overcomplicated it a lot, I mean a lot before I got the idea of it being like this:
    y = 10^L + 1 which just writes x twice. Which is always good cuz x itself is good.

Q5) I thought order of merging needed real simulation for this one, but leftmost-first is always optimal, collapsing it to one running variable: merge if bigger, 
    else reset.
