/*
Singly Linked list operations

*/
#include<iostream>
using namespace std;
class Node
{
    int data;
    Node * next;
    friend class SLL;
    public:
    Node()
    {
        next = nullptr;
    }     
};
class SLL
{
    Node * root;

    public:
    SLL()
    {
        root = nullptr;
    }
    void traversal();//Done
    void insertEnd();//Done
    void insertFront();//Done
    void insertPosition(int);//Done
    void deleteFirst();//Done
    void deleteEnd();//Done
    void deletePosition(int);//Done
    void sortList();
    void reverseList();
    void MiddleOfList();
};
void SLL::insertFront()
{
    cout<<"Enter the data for a new node to be added at front:";
    Node * curr = new Node();
    cin>>curr->data;
    if(root==nullptr)
    {
        root = curr;
    }
    else
    {
        curr->next = root;
        root=curr;
    }
}
void SLL::insertEnd()
{
    cout<<"Enter the data for a new node:";
    Node * curr = new Node();
    cin>>curr->data;
    if(root==nullptr)
    {
        root = curr;
    }
    else
    {
        Node * temp = root;

        while(temp->next!=nullptr)
            temp=temp->next;

        temp->next = curr;
    }
}
void SLL::traversal()
{
    cout<<"Data from linked list is:"<<endl;
    Node * temp = root;

    if(temp==nullptr)
        cout<<"Linked list is empty!!"<<endl;
    else
    {
        while(temp!=nullptr)
        {
            cout<<temp->data<<" ";
            temp= temp->next;
        }
    }
}
void SLL::insertPosition(int value)
{
    cout<<"Enter the data for a new node:";
    Node * curr = new Node();
    cin>>curr->data;
    if(root==nullptr)
    {
        return;
    }
    else
    {
        Node * temp = root;
        while(temp!=nullptr)
        {
            if(temp->data == value)
            {
                curr->next = temp->next;
                temp->next = curr;
                break;
            }
            else
                temp = temp->next;
        }   
    }   
}
void SLL::deleteFirst()
{
    if(root==nullptr)
    {
        cout<<"Linked list is empty"<<endl;
        return;
    }
    else 
    {
        if(root->next==nullptr)
            {
                delete root;
                root=nullptr;
            }
            else
            {
                Node * temp = root->next;
                delete root;
                root = temp;
            }
    }
}
void SLL::deleteEnd()
{
    if(root==nullptr)
    {
        cout<<"Linked list is empty"<<endl;
        return;
    }
    else 
    {
        Node * temp = root;
        Node * prevNode;
        int flag = 0;
        while(temp->next!=nullptr)
        {
            prevNode = temp;
            temp=temp->next;
            flag=1;
        }
        delete temp;
        if(flag)
            prevNode->next=nullptr;
        else
            root = nullptr;  // when there is just one node in list. while will fail
    }
}
void SLL::deletePosition(int value)
{
    if(root==nullptr)
    {
        cout<<"Linked list is empty"<<endl;
        return;
    }

    if(root->data==value)
    {
        if(root->next==nullptr)
        {
            delete root;
            root = nullptr;
        }
        else
        {
            Node * temp;
            temp = root->next;
            delete root;
            root = temp;
        }
    }
    else
    {
        Node * temp = root;
        Node * prev;
        while(temp->next!=nullptr)
        {
            prev = temp;
            temp = temp->next;
            if(temp->data==value)
            {
                if(temp->next==nullptr)
                {   
                    delete temp;
                    prev->next = nullptr;
                    break;
                }
                else
                {
                    Node * temp1 = temp->next;
                    delete temp;
                    prev->next = temp1;
                    break;
                }
            }
        }
    }   
}
void SLL::sortList(){}
void SLL::reverseList()
{
    Node * newHead = nullptr;

    while(root!=nullptr)
    {
        Node * next = root->next;
        root->next = newHead;
        newHead = root;
        root = next;
    }
    root = newHead;

}
void SLL::MiddleOfList()
{

}
int main()
{
    SLL sll;
    sll.insertEnd();
   // sll.insertEnd();
   // sll.insertEnd();
   // sll.insertEnd();
    //sll.insertFront();
    //sll.insertPosition(45);
    sll.traversal();
    //sll.deleteFirst();
    //sll.deleteEnd();
    //sll.deletePosition(12);
    sll.reverseList();
    sll.traversal();
    return 1;
}