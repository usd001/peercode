#ifndef CME212_GRAPH_HPP
#define CME212_GRAPH_HPP

/** @file Graph.hpp
* @brief An undirected graph type
*/

#include <algorithm>
#include <vector>
#include <cassert>
#include <iostream>
#include "CME212/Util.hpp"
#include "CME212/Point.hpp"


/** @class Graph
* @brief A template for 3D undirected graphs.
*
* Users can add and retrieve nodes and edges. Edges are unique (there is at
* most one edge between any pair of distinct nodes).
*/
class Graph {

private:

// HW0: YOUR CODE HERE
// Use this space for declarations of important internal types you need
// later in the Graph's definition.
// (As with all the "YOUR CODE HERE" markings, you may not actually NEED
// code here. Just use the space if you need it.)


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
Graph(){}

/** Default destructor */


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
Node() {
// HW0: YOUR CODE HERE
}

/** Return this node's position. */
const Point& position() const {
// HW0: YOUR CODE HERE
return graph_->node_coods_[node_id_];
}

/** Return this node's index, a number in the range [0, graph_size). */
size_type index() const {
// HW0: YOUR CODE HERE
return node_id_;
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
// HW0: YOUR CODE HERE
if (graph_==n.graph_ && node_id_==n.node_id_)
return true;

return false;
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
if (node_id_ < n.node_id_)
return true;

return false;
}

private:
// HW0: YOUR CODE HERE
// Use this space to declare private data members and methods for Node
// that will not be visible to users, but may be useful within Graph.
// i.e. Graph needs a way to construct valid Node objects
// Pointer back to the SimpleSet container
graph_type* graph_;
size_type node_id_;

/** Private Constructor */
Node(const Graph* g, size_type n_id)
:graph_(const_cast<Graph*>(g)), node_id_(n_id)
{
}
// Allow Graph to access Node's private member data and functions.
friend class Graph;
};

/** Return the number of nodes in the graph.
*
* Complexity: O(1).
*/
size_type size() const {
// HW0: YOUR CODE HERE
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
Node add_node(const Point& position) {
// HW0: YOUR CODE HERE 
nodes_.push_back(Node(this,nodes_.size()));
node_coods_.push_back(Point(position.elem[0], position.elem[1], position.elem[2]));

return Node(this,nodes_.size()-1);       
}

/** Determine if a Node belongs to this Graph
* @return True if @a n is currently a Node of this Graph
*
* Complexity: O(1).
*/
bool has_node(const Node& n) const {
// HW0: YOUR CODE HERE
return nodes_[n.node_id_] == n;
}

/** Return the node with index @a i.
* @pre 0 <= @a i < num_nodes()
* @post result_node.index() == i
*
* Complexity: O(1).
*/
Node node(size_type i) const {
// HW0: YOUR CODE HERE
assert(i< num_nodes()); 
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
class Edge {
public:
/** Construct an invalid Edge. */
Edge() {
// HW0: YOUR CODE HERE
}

/** Return a node of this Edge */
Node node1() const {
// HW0: YOUR CODE HERE
return node_pair_.first;
}

/** Return the other node of this Edge */
Node node2() const {
// HW0: YOUR CODE HERE
return node_pair_.second;      // Invalid Node
}

/** Test whether this edge and @a e are equal.
* 
* Equal edges represent the same undirected edge between two nodes.
*/
bool operator==(const Edge& e) const {

	if (node_pair_.first == e.node_pair_.first
		&&
	node_pair_.second == e.node_pair_.second)

		return true;
	else

		return false;


}

/** Test whether this edge is less than @a e in a global order.
*
* This ordering function is useful for STL containers such as
* std::map<>. It need not have any interpretive meaning.
*/
bool operator<(const Edge& e) const {


	if (node_pair_.first < e.node_pair_.first)
	{
		return true;
	}
	else if (node_pair_.first == e.node_pair_.first)
	{	
		if (node_pair_.second < e.node_pair_.second)
			return true;
	}


}

private:
// Allow Graph to access Edge's private member data and functions.
friend class Graph;
// HW0: YOUR CODE HERE
// Use this space to declare private data members and methods for Edge
// that will not be visible to users, but may be useful within Graph.
// i.e. Graph needs a way to construct valid Edge objects
//size_type edge_id_;
std::pair<const node_type,const node_type> node_pair_;

/** Private Constructor */
Edge(const node_type& e1, const node_type& e2 )
: node_pair_(e1,e2)  {}
};

/** Return the total number of edges in the graph.
*
* Complexity: No more than O(num_nodes() + num_edges()), hopefully less
*/
size_type num_edges() const {
// HW0: YOUR CODE HERE
return edges_.size();
}

/** Return the edge with index @a i.
* @pre 0 <= @a i < num_edges()
*
* Complexity: No more than O(num_nodes() + num_edges()), hopefully less
*/
Edge edge(size_type i) const {
// HW0: YOUR CODE HERE
assert (i < num_edges());
return Edge(edges_[i].node_pair_.first, edges_[i].node_pair_.second);     
}

/** Test whether two nodes are connected by an edge.
* @pre @a a and @a b are valid nodes of this graph
* @return True if for some @a i, edge(@a i) connects @a a and @a b.
*
* Complexity: No more than O(num_nodes() + num_edges()), hopefully less
*/
bool has_edge(const Node& a, const Node& b) const {
// HW0: YOUR CODE HERE
assert(has_node(a) && has_node(b));

Edge tempE(a, b);

for(size_type i =0; i < edges_.size(); ++i)
{
if (edges_[i]==tempE)
return true;
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

assert(has_node(a) && has_node(b));

if (a<b)
{
if(!has_edge(a,b))
{
edges_.push_back(Edge(a,b));

}
return Edge(a,b);
}	
else
{
if(!has_edge(b,a))
{
edges_.push_back(Edge(b,a));

}
return Edge(b,a);

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
	node_coods_.clear();
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

// HW0: YOUR CODE HERE
// Use this space for your Graph class's internals:
//   helper functions, data members, and so forth.

//size_type Nnodes_, Nedges_;
std::vector<node_type> nodes_;
std::vector<Point> node_coods_;
std::vector<edge_type> edges_;
};

#endif // CME212_GRAPH_HPP
