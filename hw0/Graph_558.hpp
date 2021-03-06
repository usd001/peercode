#ifndef CME212_GRAPH_HPP
#define CME212_GRAPH_HPP

/** @file Graph.hpp
 * @brief An undirected graph type
 */

#include <algorithm>
#include <cassert>
#include <map>
#include <set>
#include <vector>

#include "CME212/Util.hpp"
#include "CME212/Point.hpp"


/** @class Graph
 * @brief A template for 3D undirected graphs.
 *
 * Users can add and retrieve nodes and edges. Edges are unique (there is at
 * most one edge between any pair of distinct nodes).
 */
class Graph {

 public:

  //
  // PUBLIC TYPE DEFINITIONS
  //

  /** Type of this graph. */
  using graph_type = Graph;

  /** Predeclaration of Node type. */
  class Node;
  /** Synonym for Node (following STL conventions). */
  using node_type = Node;

  /** Predeclaration of Edge type. */
  class Edge;
  /** Synonym for Edge (following STL conventions). */
  using edge_type = Edge;

  /** Type of node iterators, which iterate over all graph nodes. */
  class NodeIterator;
  /** Synonym for NodeIterator */
  using node_iterator = NodeIterator;

  /** Type of edge iterators, which iterate over all graph edges. */
  class EdgeIterator;
  /** Synonym for EdgeIterator */
  using edge_iterator = EdgeIterator;

  /** Type of incident iterators, which iterate incident edges to a node. */
  class IncidentIterator;
  /** Synonym for IncidentIterator */
  using incident_iterator = IncidentIterator;

  /** Type of indexes and sizes.
      Return type of Graph::Node::index(), Graph::num_nodes(),
      Graph::num_edges(), and argument type of Graph::node(size_type) */
  using size_type = unsigned;

  //
  // CONSTRUCTORS AND DESTRUCTOR
  //

  /** Construct an empty graph. */
  Graph() : points(), edg_by_ord(), edg_by_idx(), nr_nodes(0) , nr_edges(0) {
  }

  /** Default destructor */
  ~Graph() = default;

  //
  // NODES
  //

  /** @class Graph::Node
   * @brief Class representing the graph's nodes.
   *
   * Node objects are used to access information about the Graph's nodes.
   */
  class Node {
   public:
    /** Construct an invalid node.
     *
     * Valid nodes are obtained from the Graph class, but it
     * is occasionally useful to declare an @i invalid node, and assign a
     * valid node to it later. For example:
     *
     * @code
     * Graph::node_type x;
     * if (...should pick the first node...)
     *   x = graph.node(0);
     * else
     *   x = some other node using a complicated calculation
     * do_something(x);
     * @endcode
     */
    Node() {}

    /** standard Node constructor
     * param[in] g, pointer to a Graph
     *           i, index for which Node was initialised
     */
    Node(const Graph* g, size_type i) {
      idx = i;
      graph = g;
    }

    /** Return this node's position. */
    const Point& position() const { 
      // position saved inside the Graph's points vector
      return (*graph).points[this->idx];
    }

    /** Return this node's index, a number in the range [0, graph_size). */
    size_type index() const {
      return this->idx;
    }

    // HW1: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:
    // node_value_type& value();
    // const node_value_type& value() const;
    // size_type degree() const;
    // incident_iterator edge_begin() const;
    // incident_iterator edge_end() const;

    /** Test whether this node and @a n are equal.
     *
     * Equal nodes have the same graph and the same index.
     */
    bool operator==(const Node& n) const {
      if (this->idx != n.index()) {
        return false;
      }
      if (this->graph != n.graph) {
        return false;
      }
      return true;
    }

    /** Test whether this node is less than @a n in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any geometric meaning.
     *
     * The node ordering relation must obey trichotomy: For any two nodes x
     * and y, exactly one of x == y, x < y, and y < x is true.
     * 
     * Nodes are order by the 2-norm for their points and by their
     * Graph pointer if the norms are the same
     */
    bool operator<(const Node& n) const {
      // calculate 2-norms
      double norm1 = norm(position());
      double norm2 = norm(n.position());

      // determine lower 2-norm
      if (norm1 < norm2) {
        return true;
      }
      else if (norm1 > norm2) {
        return false;
      }
      // 2-norms equal: compare Graphs
      else {
        return (std::less<const Graph*>{}(graph, n.graph));
      }
    }

   private:
    // Allow Graph to access Node's private member data and functions.
    friend class Graph;
    
    size_type idx;
    const Graph* graph;
  };

  /** Return the number of nodes in the graph.
   *
   * Complexity: O(1).
   */
  size_type size() const {
    return nr_nodes;
  }

  /** Synonym for size(). */
  size_type num_nodes() const {
    return size();
  }

  /** Add a node to the graph, returning the added node.
   * @param[in] position The new node's position
   * @post new num_nodes() == old num_nodes() + 1
   * @post result_node.index() == old num_nodes()
   *
   * Complexity: O(1) amortized operations.
   */
  Node add_node(const Point& position) {
    // add this point to the saved points
    // and increment the nodes counter
    points.push_back(const_cast<Point&>(position));
    Node ret_node = node(nr_nodes);
    nr_nodes += 1;
    return ret_node;        // Invalid node
  }

  /** Determine if a Node belongs to this Graph
   * @return True if @a n is currently a Node of this Graph
   *
   * Complexity: O(1).
   */
  bool has_node(const Node& n) const {
    // check if index is inside the vector's range
    if (n.index() >= nr_nodes) {
      return false;
    }
    // check if node goves the correct Graph
    else if (this != n.graph) {
      return false;
    }
    return true;
  }

  /** Return the node with index @a i.
   * @pre 0 <= @a i < num_nodes()
   * @post result_node.index() == i
   *
   * Complexity: O(1).
   */
  Node node(size_type i) const  {
    // give out a new Node with the known properties
    return Node(this, i);
  }

  //
  // EDGES
  //

  /** @class Graph::Edge
   * @brief Class representing the graph's edges.
   *
   * Edges are order-insensitive pairs of nodes. Two Edges with the same nodes
   * are considered equal if they connect the same nodes, in either order.
   */
  class Edge {
   public:
    /** Construct an invalid Edge. */
    Edge() {}

    /** Normal Edge constructor
     * param[in] g, a pinter to the Graph
     *           i, index of the first Node
     *           j, index of the second node
     */
    Edge(const Graph* g, size_type i, size_type j) {
      idx_1 = i;
      idx_2 = j;
      graph = g;
    }

    /** Return a node of this Edge */
    Node node1() const {
      // generate new Node with knwon properties
      return (*graph).node(idx_1);      
    }

    /** Return the other node of this Edge */
    Node node2() const {
      // generate new Node with knwon properties
      return (*graph).node(idx_2); 
    }

    /** Test whether this edge and @a e are equal.
     *
     * Equal edges represent the same undirected edge between two nodes.
     */
    bool operator==(const Edge& e) const {
      // 2 equality cases: 
      // * node 1 of this edge equals node 1 of e
      // * node 2 of this graph equals node 1 of e
      if (node1() == e.node1()) {
        if (node2() == e.node2()) {
          return true;
        }
        else {
          return false;
        }
      }
      else {
        if (node2() == e.node1()) {
          if (node1() == e.node2()) {
            return true;
          }
        }
        else {
          return false;
        }
      }
      // to surpress compiler warnings
      return false;
    }

    /** Test whether this edge is less than @a e in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any interpretive meaning.
     * 
     * First compares by the Node's dot product for each Edge,
     * then by theiur Graph pointers
     */
    bool operator<(const Edge& e) const {
      // compute the 2 dot products
      double norm1 = dot((*graph).points[idx_1], (*graph).points[idx_1]);
      Point p1 = (*(e.graph)).points[e.idx_1];
      Point p2 = (*(e.graph)).points[e.idx_2];
      double norm2 = dot(p1, p2);
      
      // check if one dot product is smaller
      if (norm1 < norm2) {
        return true;
      }
      else if (norm1 > norm2) {
        return false;
      }
      // equal dot products: compare the Graph pointers
      else {
        return (std::less<const Graph*>{}(graph, e.graph));
      }
    }

   private:
    // Allow Graph to access Edge's private member data and functions.
    friend class Graph;
    
    size_type idx_1;
    size_type idx_2;
    const Graph* graph;
  };

  /** Return the total number of edges in the graph.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   * number of edges saved as incremented variable -> complexity O(1)
   */
  size_type num_edges() const {
    return nr_edges;
  }

  /** Return the edge with index @a i.
   * @pre 0 <= @a i < num_edges()
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   * Separate data structure for node as grrouped by order of creation
   * Call in O(1)
   */
  Edge edge(size_type i) const {
    // call indices from edg_by_ord vector
    // and generate the 2 Nodes with known information
    size_type idx_1 = edg_by_ord[i][0];
    size_type idx_2 = edg_by_ord[i][1];
    return Edge(this, idx_1, idx_2);    
  }

  /** Test whether two nodes are connected by an edge.
   * @pre @a a and @a b are valid nodes of this graph
   * @return True if for some @a i, edge(@a i) connects @a a and @a b.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  bool has_edge(const Node& a, const Node& b) const {
    // if no edges: can not have this edge
    if (nr_edges == 0) {
      return false;
    }
    // order the 2 indices of theb 2 Nodes by their size
    size_type idx_1 = a.index();
    size_type idx_2 = b.index();

    size_type idx_a;
    size_type idx_b;
    if (idx_1 > idx_2) {
      idx_a = idx_2;
      idx_b = idx_1;
    }
    else {
      idx_a = idx_1;
      idx_b = idx_2;
    }

    // try to find lower index 
    // (which would be a key of the map)
    auto idx_check = edg_by_idx.find(idx_a);
    if (idx_check != edg_by_idx.end()) {
      // key exists
      // check if the higher index is in the corresponding value set
      if ((idx_check->second).find(idx_b) != (idx_check->second).end()) {
        return true;
      }
    }
    return false;

  }

  /** Add an edge to the graph, or return the current edge if it already exists.
   * @pre @a a and @a b are distinct valid nodes of this graph
   * @return an Edge object e with e.node1() == @a a and e.node2() == @a b
   * @post has_edge(@a a, @a b) == true
   * @post If old has_edge(@a a, @a b), new num_edges() == old num_edges().
   *       Else,                        new num_edges() == old num_edges() + 1.
   *
   * Can invalidate edge indexes -- in other words, old edge(@a i) might not
   * equal new edge(@a i). Must not invalidate outstanding Edge objects.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge add_edge(const Node& a, const Node& b) {
    // get the indices for the 2 Nodes
    size_type idx_1 = a.index();
    size_type idx_2 = b.index();
    
    if (!has_edge(a, b)) {
      // Node not already in the Graph
      // increment edge counter
      nr_edges += 1;
      // append Node to the vector storing the indices by order
      // of creation
      std::vector<size_type> indices = {idx_1, idx_2};
      edg_by_ord.push_back(indices);

      // sort the 2 indices
      size_type idx_a;
      size_type idx_b;
      if (idx_1 > idx_2) {
        idx_a = idx_2;
        idx_b = idx_1;
      }
      else {
        idx_a = idx_1;
        idx_b = idx_2;
      }

      // check whether lower index is already a key in the map
      auto idx_check = edg_by_idx.find(idx_a);
      if (idx_check != edg_by_idx.end()) {
        // is key
        // append new corresponding index to the value set
        std::set<size_type> to_add = idx_check->second;
        to_add.insert(idx_b);
        // replace old value set with new one for this key
        edg_by_idx[idx_a] = to_add;
      }
      else {
        // lower index not a key yet
        // generate new value set and append to map
        std::set<size_type> to_add = {idx_b };
        edg_by_idx[idx_a] = to_add;
      }

    }
    return Edge(this, idx_1, idx_2);
  }

  /** Remove all nodes and edges from this graph.
   * @post num_nodes() == 0 && num_edges() == 0
   *
   * Invalidates all outstanding Node and Edge objects.
   */
  void clear() {
    // set all variables back to default
    points = {};
    edg_by_ord = {};
    edg_by_idx = {};
    nr_nodes = 0;
    nr_edges = 0;
  }

  //
  // Node Iterator
  //

  /** @class Graph::NodeIterator
   * @brief Iterator class for nodes. A forward iterator. */
  class NodeIterator {
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Node;                     // Element type
    using pointer           = Node*;                    // Pointers to elements
    using reference         = Node&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

    /** Construct an invalid NodeIterator. */
    NodeIterator() {
    }

    // HW1 #2: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:
    // Node operator*() const
    // NodeIterator& operator++()
    // bool operator==(const NodeIterator&) const

   private:
    friend class Graph;
    // HW1 #2: YOUR CODE HERE
  };

  // HW1 #2: YOUR CODE HERE
  // Supply definitions AND SPECIFICATIONS for:
  // node_iterator node_begin() const
  // node_iterator node_end() const

  //
  // Incident Iterator
  //

  /** @class Graph::IncidentIterator
   * @brief Iterator class for edges incident to a node. A forward iterator. */
  class IncidentIterator {
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Edge;                     // Element type
    using pointer           = Edge*;                    // Pointers to elements
    using reference         = Edge&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

    /** Construct an invalid IncidentIterator. */
    IncidentIterator() {
    }

    // HW1 #3: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:
    // Edge operator*() const
    // IncidentIterator& operator++()
    // bool operator==(const IncidentIterator&) const

   private:
    friend class Graph;
    // HW1 #3: YOUR CODE HERE
  };

  //
  // Edge Iterator
  //

  /** @class Graph::EdgeIterator
   * @brief Iterator class for edges. A forward iterator. */
  class EdgeIterator {
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Edge;                     // Element type
    using pointer           = Edge*;                    // Pointers to elements
    using reference         = Edge&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

    /** Construct an invalid EdgeIterator. */
    EdgeIterator() {
    }

    // HW1 #5: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:
    // Edge operator*() const
    // EdgeIterator& operator++()
    // bool operator==(const EdgeIterator&) const

   private:
    friend class Graph;
    // HW1 #5: YOUR CODE HERE
  };

  // HW1 #5: YOUR CODE HERE
  // Supply definitions AND SPECIFICATIONS for:
  // edge_iterator edge_begin() const
  // edge_iterator edge_end() const

 private:

  std::vector<Point> points;
  std::vector<std::vector<size_type>> edg_by_ord;
  std::map<size_type, std::set<size_type>> edg_by_idx;
  size_type nr_nodes;
  size_type nr_edges;

};

#endif // CME212_GRAPH_HPP