#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<forward_list>
#include<stack>

using namespace std;

class TaskNode
{
    private:
        string title;
        string description;
        string dueDate;
    public:
        TaskNode() {}
        TaskNode( string &title, string &description, string &dueDate)
        {
            this->title = title;
            this->description =description;
            this->dueDate = dueDate;
        }
        string getTitle() { return title; }
        string getDescription() { return description; }
        string getDueDate() { return dueDate; }
        void printTask()
        {
            cout<<endl;
            cout<<"Task title : "<<title<<endl;
            cout<<"Task Description : "<<description<<endl;
            cout<<"Task Due Date : "<<dueDate<<endl;
        }
};

class TaskManager
{
    private:
        forward_list<TaskNode> tasks;
        TaskManager(){ loadTasks(); }
        static bool isObjectCreated;

    protected:
        void loadTasks();
        void unloadTasks();

    public:
        static TaskManager* getTaskManager()
        {
            if(isObjectCreated == false)
            {
                isObjectCreated=true;
                return new TaskManager();
            }
            else{
                cout<<"\n You can get only One Task Manager "<<endl;
                return nullptr;
            }
        }
        int menu();
        void addTask();
        void deleteTask(string& );
        void viewAllTasks();
        ~TaskManager();

};
bool TaskManager::isObjectCreated = false;

void TaskManager::loadTasks() {
    tasks.clear();
    ifstream fin("mytasks.txt", ios::in);
    if (!fin.is_open()) {
        cout << "\nNo existing tasks found.";
        return;
    }

    string t, d, dd;
    while (getline(fin, t) && getline(fin, d) && getline(fin, dd)) {
        TaskNode task(t, d, dd);
        tasks.push_front(task);
    }
    fin.close();
}

void TaskManager::unloadTasks() {
    //ofstream fout("mytasks.txt", ios::out); // Open file in text mode
    ofstream fout("mytasks.txt", ios::out);

    if (!fout.is_open()) {
        cout << "\nCould not create file";
        return;
    }

    while (!tasks.empty()) {
        TaskNode temp = tasks.front();
        fout << temp.getTitle() << endl;
        fout << temp.getDescription() << endl;
        fout << temp.getDueDate() << endl;
        tasks.pop_front();
    }
    fout.close();
    cout << "\nTasks saved successfully to file." << endl;
}

// void TaskManager::loadTasks(){
//     tasks.clear();
//     ifstream fin;
//     string t,d,dd;
//     fin.open("myTasks.txt",ios::in|ios::binary);
//     if(!fin)
//         cout<<"\nTask Manager is Empty";
//     else{
//         fin.ignore();
//         while(!fin.eof()){

//             getline(fin,t);
//             getline(fin,d);
//             getline(fin,dd);
//             tasks.push_front(*(new TaskNode(t,d,dd)));
//         }
//     }
//     fin.close();
// }
// void TaskManager::unloadTasks(){

//     TaskNode temp;
//     ofstream fout;
//     if(tasks.empty()){
//         fout.open("mytasks.txt",ios::out|ios::binary);
//         if(!fout.is_open()){
//             cout<<"\nCould not create file";
//             return;
//         }

//         while(!tasks.empty()){
//             temp = tasks.front();
//             fout<<"\n"<<temp.getTitle();
//             fout<<'\n'<<temp.getDescription();
//             fout<<'\n'<<temp.getDueDate();
//             tasks.pop_front();
//         }
//         fout.close();
//         cout<<"\nFile Written Successfully"<<endl;
//     }
//     else{
//         remove("mytasks.txt");
//     }

// }
int TaskManager::menu(){
    int choice;
    cout<<endl;
    cout<<"1. Add New Task"<<endl;
    cout<<"2. Delete Task"<<endl;
    cout<<"3. View All Tasks"<<endl;
    cout<<"4. Exit"<<endl;
    cout<<"Enter Your Choice"<<endl;
    cin>>choice;
    return choice;
}
bool isValidDate(const string& );
void TaskManager::addTask(){
    string t,d,dd;
    cin.ignore();

    cout<<"\nEnter task title: "<<endl;
    getline(cin,t);
    
    cout<<"\nEnter task description: "<<endl;
    getline(cin,d);
    
    do {
        cout << "\nEnter task due date (dd/mm/yyyy): " << endl;
        getline(cin, dd);
        if (!isValidDate(dd)) {
            cout << "Invalid date format. Please enter a valid date in dd/mm/yyyy format." << endl;
        }
    } while (!isValidDate(dd));


    TaskNode newTask(t,d,dd);
    tasks.push_front(newTask);
    cout<<"\nTask Added Successfully"<<endl;

}
bool isValidDate(const string &date) {

    // Check the format is exactly "dd/mm/yyyy"
    if (date.length() != 10 || date[2] != '/' || date[5] != '/') {
        return false;
    }

    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (month < 1 || month > 12 || day < 1 || year < 1900) {
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    return day <= daysInMonth[month - 1];
}

void TaskManager::deleteTask(string &t){

    forward_list<TaskNode> :: iterator it, it1;
    bool flag = true;
    it = tasks.begin();
    it1 = tasks.before_begin();

    while(it!= tasks.end()){
        if(it->getTitle() == t){
            tasks.erase_after(it1);
            cout<<"\nTask Deleted Successfully"<<endl;
            flag = false;
            break;
        }
        it1 = it++;
    }
    if(flag) cout<<"\nTask Not Found";

    cout<<"\nPress Enter to Continue..."<<endl;
    cin.ignore();
    cin.get();

}
void TaskManager::viewAllTasks(){
    if(tasks.empty())
        cout<<"\nNo Task in Task Manager";
    else
        for(auto task: tasks)
            task.printTask();
    
    cout<<"\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}
TaskManager::~TaskManager(){
    unloadTasks();
    isObjectCreated = false;
}

int main()
{
    TaskManager *tm;
    tm = TaskManager::getTaskManager();
    string temp;
    while(true){
        system("clear");
        switch(tm->menu()){
            case 1:
                tm->addTask();
                break;
            case 2:
                cout<<"\nEnter Title of the task you want to delete"<<endl;
                cin.ignore();
                getline(cin,temp);
                tm->deleteTask(temp);
                break;
            case 3:
                tm->viewAllTasks();
                break;
            case 4:
                delete tm;
                exit(0);
            default:
                cout<<"\nInvalid Choice, RETRY"<<endl;

        }
    }

    cout<<endl;
    return 0;
}
