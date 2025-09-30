# Probability Chess

A hastily hacked-together implementation of probability chess. Demo [here](https://game3.party/chess).

[Algiz](https://github.com/heimskr/algiz) contains a port of the server as a plugin.

## Rules

Probability chess is like normal chess, except you can move only from the column that's randomly selected for you.
When you're in check, you're not required to immediately get out of check (and you often won't be able to).
This is because you won't always be given a chance to get yourself out of check.
However, the other player won't necessarily be able to checkmate you right away either.

To choose the column, two 6-sided dice are rolled and the results are added together. If the result is higher than 8,
subtract 8. That determines which column can be moved from. This results in a lopsided distribution. This implementation
of probability chess stays faithful to that distribution instead of fairly choosing a number between 1 and 8.
