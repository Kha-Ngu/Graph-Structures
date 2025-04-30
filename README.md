<h1>📊 Graph Algorithms Implementation</h1>

<p>This project implements a versatile graph data structure in C++, complete with fundamental graph algorithms like <strong>DFS</strong>, <strong>BFS</strong>, <strong>Dijkstra’s shortest path</strong>, and <strong>Prim’s minimum spanning tree</strong>. It was developed from a starter template and extended with helper functions for modular, object-oriented design. The system supports graph construction, edge management, file input, and visual representation of edges. A bonus implementation of <strong>Kruskal’s MST algorithm</strong> is also supported.</p>

<hr>

<h2>🛠 Technologies</h2>
<ul>
  <li>C++</li>
  <li>Object-Oriented Programming</li>
  <li>Graph Theory & Algorithms</li>
</ul>

<hr>

<h2>📄 Features</h2>
<ul>
  <li>Constructors and Destructor for dynamic memory management</li>
  <li>Graph modification: add, connect, disconnect vertices/edges</li>
  <li>Read graphs from file using <code>readFile()</code></li>
  <li>Traversal algorithms: <code>dfs()</code>, <code>bfs()</code></li>
  <li>Shortest path: <code>dijkstra()</code></li>
  <li>Minimum Spanning Tree: <code>mstPrim()</code> and optional <code>mstKruskal()</code></li>
  <li>Utility methods: vertex count, edge count, degree of a vertex, and edge display</li>
</ul>

<hr>

<h2>📂 Key Functions</h2>
<pre><code>
Constructors:
  - Graph()
  - ~Graph()

Helpers:
  - contains
  - verticesSize
  - edgesSize
  - vertexDegree
  - getEdgesAsString

Modifiers:
  - add
  - connect
  - disconnect
  - readFile

Algorithms:
  - dfs
  - bfs
  - dijkstra
  - mstPrim
  - mstKruskal (bonus)
</code></pre>

<hr>

<h2>✅ Example Use Case</h2>
<pre><code>
// Create a graph and run Dijkstra
Graph g;
g.readFile("graphdata.txt");
g.dijkstra("A");
</code></pre>
