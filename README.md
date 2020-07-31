# gobang_AI
A trivial gobang AI (with a rule that white can change all chess color in the second round) based on minimax search algorithm, alpha-beta pruning algorithm and heuristic search algorithm. The AI part of this ai can be found in src/gobang/gobang/code.cpp. The UI part is in the gobangQt folder

## Algorithm 

### Minimax 

**Minimax** is a decision rule used in artificial intelligence, game theory and other field. It's goal is to minimize the worst case. It's originally formulated for two-player zero-sum game theory. From down to up in the max layer, maximize player chooose the max situation of its son in the game tree while the minimize player choose the min situation of its son.

This is ther pseudocode from wikipedia:

~~~
function minimax(node, depth, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, minimax(child, depth − 1, FALSE))
        return value
    else (* minimizing player *)
        value := +∞
        for each child of node do
            value := min(value, minimax(child, depth − 1, TRUE))
        return value
~~~



### Alpha-beta prunning

**Alpha-beta prunning** is a search algorithm that seeks to decrease the number of nodes that are evaluated by the minimax algorithm in its search tree.

In the gobang game, it's a certain that there is no need to consider many blank place. So this algorihm can significantly improve the efficiency of the AI;

This is the pseudocode from wikipedia:

~~~
function alphabeta(node, depth, α, β, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, alphabeta(child, depth − 1, α, β, FALSE))
            α := max(α, value)
            if α ≥ β then
                break (* β cut-off *)
        return value
    else
        value := +∞
        for each child of node do
            value := min(value, alphabeta(child, depth − 1, α, β, TRUE))
            β := min(β, value)
            if β ≤ α then
                break (* α cut-off *)
        return value
~~~



### Heuristic 

It's easy to find out that the branch prunned by alpha-beta prunning algorithm is changeable. If the better branch is search earlier, more branches will be prunned. In this AI, I weight the heuristic value of each place and use std::map to sort them, and also use a WIDTH array to limit the width of search of every depth.



## UI

The UI part is based on Qt Creator. 

It has PVP mode and PVE mode, suppots retract and give-in function. It also has a text browser show the game situation and a text box shows what the AI says to you.





## TO DO

- [ ] Write a js file and realise gobang-AI web
- [ ] Add sound function to Qt project