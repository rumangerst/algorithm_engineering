#include <cstdlib>
#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;

template <typename T>
class LinkedList
{
public:

	LinkedList()
	{
	}

	~LinkedList()
	{
		if(next != 0)
		{

			//Delete all the entries
			for(int i = size() - 1; i >= 0; i--)
			{
				LinkedListNode *node = getNodeAt(i);
				delete node;
			}
		}
	}

	void addAt(T item, int index)
	{
		if(index == 0)
		{
			LinkedListNode *new_first = new LinkedListNode();
			new_first->value = item;
			new_first->next = next;
			next = new_first;
		}
		else if( index == _size)
		{
			LinkedListNode *last = getNodeAt(size() - 1);
			LinkedListNode *new_last = new LinkedListNode();
			new_last->value = item;
			last->next = new_last;
		}
		else
		{
			if(index < 0 || index >= size())
				throw 1;
				
			LinkedListNode* parent = getNodeAt(index - 1);
			LinkedListNode* after = parent->next;
			LinkedListNode* new_node = new LinkedListNode();
			new_node->value = item;
			
			parent->next = new_node;
			new_node->next = after;
		}
		
		//Increase size!
		_size++;
	}
	
	void removeAt(int index)
	{
		if(index < 0 || index >= size())
				throw 1;
	
		if(index == 0)
		{
			LinkedListNode* new_first = next->next;
			delete next;
			
			next = new_first;
		}
		else
		{
			LinkedListNode* parent = getNodeAt(index - 1);
			LinkedListNode* new_child = parent->next->next;
			delete parent->next;
			parent->next = new_child;
		}
		
		//Decrease size!!
		_size--;
	}
	
	int size()
	{
		return _size;
	}
	
	T getAt(int index)
	{
		if(index < 0 || index >= size())
			throw 1;
			
		LinkedListNode* current = next;
			
		while(index > 0)
		{
			current = current->next;
			index--;
		}
		
		return current->value;
	}
	
	void print()
	{
		for(int i = 0; i < size(); i++)
		{
			cout << "i: " << getAt(i) << endl;
		}
	}
	
private:

	struct LinkedListNode
	{
		T value;
		LinkedListNode *next = 0;		
	};

	int _size = 0;
	LinkedListNode *next = 0;
	
	LinkedListNode *getNodeAt(int index)
	{
		if(index < 0 || index >= size())
			throw 1;
			
		LinkedListNode* current = next;
			
		while(index > 0)
		{
			current = current->next;
			index--;
		}
		
		return current;
	}
};

int rand_ibw(int min, int max)
{
	return min + rand() % (max - min);
}

void test_list(int N)
{
	auto start = std::chrono::high_resolution_clock::now();
	
	LinkedList<int> list;
	
	for(int i = 0; i < N; i++)
	{
		list.addAt(i, rand_ibw(0, list.size() + 1));
	}
	
	auto finish = chrono::high_resolution_clock::now();	
	cout << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns [List]" <<  endl;
}

void test_vector(int N)
{
	auto start = std::chrono::high_resolution_clock::now();
	
	vector<int> list;
	
	for(int i = 0; i < N; i++)
	{
		list.insert(list.begin() + rand_ibw(0, list.size() + 1), i);
	}
	
	auto finish = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns [Vector]" << endl;
}


int main(int argc, char* argv[])
{	
	int N = stoi(argv[1]);

	if(N <= 0)
		return 1;
		
	srand(time(0));


	/*LinkedList<int> list;
	list.addAt(1,0);
	list.addAt(2,0);
	list.addAt(3,0);
	list.addAt(4,0);
	list.addAt(5,0);
	list.removeAt(3);
	
	for(int i = 0; i < list.size(); i++)
	{
		cout << "i: " << list.getAt(i) << endl;
	}*/
	
	test_list(N);
	test_vector(N);

	return 0;
}
