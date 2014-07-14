/*

John Colarusso 995957815

*/
#define MAX_SIZE 200000			// 200,000
#define INT_MAX 32767
#define ULONG_MAX 4294967295
#include <iostream>
#include <cstdlib>

using namespace std;

struct Edge
{
	unsigned long int v1;
	unsigned long int v2;
	unsigned long int w;
	Edge() {}
	Edge( int vertex1, int vertex2, int weight ) {
		v1 = vertex1;
		v2 = vertex2;
		w = weight;
	}
	bool operator <( Edge& e2 ) {
		if( w == e2.w ) {
			if( (*this).min() < e2.min() )	{
				return true;
			} else if( (*this).min() == e2.min() 
					&& (*this).max() < e2.max() ) {
				return true;
			}
			return false;
		} else if( w < e2.w ) {
			return true;
		} else {
			return false;
		}
	}
	int min()
	{
		if( v1 < v2 )
			return v1;
		else
			return v2;
	}
	int max()
	{
		if( v1 > v2 )
			return v1;
		else
			return v2;
	}
};
void swap( Edge &e1, Edge &e2 )
{
	Edge temp;
	temp = e1;
	e1 = e2;
	e2 = temp;
}

class Graph
{
	public:
		Graph() { 
			count_v = 0;
			count_e = 0;
		};
		void init( int vertices, int edges )
		{
			listV = new int[vertices];
			listE = new Edge[edges];
		}
		void addEdge(int vertex1, int vertex2, int weight)
		{
			Edge e(vertex1, vertex2, weight);
			listE[count_e] = e;
			if( this -> findVertex(vertex1) == endv() )
				this -> addVertex(vertex1);
			if( this -> findVertex(vertex2) == endv() )
				this -> addVertex(vertex2);
			count_e++;
		}	// adds edge to graph and adds vertex if not already counted
		void addEdge( Edge& e)
		{
			listE[count_e] = e;
			if( this -> findVertex(e.v1) == endv() )
				this -> addVertex(e.v1);
			if( this -> findVertex(e.v2) == endv() )
				this -> addVertex(e.v2);
			count_e++;
		}
		int getVertex(int index)
		{
			return listV[index];
		}
		void addVertex(int v)
		{
			listV[count_v] = v;
			count_v++;
		}
		int findVertex(int vertex)
		{
			for(int i = 0; i < count_v; i++)
				if(listV[i] == vertex)
					return i;
			return endv();
		}
		int endv()
		{
			return count_v;
		}
		int ende()
		{
			return count_e;
		}
		void printVertices() /* debug */
		{
			for(int i = 0; i < count_v; i++)
				cout << listV[i] << "  ";
			cout << endl;
		}
		void printEdges() /* debug */
		{
			for(int i = 0; i < count_e; i++)
			{
				cout << listE[i].v1 << " "
					<< listE[i].v2 << " "
					<< listE[i].w << endl;
			}
		}
		int numV;
		int numE;
		int count_v;
		int count_e;
		int* listV;
		Edge* listE;
};
Graph g;
class MinHeap
{
	public:
		MinHeap() { 
			size = 0;
		};
		void init()
		{
			arr = new Edge[capacity];
		}
		void insert(Edge e)
		{
			arr[size] = e;
			int pos = size;
			int parent;
			while(pos > 0)
			{
				parent = (pos-1) / 2;
				if( arr[pos] < arr[parent] ) {
					swap(arr[pos], arr[parent]);
				} else {
					break;
				}
				pos = parent;
			}
			size++;
		}
		int getSize()
		{
			return size;
		}
		Edge deleteMin()
		{
			Edge rtn = arr[0];
			arr[0] = arr[size-1];
			size--;
			int root = 0;
			int left = 1;
			int right = 2;
			while( root < size ) {
				if( left < size ) {
					if( arr[left] < arr[root] ) {
						if( right < size ) {
							if( arr[right] < arr[left] ) {
								swap( arr[root], arr[right] );
								root = right;
							} else {
								swap( arr[root], arr[left] );
								root = left;
							}
						} else {
							swap( arr[root], arr[left] );
							root = left;
						}
						left = 2*root+1;
						right = 2*root+2;
						continue;
					} else if( right < size ) {
						if( arr[right] < arr[root] ) {
							swap( arr[root], arr[right] );
							root = right;
							left = 2*root+1;
							right = 2*root+2;
							continue;
						}
					} else {
						break;
					}
				}
				break;
			}
			return rtn;
		}
		void printHeap() /* debug */
		{
			cout << "size " <<  size << ": ";
			if( size == 0 )
				cout << "empty" << endl;
			else {
				for(int i = 0; i < size; i++)
					cout << arr[i].w << " ";
				cout << endl;
			}
		
		}
		Edge* arr;
		int size;		// size is the index after last element of heap 
						// and # of elements
		int capacity;
};
MinHeap h;
struct Region
{
	Region() { }
	Region( int k ) 
	{
		count_e = 0;
		size = g.numE;
		edges = new Edge[size];
		key = k;
	}
	void addEdge( Edge & e )
	{
		edges[count_e] = e;
		count_e++;
	}
	void print()
	{
		cout << "<region>\n";
		for(int i = 0; i < count_e; i++)
		{
			cout << "<road>";
			cout << edges[i].min() << " ";
			cout << edges[i].max() << " ";
			cout << edges[i].w;
			cout << "</road>\n";
		}
		cout << "</region>\n";
	}
	int key;
	int count_e;
	int size;
	Edge* edges;
};
int min( Region& r )
{
	int min = ULONG_MAX;
	Edge temp;
	for(int i = 0; i < r.count_e; i++)
	{
		temp = r.edges[i];
		if( temp.v1 < min )
			min = temp.v1;
		if( temp.v2 < min )
			min = temp.v2;
		if( temp.w < min )
			min = temp.w;
	}
	return min;
}
int compareRegions( const void * a, const void *b )
{
	Region* r1 = (Region*)a;
	Region* r2 = (Region*)b;
	if( (*r1).count_e != (*r2).count_e )
		return (*r1).count_e -(*r2).count_e;
	else {
		return min(*r1) - min(*r2);
	}
}
struct Country
{
	Country() { }
	Country( Graph& gr )
	{
		count_r = 0;
		capacity = g.numE;
		regions = new Region[capacity];
	}
	void addRegion( int key )
	{
		Region temp(key);
		regions[count_r] = temp;
		count_r++;
	}
	int end()
	{
		return count_r;
	}
	int findRegion( int key )
	{
		if(count_r == 0)
			return end();
		else {
			for(int i = 0; i < count_r; i++)
			{
				if( regions[i].key == key )
				{
					return i;
				}
			}
			return end();
		}
	}
	void addEdgetoRegion( int k, Edge& e )
	{
		int index = findRegion(k);
		regions[index].addEdge(e);
	}
	void sort()
	{
		qsort(
			regions,
			count_r,
			sizeof( Region ),
			compareRegions
		);
	}
	void print()
	{
		cout << "<?xml version=\"1.2\"?>\n";
		cout << "<country>\n";
		for(int i = 0; i < count_r; i++)
		{
			regions[i].print();
		}
		cout << "</country>\n";
	}
	Region* regions;
	int count_r;
	int capacity;
};
class UnionFind
{
	public:
		UnionFind() { }
		UnionFind( int length ) 
		{ 
			size = length;
			arr = new int[size];
			
			for(int i = 0; i < size; i++) {
				arr[i] = -1;
			}
		}
		bool unionfind( const Edge& e )
		{
			int node1 = g.findVertex(e.v1);
			int node2 = g.findVertex(e.v2);
			int p1 = find( node1 );
			int p2 = find( node2 );
			if( p1 == p2 ) {
				return false;
			} else if( arr[p1] == arr[p2] ) {
				arr[p1] = node2;
				arr[p2]--;
			}	// point left to right
			else if( arr[p1] > arr[p2] ) {
				arr[p1] = node2;
				arr[p2]--;	// point lesser nodes to greater nodes
			} else { 
				arr[p2] = node1;
				arr[p1]--;
			}
			return true;
		}
		int find( int index )
		{
			int parent = index;
			while( arr[parent] >= 0 )
			{
				parent = arr[parent];
			}
			return parent;
		}	// returns the index of the parent of vertex
		/*
			Region of vertex assigned based on index of root in
			union-find. Edges are assigned to regions based on
			first vertex.
		*/
		void constructCountry(Graph & gr)
		{
			Country Maf(gr);
			
			Edge left;
			int index;
			int key;
			for(int i = 0; i < gr.count_e; i++)
			{
				left = gr.listE[i];
				index = g.findVertex(left.v1);
				key = find( index );
				
				if( Maf.findRegion( key ) == Maf.end() )
				{
					Maf.addRegion( key );
				}
				Maf.addEdgetoRegion( key, gr.listE[i] );
			}
			Maf.sort();
			Maf.print();
		}
		void print()
		{
			for(int i = 0; i < size; i++) 
				cout << arr[i] << " ";
			cout << endl;
		}
		int* arr;
		int size;
};
void parse()
{
	int v1, v2, w;
	cin >> g.numV;
	cin >> g.numE;
	//cout<< g.numV << endl; // db
	//cout << g.numE << endl; // db
	g.init(g.numV, g.numE);
	for(int i = 0; i < g.numE; i++)
	{
		cin >> v1;
		cin >> v2;
		cin >> w;
		g.addEdge(v1, v2, w);
	}
	//g.printEdges();	// db
	//g.printVertices();	// db
}
void buildHeap()
{
	h.capacity = g.numE;
	h.init();
	
	for(int i = 0; i < g.numE; i++ ) {
		h.insert(g.listE[i]);
	}
	//h.printHeap();	// db
}
void findMinSpanTree()
{
	buildHeap();
	Graph mst;
	mst.init(g.numV, g.numE);
	UnionFind uf(g.numV);
	Edge temp;
	
	while( h.size > 0 )
	{
		temp = h.deleteMin();
		if( uf.unionfind( temp ) ) {
			mst.addEdge( temp );
			//uf.print();	// db
		}
	}
	//mst.printEdges();		// db
	//mst.printVertices();		// db
	uf.constructCountry(mst);
}
int main()
{
	parse();
	findMinSpanTree();
	return 0;
}