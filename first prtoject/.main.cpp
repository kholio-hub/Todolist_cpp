#include <iostream>
#include <vector>
#include <string>

using namespace std;
struct Task {
    string title;
    string desc;
    bool done;
};
vector<Task> tasks;

void addTask() 
{
    Task t;
    cout<<"\nEnter task title: ";
    getline(cin,t.title);
    cout<<"Enter task description (optional): ";
    getline(cin, t.desc);
    t.done=false;
    tasks.push_back(t);
    cout<<"Task added successfully!\n";
}
void listTasks() 
{
    cout<<"\n=== Task List ===\n";
    for(size_t i=0;i<tasks.size();i++)
     {
        cout<<i+1<<". ["<<(tasks[i].done ? "done" : "not done")<<"] "
               <<tasks[i].title<<endl;
        if(!tasks[i].desc.empty())
            cout<<"Description: "<<tasks[i].desc<<endl;
    }
    if(tasks.empty()) cout << "No tasks yet.\n";
}
void markDone() 
{
    listTasks();
    if(tasks.empty()) return;
    cout<<"\nEnter task number to mark as done: ";
    int num;
    cin>>num;
    cin.ignore(); 
    if(num>=1&& num<=tasks.size())
     {
        tasks[num-1].done=true;
        cout<<"Task marked as done!\n";
    } 
    else 
    {
        cout<<"Invalid task number!\n";
    }
}

int main() {
    while (true) 
    {
        cout<<"\nChoose an action: \n";
        cout<<"1. Add task\n";
        cout<<"2. List tasks\n";
        cout<<"3. Mark task as done\n";
        cout<<"4. Exit\n";
        cout<<"Your choice: ";

        string choice;
        getline(cin, choice);

        if(choice=="1") addTask();
        else if (choice=="2") listTasks();
        else if (choice=="3") markDone();
        else if (choice=="4") break;
        else cout<<"Invalid choice\n";
    }

    
    return 0;
}