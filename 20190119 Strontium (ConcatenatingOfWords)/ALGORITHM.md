**The longest uniform run can be:**

1. Part or whole of a single word (e.g. "bb" in "abba" or "bb")
2. The result of concatenating two words with a prefix and suffix consisting entirely of that letter (e.g. "aa" or "bb" from "ab" and "ba")
3. As above, but with any number of words consisting entirely of that letter interposed (e.g. "a" and "aaa" between "ba" and "ab")

To calculate this, we need to iterate over all words and remember:

**Globally:**

- The length of the longest uniform run seen in any word

**For each letter individually:**

- The lengths of its longest uniform prefix and/or suffix **not** belonging to completely uniform words (adding the two will cover case 2 above)
- The total length of words consisting entirely of that letter (adding this to the result of case 2 solves the more general case 3 above)

**There is one additional complication:**

It might be that there is a longest prefix *and* a longest suffix consisting entirely of the same letter, and these are part of the same single word. In this case adding their lengths would be the same as considering the concatenation of this word to itself, which is not allowed unless the input includes multiple identical copies.

To overcome this, we can simply remember the lengths of the *two* longest affixes consisting entirely of each letter (they can of course be equal), and whenever we encounter a word having a new longest affix, check if the word's other affix is also of the same letter. If it is, then we remember the length of the other affix and will disqualify it if it turns out to also be the longest (by considering the second-longest instead).

This needs careful consideration, because e.g. with the input words "abaa" and "aba" we can consider as longest the prefix of "abaa" and end up disqualifying the suffix "aa", at first sight incorrectly as the prefix of "aba" is equally long (1 char) and does not require disqualifying "aa" which is the longest possible suffix and indeed part of the correct answer; however, if we then also consider a solution beginning with the longest suffix, all will be well.