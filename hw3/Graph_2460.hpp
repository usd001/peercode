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

template <typename V, typename E>
class Graph {
 private:

  // HW0: YOUR CODE HERE
  // Use this space for declarations of important internal types you need
  // later in the Graph's definition.
  // (As with all the "YOUR CODE HERE" markings, you may not actually NEED
  // code here. Just use the space if you need it.)

  // JX: Predeclare the internal struct, which is used later in Helper method
  //     of node and edge class. 
  struct internal_node;
  struct internal_edge;

 public:

  //
  // PUBLIC TYPE DEFINITIONS
  //

  // HW2: add edge value
  typedef V node_value_type;
  typedef E edge_value_type;

  /** Type of this graph. */
  using graph_type = Graph;

  /** Predeclaration of Node type. */
  class Node;
  /** Synonym for Node (following STL conventions). */
  using node_type = Node;
  // using node_value_type = V;

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
  Graph(): nodes_(), edges_(), adjacency_() {
    // HW0: YOUR CODE HERE
    // JX: use the constructor of node,edge,and ajacency list here
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
      // HW0: YOUR CODE HERE
      // JX: return the position by internal_node
       //    position: internal attribute of node in graph
      return graph_->nodes_[index_].position;
    }

    // HW2: make node position modifiable
    /** Return this node's position which is modifiable. */
    Point& position() {
      return graph_->nodes_[index_].position;
    }

    /** Return this node's index, a number in the range [0, graph_size). */
    size_type index() const {
      // HW0: YOUR CODE HERE
      // JX: return node's unique index number 
      return index_;
    }

    // HW1: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:

     /** Return this node's value; value content depends on need. */
     node_value_type& value(){
       return graph_->nodes_[index_].value;
     };
    
    /** Return this node's value as const; value content depends on need */
     const node_value_type& value() const{
       return graph_->nodes_[index_].value;
     };
     
    /** Return the number of edges connected to node with index_*/
     size_type degree() const{
      return graph_-> adjacency_[index_].size();
     };

    /** Return the beginning iterator of incident list.
     *  @return an iterator pointing at the start of incident list*/
     incident_iterator edge_begin() const{
       return IncidentIterator(graph_, index_, 0);
     };
    
    /** Return the end iterator of incident list.
     *  @return an iterator pointing at the end of incident list*/
     incident_iterator edge_end() const{
       return IncidentIterator(graph_,index_, degree());
     };

    /** Test whether this node and @a n are equal.
     *
     * Equal nodes have the same graph and the same index.
     */
    bool operator==(const Node& n) const {
      // HW0: YOUR CODE HERE
      // JX: Equal nodes have the same graph and the same index.
      //     thus we need to retrun true when both conditoins are satisfied
      return graph_==n.graph_ && index_==n.index_;
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
      // HW0: YOUR CODE HERE
      // JX: there are two possible sitation where this node is less than n:
      //     1) graph is less than n's graph in order
      //     2) in same graph, but the index is less than n's index in order

      // Version 2:
      return (this->graph_ < n.graph_) || (this->graph_ == n.graph_ && this->index_ < n.index_);
    }

   private:
    // Allow Graph to access Node's private member data and functions.
    friend class Graph;
    // HW0: YOUR CODE HERE
    // Use this space to declare private data members and methods for Node
    // that will not be visible to users, but may be useful within Graph.
    // i.e. Graph needs a way to construct valid Node objects

    // JX: sizeof(Graph::Node) ≤ 16 bytes
    Graph* graph_;       // pointer back to the Graph container
    size_type index_;    // node's unique index number


    // JX: /** Private Constructor */
    Node(const Graph* graph, size_type index)
        : graph_(const_cast<Graph*>(graph)), index_(index) {
    }

  };

  /** Return the number of nodes in the graph.
   *
   * Complexity: O(1).
   */
  size_type size() const {
    // HW0: YOUR CODE HERE
    // JX: #nodes = size of nodes_, vector of nodes in graph
    return nodes_.size();
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
  Node add_node(const Point& position, const node_value_type& value = node_value_type()) {
    // HW0: YOUR CODE HERE

    // JX: create the index for new node
    size_type new_nidx = num_nodes();
    //    create the new node with given position : 
    //    Version 2: 
              // internal_node new_node;
              // new_node.position = position;
    internal_node new_node {position, value};
    //    push the new node to nodes_
    nodes_.push_back(new_node);
    //    add the node in adjacency list
    adjacency_.push_back(std::vector<std::pair<size_type,size_type>> ());
    //    return added node; 
    return Node(this, new_nidx);

  }

  /** Determine if a Node belongs to this Graph
   * @return True if @a n is currently a Node of this Graph
   *
   * Complexity: O(1).
   */
  bool has_node(const Node& n) const {
    // HW0: YOUR CODE HERE
    // JX:, this is a graph class object, check if n is a Node of this Graph
    if (n.graph_ == this){
      return true;
    } else{
      return false;
    }
  }

  /** Return the node with index @a i.   
   * @pre 0 <= @a i < num_nodes()
   * @post result_node.index() == i
   *
   * Complexity: O(1).
   */
  Node node(size_type i) const {
    // HW0: YOUR CODE HERE
    // JX: Return the node with index i in graph
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
  class Edge : private totally_ordered<Edge>{
   public:
    /** Construct an invalid Edge. */
    Edge() {
    }

    /** Return a node of this Edge */
    Node node1() const {
      // HW0: YOUR CODE HERE
      // JX: 
      return  Node(graph_, n1idx_);
    }

    /** Return the other node of this Edge */
    Node node2() const {
      // HW0: YOUR CODE HERE
      // JX: 
      return  Node(graph_, n2idx_);
    }
  
    // HW2: add value to edge
    /** Return this edge's value; value content depends on need. */
    edge_value_type& value(){
      return graph_->edges_[eidx_].value;
    };

    // HW2: add value to edge
    /** Return this edge's value as const; value content depends on need. */
    const edge_value_type& value() const{
      return graph_->edges_[eidx_].value;
    };

    // HW2: add length to edge (recommend)
    /** Return this edge's length*/
    double length() const{
      Node n1 = graph_->node(n1idx_);
      Node n2 = graph_->node(n2idx_);
      return norm(n1.position() - n2.position());
    }

    /** Test whether this edge and @a e are equal.
     *
     * Equal edges represent the same undirected edge between two nodes.
     */
    bool operator==(const Edge& e) const {
      // JX: since equal edges represent the same "undirected" edge
      //     we have to consider two situations where (a',b')==(b,a)==(a,b)
      if (this->graph_ == e.graph_ && this->n1idx_ == e.n1idx_ && this->n2idx_ == e.n2idx_){
        return true;
      } else if (this->graph_ == e.graph_ && this->n1idx_ == e.n2idx_ && this->n2idx_ == e.n1idx_){
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
      // JX:
      // Version 1:
      if (this->graph_ < e.graph_ ){
        return true;
      // this->eidx_ & eidx_ should both work
      } else  if (this->graph_ == e.graph_ && this->eidx_ < e.eidx_) {
        return true;
      } else{
        return false;
      }


    }

   private:
    // Allow Graph to access Edge's private member data and functions.
    friend class Graph;
    // HW0: YOUR CODE HERE
    // Use this space to declare private data members and methods for Edge
    // that will not be visible to users, but may be useful within Graph.
    // i.e. Graph needs a way to construct valid Edge objects

    // JX: sizeof(Graph::Edge) ≤ 32 bytes
    Graph* graph_;       // pointer back to the Graph container 
    size_type eidx_;     // edge's unique index number
    size_type n1idx_;    // index of the first vertex of edge
    size_type n2idx_;    // index of the second vertex of edge

    // JX: /** Private Constructor */
    Edge(const Graph* graph, size_type eidx, size_type n1idx, size_type n2idx)
        : graph_(const_cast<Graph*>(graph)), eidx_(eidx), n1idx_(n1idx), n2idx_(n2idx) {
    }
  };

  /** Return the total number of edges in the graph.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  size_type num_edges() const {
    // HW0: YOUR CODE HERE
    // JW: number of edge is size of edge vector, edges_
    return edges_.size();
  }

  /** Return the edge with index @a i.
   * @pre 0 <= @a i < num_edges()
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge edge(size_type i) const {
    // HW0: YOUR CODE HERE
    return Edge(this, i, edges_[i].idx1, edges_[i].idx2);

  }

  /** Test whether two nodes are connected by an edge.
   * @pre @a a and @a b are valid nodes of this graph
   * @return True if for some @a i, edge(@a i) connects @a a and @a b.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  bool has_edge(const Node& a, const Node& b) const {
    // HW0: YOUR CODE HERE
    // JX: recursion, find if b is in the adj_list of a, and also the other side
    //     for my method, I have check both (a,b), (b,a)
    //     cpmplexity: O(num_nodes)

    size_type a_idx = a.index();
    size_type b_idx = b.index();

    // with a, check if b in adjacency_ of a
    for (auto it = adjacency_[a_idx].begin(); it != adjacency_[a_idx].end(); ++it){
      if ((*it).first == b_idx){
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
  Edge add_edge(const Node& a, const Node& b, const edge_value_type& value = edge_value_type()) {
    // HW0: YOUR CODE HERE
    // JX: the main part is to update adj_list and update number of edge
    //     complexity: O(num_edges)
    size_type a_idx = a.index();
    size_type b_idx = b.index();

    if (has_edge(a,b)){
      // JX: if the edge exist, find the edge id
      // JX: existe_idx is used here to fix a error happen when only 
      //     return in the two if statements below
      size_type existe_idx;
      for(unsigned int i = 0; i != edges_.size(); i++) {
        if (edges_[i].idx1 == a_idx && edges_[i].idx2 == b_idx){
          existe_idx = i;
        }
        if (edges_[i].idx1 == b_idx && edges_[i].idx2 == a_idx){
          // return Edge(this, i, a_idx, b_idx);
          existe_idx = i;
        }
      }
      return Edge(this, existe_idx, a_idx, b_idx);
    }else {
      //  JX: if the edge not exist, add the edge and update adjacency list

      // create index for new edge
      size_type newe_idx = num_edges();
      // add the new edge to edge list
      internal_edge new_edge {a_idx, b_idx, value};
      edges_.push_back(new_edge);
      // add the new adjacency relation to adjacenct list
      adjacency_[a_idx].push_back(std::make_pair(b_idx, newe_idx));
      adjacency_[b_idx].push_back(std::make_pair(a_idx, newe_idx));
      // return the new edge added
      return Edge(this, newe_idx, a_idx, b_idx);
    }

  }

  /** Remove all nodes and edges from this graph.
   * @post num_nodes() == 0 && num_edges() == 0
   *
   * Invalidates all outstanding Node and Edge objects.
   */
  void clear() {
    // HW0: YOUR CODE HERE

    nodes_.clear();
    edges_.clear();
    adjacency_.clear();

  }



  // HW2：implementing removal method：
  /** Remove a node in the graph.
   * @param[in] n The node to remove
   * @return number of nodes that has been removed, 0 or 1
   * @post - new num_nodes() == old num_nodes() - 1
   *       - all edge in incident to @a n node should be removed
   *       - move last node in nodes_ to the position of @a n after n is removed
   *         (This will cause only O(1) ops rather than O(n) ops by removing all nodes after forward)
   *       - node @a n and its correponding NodeIterator, last node in nodes_ and its correponding NodeIterator, 
   *         and edges connected to @a n and correponding IncidentIterator are INVALIDATED.
   *
   * Complexity: O(dmax^2) < O(num_nodes) where dmax is the max degree among nodes. 
   *             since G is sparse, dmax^2 should be less than num_nodes
   */
  size_type remove_node(const Node& n){
    size_type nidx = n.index();
    // check if the edge exist; if not exist return 0;
    if (!has_node(n)){
      return 0;
    }

    // remove all the connected edges
    for (auto e_it =n.edge_begin(); e_it != n.edge_end();){
      remove_edge(*e_it);
      e_it = n.edge_begin();
    }
    
    // move last node to current node position, update adjacency list
    size_type pre_idx = size() -1;
    nodes_[nidx] = nodes_.back();
    nodes_.pop_back();

    for (auto it = adjacency_[pre_idx].begin(); it != adjacency_[pre_idx].end(); ++it){
      size_type n2idx = (*it).first;
      size_type eidx = (*it).second;

      // update the edge info of incident edge of node at pre_index
      if (edges_[eidx].idx1 == pre_idx){
        edges_[eidx].idx1 = nidx;
      }else{
        edges_[eidx].idx2 = nidx;
      }

      // erase old adjacency associated with nodes connected to pre_index
      for (auto it = adjacency_[n2idx].begin(); it != adjacency_[n2idx].end(); ++it){
        if ((*it).first == pre_idx){
        adjacency_[n2idx].erase(it);
        break;          
        }
      }
      // add adjacency of last node with its new node index
      adjacency_[nidx].push_back(std::make_pair(n2idx, eidx));
      adjacency_[n2idx].push_back(std::make_pair(nidx, eidx));
    }

    adjacency_.erase(adjacency_.begin() + pre_idx);


    return 1;


  };

  /** Remove a node in the graph.
   * @param[in] n_it The iterator pointing to node to be removed
   * @return the iterator pointing to position of node to be removed
   * @post - new num_nodes() == old num_nodes() - 1
   *       - all edge in incident to @a n node should be removed
   *       - move last node in nodes_ to the position of node associated with @a n_it
   *         (This will cause only O(1) ops rather than O(n) ops by removing all nodes after forward)
   *       - node n associated with n_it and @a n_it, last node in nodes_ and its correponding NodeIterator, 
   *         and edges connected to node n and correponding IncidentIterator are INVALIDATED.
   *
   * Complexity: O(dmax^2) < O(num_nodes) where dmax is the max degree among nodes. 
   *             since G is sparse, dmax^2 should be less than num_nodes
   */
  node_iterator remove_node(node_iterator n_it){
    remove_node(*n_it);
    return n_it;
  };

  /** Remove an edge in the graph.
   * @param[in] n1,n2 The nodes connected to the edge to be removed
   * @return number of edge that has been removed, 0 or 1
   * @post - remove an edge if there is an edge between two nodes
   *       - new num_edges() == old num_edges() - 1
   *       - move last edge in edges_ to the position of edge connected by @a n1, @a n2
   *       - edges connected to @a n1 and @a n2 and correponding EgdeIterator, last edge 
   *         in edges_ and its correponding EgdeIterator are INVALIDATED.
   * 
   * Complexity: O(dmax) where dmax is the max degree among nodes. 
   */
  size_type remove_edge(const Node& n1, const Node&n2){
    size_type n1_idx = n1.index_;
    size_type n2_idx = n2.index_;

    for (auto it = adjacency_[n1_idx].begin(); it != adjacency_[n1_idx].end(); ++it){
      if ((*it).first == n2_idx){
        Edge e = Edge(this, (*it).second, n1_idx, n2_idx);
        return remove_edge(e);
      }
    } 
    // if there is no edge between n1 and n2, return 0.
    return 0;
  };

  /** Remove an edge in the graph.
   * @param[in] e The edge to be removed
   * @return number of edge that has been removed, 0 or 1
   * @post - new num_edges() == old num_edges() - 1
   *       - move last edge in edges_ to the position of edge @a e
   *       - edges @a e and correponding EgdeIterator, last edge 
   *         in edges_ and its correponding EgdeIterator are INVALIDATED.
   *
   * Complexity: O(dmax) where dmax is the max degree among nodes.           
   */
  size_type remove_edge(const Edge& e){
    size_type eidx = e.eidx_;
    size_type n1_idx = e.node1().index_;
    size_type n2_idx = e.node2().index_;

    // check if the edge exist; if not exist return 0;
    if (!has_edge(e.node1(), e.node2())){
      return 0;
    }

    // remove edge from adjacency list for n1 and n2
    for (auto it = adjacency_[n1_idx].begin(); it != adjacency_[n1_idx].end(); ++it){
      if ((*it).first == n2_idx){
        adjacency_[n1_idx].erase(it);
        break;          
      }
    }
    for (auto it = adjacency_[n2_idx].begin(); it != adjacency_[n2_idx].end(); ++it){
      if ((*it).first == n1_idx){
        adjacency_[n2_idx].erase(it);
        break;          
      }
    }

    // move last edge to current position of e, update edge index in adjacency list
    edges_[eidx] = edges_.back();
    edges_.pop_back();

    size_type new_n1_idx = edges_[eidx].idx1;
    size_type new_n2_idx = edges_[eidx].idx2;
    for (auto it = adjacency_[new_n1_idx].begin(); it != adjacency_[new_n1_idx].end(); ++it){
      if ((*it).first == new_n2_idx){
        (*it).second = eidx;
        break;          
      }
    }
    for (auto it = adjacency_[new_n2_idx].begin(); it != adjacency_[new_n2_idx].end(); ++it){
      if ((*it).first == new_n1_idx){
        (*it).second = eidx;
        break;          
      }
    }

    return 1;

  };

  /** Remove an edge in the graph.
   * @param[in] e_it The iterator pointing to edge to be removed
   * @return the iterator pointing to position of edge to be removed
   * @post - new num_edges() == old num_edges() - 1
   *       - move last edge in edges_ to the position of edge associated with @a e_it
   *       - edges associated to @a e_it and @a e_it, last edge 
   *         in edges_ and its correponding EgdeIterator are INVALIDATED.
   *
   * Complexity: O(dmax) where dmax is the max degree among nodes. 
   */
  edge_iterator remove_edge(edge_iterator e_it){
    remove_node(*e_it);
    return e_it;
  };




  //
  // Node Iterator
  //

  /** @class Graph::NodeIterator
   * @brief Iterator class for nodes. A forward iterator. */
  class NodeIterator  : private totally_ordered<NodeIterator>{
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

    // HW1 #2: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:

    /** Dereference operator */
    Node operator*() const{
      return graph_->node(index_);
    }

    /** Increments to the next iterator in the NodeIterator class */
    NodeIterator& operator++(){
      index_++;
      return *this;
    }

    /** Test whether this iterator and @a n_iter are equal.
    *
    *  Equal iterators have the same graph and the same node index.
    *  @param[in] n_iter NodeIterator to compare with.
    */
    bool operator==(const NodeIterator& n_iter) const{
      return (graph_==n_iter.graph_) && (index_==n_iter.index_);
    }

   private:
    friend class Graph;
    // HW1 #2: YOUR CODE HERE
    Graph* graph_;    
    size_type index_; 

    // JX: /** Private Constructor */  
    NodeIterator(const Graph* graph, size_type index)
        : graph_(const_cast<Graph*>(graph)), index_(index) {
    }



  };

  // HW1 #2: YOUR CODE HERE
  // Supply definitions AND SPECIFICATIONS for:

  /** Return the beginning iterator of nodes
   *  @return an iterator pointing at the start of nodes*/
  node_iterator node_begin() const {
    return NodeIterator(this, 0);
  }

  /** Return the end iterator of nodes
   *  @return an iterator pointing at the end of nodes*/
  node_iterator node_end() const{
    return NodeIterator(this, num_nodes());
  }

  //
  // Incident Iterator
  //

  /** @class Graph::IncidentIterator
   * @brief Iterator class for edges incident to a node. A forward iterator. */
  class IncidentIterator  : private totally_ordered<IncidentIterator>{
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

    // HW1 #3: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:


    /** Dereference operator */
    Edge operator*() const{
      size_type eidx = graph_->adjacency_[index_][curr_].second; // find eidx
      size_type n1idx = index_; // find n1idx
      size_type n2idx = graph_->adjacency_[index_][curr_].first; // find n2idx
      return Edge(graph_, eidx, n1idx, n2idx);
    }

    /** Increments to the next iterator in the IncidentIterator class */
    IncidentIterator& operator++(){
      curr_++;
      return *this;
    }

    /** Test whether this iterator and @a i_iter are equal.
    *
    * Equal iterators have the same graph and the same node index 
    * and same current adjacency index.
    * @param[in] i_iter IncidentIterator to compare with
    */
    bool operator==(const IncidentIterator& i_iter) const{
      return graph_==i_iter.graph_ && index_==i_iter.index_ && curr_ == i_iter.curr_;
    }

   private:
    friend class Graph;
    // HW1 #3: YOUR CODE HERE
    Graph* graph_;
    size_type index_; 
    size_type curr_; // current index in the adjacency/incident list

    // JX: /** Private Constructor */
    IncidentIterator(const Graph* graph, size_type index, size_type curr)
        : graph_(const_cast<Graph*>(graph)), index_(index), curr_(curr) {
    }

  };

  //
  // Edge Iterator
  //

  /** @class Graph::EdgeIterator
   * @brief Iterator class for edges. A forward iterator. */
  class EdgeIterator  : private totally_ordered<EdgeIterator>{
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Edge;                     // Element type
    using pointer           = Edge*;                    // Pointers to elements
    using reference         = Edge&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::forward_iterator_tag;  // Weak Category, Proxy

    /** Construct an invalid EdgeIterator. */
    EdgeIterator() {
    }

    // HW1 #5: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:

    /** Dereference operator */
    Edge operator*() const{
      return graph_->edge(eidx_);
    }

    /** Increments to the next iterator in the EdgeIterator class */
    EdgeIterator& operator++(){
      eidx_++;
      return *this;
    }

    /** Test whether this iterator and @a e_iter are equal.
    *
    * Equal iterators have the same graph and the same edge index.
    * @param[in] e_iter EdgeIterator to compare with
    */
    bool operator==(const EdgeIterator& e_iter) const{
      return (graph_==e_iter.graph_) && (eidx_==e_iter.eidx_); 
    }

   private:
    friend class Graph;
    // HW1 #5: YOUR CODE HERE
    Graph* graph_;
    size_type eidx_; // current index in edge list
    // JX: /** Private Constructor */
    EdgeIterator(const Graph* graph, size_type eidx)
        : graph_(const_cast<Graph*>(graph)), eidx_(eidx) {
    }

  };

  // HW1 #5: YOUR CODE HERE
  // Supply definitions AND SPECIFICATIONS for:

  /** Return the beginning iterator of edges 
   *  @return an iterator pointing at the start of edges*/
  edge_iterator edge_begin() const{
    return EdgeIterator(this, 0);
  }

  /** Return the end iterator of edges 
   *  @return an iterator pointing at the end of edges*/
  edge_iterator edge_end() const{
    return EdgeIterator(this, num_edges());
  }

 private:

  // HW0: YOUR CODE HERE
  // Use this space for your Graph class's internals:
  //   helper functions, data members, and so forth.

  // JX:  Internal type for graph node and edge
  struct internal_node {
    Point position; // postion of a node
    node_value_type value;
  };

  struct internal_edge{
    size_type idx1; // index of first vertex of edge
    size_type idx2; // index of second vertex of edge
    edge_value_type value;
  };

  // JX: data member
  std::vector<internal_node> nodes_;  // vector of nodes 
  std::vector<internal_edge> edges_;  // vector of edges
  // JX: [[(1,3),(5,10),(3,9)],,..], index represent the idx of node1, vector inside
  //                   represent the index of (adjacent node, edge_idx) of node1
  std::vector<std::vector<std::pair<size_type,size_type>>> adjacency_; 

};

#endif // CME212_GRAPH_HPP
