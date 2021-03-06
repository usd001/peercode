#ifndef CME212_GRAPH_HPP
#define CME212_GRAPH_HPP

/** @file Graph.hpp
 * @brief An undirected graph type
 */

#include <algorithm>
#include <vector>
#include <cassert>
#include <tuple>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <utility>

#include "CME212/Util.hpp"
#include "CME212/Point.hpp"

//--documentation_-1
//--pretty good docs although operator++ would benefit from a postcondition
//--END
/** @class Graph
 * @brief A template for 3D undirected graphs.
 *
 * Users can add and retrieve nodes and edges. Edges are unique (there is at
 * most one edge between any pair of distinct nodes).
 */
template <typename V> 
class Graph {
 private:

  // Predeclare the struct which will hold node info internally
  struct node_info;


 public:

  //
  // PUBLIC TYPE DEFINITIONS
  //

  /** Type of nodal values (depends on template parameter)*/
  using node_value_type = V;

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
  Graph() {
    // HW0: Constructor, vectors are default initialized to size 0
    num_nodes_ = internal_nodes_.size();
    num_edges_ = internal_edges_.size();
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
      // HW0: Construct invalid node with public constructor
      // Do nothing on purpose, this node is invalid
    }

    /** Return this node's position. */
    const Point& position() const {
      // HW0: return the Point located at this Node's index within
      // this Node's associated Graph
      return graph_->internal_nodes_[index_].node_loc;
    }

    /** Return this node's index, a number in the range [0, graph_size). */
    size_type index() const {
      // HW0: return the value stored as index_
      return index_;
    }

    /** Return a reference to the value stored by this Node 
     * @return Reference to the value (type determined by template parameter)
     *     stored in internal node data at location given by @a index_
     * @pre The current node is valid
    */
    node_value_type& value() {
      return graph_->internal_nodes_[index_].node_val;
    }

    /** Return a const reference to the value stored by this Node 
     * @return Reference to the value (type determined by template parameter)
     *     stored in internal node data at location given by @a index_
     * @pre The current node is valid
     * This is meant as a read-only method, so should not be called to change values
    */
    const node_value_type& value() const {
      return graph_->internal_nodes_[index_].node_val;
    }

    /** Return an incident iterator pointing to first adjacent edge to current Node
     * @return incident_iterator type constructed with begin() of adjacency map
     *     for the current node
     * @pre This Node is valid
    */
    incident_iterator edge_begin() const{
      return IncidentIterator(graph_,index_,graph_->internal_adj_[index_].begin());
    }

    /** Return an incident iterator pointing to last adjacent edge to current Node
     * @return incident_iterator type constructed with end() of adjacency map
     *     for the current node
     * @pre This Node is valid
    */
    incident_iterator edge_end() const {
      return IncidentIterator(graph_,index_,graph_->internal_adj_[index_].end());
    }

    /** Return the number of adjacent edges/nodes to the current Node
     * @return Type size_type value giving number of nodes in this Node's adjacency
     * @pre This Node is valid
    */
    size_type degree() const {

      // Loop through adjacent edges, add one to counter for each edge visited
      size_type num_incident = 0;
      for (incident_iterator iter = edge_begin(); iter != edge_end(); ++iter){
        num_incident++;
      }

      return num_incident;
    }


    /** Test whether this node and @a n are equal.
     *
     * Equal nodes have the same graph and the same index.
     */
    bool operator==(const Node& n) const {
      // HW0: if both do not point to same Graph or have different 
      // indices, return false, otherwise return true
      if ((this->graph_ != n.graph_) or (this->index_ != n.index_))
        return false;
      else
        return true;
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
      // HW0: If both Nodes point to same graph, then just compare index
      if (this->graph_ == n.graph_){
        if (this->index_ < n.index_)
          return true;
        else
          return false;
      }

      // HW0: If Nodes point to different graphs, use ordering on pointers
      // to Graph objects
      std::less<graph_type*> check;
      if (check(this->graph_,n.graph_))
        return true;
      else
        return false;
    }

   private:
    // Allow Graph to access Node's private member data and functions.
    friend class Graph;

    // HW0: Initialize member variables: pointer to Graph and index 
    // telling us where in Graph internal node data to get Point
    graph_type* graph_;
    size_type index_;

    // HW0: Private constructor for valid Nodes, given Graph and index
    Node(const graph_type* graph, size_type ind)
      : graph_(const_cast<graph_type*>(graph)), index_(ind) {}

  };

  /** Return the number of nodes in the graph.
   *
   * Complexity: O(1).
   */
  size_type size() const {
    // HW0: return value stored in num_nodes member variable
    return num_nodes_;
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
  Node add_node(const Point& position, const node_value_type& val = node_value_type()) {
    // Use initializer list to add position and value to internal nodes vector
    // then increment # nodes, allocate space for adjacency, and call Node constructor
    // to return

    internal_nodes_.push_back({position,val});
    num_nodes_++;
    internal_adj_.emplace_back();

    return Node(this, num_nodes_-1);
  }

  /** Determine if a Node belongs to this Graph
   * @return True if @a n is currently a Node of this Graph
   *
   * Complexity: O(1).
   */
  bool has_node(const Node& n) const {
    // HW0: check if n points to the current Graph
    // and ensure the index value is valid
    if ((this == n.graph_) and (n.index_ < this->num_nodes_) and (n.index_ >= 0))
      return true;
    else
      return false;
  }

  /** Return the node with index @a i.
   * @pre 0 <= @a i < num_nodes()
   * @post result_node.index() == i
   *
   * Complexity: O(1).
   */
  Node node(size_type i) const {
    // HW0: Construct valid Node using this and input index, 
    // ensuring input index is valid for this graph
    assert(i < this->num_nodes_);
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
//--functionality_0
//--don't forget to fix this
//--START
  class Edge : private totally_ordered<Node> {
//--END
   public:
    /** Construct an invalid Edge. */
    Edge() {
      // HW0: Public constructor for invalid edge
      // Do nothing on purpose, this edge is invalid
    }

    /** Return a node of this Edge */
    Node node1() const {
      // HW0: return a Node created with the current Graph and index
      // given by the first entry in this Edge's internal tuple
      return Node(graph_, node1_); 
    }

    /** Return the other node of this Edge */
    Node node2() const {
      // HW0: return a Node created with the current Graph and index
      // given by the second entry in this Edge's internal tuple
      return Node(graph_, node2_); 
    }

    /** Test whether this edge and @a e are equal.
     *
     * Equal edges represent the same undirected edge between two nodes.
     */
    bool operator==(const Edge& e) const {
      // HW0: check if both point to same graph and have same internal index
      if ((this->graph_ != e.graph_) or (this->index_ != e.index_))
        return false;
      else
        return true;
    }

    /** Test whether this edge is less than @a e in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any interpretive meaning.
     */
    bool operator<(const Edge& e) const {
      // HW0: If the two have same graph, simply compare the index_ values, 
      // which refer to the order of the edges within the internal structure
      if (this->graph_ == e.graph_){
        if (this->index_ < e.index_)
          return true;
        else
          return false;
      }

      // HW0: Otherwise, use ordering on Graph pointers
      std::less<graph_type*> check;
      if (check(this->graph_,e.graph_))
        return true;
      else
        return false;
    }

   private:
    // Allow Graph to access Edge's private member data and functions.
    friend class Graph;

    // HW0: Initialize member variables: pointer to Graph and index
    // which tells us where to look in internal data to get the endpoints
    graph_type* graph_;
    size_type index_;
    size_type node1_;
    size_type node2_;

    // HW0: Private constructor for valid Edges, given Graph and index
    Edge(const graph_type* graph, size_type ind, size_type node1, size_type node2)
      : graph_(const_cast<graph_type*>(graph)), index_(ind), node1_(node1), node2_(node2) {}

  };

  /** Return the total number of edges in the graph.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  size_type num_edges() const {
    // HW0: return the value stored in internal num_edges_ variable
    return num_edges_;
  }

  /** Return the edge with index @a i.
   * @pre 0 <= @a i < num_edges()
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge edge(size_type i) const {
    // HW0: create valid Edge using the current Graph and i,
    // ensuring index is valid
    assert(i < this->num_edges_);
    return Edge(this, i, std::get<0>(this->internal_edges_[i]), std::get<1>(this->internal_edges_[i]));
  }

  /** Test whether two nodes are connected by an edge.
   * @pre @a a and @a b are valid nodes of this graph
   * @return True if for some @a i, edge(@a i) connects @a a and @a b.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  bool has_edge(const Node& a, const Node& b) const {
    // HW0: Get the nodal indices, search through adjacency structure
    // to see if these nodes are connected

    size_type inda = a.index_;
    size_type indb = b.index_;

    auto search = internal_adj_[inda].find(indb);

    if (search != internal_adj_[inda].end())
      return true;
    else
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
//--functionality_1
//--edge ids in adjacency mapper are still off by 1
//--START
  Edge add_edge(const Node& a, const Node& b) {

    // HW0: Check if there is an edge by calling the previous method,
    // if it does exist then get the index associated with this edge and return
    // a valid edge

    bool exists = has_edge(a, b);
    size_type inda = a.index_;
    size_type indb = b.index_;
    if (exists) {
      size_type edge_ind = internal_adj_[inda][indb];
      return Edge(this,edge_ind,inda,indb);
    }

    // Otherwise, create this edge in edge list, modify the nodal
    // adjacencies, increment # edges, and return valid Edge
    else {
      std::tuple <size_type, size_type> add;
      add = std::make_tuple(inda,indb);
      num_edges_++;
      internal_edges_.push_back(add);
      internal_adj_[inda][indb] = num_edges_;
      internal_adj_[indb][inda] = num_edges_;
      return Edge(this,num_edges_-1,inda,indb);
    }
    
  }
//--END

  /** Remove all nodes and edges from this graph.
   * @post num_nodes() == 0 && num_edges() == 0
   *
   * Invalidates all outstanding Node and Edge objects.
   */
  void clear() {
    // HW0: clear internal vectors and set counters to zero
    internal_nodes_.clear();
    num_nodes_ = 0;
    internal_edges_.clear();
    num_edges_ = 0;
    internal_adj_.clear();

  }

  //
  // Node Iterator
  //

  /** @class Graph::NodeIterator
   * @brief Iterator class for nodes. A forward iterator. */
  class NodeIterator : private equality_comparable<NodeIterator> {
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

    /** Define dereferencing to return a Node based on info in iterator
     * @return Node constructed with the iterator's graph pointer and
     *     current index value
     * @pre The current value of @a index_ will create a valid Node
     *     (end iterator should never be dereferenced)
     */
    Node operator*() const{
      return Node(this->graph_, this->index_);
    }

    /** Define incrementing to simply increment the index member
     * @return Reference to NodeIterator with same graph but one higher
     *     @a index_ value
     */
    NodeIterator& operator++() {
      index_++;
      return *this;
    }

    /** Check equality by checking the internal graph pointers and @a index_ value
     * @return True if both member variables are the same, false otherwise
     */
    bool operator==(const NodeIterator& iter) const {
      return ((this->graph_ == iter.graph_) and (this->index_ == iter.index_));
    }

   private:
    friend class Graph;

    // Member variables: pointer to underlying graph and index giving the location 
    // of the current internal node being pointed to by iterator
    graph_type* graph_;
    size_type index_;

    /** Private constructor for NodeIterator (to be called within Graph)
     * @param[in] graph    pointer to graph object
     * @param[in] ind      index to desired node in internal node data
     */
    NodeIterator(const graph_type* graph, size_type ind)
      : graph_(const_cast<graph_type*>(graph)), index_(ind) {}
  };

  /** Function to initialize NodeIterator pointing to first node
   * @return NodeIterator with the current graph and @a index_ = 0
   * to point to first node
   */
  node_iterator node_begin() const{
    return NodeIterator(this,0);
  }

  /** Function to initialize NodeIterator pointing to one past the last node
   * @return NodeIterator with the current graph and @a index_ = total number
   *     of nodes, which will be one more than the index of the last node
   */
  node_iterator node_end() const {
    return NodeIterator(this,num_nodes_);
  }

  //
  // Incident Iterator
  //

  /** @class Graph::IncidentIterator
   * @brief Iterator class for edges incident to a node. A forward iterator. */
  class IncidentIterator : private equality_comparable<IncidentIterator> {
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

    /** Define dereferencing to give the current adjacent Edge
     * @return Edge object with graph pointer from the iterator, _node1_ from iterator,
     *     and _node2_ and _index_ from the internal map at location given by the map iterator
     * @pre iterator is not currently the end iterator
     * @post node1 of the Edge will always be the node spawning the incident iterator, 
     *     and _node2_ will be the adjacent node
     */
    Edge operator*() const {
      std::pair<size_type,size_type> key_value = *itr_;
      size_type node2 = key_value.first;
      size_type edge_ind = key_value.second;
      return Edge(graph_,edge_ind,node1_,node2);
    }

    /** Define incrementing to simply increment the internal unordered map iterator
     * @return Reference to IncidentIterator with the internal iterator incremented by 1
     */
    IncidentIterator& operator++() {
      ++itr_;
      return *this;
    }

    /** Define equality checking through graph pointers, original nodes, and value of iterator
     * @return True if all members of the Incident iterator are the same, false otherwise
     */
    bool operator==(const IncidentIterator& other_itr) const {
      return ((this->graph_ == other_itr.graph_) and (this->node1_ == other_itr.node1_)
               and (this->itr_ == other_itr.itr_));
    }

   private:
    friend class Graph;

    // Member variables: pointer to underlying graph, the root node for the adjacent edges,
    // and an unordered_map iterator to move through the edges in the internal
    // adjacency structure
    graph_type* graph_;
    size_type node1_;
    typedef typename std::unordered_map<size_type,size_type>::iterator iterator;
    iterator itr_;

    /** Private constructor for IncidentIterator (called from within Graph)
     * @param[in] graph    pointer to graph object
     * @param[in] ind      index to desired root node in internal node data
     * @param[in] iter     unordered_map iterator to move with
     */
    IncidentIterator(const graph_type* graph, size_type ind, iterator iter)
      : graph_(const_cast<graph_type*>(graph)), node1_(ind), itr_(iter) {}

  };

  //
  // Edge Iterator
  //

  /** @class Graph::EdgeIterator
   * @brief Iterator class for edges. A forward iterator. */
  class EdgeIterator : private totally_ordered<EdgeIterator> {
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

    /** Define dereferencing to return an Edge object
     * @return Edge with same pointer to graph, @a index_ giving the edge number,
     *     and nodes in order that they are listed in the internal vector of edge tuples
     * @pre iterator is not currently the end iterator
     */
    Edge operator*() const{
      return Edge(this->graph_, this->index_, std::get<0>(this->graph_->internal_edges_[index_]), 
                  std::get<1>(this->graph_->internal_edges_[index_]));
    }

    /** Increment iterator by incrementing its node member
     * @return EdgeIterator with @a index_ of value one larger
     */
    EdgeIterator& operator++() {
      index_++;
      return *this;
    }

    /** Define equality checking through graph pointers and edge indices
     * @return True if all members of the Incident iterator are the same, false otherwise
     */
    bool operator==(const EdgeIterator& iter) const {
      return ((this->graph_ == iter.graph_) and (this->index_ == iter.index_));
    }

   private:
    friend class Graph;

    // Member variables - pointer to the underlying graph and index giving location
    // of edge in the internal vector of tuples for edges
    graph_type* graph_;
    size_type index_;

    /** Private constructor for EdgeIterator (called from within Graph)
     * @param[in] graph    pointer to graph object
     * @param[in] ind      index to desired edge in internal edge vector
     */
    EdgeIterator(const graph_type* graph, size_type ind)
      : graph_(const_cast<graph_type*>(graph)), index_(ind) {}
  };

  /** Function to create iterator pointing to beginning of edges
   * @return EdgeIterator with the current graph and an index of 0,
   *     symbolizing the first edge
   */
  edge_iterator edge_begin() const {
    return EdgeIterator(this,0);
  }

  /** Function to create iterator pointing to end of edges
   * @return EdgeIterator with the current graph and an index = # edges,
   *     symbolizing one past the last edge
   */
  edge_iterator edge_end() const {
    return EdgeIterator(this,num_edges_);
  }

 private:

  // Initialize (private) member variables

  // Total number of nodes in graph 
  size_type num_nodes_;

  // Vector of tuples of nodal indices defining edges, index of edge is location in vector
  std::vector< std::tuple<size_type, size_type> > internal_edges_;
  size_type num_edges_;

  // Vector of maps, each element of vector is a map where the keys are the indices
  // of adjacent nodes, and the values are the indices of that edge in internal_edges_
  std::vector< std::unordered_map<size_type,size_type> > internal_adj_; 

  // Define a struct to hold node data - location and value (type given by template)
  struct node_info {
    Point node_loc;
    node_value_type node_val;
  };

  // Vector of structs holding the internal nodal data
  std::vector<node_info> internal_nodes_; 

};

#endif // CME212_GRAPH_HPP
