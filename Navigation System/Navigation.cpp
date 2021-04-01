#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <iomanip>

using namespace std;

class CMPT135_String
{
private:
	char *buffer; //The dynamic array to store the printable characters and a null terminating character 
public:
	CMPT135_String(); //The buffer is initialized to nullptr value
	CMPT135_String(const char *); //A non-default constructor with a null terminated char array argument
	CMPT135_String(const CMPT135_String &); //Deep copy constructor
	~CMPT135_String();  //Delete any heap memory and assign buffer nullptr value
	CMPT135_String& operator = (const CMPT135_String &); //Memory cleanup and deep copy assignment
	int length() const; //Return the number of printable characters. Return zero if buffer is nullptr
	bool empty() const;  //Return true if length is 0. Otherwise return false
	char& operator [] (const int &) const;  //Assert index and then return the char at the index
	CMPT135_String operator + (const char &) const; //See assignment 1
	CMPT135_String& operator += (const char &); //See assignment 1
	bool operator == (const CMPT135_String &) const; //See assignment 1
	bool operator != (const CMPT135_String &) const; //See assignment 1
	friend istream& operator >> (istream &, CMPT135_String &); //Implemented for you
	friend ostream& operator << (ostream &, const CMPT135_String &); //Implemented for you
};

CMPT135_String::CMPT135_String()
{
	buffer = nullptr;
}

CMPT135_String::CMPT135_String(const char *cStr)
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

CMPT135_String::CMPT135_String(const CMPT135_String &s)
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

CMPT135_String::~CMPT135_String()
{
	if (buffer != nullptr)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}

CMPT135_String& CMPT135_String::operator = (const CMPT135_String &s)
{
	// Check for self assignment, for example s = s
	if (this == &s)
		return *this;

	// Delete the left hand side object's memory
	this->~CMPT135_String();

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

int CMPT135_String::length() const
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

bool CMPT135_String::empty() const
{
	if (this->length() == 0)
		return true;

	return false;
}

char& CMPT135_String::operator [] (const int &index) const
{
	assert(index >= 0 && index < this->length());
	
	return buffer[index];
}

CMPT135_String CMPT135_String::operator + (const char &c) const
{
	int length = this->length();
	CMPT135_String s1;
	s1.buffer = new char[length + 2];

	for (int i = 0; i < length; i++)
		s1[i] = (*this)[i];

	s1[length] = c;
	s1[length + 1] = '\0';

	return s1;
}

CMPT135_String& CMPT135_String::operator += (const char &c)
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

bool CMPT135_String::operator == (const CMPT135_String &s) const
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

bool CMPT135_String::operator != (const CMPT135_String &s) const
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


istream& operator >> (istream &in, CMPT135_String &s)
{
	//This function reads characters input from a keyboard or a file until either a TAB, EOL, or EOF is 
	//reached. The function ignores any leading or trailing spaces. It is designed to be able to read a 
	//string of characters that may or may not contain spaces without any problem.

	//Define some useful constant values
	#define SPACE ' '
	#define TAB '\t'
	#define	EOL	'\n'

	//Delete the old value of s
	s.~CMPT135_String();

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
	CMPT135_String temp;
	for (int i = 0; i < s.length()-trailingSpacesCount; i++)
		temp += s[i];
	s = temp;

	return in;
}

ostream& operator << (ostream &out, const CMPT135_String &s)
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
	T *A; //The dynamic array to store the elements
	int size; //The number of elements in the array

public:
	//Constructors
	SmarterArray(); //Implemented for you
	SmarterArray(const T*, const int&); //Deep copy of the argument dynamic array with the given size
	SmarterArray(const SmarterArray<T>&); //Deep copy of the argument

	//Assignment operator
	SmarterArray<T>& operator = (const SmarterArray<T>&); //Memory cleanup and deep copy of the argument

	//Destructor
	~SmarterArray(); //Memory cleanup and resetting the size

	//Getters, Setters, operators and other functions
	int getSize() const; //Return the number of elements in the container
	T& operator[](const int&) const; //Assert index and then return the element at the given index
	int find(const T&) const; //Return the index of the first element that is == to the argument. 
								//Return -1 if not found.
	void append(const T&);  //Store the argument value after the last element
	void insert(const int &, const T&); //Assert the integer argument index >= 0 && index <= size and then
										//Insert the T type argument into the calling object at the index.
										//If the index argument is equal to size, then perform append
	bool remove(const int&); //If the index is valid, then remove the element at the index argument
							//from the calling object and return true. Otherwise do nothing and return 
							//false. Do not assert the index argument.
	bool operator == (const SmarterArray<T>&) const; //Return true if sizes are equal and elements at the
														//same indexes are equal. Otherwise return false
	
	template <class T1> //Those of you working with xCode, don't use the same template name T. T1 is ok.
	friend ostream& operator << (ostream&, const SmarterArray<T1>&); //Implemented for you
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
	int desVertexIndex; //the index (in the underlying graph) of the destination vertex of this edge
	double cost; //cost of an edge
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Vertex
{
private:
	CMPT135_String name; //Name of the city at the vertex
	SmarterArray<Edge> E; //A container to store the edges emanating from this vertex. All the elements of 
	         //E have the same origin vertex which is the this object. But they have different destination
		     //vertices which are given by the desVertexIndex member variable of each element

public:
	Vertex(); //Assign name = "N/A" and initialize E to an empty container (default object E)
	Vertex(const CMPT135_String &); //Assign name = the argument and initialize E to an empty container
	CMPT135_String getName() const; //Return the name
	SmarterArray<Edge> getEdgeSet() const; //Return E
	int getEdgeSetSize() const; //Return the size of E
	Edge getEdge(const int & desVertexIndex) const; //Assert an edge whose destination vertex index is 
										        //equal to the argument is found in E. Then return the edge
	double getEdgeCost(const int &desVertexIndex) const; //Assert an edge whose destination vertex index 
					                        //is equal to the argument is found in E. Then return its cost
	void appendEdge(const int &desVertexIndex, const double &cost); //Assert there is no element of E 
				//whose destination vertex index and cost are equal to the argument values. Then append
				//a new element whose destination vertex index and cost are initialized with the
				//argument values to E
	friend ostream& operator << (ostream &, const Vertex &); //Implemented for you
};

Vertex::Vertex(): name("N/A"), E() {}

Vertex::Vertex(const CMPT135_String &n): name(n), E() {}

CMPT135_String Vertex::getName() const
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
	Graph();//Construct empty graph (default object V)
	Graph(const char *); //Construct a graph from a text file whose path is given by the argument cstring.
        //The input text file will consist a pair of cities and the cost to go from one to the other on  
        //every line. The pair of cities and their cost can be separated by one or more SPACE, TAB, or EOL 
        //(end of line) characters. It doesn't matter how many SPACEs, TABs, or EOLs are present. BUT 
        //THAT THERE HAS TO BE AT LEAST ONE TAB or EOL CHARACTER between the pairs of cities and AT LEAST 
        //ONE TAB or EOL between the second city and the cost. This is because the CMPT135_String class 
        //uses TAB or EOL as a deliminator (separator or terminating character of input reading). This 
        //means city names can be made up of one or more words separated by spaces. An example of one line
        //of text in the input text file is:
        //        New Westminster             Port Coquitlam        4.5
        //In this example, there has to be at least one TAB or EOL character between "New Westminster" and 
        //"Port Coquitlam" and at least one TAB or EOL character between "Port Coquitlam" and 4.5. Of 
        //course there can be more than one TAB or EOL characters. The number of TAB or EOL characters 
        //between the cities names and the second city and the cost is arbitrary and can be equal or 
        //different. There can also be zero or more spaces as much as you like without causing any 
        //problem. Moreover there can be as many empty lines as one likes anywhere in the input file and 
        //these empty lines will not cause any problem thanks to the design of the istream friend function
        //of the CMPT135_String class. In order to demonstrate such flexibility in the input text file, a 
        //sample input text file (see the CMPT 135 2020-03 SYN Class Final Project: Connectivity Map 
        //Large file) is provided to you together with this project. In the input file we arbitrarily put 
        //useless empty lines here and there just to demonstrate the fact that our input text file will be
        //read perfectly without any problem.
        /*
        Thus this function should perform the following tasks
	Construct a non-default file input streaming object using the cstring argument file name
	Assert the file is opened successfully
	Declare two CMPT135_string data type variables named departure and destination
	While EOF is not reached do
•	Read city name. This is the departure city. (fin >> departure;)
•	If departure city is empty CMPT135_String object, then break.
•	Read city name. This is the destination city. (fin >> destination;)
•	Read the cost (double data type)
•	Append a new vertex whose name is the departure city and whose edge set is empty to the this object. You must use the appendVertex member function of this class (see below) to append appropriately.
•	Append a new vertex whose name is the destination city and whose edge set is empty to the this object. You must use the appendVertex member function of this class to append appropriately.
•	Append a new edge from the departure city to the destination city with a cost read in part (d) above to the this object. You must use the appendEdge member function of this class (see below) to append appropriately.
•	Append a new edge from the destination city to the departure city with a cost read in part (d) above to the this object. You must use the appendEdge member function of this class (see below) to append appropriately.
	Close the input file stream object and you are done.
        */
	SmarterArray<Vertex> getVertexSet() const; //Return V
	int getVertexSetSize() const; //Return the number of elements of V
	Vertex& operator[](const int &) const;//Assert the index argument and then return the element at index
	int getVertexIndex(const CMPT135_String &) const; //Return the index of an element whose name matches														//the argument. If no such element is found, return -1
													//Assertion should not be performed.
	int getVertexIndex(const Vertex &) const; //Return the index of the element whose name matches the
											//name of the vertex argument. If no such element is found, 
											//return -1. Assertion should not be performed.
	CMPT135_String getRandomVertexName() const; //Pick a vertex at random and return its name
	void appendVertex(const Vertex &); //Append the argument only if no such vertex already exists in the 
									    // calling object. If same name vertex already exists then do nothing 
										//and return. Assertion should not be performed.
	void appendVertex(const CMPT135_String &); //Append a new vertex with the given name and empty E only 
				           //if no such vertex already exists in the calling object. If same name vertex 
							//already exists then do nothing and return. Assertion should not be performed.

	void appendEdge(const CMPT135_String &dep, const CMPT135_String &des, const double &cost); //Assert 
//two vertices whose names match the arguments exist. Then append an edge to the vertex whose name matches
//the dep argument. The destination vertex index of the edge must be set to the index of the vertex whose 
//name matches des and its cost must be set to the cost argument
	friend ostream& operator << (ostream &, const Graph &); //Implemented for you
};

Graph::Graph(): V() {}

Graph::Graph(const char *path)
{
	ifstream fin(path);

	if (fin.fail())
	{
		assert(fin.fail());
	}

	CMPT135_String departure, destination;
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

int Graph::getVertexIndex(const CMPT135_String &name) const
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

CMPT135_String Graph::getRandomVertexName() const
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

void Graph::appendVertex(const CMPT135_String &name)
{
	for (int i = 0; i < V.getSize(); i++)
	{
		if (V[i].getName() == name)
			return;
	}

	Vertex v1(name);
	V.append(v1);
}

void Graph::appendEdge(const CMPT135_String &dep, const CMPT135_String &des, const double &cost)
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
	SmarterArray<CMPT135_String> P; //The names of the vertices along the path
public:
	Path(); //Construct an empty path. Default object P.
	int length() const; //Return the number of vertices along the path (the number of elements of P)
	int find(const CMPT135_String &) const; //Return the index of an element of P whose name matches the 
											//argument. If no element satisfies the condition, then return -1
											//Do not perform assertion operation.
	double computePathCost(const Graph &) const; //Compute the sum of the costs of edges along this path 
		//given the underlying Graph argument. Remember that the path object stores only city names. Thus 
       //you need the underlying graph argument to determine the vertices in the graph that belong to the 
       //cities. Then you will be able to find the edges that connect the vertices which will enable you to 
       //get the costs of the edges. The sum of the costs of these edges is returned from this function. If 
       //during the computation of the path cost, you find that there is no any edge in the underlying graph
       //that connects successive elements in P, then it means your path is an invalid path and you
       //need to abort your application.
	CMPT135_String& operator [] (const int &) const; //Assert index is valid and then return the 
											//element of P at the given index
	void append(const CMPT135_String &); //Append the argument to the calling object. No need to assert.
	void insert(const int &index, const CMPT135_String &); //Assert the condition index >= 0 && 
											//index <= the length and then insert the CMPT135_String argument 
											//at the specified index. If index is equal to the length, then 
											//perform append.
	void remove(const int &); //Assert the index argument and then remove the element at the specified index
	friend ostream& operator << (ostream &, const Path &); //Implemented for you.
};

Path::Path(): P() {}

int Path::length() const
{
	return P.getSize();
}

int Path::find(const CMPT135_String &name) const
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

CMPT135_String& Path::operator [] (const int &index) const
{
	assert(index >= 0 && index < P.getSize());

	return P[index];
}

void Path::append(const CMPT135_String &name)
{
	P.append(name);
}

void Path::insert(const int &index, const CMPT135_String &name)
{
	assert(index >= 0 && index <= P.getSize());

	if (index == P.getSize())
		P.append(name);
	else
	{
		CMPT135_String *temp = new CMPT135_String [P.getSize() + 1];

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

Path computeMinCostPath(const Graph &g, const CMPT135_String &departure, const CMPT135_String &destination, Path &currentPath = Path())
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
			CMPT135_String nextVertexName = nextVertex.getName();

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

	CMPT135_String departure = g.getRandomVertexName();
	CMPT135_String destination = g.getRandomVertexName();
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