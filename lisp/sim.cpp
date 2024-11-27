#include <iostream>
#include <stdexcept>

using namespace std;

class Integer
{
    int value;
public:
    Integer(int value)
        :value(value)
    {
    }

    Integer()
        :value(0)
    {
    }

    Integer& operator = (int value)
    {
        this->value = value;
        return *this;
    }

    friend ostream& operator << (ostream& out, Integer i);
};

ostream& operator << (ostream& out, Integer i)
{
    out << i.value;
    return out;
}

template <typename T>
class LinkedList 
{
    struct ListNode
    {
        T* data;
        ListNode* next;
        
        ListNode(T* data, ListNode* next)
            :data(data), next(next)
        {
        }
    };

    ListNode* head;

public:
    LinkedList()
        :head(nullptr)
    {    
    }

    // write copy constructor to persist across scopes with listnodes pointing to different data. 

    void insert(T* addr)
    {
        head = new ListNode(addr, head);
    }

    bool find(T const& word)
    {
        return false;
    }

    T middle()
    {
        if (is_empty())
            return T();

        ListNode* slow = head;
        int counter = 0;
        for (ListNode* p = head; p != nullptr; p = p->next)
        {
            if (counter == 1)
            {
                slow = slow->next;
            }
            ++counter;
            counter %= 2;
        }
        return *(slow->data);
    }

    void remove(T const& data)
    {
    }

    bool is_empty()
    {
        return head == nullptr;
    }

    bool is_full()
    {
        return false;
    }

    void print(ostream & out)
    {
        for (ListNode* p = head; p != nullptr; p = p->next)
        {
            out << *(p->data) << ", ";
        }
        out << endl;
    }
    
    ~LinkedList()
    {
        ListNode* chopping_block;
        for (ListNode* p = head; p != nullptr; )
        {
            chopping_block = p;
            p = p->next;
            delete chopping_block;
        }
    }
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        throw std::invalid_argument("Invalid command line argument");
    }
    
    int len = stoi(argv[1]);
    LinkedList<Integer>* list_o_lists = new LinkedList<Integer>[len];
    for (int i = 0; i < len; ++i)
    {
        for (int j = 1; j < i + 2; ++j)
            list_o_lists[i].insert(new Integer(j));
        list_o_lists[i].print(cout);
        cout << list_o_lists[i].middle() << endl;
    }
}

