#include<iostream>
#include<string>
#include<fstream>
#include <algorithm>
#include <cctype>
using namespace std;
bool checkUsername(string username)
{
    bool usernameExists= false;
    usernameExists=false;
    ifstream login;
    login.open("customer login details.txt");
    if(login.is_open())
    {
        string user,pass;
        while(login>>user>>pass)
        {
            if(user==username)
            {
                usernameExists=true;
                break;
            }
        }
    } 
    login.close();
    return usernameExists;
}
bool checkPassword(string username,string password)
{
    bool passwordExists=false;
    ifstream login;
    login.open("customer login details.txt");
    if(login.is_open())
    {
        string user,pass;
        while(login>>user>>pass)
        {
            if(user==username&&pass==password)
            {
                passwordExists=true;
                break;
            }
        }
    } 
    login.close();
    return passwordExists;
}
class customer
{
    string username;
    string password;
    string con_password;
    public:
    void set_details()
    {
        cout<<"Enter your username:";
        cin.ignore();
        getline(cin,username);
        while(checkUsername(username)==true)
        {
            cout<<"Username already exists!!!\nPlease try another username!!!\n";
            cout<<"Enter your username:";
            getline(cin,username);
        }
        cout<<"Enter password:";
        getline(cin,password);
        while(password.length()<8)
        {
            cout<<"Password must be atleast 8 characters long!!!\nPlease enter a new password:";
            getline(cin,password);
        }
        cout<<"Confirm password:";
        getline(cin,con_password);
        while(password!=con_password)
        {
            cout<<"Password did not match\nRenter password to confirm:";
            getline(cin,con_password);
        }
        fstream login;
        login.open("customer login details.txt",ios::app);
        login<<username<<" "<<password<<endl;
        login.close();
    }
};
int main() 
{
    customer c;
    int ch;
    string username;
    string password;
    cout<<"Press 1 to log in!!!\nPress 2 to create account!!!";
    cin>>ch;
    if(ch==1)
    {
        cout<<"Enter username:";
        cin.ignore();
        getline(cin,username);
        while(checkUsername(username)==false||checkPassword(username,password)==false)
        {
            if(checkUsername(username)==false)
            {
                cout<<"Username does not exist!!!\nTry again!!!\nEnter correct username:";
                getline(cin,username);
            }
            if(checkUsername(username)==true)
            {
                cout<<"Enter password:";
                getline(cin,password);
                if(checkPassword(username,password)==false)
                {
                    cout<<"Incorrect password!!! Try again!!!\nEnter correct password:";
                    getline(cin,password);
                }
            }
        }
    }
    else if(ch==2)
    {
        c.set_details();
    }    
}
