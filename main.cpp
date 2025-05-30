#include <iostream>
using namespace std;
#include ".\renderer\window.h"
#include "project_maker.h"
#include <string>

int main (){
    bool selection = false;
    while (selection == false) {
        cout<<"what renderer do you want to use ? Opengl or vulkan - 1 or 2 -"<<endl;
        int answer ;
        cin>>answer;
        if (answer == 1){
            selection = true;
            break;
        }
        else if (answer == 2) {
            cout<<"not avalable right now"<<endl;
        }
        else {
            cout<<"please select with the number 1 or 2 "<<endl;
        }

    }

    project_window();
    if (project_created == 1){
        window_manager();
    }; 

}