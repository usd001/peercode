#ifndef CME212_GRAPH_HPP
#define CME212_GRAPH_HPP

/** @file Graph.hpp
 * @brief An undirected graph type
 */

#include <algorithm>
#include <vector>
#include <cassert>

#include "CME212/Util.hpp"
#include "CME212/Point.hpp"


/** @class Graph
 * @brief A template for 3D undirected graphs.
 *
 * Users can add and retrieve nodes and edges. Edges are unique (there is at
 * most one edge between any pair of distinct nodes).
 */
template <typename V>
class Graph {

 public:

  //
  // PUBLIC TYPE DEFINITIONS
  //

  using size_type = unsigned;

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

  /** Synonym for V, represents node attributes */
  using node_value_type = V;

  /** Type of indexes and sizes.
      Return type of Graph::Node::index(), Graph::num_nodes(),
      Graph::num_edges(), and argument type of Graph::node(size_type) */

  //
  // CONSTRUCTORS AND DESTRUCTOR
  //

  /** Construct an empty graph. */
  Graph() {}
    // HW0: DONE

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
  class Node: private totally_ordered<Node> {
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
    
    Node() {
      // HW0: DONE
      // Nothing to put here as it creates a non-valid node
    }

    /** Return this node's position. */
    const Point& position() const {
      // HW0: DONE
      return (g_->points_).at(id_);
    }

    /** Return this node's index, a number in the range [0, graph_size). */
    size_type index() const {
      // HW0: DONE
      return id_;
    }

    /** Test whether this node and @a n are equal.
     *
     * Equal nodes have the same graph and the same index.
     */
    bool operator==(const Node& n) const {
      // HW0: DONE
      return (g_ == n.g_ && id_ == n.id_);
    }

    /** Test whether this node is less than @a n in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any geometric meaning.
     *
     * The node ordering relation must obey trichotomy: For any two nodes x
     * and y, exactly one of x == y, x < y, and y < x is true.
     */
    bool operator<(const Node& n) const {
      // HW0: DONE
      return (
        position()[0] < n.position()[0]
        || (position()[0] == n.position()[0] 
          && position()[1] < n.position()[1])
        || (position()[0] == n.position()[0] 
          && position()[1] == n.position()[1] 
          && position()[2] < n.position()[2])
        );
    }

    // HW1: DONE

    /** Return this node's value */
    node_value_type& value() {
      return g_->point_values_.at(id_);
    }

    /** Return this node's value */
    const node_value_type& value() const {
      return g_->point_values_.at(id_);
    }

    /** Return this node's degree, 
     * i.e. number of neighbors in the undirected graph 
    */
    size_type degree() const{
      return g_->point_to_neighbs_[id_].size();
    }

    /** Return begin iterator over the neighbors of this node */
    incident_iterator edge_begin() const {
      return incident_iterator(g_, id_, 0);
    }

    /** Return end iterator over the neighbors of this node */
    incident_iterator edge_end() const {
      return incident_iterator(g_, id_, degree());
    }

   private:
    // Allow Graph to access Node's private member data and functions.
    friend class Graph;

    // HW0: DONE

    Node(const Graph* g, size_type uid):
       g_(const_cast<Graph*>(g)), id_(uid) {
    }
    
    //--functionality_0
    //--Nice job.
    //--But I had some trouble getting the code to compile, can you check next time
    //--that the code compiles with gcc version 5.4.0 please (on rice)? 
    //--Don't want this to cause you any points!
    //--Removing the const qualifier for the graph pointer fixed the compilation error.
    //--START
    Graph* g_; //points to the graph which contains the points
    size_type id_; // identification number 
    //--END
  };

  /** Return the number of nodes in the graph.
   *
   * Complexity: O(1).
   */
  size_type size() const {
    // HW0: DONE
    return points_.size();
  }

  /** Synonym for size(). */
  size_type num_nodes() const {
    return size();
  }

  /** Add a node to the graph, returning the added node.
   * @param[in] position The new node's position
   * @param[in] value The value of the node (if any)
   * @post new num_nodes() == old num_nodes() + 1
   * @post result_node.index() == old num_nodes()
   *
   * Complexity: O(1) amortized operations.
   */
  Node add_node(const Point& position, const node_value_type& value = node_value_type()){
    // HW0: DONE
    node_type new_node = Node(this, points_.size());
    points_.push_back(position);
    point_values_.push_back(value);
    point_to_neighbs_.push_back(std::vector<size_type>());
    return new_node;
  }

  /** Determine if a Node belongs to this Graph
   * @return True if @a n is currently a Node of this Graph
   *
   * Complexity: O(1).
   */
  bool has_node(const Node& n) const {
    // HW0: DONE
    return (n.g_ == this);
  }

  /** Return the node with index @a i.
   * @pre 0 <= @a i < num_nodes()
   * @post result_node.index() == i
   *
   * Complexity: O(1).
   */
  Node node(size_type i) const {
    // HW0: DONE
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
  class Edge: private totally_ordered<Edge> {
   public:
    /** Construct an invalid Edge. */
    Edge() {
      // HW0: DONE
      // invalid construction so we do nothing
    }

    /** Return a node of this Edge */
    Node node1() const {
      // HW0: DONE
      return Node(g_, node1_);      
    }

    /** Return the other node of this Edge */
    Node node2() const {
      // HW0: DONE
      return Node(g_, node2_); 
    }

    /** Test whether this edge and @a e are equal.
     *
     * Equal edges represent the same undirected edge between two nodes.
     */
    bool operator==(const Edge& e) const {
      return (g_ == e.g_ && id_ == e.id_);
    }

    /** Test whether this edge is less than @a e in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any interpretive meaning.
     */
    bool operator<(const Edge& e) const {
      return (
        node1() < e.node1() 
        || (!(e.node1() < node1()) // i.e. equal in position
          && node2() < e.node2())
      );
    }

   private:
    // Allow Graph to access Edge's private member data and functions.
    friend class Graph;
    // HW0: DONE

    /** Edge initialization with explicit order on nodes */
    Edge(const Graph* g, size_type eid, size_type node1, size_type node2):
      g_(g), id_(eid), node1_(node1), node2_(node2) {
    }

    /** Edge initialization without explicit order on nodes */
    Edge(const Graph* g, size_type eid):
      g_(g), id_(eid){
        node1_ = (g_->edges_).at(id_).first;
        node2_ = (g_->edges_).at(id_).second;
    }

    const Graph* g_;
    size_type id_;
    size_type node1_;
    size_type node2_;
  };

  /** Return the total number of edges in the graph.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  size_type num_edges() const {
    // HW0: DONE
    return edges_.size();
  }

  /** Return the edge with index @a i.
   * @pre 0 <= @a i < num_edges()
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge edge(size_type i) const {
    // HW0: DONE
    return Edge(this, i);
  }

  Edge edge(size_type i, size_type node1, size_type node2) const {
    // HW0: DONE
    return Edge(this, i, node1, node2);
  }

  /** Test whether two nodes are connected by an edge.
   * @pre @a a and @a b are valid nodes of this graph
   * @return True if for some @a i, edge(@a i) connects @a a and @a b.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  bool has_edge(const Node& a, const Node& b) const {

    // HW0: YOUR CODE HERE
    size_type index_a = a.index();
    size_type index_b = b.index();

    size_type min_index = std::min(index_a, index_b);
    size_type max_index = std::max(index_a, index_b);

    return (points_to_edge_.find(std::pair<int,int>(min_index,max_index)) != points_to_edge_.end());
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
    // HW0: DONE

    size_type index_a = a.index();
    size_type index_b = b.index();

    size_type min_index = std::min(index_a, index_b);
    size_type max_index = std::max(index_a, index_b);

    if (has_edge(a, b)){
      return edge(points_to_edge_.find(std::pair<int,int>(min_index,max_index))->second);
    }

    // if the edge doesn't exist, create the new edge and returns it
    size_type edge_id = edges_.size();
    edges_.push_back(std::make_pair(min_index, max_index));
    points_to_edge_.insert({std::pair<int,int>(min_index, max_index), edge_id});
    point_to_neighbs_[min_index].push_back(max_index);
    point_to_neighbs_[max_index].push_back(min_index);
    edge_type new_edge = Edge(this, edge_id);

    return new_edge;
  }

  /** Remove all nodes and edges from this graph.
   * @post num_nodes() == 0 && num_edges() == 0
   *
   * Invalidates all outstanding Node and Edge objects.
   */
  void clear() {
    // HW0: DONE
    points_.clear();
    point_values_.clear();
    edges_.clear();
    points_to_edge_.clear();
    point_to_neighbs_.clear();
  }

  //
  // Node Iterator
  //

  /** @class Graph::NodeIterator
   * @brief Iterator class for nodes. A forward iterator. */
  class NodeIterator: private equality_comparable<NodeIterator> {
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

    // HW1 #2: DONE

    Node operator*() const{
      return g_->node(current_);
    }

    NodeIterator& operator++(){
      ++current_;
      return *this;
    }

    bool operator==(const NodeIterator& nodeIter) const{
      return (current_ == (nodeIter.current_) && g_ == nodeIter.g_);
    }

   private:
    friend class Graph;
    const Graph* g_;
    int current_;

    NodeIterator(const Graph* graph, int start_pos):
      g_(graph), current_(start_pos){
      }
  };

  // HW1 #2: DONE

  node_iterator node_begin() const{
    return node_iterator(this, 0);
  }

  node_iterator node_end() const{
    return node_iterator(this, num_nodes());
  }


  //
  // Incident Iterator
  //

  /** @class Graph::IncidentIterator
   * @brief Iterator class for edges incident to a node. A forward iterator. */
  class IncidentIterator: private equality_comparable<IncidentIterator> {
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

    // HW1 #3: DONE

    Edge operator*() const{
      size_type neighb_id = g_->point_to_neighbs_[node_id_][current_in_neighbs_];
      size_type min_index = std::min(neighb_id, node_id_);
      size_type max_index = std::max(neighb_id, node_id_);
      size_type id = (g_->points_to_edge_.find(std::pair<int,int>(min_index,max_index)))->second;
      return Edge(g_, id, node_id_, neighb_id);
    }

    IncidentIterator& operator++(){
      ++current_in_neighbs_;
      return *this;
    }

    bool operator==(const IncidentIterator& incidentIter) const{
      return (g_ == incidentIter.g_ && 
              current_in_neighbs_ == incidentIter.current_in_neighbs_ &&
              node_id_ == incidentIter.node_id_);
    }

   private:
    // HW1 #3: DONE
    friend class Graph;

    const Graph* g_;
    const size_type node_id_;
    size_type current_in_neighbs_; //current index in the vector of the node's neighbors

    IncidentIterator(const Graph* graph, const size_type node_id, const size_type start_pos):
      g_(graph), node_id_(node_id), current_in_neighbs_(start_pos){
    }
  };

  //
  // Edge Iterator
  //

  /** @class Graph::EdgeIterator
   * @brief Iterator class for edges. A forward iterator. */
  class EdgeIterator: private equality_comparable<EdgeIterator>{
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

    // HW1 #5: DONE

    Edge operator*() const{
      return g_->edge(current_);
    }

    EdgeIterator& operator++(){
      ++current_;
      return *this;
    }

    bool operator==(const EdgeIterator& edgeIter) const{
      return (current_ == (edgeIter.current_) && g_ == edgeIter.g_);
    }

   private:
    friend class Graph;
    // HW1 #5: DONE

    const Graph* g_;
    size_type current_;

    EdgeIterator(const Graph* graph, const size_type start_pos):
      g_(graph), current_(start_pos){
    }
  };

  // HW1 #5: DONE

  edge_iterator edge_begin() const{
    return edge_iterator(this, 0);
  }

  edge_iterator edge_end() const{
    return edge_iterator(this, num_edges());
  }

 private:

  // HW0: DONE
  std::vector<Point> points_;
  std::vector<std::pair<size_type, size_type>> edges_;
  std::map<std::pair<size_type, size_type>, size_type> points_to_edge_;
  std::vector<node_value_type> point_values_;
  std::vector<std::vector<size_type>> point_to_neighbs_;
};

#endif // CME212_GRAPH_HPP
