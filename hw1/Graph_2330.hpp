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


  private:
  // Use this space for declarations of important internal types you need
  // later in the Graph's definition.

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

  /** Synonym for templated node value V . */
  using node_value_type = V;


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
  Graph()
    : node_size_(0), edge_size_(0) {
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
  class Node : private totally_ordered<Node> {
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

    /** Return this node's position. */
    const Point& position() const {
      return graph_->node_elements_[uid_]->position;
    }

    /** Return this node's index, a number in the range [0, graph_size). */
    size_type index() const {
      return uid_;
    }

    /** Return this node's value. */
    node_value_type& value() {
      return graph_->node_elements_[uid_]->value;
    };

    /** Return this node's value as const. */
    const node_value_type& value() const {
      return graph_->node_elements_[uid_]->value;
    }

    /* Return the number of incident edges of this node. */
    size_type degree() const {
      return graph_->nodes_degrees_[uid_];
    }

    /* Returns an iterator to the beginning of incident edges/ */
    incident_iterator edge_begin() const {
      return IncidentIterator(graph_, uid_, 0);
    }

    /* Returns an iterator to the end of incident edges. */
    incident_iterator edge_end() const {
      return IncidentIterator(graph_, uid_, graph_->nodes_degrees_[uid_]);
    }

    /** Test whether this node and @a n are equal.
     *
     * Equal nodes have the same graph and the same index.
     */
    bool operator==(const Node& n) const {
      return n.graph_ == graph_ && n.uid_ == uid_;
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
      if(n.graph_ == graph_ && n.uid_ == uid_){
        return false;
      }
      if(graph_ < n.graph_ || (graph_ == n.graph_ && uid_ < n.uid_)){
        return true;
      }
      return false;
    }

   private:
    // Allow Graph to access Node's private member data and functions.
    friend class Graph;
    graph_type* graph_;
    size_type uid_;
    Node(const graph_type* graph, size_type idx):
                graph_(const_cast<Graph*>(graph)), uid_(idx){}

  };

  /** Return the number of nodes in the graph.
   *
   * Complexity: O(1).
   */
  size_type size() const {
    return node_size_;
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
    internal_node* new_node = new internal_node();
    new_node->position = position;
    new_node->uid = node_size_;
    node_elements_.push_back(new_node);
    node_size_ ++;

    std::vector<size_type> incident_e;
    incident_edges_.push_back(incident_e);
    nodes_degrees_.push_back(0);

    return Node(this, node_size_ - 1);        // return new node
  }

  /** Add a node to the graph, returning the added node, also support node value.
   * @param[in] position The new node's position
   * @post new num_nodes() == old num_nodes() + 1
   * @post result_node.index() == old num_nodes()
   *
   * Complexity: O(1) amortized operations.
   */
  Node add_node(const Point& position, const node_value_type& val) {
    internal_node* new_node = new internal_node();
    new_node->position = position;
    new_node->uid = node_size_;
    new_node->value = val;
    node_elements_.push_back(new_node);
    node_size_ ++;

    std::vector<size_type> incident_e;
    incident_edges_.push_back(incident_e);
    nodes_degrees_.push_back(0);

    return Node(this, node_size_ - 1);        // return new node
  }

  /** Determine if a Node belongs to this Graph
   * @return True if @a n is currently a Node of this Graph
   *
   * Complexity: O(1).
   */
  bool has_node(const Node& n) const {
    return n.graph_ == this;
  }

  /** Return the node with index @a i.
   * @pre 0 <= @a i < num_nodes()
   * @post result_node.index() == i
   *
   * Complexity: O(1).
   */
  Node node(size_type i) const {
    return Node(this, i);    // return node with index i
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
  class Edge : private totally_ordered<Edge>{
   public:
    /** Construct an invalid Edge. */
    Edge() {
    }

    /** Return a node of this Edge */
    Node node1() const {
      // size_type n1idx = graph_->edge_elements_[uid_]->uid1;
      // return Node(graph_, n1idx);      // Invalid Node

      return Node(graph_, uid1_);
    }

    /** Return the other node of this Edge */
    Node node2() const {
      // size_type n2idx = graph_->edge_elements_[uid_]->uid2;
      // return Node(graph_, n2idx);     // Invalid Node

      return Node(graph_, uid2_);
    }

    /** Test whether this edge and @a e are equal.
     *
     * Equal edges represent the same undirected edge between two nodes.
     */
    bool operator==(const Edge& e) const {
      // return this->node1() == e.node1() && this->node2() == e.node2();
      //
      // return (uid1_ == e.uid1_ && uid2_ == e.uid2_) ||
      //         (uid2_ == e.uid1_ && uid1_ == e.uid2_);;
//--design_1
//--This needs to be e.graph_ == graph_ or it doesn't compile. Also your Makefile
//--seems to have some git merge stuff in there that needs to be removed.
//--START
      return e.graph_ = graph_ && e.uid_ == uid_;
//--END
    }

    /** Test whether this edge is less than @a e in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any interpretive meaning.
     */
    bool operator<(const Edge& e) const {
      if(this->node1() == e.node1() && this->node2() == e.node2() ){
        return false;
      }
      if(this->node1() < e.node1() || (this->node1() == e.node1() && this->node2() < e.node2())){
        return true;
      }
      return false;
    }

   private:
    // Allow Graph to access Edge's private member data and functions.
    friend class Graph;

    graph_type* graph_;
    size_type uid_;    // index for undirected edge
    size_type uid1_;   // need to store node order for incident iterator
    size_type uid2_;
    // Edge(const graph_type* graph, size_type idx):
    //             graph_(const_cast<Graph*>(graph)), uid_(idx){}

    Edge(const graph_type* graph, size_type idx, size_type idx1, size_type idx2):
                graph_(const_cast<Graph*>(graph)), uid_(idx), uid1_(idx1), uid2_(idx2){}
  };

  /** Return the total number of edges in the graph.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  size_type num_edges() const {
    return edge_size_;
  }

  /** Return the edge with index @a i.
   * @pre 0 <= @a i < num_edges()
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge edge(size_type i) const {
    size_type idx1 = edge_elements_[i]->uid1;
    size_type idx2 = edge_elements_[i]->uid2;
    return Edge(this, i, idx1, idx2);
  }

  /** Test whether two nodes are connected by an edge.
   * @pre @a a and @a b are valid nodes of this graph
   * @return True if for some @a i, edge(@a i) connects @a a and @a b.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  bool has_edge(const Node& a, const Node& b) const {
    if(!has_node(a) || !has_node(b)){
      return false;
    }
    std::vector<size_type> a_incident = incident_edges_[a.index()];   // vector of incident edges
    for(size_type i = 0; i < nodes_degrees_[a.index()]; i++){
      // check incident edge edge_elements_[a_incident[i]]
      if(edge_elements_[a_incident[i]]->uid2 == b.index() || edge_elements_[a_incident[i]]->uid1 == b.index()) {
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

    std::vector<size_type> a_incident = incident_edges_[a.index()];   // vector of incident edges
    for(size_type i = 0; i < nodes_degrees_[a.index()]; i++){
      // check incident edge edge_elements_[a_incident[i]]
      if(edge_elements_[a_incident[i]]->uid2 == b.index() || edge_elements_[a_incident[i]]->uid1 == b.index()) {
        return Edge(this, a_incident[i], a.index(), b.index());
      }
    }

    // not found, add edge
    internal_edge* new_edge = new internal_edge();
    new_edge->uid1 = a.index();
    new_edge->uid2 = b.index();
    edge_elements_.push_back(new_edge);
    edge_size_ ++;

    // update incident edges
    incident_edges_[a.index()].push_back(edge_size_ - 1);
    incident_edges_[b.index()].push_back(edge_size_ - 1);
    nodes_degrees_[a.index()]++;
    nodes_degrees_[b.index()]++;

    return Edge(this, edge_size_ - 1, a.index(), b.index());
  }

  /** Remove all nodes and edges from this graph.
   * @post num_nodes() == 0 && num_edges() == 0
   *
   * Invalidates all outstanding Node and Edge objects.
   */
  void clear() {
    node_size_ = 0;
    edge_size_ = 0;
    for(size_type i = 0; i < node_size_; i++){
      delete node_elements_[i];
    }
    node_elements_.clear();
    for(size_type i = 0; i < edge_size_; i++){
      delete edge_elements_[i];
    }
    edge_elements_.clear();

    incident_edges_.clear();
    nodes_degrees_.clear();
  }

  //
  // Node Iterator
  //

  /** @class Graph::NodeIterator
   * @brief Iterator class for nodes. A forward iterator. */
  class NodeIterator : totally_ordered<NodeIterator>{
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Node;                     // Element type
    using pointer           = Node*;                    // Pointers to elements
    using reference         = Node&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::forward_iterator_tag;  // Weak Category, Proxy

    /** Construct an invalid NodeIterator. */
    NodeIterator() {
    }

    /** deference a node iterator
     * @return a node the iterator is pointing to
     */
    Node operator*() const {
      return Node(graph_, ni_id_);
    }

    /** forward increment of the NodeIterator
     * @return the next NodeIterator
     */
    NodeIterator& operator++() {
      ni_id_++;
      return *this;
    }

    /** test if this iterator is equal to @a ni.
     *
     * Equal NodeIterator points to the same node.
     */
    bool operator==(const NodeIterator& ni) const {
      return graph_ == ni.graph_ && ni_id_ == ni.ni_id_;
    }

   private:
    friend class Graph;

    graph_type* graph_;
    size_type ni_id_;

    /* NodeIterator constructor */
    NodeIterator(const graph_type* g, size_type i) : graph_(const_cast<graph_type*>(g)), ni_id_(i) {
    }

  };

  /* return the first node iterator. */
  node_iterator node_begin() const {
    return NodeIterator(this, 0);
  }

  /* return the iterator pointing to one past the last node.  */
  node_iterator node_end() const {
    return NodeIterator(this, node_elements_.size());
  }

  //
  // Incident Iterator
  //

  /** @class Graph::IncidentIterator
   * @brief Iterator class for edges incident to a node. A forward iterator. */
  class IncidentIterator :totally_ordered<IncidentIterator> {
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Edge;                     // Element type
    using pointer           = Edge*;                    // Pointers to elements
    using reference         = Edge&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::forward_iterator_tag;  // Weak Category, Proxy

    /** Construct an invalid IncidentIterator. */
    IncidentIterator() {
    }

    /* Deference an IncidentIterator.
     * @return the Edge the iterator is pointing to.
     */
    Edge operator*() const {
      // Edge should know its own node1 and node2 （which may not be represented in graph data）
      // should not modify graph data. Consider the case where two ii are instantiated.
      size_type id;
//--style_0
//--wouldn't it be clearer to just compute graph_->edge_elements[...] once and keep using that?
//--START
      if (graph_->edge_elements_[graph_->incident_edges_[node_index_][incident_index_]]->uid1 == node_index_) {
        id = graph_->edge_elements_[graph_->incident_edges_[node_index_][incident_index_]]->uid2;
      }else{
        id = graph_->edge_elements_[graph_->incident_edges_[node_index_][incident_index_]]->uid1;
      }
      return Edge(graph_, graph_->incident_edges_[node_index_][incident_index_],
                    node_index_, id);
//--END
    }
//--documentation_0
//--continue using doxygen style docs
//-START
    /* Forward increment the incident iterator. */
    IncidentIterator& operator++() {
      incident_index_++;
      return *this;
    }

    /* Check if the given incident iterator is equal to this incident iterator.
     * Equal incident iterator belongs to the same graph, the same node and points to the same edge.
     */
    bool operator==(const IncidentIterator& ii) const {
      return (graph_ == ii.graph_) && (node_index_ == ii.node_index_) && (incident_index_ == ii.incident_index_);
    }
//--END

   private:
    friend class Graph;
    graph_type* graph_;
    size_type node_index_;
    size_type incident_index_;

    IncidentIterator(const graph_type* g, size_type ni, size_type ii):
        graph_(const_cast<graph_type*>(g)), node_index_(ni), incident_index_(ii){}
  };

  //
  // Edge Iterator
  //

  /** @class Graph::EdgeIterator
   * @brief Iterator class for edges. A forward iterator. */
  class EdgeIterator : totally_ordered<EdgeIterator>{
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Edge;                     // Element type
    using pointer           = Edge*;                    // Pointers to elements
    using reference         = Edge&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::forward_iterator_tag;  // Weak Category, Proxy

//--documentation_1
//--documentation should be in doxygen style.
//--START
    /** Construct an invalid EdgeIterator. */
    EdgeIterator() {
    }

    /* Deference edge iterator. Returns the edge its pointing to. */
    Edge operator*() const {
      return Edge(graph_, uid_, graph_->edge_elements_[uid_]->uid1, graph_->edge_elements_[uid_]->uid2);
    }

    /* Forward increment the iterator. */
    EdgeIterator& operator++() {
      uid_++;
      return *this;
    }

    /* Check if the iterator is equal to a given edge iterator. */
    bool operator==(const EdgeIterator& ei) const {
      return (graph_ == ei.graph_) && (uid_ == ei.uid_);
    }
//--END

   private:
    friend class Graph;
    graph_type* graph_;
    size_type uid_;
    EdgeIterator(const graph_type* g, size_type i) : graph_(const_cast<graph_type*>(g)), uid_(i) {}
  };

  /* Returns an iterator to the beginning. */
  edge_iterator edge_begin() const {
    return EdgeIterator(this, 0);
  }

  /* Returns an iterator to the end. */
  edge_iterator edge_end() const {
    return EdgeIterator(this, edge_size_);
  }

 private:

  struct internal_node {
    Point position;   // The text held by an element
    Point::size_type uid;      // The unique identifcation for an element
    node_value_type value;
  };

  std::vector<internal_node*> node_elements_;
  size_type node_size_;

  struct internal_edge {    // undirected nodes
    Point::size_type uid1;      // The unique identifcation for node 1
    Point::size_type uid2;      // The unique identifcation for node 2
  };
  std::vector<internal_edge*> edge_elements_;
  size_type edge_size_;

  std::vector<std::vector<size_type>> incident_edges_;      // vector<size_type> is the list of edge indexes incident to this node
  std::vector<size_type> nodes_degrees_;

};

#endif // CME212_GRAPH_HPP
