# neighborly-tour

Applies the nearest-neighbor algorithm to generate highest-scoring tours through an undirected graph with weighted edges.
The tour generation is restarted from every node in the graph and the tours are ranked by score to find an optimal nearest-neighbor tour.

Example usage:

```
make
./neighborly-tour sample_graph.txt
```
