#ifndef CME212_GRAPH_HPP
#define CME212_GRAPH_HPP

/** @file Graph.hpp
 * @brief An undirected graph type
 */

#include <algorithm>
#include <vector>
#include <cassert>
#include <unordered_map>

#include "CME212/Util.hpp"
#include "CME212/Point.hpp"

template <typename V, typename E>
/** @class Graph
 * @brief A template for 3D undirected graphs.
 *
 * Users can add and retrieve nodes and s. Edges are unique (there is at
 * most one edge between any pair of distinct nodes).
 */
class Graph 
{
	public:

		//
		// PUBLIC TYPE DEFINITIONS
	 	//
		using size_type = unsigned;
		using node_value_type = V;
		using edge_value_type = E;
		/** Type of this graph. */
		using graph_type = Graph;

		/** Predeclaration of Node type. */
		class Node;
		/** Synonym for Node (following STL conventions). */
		using node_type = Node;
		/** making a tuple for endpoints of an edge. */
		typedef std::tuple<size_type, size_type> node_tuple;

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

		/** Construct an empty graph. */
		Graph() 
		{}
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
		class Node : private totally_ordered<Node>
		{
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
				Node() 
				{}
				
				size_type get_index() const
				{
					return value().new_id;
				}

				/** Return this node's position. */
				Point& position() 
				{
					return graph_ptr->node_positions.at(node_index);
				}

				const Point& position()  const
				{
					return const_cast<Point&>(graph_ptr->node_positions.at(node_index));
				}

				/** Return this node's index, a number in the range [0, graph_size). */
				size_type index() const 
				{
					return node_index;
				}

				// Supply definitions AND SPECIFICATIONS for:
				node_value_type& value()
				{
					return graph_ptr->node_values.at(this->node_index);
				}

				// return value corresponding to the node
				const node_value_type& value() const
				{ 
					return const_cast<node_value_type&> (graph_ptr->node_values.at(this->node_index));
				}

				// return degree of the node
				size_type degree() const
				{
					if(graph_ptr->degrees.count(node_index)==0)
						return 0;
					return graph_ptr->degrees.at(node_index);
				}

				// create incident iterator pointing at first edge incident on node
				incident_iterator edge_begin() const
				{
					return IncidentIterator(node_index, 0, graph_ptr);
				}

				// create incident iterator pointing at one past the last edge incident on node
				incident_iterator edge_end() const
				{
					return IncidentIterator(node_index, degree(), graph_ptr);
				}

				/** Test whether this node and @a n are equal.
				 *
				 * Equal nodes have the same graph and the same index.
				 */
				bool operator==(const Node& n) const 
				{
					if(graph_ptr != n.graph_ptr)  return false;
					// now just deal with nodes of same graph
					return n.node_index == node_index;
				}

				/** Test whether this node is less than @a n in a global order.
				 *
				 * This ordering function is useful for STL containers such as
				 * std::map<>. It need not have any geometric meaning.
				 *
				 * The node ordering relation must obey trichotomy: For any two nodes x
				 * and y, exactly one of x == y, x < y, and y < x is true.
				 */
				bool operator<(const Node& n) const 
				{
					// graph_1 smaller than graph_2
					if(std::less<const Graph*>{} (graph_ptr, n.graph_ptr))  
						return true;
					// graph_1 is larger than graph_2
					if(std::greater<const Graph*>{} (graph_ptr, n.graph_ptr))  
						return false;
					// graph_1 and graph_2 are same
					return node_index < n.node_index;
				}

			private:
				// store unique index of the node
				size_type node_index;
				// store graph pointer 
				Graph* graph_ptr;
				// private constructor
				Node(size_type ind, const graph_type* g)
				{
					node_index = ind;
					graph_ptr = const_cast<graph_type*> (g);
				}
				// Allow Graph to access Node's private member data and functions.
				friend class Graph;
		};

		/** Return the number of nodes in the graph.
		 *
		 * Complexity: O(1).
		 */
		size_type size() const 
		{
			return num_nod;
		}

		/** Synonym for size(). */
		size_type num_nodes() const 
		{
			return valid_nodes.size();
		}

		/** Determine if a Node belongs to this Graph
		 * @return True if @a n is currently a Node of this Graph
		 *
		 * Complexity: O(1).
		 */
		bool has_node(const Node& n) const 
		{
			// if the graphs are unequal, always return false
			if(n.graph_ptr != this)  return false;
			// now we have nodes of same graph
			return n.node_index < this->num_nod;
		}

		/** Add a node to the graph, returning the added node.
		 * @param[in] position The new node's position
		 * @post new num_nodes() == old num_nodes() + 1
		 * @post result_node.index() == old num_nodes()
		 *
		 * Complexity: O(1) amortized operations.
		 */
		Node add_node(const Point& position, const node_value_type& val = node_value_type()) 
		{
			// create a node
			node_type n = Node(num_nod, this);
			// update the data members of graph class
			node_positions[n.node_index] = position;
			node_values[n.node_index] = val;
			nodes.push_back(n);
			valid_nodes.push_back(n);
			num_nod++;
			return n;       
		}

		/** Return the node with index @a i.
		 * @pre 0 <= @a i < num_nodes()
		 * @post result_node.index() == i
		 *
		 * Complexity: O(1).
		 */
		Node node(size_type i) const 
		{
			return valid_nodes.at(i);
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
		class Edge : private totally_ordered<Edge>
		{
			public:
				/** Construct an invalid Edge. */
				Edge() 
				{}

				/** Return a node of this Edge */
				Node node1() const 
				{
					return g->nodes[node_1_index];      
				}

				/** Return the other node of this Edge */
				Node node2() const 
				{
					return g->nodes[node_2_index];      
				}

				/** Test whether this edge and @a e are equal.
				 *
				 * Equal edges represent the same undirected edge between two nodes.
				 */
				bool operator==(const Edge& e) const 
				{
					return node_1_index == e.node_1_index and node_2_index == e.node_2_index;
				}

				/** Test whether this edge is less than @a e in a global order.
				 *
				 * This ordering function is useful for STL containers such as
				 * std::map<>. It need not have any interpretive meaning.
				 */
				bool operator<(const Edge& e) const 
				{
					return edge_index < e.edge_index;
				}

				// return value associated with the edge
				const edge_value_type& value() const
				{
					return const_cast<edge_value_type&>(g->edge_values.at(edge_index));
				}

				edge_value_type& value() 
				{
					return g->edge_values.at(edge_index);
				}
	
			private:
				// node_1_index stores the smaller index
				size_type node_1_index;
				// node_2_index stores the larger index
				size_type node_2_index;
				// edge_index stores unique id for the edge
				size_type edge_index;
				// store the graph pointer
				Graph* g;

				// private constructor
				Edge(size_type a, size_type b, size_type ind, const Graph* gp)
				{
					// the smaller node goes first
					// this property is taken care of in add_edge method of Graph class
					node_1_index = a;
					node_2_index = b;
					edge_index = ind;
					g = const_cast<Graph*> (gp);
				}
				friend class Graph;
		};

	/** Return the total number of edges in the graph.
	 *
	 * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
	 */
	size_type num_edges() const 
	{
		return num_edg;
	}

	/** Return the edge with index @a i.
	 * @pre 0 <= @a i < num_edges()
	 *
	 * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
	 */
	Edge edge(size_type i) const 
	{
		return edges.at(i);        
	}

	/** Test whether two nodes are connected by an edge.
	 * @pre @a a and @a b are valid nodes of this graph
	 * @return True if for some @a i, edge(@a i) connects @a a and @a b.
	 *
	 * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
	 */
	bool has_edge(const Node& a, const Node& b) const 
	{
		node_type first = a < b ? a : b;    // smaller node
		node_type second = a < b ? b : a;   // larger node
		node_tuple tup = std::make_tuple(first.node_index, second.node_index);
		bool temp1 = endpts_edge.count(tup) == 1;
		// if the edge was never created
		if(! temp1)
			return false;
		// if the edge was created at some point but is invalid
		return edges.at(endpts_edge.at(tup)).value().valid;
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
	Edge add_edge(const Node& a, const Node& b, edge_value_type val = edge_value_type()) 
	{
		node_type first = a < b ? a : b;    // smaller node
		node_type second = a < b ? b : a;   // larger node
		if(has_edge(first, second))
		{
			node_tuple tup = std::make_tuple(first.node_index, second.node_index);
			return edges.at(endpts_edge[tup]);
		}
		else
		{
			edge_type e = Edge(first.node_index, second.node_index, num_edg, this);
			edges.push_back(e);
			degrees[first.node_index] += 1;
			degrees[second.node_index] += 1;
			node_tuple tup = std::make_tuple(first.node_index, second.node_index);
			endpts_edge[tup] = e.edge_index;
			edge_values[e.edge_index] = val;
			adj_list[first.node_index].push_back(e);
			adj_list[second.node_index].push_back(e);
			num_edg++;
			return e;
		}
	}


	/** Remove all nodes and edges from this graph.
	 * @post num_nodes() == 0 && num_edges() == 0
	 *
	 * Invalidates all outstanding Node and Edge objects.
	 */
	void clear() 
	{
		num_nod = 0;
		num_edg = 0;
		edges.clear();
		nodes.clear();
		endpts_edge.clear();
		node_positions.clear();
	}

	//
	// Node Iterator
	//
	/** @class Graph::NodeIterator
	 * @brief Iterator class for nodes. A forward iterator. */
	class NodeIterator : private totally_ordered<NodeIterator> 
	{
		public:
			// These type definitions let us use STL's iterator_traits.
			using value_type        = Node;                     // Element type
			using pointer           = Node*;                    // Pointers to elements
			using reference         = Node&;                    // Reference to elements
			using difference_type   = std::ptrdiff_t;           // Signed difference
			using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

			/** Construct an invalid NodeIterator. */
			NodeIterator() {}

			// dereference operator
			Node operator*() const
			{
				return gp->nodes.at(ind);
			}

			// pre increment operator
			NodeIterator& operator++()
			{	
				ind++;
				while(ind < gp->nodes.size())
				{
					if(gp->nodes.at(ind).value().valid == false)
						ind++;
					else
						break;
				}
				return *this;
			}

			// equality operator
			bool operator==(const NodeIterator& it) const
			{
				return ind == it.ind;
			}

		private:
			size_type ind;
			graph_type* gp;

			// constructor
			NodeIterator(size_type i, const graph_type* g) 
			{
				ind = i;
				gp = const_cast<graph_type*> (g);
				// make the iterator point to first valid node
				while(ind < gp->nodes.size())
				{
					if(gp->nodes.at(ind).value().valid == false)
						ind++;
					else
						break;
				}
			}
			friend class Graph;
	};

	// begin() method for node_iterator
	node_iterator node_begin() const
	{
		return NodeIterator(0, this);
	}

	// end() method for node_iterator
	node_iterator node_end() const
	{
		return NodeIterator(nodes.size(), this);
	}

	//
	// Incident Iterator
	//
	/** @class Graph::IncidentIterator
	 * @brief Iterator class for edges incident to a node. A forward iterator. */
	class IncidentIterator : private totally_ordered<IncidentIterator>
	{
		public:
			// These type definitions let us use STL's iterator_traits.
			using value_type        = Edge;                     // Element type
			using pointer           = Edge*;                    // Pointers to elements
			using reference         = Edge&;                    // Reference to elements
			using difference_type   = std::ptrdiff_t;           // Signed difference
			using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

			/** Construct an invalid IncidentIterator. */
			IncidentIterator() 
			{}

			// dereference operator
			Edge operator*() const
			{
				return edge_vec->at(curr_ind);
			}
			
			// pre increment operator
			IncidentIterator& operator++()
			{
				curr_ind++;
				while(curr_ind < edge_vec->size())
				{
					if(edge_vec->at(curr_ind).value().valid == false)
						curr_ind++;
					else
						break;
				}
				return *this;
			}

			// equality operator
			bool operator==(const IncidentIterator& it) const
			{
				return nod_ind == it.nod_ind and curr_ind == it.curr_ind;
			}

		private:
			// nod_ind stores index of the node that created incident_iterator
			size_type nod_ind;
			// curr_ind stores index of edge in adjacency list
			size_type curr_ind;
			// edge_vec stores all incident edges to node that created incident_iterator
			std::vector<edge_type>* edge_vec;
			// private constructor
			IncidentIterator(size_type n, size_type i, const graph_type* g)
			{
				nod_ind = n;
				curr_ind = i;
				edge_vec = const_cast<std::vector<edge_type>*> (&(g->adj_list.at(n)));
				while(curr_ind < edge_vec->size())
				{
					if(edge_vec->at(curr_ind).value().valid == false)
						curr_ind++;
					else
						break;
				}

			}
			friend class Graph;
	};

	//
	// Edge Iterator
	//
	/** @class Graph::EdgeIterator
	 * @brief Iterator class for edges. A forward iterator. */
	class EdgeIterator : private totally_ordered<EdgeIterator>
	{
		public:
			// These type definitions let us use STL's iterator_traits.
			using value_type        = Edge;                     // Element type
			using pointer           = Edge*;                    // Pointers to elements
			using reference         = Edge&;                    // Reference to elements
			using difference_type   = std::ptrdiff_t;           // Signed difference
			using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

			/** Construct an invalid EdgeIterator. */
			EdgeIterator() {}

			// dereference operator
			Edge operator*() const
			{ 
				return *inc_it;
			}
			
			// pre increment operator
			EdgeIterator& operator++()
			{
				// case 1 - where current node has more edges 
				++inc_it;
				// case 2 - where current node has no more edges and go to th enext node
				if(inc_it == (*node_it).edge_end() and *node_it != last_node)
				{
					++node_it;
					inc_it = (*node_it).edge_begin(); 
				}	
				return *this;
			}
			
			// equality operator
			bool operator==(const EdgeIterator& iter) const
			{
				return  inc_it == iter.inc_it;
			}

		private:
			// stores current node 
			node_iterator node_it;
			// stores current edge
			incident_iterator inc_it;
			// stores last node
			node_type last_node;

			// private constructor
			EdgeIterator(node_iterator n_it, incident_iterator i_it, node_type l)
			{
				node_it = n_it;
				inc_it = i_it;
				last_node = l;
			}
			friend class Graph;
	};

	// begin() method for edge_iterator
	edge_iterator edge_begin() const
	{
		return EdgeIterator(node_begin(), nodes.front().edge_begin(), nodes.back());
	}

	// end() method for edge_iterator
	edge_iterator edge_end() const
	{
		return EdgeIterator(node_end(), nodes.back().edge_end(), nodes.back());
	}

	// remove edge using nodes
	size_type remove_edge(const node_type& n1, const node_type& n2)
	{
		// update adjacency list
		adj_list.at(n1.node_index).at(n2.node_index).value().valid = false;
		adj_list.at(n2.node_index).at(n1.node_index).value().valid = false;
		// update degrees
		degrees.at(n1.node_index) -= 1;
		degrees.at(n2.node_index) -= 1;
		// update the edges container
		size_type smaller = n1.node_index < n2.node_index ? n1.node_index : n2.node_index;
		size_type larger = n2.node_index < n1.node_index ? n1.node_index : n2.node_index;
		node_tuple tup =  std::make_tuple(smaller, larger);
		edges.at(endpts_edge[tup]).value().valid = false;
		// update number of edges
		num_edg -= 1;
		return 1;
	}

	// remove edge using edge object
	size_type remove_edge(const edge_type& e)
	{
		return remove_edge(e.node1(), e.node2());
	}


	edge_iterator remove_edge(edge_iterator e_it)
	{
		edge_type e = *e_it;
		++e_it;
		remove_edge(e);
		return e_it;
	}

	size_type remove_node(const node_type& n)
	{
		// make the status of node invalid
		nodes.at(n.node_index).value().valid = false;
		// remove from valid nodes
		auto id = n.get_index();
		auto it = valid_nodes.begin() + id;
		*it = valid_nodes.back();
		(*it).value().new_id = id;
		valid_nodes.pop_back();
		// reomve the edges
		for(auto it = n.edge_begin(); it != n.edge_end(); ++it)
		{
			remove_edge(*it);
		}
		return 1;
	}

	node_iterator remove_node(node_iterator& n_it)
	{
		node_type n = *n_it;
		++n_it;
		remove_node(n);
		return n_it;
	}

	private:

		/** Containers for nodes*/

		// stores list of nodes
		std::vector<node_type> nodes {};
		// stores list of valid nodes
		std::vector<node_type> valid_nodes {};
		// stores values associated with nodes
		std::unordered_map<size_type, node_value_type> node_values;
		// stores 3D positions of nodes
		std::unordered_map<size_type, Point> node_positions;
		// stores degrees of nodes
		std::unordered_map<size_type, size_type> degrees;


		/** Containers for edges */

		// stores map of edges
		std::vector<edge_type> edges;
		// stores values associated with edges
		std::unordered_map<size_type, edge_value_type> edge_values;
		// stores mapping from (node_1, node_2) to edge
		std::map<node_tuple, size_type> endpts_edge;


		/** Containers and attribute for the graph*/

		// stores adjacency lists for the graph
		std::unordered_map<size_type, std::vector<edge_type>> adj_list;
		// stores total number of nodes
		size_type num_nod {};
		// stores total number of edges
		size_type num_edg {};

};

#endif // CME212_GRAPH_HPP