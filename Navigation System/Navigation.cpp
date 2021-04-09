#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <iomanip>

using namespace std;

class Custom_String
{
private:
	char *buffer;
public:
	Custom_String();
	Custom_String(const char *);
	Custom_String(const Custom_String &);
	~Custom_String();
	Custom_String& operator = (const Custom_String &);
	int length() const;
	bool empty() const;
	char& operator [] (const int &) const;
	Custom_String operator + (const char &) const;
	Custom_String& operator += (const char &);
	bool operator == (const Custom_String &) const;
	bool operator != (const Custom_String &) const;
	friend istream& operator >> (istream &, Custom_String &);
	friend ostream& operator << (ostream &, const Custom_String &);
};

Custom_String::Custom_String()
{
	buffer = nullptr;
}

Custom_String::Custom_String(const char *cStr)
{
	int len;

	if (cStr == nullptr)
		len = 0;
	else
	{
		len = 0;
		while (cStr[len] != '\0')
			len++;
	}

	if (len == 0)
		buffer = nullptr;
	else
	{
		buffer = new char[len+1];
		for (int i = 0; i < len; i++)
			buffer[i] = cStr[i];
		buffer[len] = '\0';
	}
}

Custom_String::Custom_String(const Custom_String &s)
{
	int length = s.length();

	if (length == 0)
		buffer = nullptr;
	else
	{
		buffer = new char[length + 1];
		for (int i = 0; i < length; i++)
			buffer[i] = s.buffer[i];
		buffer[length] = '\0';
	}
}

Custom_String::~Custom_String()
{
	if (buffer != nullptr)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}

Custom_String& Custom_String::operator = (const Custom_String &s)
{
	// Check for self assignment, for example s = s
	if (this == &s)
		return *this;

	// Delete the left hand side object's memory
	this->~Custom_String();

	int length = s.length();

	if (length == 0)
		buffer = nullptr;
	else
	{
		buffer = new char[length + 1];
		for (int i = 0; i < length; i++)
			buffer[i] = s.buffer[i];
		buffer[length] = '\0';
	}

	return *this;
}

int Custom_String::length() const
{
	if (buffer == nullptr)
		return 0;
	else
	{
		int len = 0;
		while (buffer[len] != '\0')
			len++;
		return len;
	}
}

bool Custom_String::empty() const
{
	if (this->length() == 0)
		return true;

	return false;
}

char& Custom_String::operator [] (const int &index) const
{
	assert(index >= 0 && index < this->length());
	
	return buffer[index];
}

Custom_String Custom_String::operator + (const char &c) const
{
	int length = this->length();
	Custom_String s1;
	s1.buffer = new char[length + 2];

	for (int i = 0; i < length; i++)
		s1[i] = (*this)[i];

	s1[length] = c;
	s1[length + 1] = '\0';

	return s1;
}

Custom_String& Custom_String::operator += (const char &c)
{
	int length = this->length();

	char *temp;
	temp = new char[length + 2];
	
	for (int i = 0; i < length; i++)
		temp[i] = (*this)[i];

	temp[length] = c;
	temp[length + 1] = '\0';

	if (length > 0)
		delete[] buffer;

	buffer = temp;

	return *this;
}

bool Custom_String::operator == (const Custom_String &s) const
{
	int len = this->length();
	if (len != s.length())
		return false;
	else
	{
		for (int i = 0; i < len; i++)
		{
			if (buffer[i] != s[i])
				return false;
		}
		return true;
	}
}

bool Custom_String::operator != (const Custom_String &s) const
{
	int len = this->length();
	if (len != s.length())
		return true;
	else
	{
		for (int i = 0; i < len; i++)
		{
			if (buffer[i] != s[i])
				return true;
		}
		return false;
	}
}


istream& operator >> (istream &in, Custom_String &s)
{
	//This function reads characters input from a keyboard or a file until either a TAB, EOL, or EOF is 
	//reached. The function ignores any leading or trailing spaces. It is designed to be able to read a 
	//string of characters that may or may not contain spaces without any problem.

	//Define some useful constant values
	#define SPACE ' '
	#define TAB '\t'
	#define	EOL	'\n'

	//Delete the old value of s
	s.~Custom_String();

	//Skip leading spaces, tabs, and empty lines
	char ch;
	while (!in.eof())
	{
		in.get(ch);
		if (ch == SPACE || ch == TAB || ch == EOL)
			continue;
		else
		{
			s += ch;
			break;
		}
	}

	//Read characters into s until a TAB or EOL or EOF is reached
	while (!in.eof())
	{
		in.get(ch);
		if (ch == TAB || ch == EOL || in.eof())
			break;
		else
			s += ch;
	}

	//Remove any trailing spaces
	int trailingSpacesCount = 0;
	for (int i = s.length()-1; i >= 0; i--)
	{
		if (s[i] != SPACE)
			break;
		trailingSpacesCount++;
	}
	Custom_String temp;
	for (int i = 0; i < s.length()-trailingSpacesCount; i++)
		temp += s[i];
	s = temp;

	return in;
}

ostream& operator << (ostream &out, const Custom_String &s)
{
	for (int i = 0; i < s.length(); i++)
		out << s[i];
	return out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class SmarterArray
{
private:
	T *A;
	int size;

public:
	//Constructors
	SmarterArray();
	SmarterArray(const T*, const int&);
	SmarterArray(const SmarterArray<T>&);

	//Assignment operator
	SmarterArray<T>& operator = (const SmarterArray<T>&);

	//Destructor
	~SmarterArray();

	//Getters, Setters, operators and other functions
	int getSize() const;
	T& operator[](const int&) const;
	int find(const T&) const;
	void append(const T&);
	void insert(const int &, const T&);
	bool remove(const int&);
	bool operator == (const SmarterArray<T>&) const;
	
	template <class T1>
	friend ostream& operator << (ostream&, const SmarterArray<T1>&);
};

template <class T>
SmarterArray<T>::SmarterArray()
{
	this->A = nullptr;
	this->size = 0;
}

template <class T>
SmarterArray<T>::SmarterArray(const T *arr, const int &size)
{
	this->size = size;

	if (this->size > 0)
	{
		this->A = new T[this->size];
		for (int i = 0; i < size; i++)
			this->A[i] = arr[i];
	}
}

template <class T>
SmarterArray<T>::SmarterArray(const SmarterArray<T> &L)
{
	size = L.size;
	
	if (size > 0)
	{
		A = new T[size];
		for (int i = 0; i < size; i++)
			A[i] = L[i];
	}
}

template <class T>
SmarterArray<T>& SmarterArray<T>::operator = (const SmarterArray<T> &L)
{
	// Check for self assignment, for example s2 = s2
	if (this == &L)
		return *this;

	// Delete the left hand side object's memory
	this->~SmarterArray();

	size = L.size;
	if (size > 0)
	{
		A = new T[size];
		for (int i = 0; i < size; i++)
			A[i] = L[i];
	}

	return *this;
}

template <class T>
SmarterArray<T>::~SmarterArray()
{
	if (size > 0)
	{
		delete[] A;
		size = 0;
	}
}

template <class T>
int SmarterArray<T>::getSize() const
{
	return size;
}

template <class T>
T& SmarterArray<T>::operator[](const int &index) const
{
	assert(index >= 0 && index < size);

	return A[index];
}

template <class T>
int SmarterArray<T>::find(const T &t) const
{
	for (int i = 0; i < size; i++)
	{
		if (A[i] == t)
			return i;
	}

	return -1;
}

template <class T>
void SmarterArray<T>::append(const T &e)
{
	T *temp = new T[size + 1];

	for (int i = 0; i < size; i++)
		temp[i] = A[i];

	temp[size] = e;

	if (size > 0)
		delete[] A;

	A = temp;
	size++;
}

template <class T>
void SmarterArray<T>::insert(const int &index, const T &arg)
{
	assert(index >= 0 && index <= size);

	if (index == size)
		this->append(arg);
	else
	{
		T *temp = new T [size + 1];

		int i = 0;
		for (; i < index; i++)
			temp[i] = A[i];

		temp[index] = arg;

		for (int j = index + 1; j < size + 1; j++, i++)
			temp[j] = A[i];

		// We dont have to check if the size > 0 since if it was 0 then the index value would also have been 0, so it would never reach the else block
		delete[] A;

		A = temp;
		size++;
	}
}

template <class T>
bool SmarterArray<T>::remove(const int &index)
{
	if (index >= 0 && index < size)
	{
		T *temp = new T[size - 1];

		int i = 0;
		for (; i < index; i++)
			temp[i] = A[i];

		for (int j = index + 1; j < size; j++, i++)
			temp[i] = A[j];

		delete[] A;

		A = temp;
		size--;

		return true;
	}

	return false;
}

template <class T>
bool SmarterArray<T>::operator == (const SmarterArray<T> &L) const
{
	if (size == L.size)
	{
		for (int i = 0; i < size; i++)
		{
			if (A[i] != L[i])
				return false;
		}

		return true;
	}

	return false;
}

template <class T>
ostream& operator << (ostream& out, const SmarterArray<T>& L)
{
	if (L.getSize() == 0)
		out << "[Empty List]";
	else
	{
		for (int i = 0; i < L.getSize()-1; i++)
			out << L[i] << endl;
		out << L[L.getSize()-1] << endl;
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Edge
{
	int desVertexIndex;
	double cost;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Vertex
{
private:
	Custom_String name;
	SmarterArray<Edge> E;

public:
	Vertex();
	Vertex(const Custom_String &);
	Custom_String getName() const;
	SmarterArray<Edge> getEdgeSet() const;
	int getEdgeSetSize() const;
	Edge getEdge(const int & desVertexIndex) const;
	double getEdgeCost(const int &desVertexIndex) const;
	void appendEdge(const int &desVertexIndex, const double &cost);
	friend ostream& operator << (ostream &, const Vertex &);
};

Vertex::Vertex(): name("N/A"), E() {}

Vertex::Vertex(const Custom_String &n): name(n), E() {}

Custom_String Vertex::getName() const
{
	return name;
}

SmarterArray<Edge> Vertex::getEdgeSet() const
{
	return E;
}

int Vertex::getEdgeSetSize() const
{
	return E.getSize();
}

Edge Vertex::getEdge(const int &desVertexIndex) const
{
	for (int i = 0; i < E.getSize(); i++)
	{
		if (E[i].desVertexIndex == desVertexIndex)
			return E[i];
	}

	assert(false);
}

double Vertex::getEdgeCost(const int &desVertexIndex) const
{
	for (int i = 0; i < E.getSize(); i++)
	{
		if (E[i].desVertexIndex == desVertexIndex)
			return E[i].cost;
	}

	assert(false);
}

void Vertex::appendEdge(const int &desVertexIndex, const double &cost)
{
	for (int i = 0; i < E.getSize(); i++)
	{
		if (E[i].desVertexIndex == desVertexIndex && E[i].cost == cost)
			assert(false);
	}

	Edge temp;
	temp.desVertexIndex = desVertexIndex;
	temp.cost = cost;

	E.append(temp);
}

ostream& operator << (ostream &out, const Vertex &vertex)
{
	out << "Name = " << vertex.name << endl;
	out << "Edge Set" << endl;
	for (int i = 0; i < vertex.E.getSize(); i++)
		out << "\t to ---> " << vertex.E[i].desVertexIndex << ", cost = " << vertex.E[i].cost << endl;
	return out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Graph
{
private:
	SmarterArray<Vertex> V;
public:
	Graph();
	Graph(const char *);
	SmarterArray<Vertex> getVertexSet() const;
	int getVertexSetSize() const;
	Vertex& operator[](const int &) const;
	int getVertexIndex(const Custom_String &) const;
	int getVertexIndex(const Vertex &) const;
	Custom_String getRandomVertexName() const;
	void appendVertex(const Vertex &);
	void appendVertex(const Custom_String &);
	void appendEdge(const Custom_String &dep, const Custom_String &des, const double &cost);
	friend ostream& operator << (ostream &, const Graph &);
};

Graph::Graph(): V() {}

Graph::Graph(const char *path)
{
	ifstream fin(path);

	if (fin.fail())
	{
		assert(fin.fail());
	}

	Custom_String departure, destination;
	double cost;
	while(!fin.eof())
	{
		fin >> departure;
		if (departure.length() == 0)
			break;

		fin >> destination;
		fin >> cost;

		// We simply call the appendVertex() function by passing it the name of the vertex and then appendVertex() will take care by creating the empty edge set and then appending to variable V
		appendVertex(departure);
		appendVertex(destination);

		appendEdge(departure, destination, cost);
		appendEdge(destination, departure, cost);
	}

	fin.close();
}

SmarterArray<Vertex> Graph::getVertexSet() const
{
	return V;
}

int Graph::getVertexSetSize() const
{
	return V.getSize();
}

Vertex& Graph::operator[](const int &index) const
{
	assert(index >= 0 && index < V.getSize());

	return V[index];
}

int Graph::getVertexIndex(const Custom_String &name) const
{
	for (int i = 0; i < V.getSize(); i++)
	{
		if (V[i].getName() == name)
			return i;
	}

	return -1;
}

int Graph::getVertexIndex(const Vertex &v1) const
{
	for (int i = 0; i < V.getSize(); i++)
	{
		if (V[i].getName() == v1.getName())
			return i;
	}

	return -1;
}

Custom_String Graph::getRandomVertexName() const
{
	int randNumber = rand() % V.getSize();

	return V[randNumber].getName();
}

void Graph::appendVertex(const Vertex &v1)
{
	for (int i = 0; i < V.getSize(); i++)
	{
		if (V[i].getName() == v1.getName())
			return;
	}

	V.append(v1);
}

void Graph::appendVertex(const Custom_String &name)
{
	for (int i = 0; i < V.getSize(); i++)
	{
		if (V[i].getName() == name)
			return;
	}

	Vertex v1(name);
	V.append(v1);
}

void Graph::appendEdge(const Custom_String &dep, const Custom_String &des, const double &cost)
{
	// We check if both dep and des are available in the graph
	bool check = false; // by default we say dep is not available in the graph
	bool check2 = false;

	for (int i = 0; i < V.getSize(); i++)
	{
		if (V[i].getName() == dep)
			check = true;
		else if (V[i].getName() == des)
			check2 = true;
	}

	if (check == false || check2 == false) // This means either dep or des, or maybe both were not found in the graph, therefore we assert it
		assert(false);

	// Now that we have assert and we are sure they both exist in the graph, we continue with the remaining computations
	for (int i = 0; i < V.getSize(); i++)
	{
		if (V[i].getName() == dep)
		{
			for (int j = 0; j < V.getSize(); j++)
			{
				if (V[j].getName() == des)
				{
					V[i].appendEdge(j, cost);

					break;
				}
			}

			break;
		}
	}
}

ostream& operator << (ostream &out, const Graph &g)
{
	const int CITY_NAME_WIDTH = 25;
	out << endl;
	out << "The graph has " << g.getVertexSetSize() << " vertices." << endl;
	out << "These vertices are" << endl;
	for (int i = 0; i < g.getVertexSetSize(); i++)
	{
		Vertex v = g.V[i];
		out << "Vertex at index " << i << " = " << v.getName() << endl;
	}
	out << endl;
	out << "Each vertex together with its edge set looks like as follows" << endl;
	for (int i = 0; i < g.getVertexSetSize(); i++)
	{
		Vertex v = g.V[i];
		out << v << endl;
	}
	out << endl;
	out << "The graph connectivities are as follows..." << endl;
	out.setf(ios::fixed | ios::left);	//Left aligned fixed decimal places formatting
	for (int i = 0; i < g.getVertexSetSize(); i++)
	{
		Vertex depVertex = g[i];
		SmarterArray<Edge> E = depVertex.getEdgeSet();
		for (int j = 0; j < E.getSize(); j++)
		{
			int desVertexIndex = E[j].desVertexIndex;
			Vertex desVertex = g[desVertexIndex];
			out << depVertex.getName() << setw(CITY_NAME_WIDTH - depVertex.getName().length()) << " ";
			out << desVertex.getName() << setw(CITY_NAME_WIDTH - desVertex.getName().length()) << " ";
			out << setprecision(2) << E[j].cost << endl;
		}
	}
	out.unsetf(ios::fixed | ios::left);	//Removing formatting
	cout.precision(0);					//Resetting the decimal places to default
	return out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Path
{
private:
	SmarterArray<Custom_String> P;
public:
	Path();
	int length() const;
	int find(const Custom_String &) const;
	double computePathCost(const Graph &) const;
	Custom_String& operator [] (const int &) const;
	void append(const Custom_String &);
	void insert(const int &index, const Custom_String &);
	void remove(const int &);
	friend ostream& operator << (ostream &, const Path &);
};

Path::Path(): P() {}

int Path::length() const
{
	return P.getSize();
}

int Path::find(const Custom_String &name) const
{
	for (int i = 0; i < P.getSize(); i++)
	{
		if (P[i] == name)
			return i;
	}

	return -1;
}

double Path::computePathCost(const Graph &g) const
{
	double totalCost = 0;

	for (int i = 0; i < P.getSize() - 1; i++)
	{
		for (int j = 0; j < g.getVertexSetSize(); j++)
		{
			if (P[i] == g[j].getName())
			{
				bool check = false;
				for (int k = 0; k < g.getVertexSetSize(); k++)
				{
					if (P[i + 1] == g[k].getName())
					{
						totalCost += g[j].getEdge(k).cost;
						check = true;
						break;
					}
				}

				if (check == false)
					abort();
				break;
			}
		}
	}

	return totalCost;
}

Custom_String& Path::operator [] (const int &index) const
{
	assert(index >= 0 && index < P.getSize());

	return P[index];
}

void Path::append(const Custom_String &name)
{
	P.append(name);
}

void Path::insert(const int &index, const Custom_String &name)
{
	assert(index >= 0 && index <= P.getSize());

	if (index == P.getSize())
		P.append(name);
	else
	{
		Custom_String *temp = new Custom_String [P.getSize() + 1];

		int i = 0;
		for (; i < index; i++)
			temp[i] = P[i];

		temp[index] = name;

		for (int j = index + 1; j < P.getSize() + 1; j++, i++)
			temp[j] = P[i];

		int size = P.getSize() + 1;
		P.~SmarterArray();

		for (int i = 0; i < size; i++)
			P.append(temp[i]);
	}
}

void Path::remove(const int &index) //Assert the index argument and then remove the element at the specified index
{
	assert(index >= 0 && index <= P.getSize());

	P.remove(index);
}

ostream& operator << (ostream &out, const Path &path)
{
	out << "[";
	if (path.length() > 0)
	{
		for (int i = 0; i < path.length()-1; i++)
			out << path[i] << " -> ";
		out << path[path.length()-1];
	}
	out << "]";
	return out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

Path computeMinCostPath(const Graph &g, const Custom_String &departure, const Custom_String &destination, Path &currentPath = Path())
{
	assert(g.getVertexSetSize() >= 1);

	int depVertexIndex = g.getVertexIndex(departure);
	int desVertexIndex = g.getVertexIndex(destination);

	assert((depVertexIndex >= 0 && depVertexIndex < g.getVertexSetSize()) && (desVertexIndex >= 0 && desVertexIndex < g.getVertexSetSize()));

	if (departure == destination)
	{
		Path minCostPath = currentPath;
		minCostPath.append(destination);
		cout << "Path found: " << minCostPath << " with cost " << minCostPath.computePathCost(g) << endl;

		return minCostPath;
	}
	else if (currentPath.find(departure) != -1)
	{
		Path minCostPath = currentPath;
		return minCostPath;
	}
	else
	{
		int index = g.getVertexIndex(departure);
		Vertex depVertex = g[index];
		SmarterArray<Edge> E = depVertex.getEdgeSet();
		Path minCostPath;
		currentPath.append(departure);

		for (int i = 0; i < E.getSize(); i++)
		{
			Vertex nextVertex = g[E[i].desVertexIndex];
			Custom_String nextVertexName = nextVertex.getName();

			if (currentPath.find(nextVertexName) != -1)
				continue;

			Path candidatePath = computeMinCostPath(g, nextVertexName, destination, currentPath);

			if (candidatePath.length() == 0)
				continue;

			if (candidatePath[candidatePath.length() - 1] != destination)
				continue;

			if (minCostPath.length() == 0)
			{
				minCostPath = candidatePath;
				continue;
			}

			if (candidatePath.computePathCost(g) < minCostPath.computePathCost(g))
			{
				minCostPath = candidatePath;
				continue;
			}
		}

		currentPath.remove(currentPath.length() - 1);

		return minCostPath;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	srand((unsigned)time(0));
	Graph g("BiggerMap.txt");
	cout << "Graph constructed successfully." << endl;
	cout << g << endl;

	Custom_String departure = g.getRandomVertexName();
	Custom_String destination = g.getRandomVertexName();
	cout << "Computing shortest path from " << departure << " to " << destination << endl;
	Path minCostPath = computeMinCostPath(g, departure, destination);
	cout << endl;
	cout << "Departure: " << departure << endl;
	cout << "Destination: " << destination << endl;
	if (minCostPath.length() == 0)
		cout << "No path found." << endl;
	else
		cout << "The minimum cost path is: " << minCostPath << " with cost = " << minCostPath.computePathCost(g) << endl;
	
	system("Pause");
	return 0;
}