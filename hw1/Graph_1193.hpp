#ifndef CME212_GRAPH_HPP
#define CME212_GRAPH_HPP

/** @file Graph.hpp
 * @brief An undirected graph type
 */

#include <algorithm>
#include <vector>
#include <cassert>
#include <map>

#include "CME212/Util.hpp"
#include "CME212/Point.hpp"

/** @class Graph
 * @brief A template for 3D undirected graphs.
 *
 * Users can add and retrieve nodes and edges. Edges are unique (there is at
 * most one edge between any pair of distinct nodes).
 */
template<typename V>
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
  using node_value_type = V;

  /** Predeclaration of Edge type. */
  class Edge;
  /** Synonym for Edge (following STL conventions). */
  using edge_type = Edge;

  /** Type of node iterators, which iterate over all graph nodes. */
  class NodeIterator;
  /** Synonym for NodeIterator */
  using node_iterator = NodeIterator;

  // using node_value_type = V;
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
  Graph() {
    
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
  class Node:private totally_ordered<Node> {
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
      
    }

    /**
     * @brief Function to access and modify value associated with this node. 
     *
     * @return value associated with this node.
     */
    node_value_type& value() {
      return graph_->node_value[idx_];
    }

    /**                                                                        
     * @brief Function to access value associated with this constant node.   
     *                                                                         
     * @return value associated with this node.                                
     */
    const node_value_type& value() const{
      return graph_->node_value[idx_];
    }

    /** Return this node's position. */
    const Point& position() const {
      return graph_->node_list[idx_];
    }

    /** Return this node's index, a number in the range [0, graph_size). */
    size_type index() const {
      return idx_;
    }

    /**
     * @brief Function that shows the number of edges connected to this node.
     *
     * @return the number of edges connected with this node.
     */
    size_type degree() const {
      return graph_->edges_[idx_].size();
    }

    /**
     * @brief Function that creates an iterator from the beginning.
     *        This iterator goes through all edges connecting with this node.
     *
     * @return iterator from the beginning.
     */
    incident_iterator edge_begin() const {
      IncidentIterator it(graph_,idx_,graph_->edges_[idx_].begin());
      return it;
    }

    /**
     * @brief Function that creates an iterator to the end.
     *        This iterator is for going through all edges connecting 
     *        with this node.
     *
     * @return iterator to the end.
     */
    incident_iterator edge_end() const {
      IncidentIterator it(graph_,idx_,graph_->edges_[idx_].end());
      return it;
    }

    /** Test whether this node and @a n are equal.
     *
     * Equal nodes have the same graph and the same index.
     */
    bool operator==(const Node& n) const {
      if(graph_ == n.graph_ && idx_ == n.idx_){
        return true;
      } else {
        return false;
      }
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
      if(idx_ < n.idx_){
        return true;
      } else {
        return false;
      }
    }

   private:
    // Allow Graph to access Node's private member data and functions.
    friend class Graph;
    graph_type* graph_;
    size_type idx_;
    //node_value_type nvalue;
    Node(const graph_type* gr, size_type idx){
      graph_ = const_cast<graph_type*>(gr);
      idx_ = idx;
    }
  };

  /** Return the number of nodes in the graph.
   *
   * Complexity: O(1).
   */
  size_type size() const {
    return node_list.size();
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
  Node add_node(const Point& position, 
		const node_value_type& val = node_value_type()) {
    node_list.push_back(position);
    node_value.push_back(val);
    return node(node_list.size()-1);
  }

  /** Determine if a Node belongs to this Graph
   * @return True if @a n is currently a Node of this Graph
   *
   * Complexity: O(1).
   */
  bool has_node(const Node& n) const {
    if(n.graph_ != this or n.index() >= size()){
      return false;
    } else {
      return true;
    }
  }

  /** Return the node with index @a i.
   * @pre 0 <= @a i < num_nodes()
   * @post result_node.index() == i
   *
   * Complexity: O(1).
   */
  Node node(size_type i) const {
    assert(i >= 0);
    assert(i < num_nodes());
    return Node(this,i);
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
  class Edge:private totally_ordered<Node> {
   public:
    /** Construct an invalid Edge. */
    Edge() {

    }

    /** Return a node of this Edge */
    Node node1() const {
      return edgegraph_->node(small_node);
    }

    /** Return the other node of this Edge */
    Node node2() const {
      return edgegraph_->node(big_node);
    }

    /** Test whether this edge and @a e are equal.
     *
     * Equal edges represent the same undirected edge between two nodes.
     */
    bool operator==(const Edge& e) const {
      if(edgegraph_ != e.edgegraph_) {
        return false;
      }
      if(small_node == e.small_node and big_node == e.big_node) {
        return true;
      } else {
        return false;
      }
    }

    /** Test whether this edge is less than @a e in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any interpretive meaning.
     */
    bool operator<(const Edge& e) const {
      if(edgeidx_ < e.edgeidx_) {
        return true;
      } else {
        return false;
      }
    }

   private:
    // Allow Graph to access Edge's private member data and functions.
    friend class Graph;
    //friend class IncidentIterator;
    graph_type* edgegraph_;
    size_type small_node;
    size_type big_node;
    size_type edgeidx_;

    Edge(const graph_type* gr, size_type s_node, size_type b_node, size_type idx){
      edgegraph_ = const_cast<graph_type*>(gr);
      small_node = s_node;
      big_node = b_node;
      edgeidx_ = idx;
    }
  };

  /** Return the total number of edges in the graph.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  size_type num_edges() const {
    return edge_small.size();
  }

  /** Return the edge with index @a i.
   * @pre 0 <= @a i < num_edges()
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge edge(size_type i) const {
    return Edge(this,edge_small[i],edge_big[i],i);
  }

  /** Test whether two nodes are connected by an edge.
   * @pre @a a and @a b are valid nodes of this graph
   * @return True if for some @a i, edge(@a i) connects @a a and @a b.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  bool has_edge(const Node& a, const Node& b) const {
    assert(has_node(a));
    assert(has_node(b));
    auto search = edges_.find(a.index());
    if(search != edges_.end()){
      if(search->second.find(b.index())!=search->second.end()){
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
    assert(has_node(a));
    assert(has_node(b));
    assert(!(a==b));
    auto search = edges_.find(a.index());
    if(search != edges_.end()){
      if(search->second.find(b.index())!=search->second.end()){
        return edge(search->second[b.index()]);
      }
    }
    edge_small.push_back(a.index());
    edge_big.push_back(b.index());
    edges_[a.index()][b.index()] = num_edges()-1;
    edges_[b.index()][a.index()] = num_edges()-1;
    return edge(num_edges()-1);
  }

  /** Remove all nodes and edges from this graph.
   * @post num_nodes() == 0 && num_edges() == 0
   *
   * Invalidates all outstanding Node and Edge objects.
   */
  void clear() {
    node_list.clear();
    node_value.clear();
    edge_small.clear();
    edge_big.clear();
    edges_.clear();
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
    /**
     * @brief dereferecing operator for NodeIterator
     *
     * @return a Node this iterator points to
     */
    Node operator*() const {
      return graph_->node(itidx_);
    }

    /**
     * @brief increment operator for NodeIterator
     *
     * @return a NodeIterator
     */
    NodeIterator& operator++() {
      itidx_++;
      return *this;
    }

    /**
     * @brief equality operator for NodeIterator
     * 
     * @return boolean value
     */
    bool operator==(const NodeIterator& target) const {
      return (graph_ == target.graph_ && itidx_ == target.itidx_);
    }

    /**
     * @brief test if two NodeIterators are not equal
     *
     * @return boolean value
     */
    bool operator!=(const NodeIterator& target) const {
      return (!(*this==target));
    }

   private:
    friend class Graph;
    const Graph* graph_;
    size_type itidx_;
    NodeIterator(const Graph* assigned,size_type location) {
      graph_ = assigned;
      itidx_ = location;
    }
  };

  // HW1 #2: YOUR CODE HERE
  /**
   * @brief function that supplies iterator to the beginning of all nodes.
   *
   * @return iterator to the beginning of all nodes.
   */
  node_iterator node_begin() const {
    NodeIterator ptr = NodeIterator(this,0);
    return ptr;
  }

  /**
   * @brief function that supplies iterator to the end of all nodes.
   *
   * @return iterator to the end of all nodes.
   */
  node_iterator node_end() const {
    NodeIterator ptr = NodeIterator(this,size());
    return ptr;
  }

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

    /**
     * @brief dereferecing operator for IncidentIterator
     *
     * @return an Edge
     */
    Edge operator*() const {
      return Edge(graph_,selfidx,mapit->first,mapit->second);
    }

    /**
     * @brief increment operator for IncidentIterator
     *
     * @return an IncidentIterator
     */
    IncidentIterator& operator++() {
      mapit++;
      return *this;
    }

    /**
     * @brief equality operator for IncidentIterator
     *
     * @return boolean value
     */
    bool operator==(const IncidentIterator& rhs) const {
      return (graph_ == rhs.graph_ and mapit == rhs.mapit);
    }

    /**
     * @brief test if two IncidentIterator are not equal
     *
     * @return boolean value
     */
    bool operator!=(const IncidentIterator& rhs) const {
      return (!(*this == rhs));
    }

   private:
    friend class Graph;
    Graph* graph_;
    size_type selfidx;
    std::map<size_type,size_type>::iterator mapit;
    // HW1 #3: YOUR CODE HERE
    IncidentIterator(Graph* assigned,size_type idx,
		     std::map<size_type,size_type>::iterator it){
      graph_ = assigned;
      mapit = it;
      selfidx = idx;
    }
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
    /**
     * @brief dereferencing operator for EdgeIterator
     *
     * @return an Edge
     */
    Edge operator*() const {
      return graph_->edge(itidx);
    }

    /**
     * @brief increment operator for EdgeIterator
     *
     * @return an EdgeIterator
     */
    EdgeIterator& operator++() {
      itidx++;
      return *this;
    }

    /**
     * @brief equality operator for EdgeIterator
     *
     * @return boolean value
     */
    bool operator==(const EdgeIterator& rhs) const {
      return (graph_ == rhs.graph_ and itidx == rhs.itidx);
    }

    /**
     * @brief test if two EdgeIterator are not equal
     *
     * @return boolean value
     */
    bool operator!=(const EdgeIterator& rhs) const {
      return (!(*this == rhs));
    }

   private:
    friend class Graph;
    // HW1 #5: YOUR CODE HERE
    const Graph* graph_;
    size_type itidx;
    EdgeIterator(const Graph* assigned, size_type idx){
      graph_ = assigned;
      itidx = idx;
    }
  };

  /**
   * @brief function that provides iterator to the beginning of all edges.
   *
   * @return iterator to the beginning of all edges.
   */
  edge_iterator edge_begin() const {
    EdgeIterator ptr(this,0);
    return ptr;
  }

  /**
   * @brief function that provides iterator to the end of all edges.
   *
   * @return iterator to the end of all edges.
   */
  edge_iterator edge_end() const {
    EdgeIterator ptr(this,edge_small.size());
    return ptr;
  }

 private:

  std::vector<Point> node_list;
  std::vector<node_value_type> node_value;
  std::vector<size_type> edge_small;
  std::vector<size_type> edge_big;
  std::map<size_type,std::map<size_type,size_type>> edges_;
};

#endif // CME212_GRAPH_HPP
