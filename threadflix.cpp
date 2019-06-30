#include <iostream>
#include <fstream>
#include <windows.h>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;
int watchTime=0;
mutex mtx;

void play(string title, string &titleScreen) {
    cout<<"Now Playing "<<title<<endl;
    titleScreen=title;   //save the title of current video
    string line;
    ifstream myfile (title);
    if (myfile.is_open())
    {
        while (getline (myfile,line)&&line!="THE END"&&titleScreen!="") //checks if end of file; THE END; and not stopped
        {
            //wait for 5 seconds before reading next line
            mtx.lock();     //critical section
            watchTime=watchTime+5;    //add to watchTime every second
            mtx.unlock();
            Sleep(5000);    //count 1 sec
        }
        myfile.close();
    }
    else cout << "Unable to play video" << endl;
    titleScreen=""; //mark end of video
}
//check of available screens and return index of the screen
int checkAvailable (string titleScreen[], int maxScreens) {
    int index=maxScreens;
    for (int i=0; i<maxScreens; i++) {
        if (titleScreen[i]==""){
            index=i;
        }
    }
    return index;
}

int findShow (string title, string titleScreen[], int maxScreens) {
    int index=maxScreens;
    for (int i=0; i<maxScreens; i++) {
        if (titleScreen[i]==title){
            index=i;
        }
    }
    return index;
}

void stopShow (string &titleScreen) {
    cout<<"Stopped "<<titleScreen<<endl;
    titleScreen="";  //mark screen as open
}

main () {
    string command, cmd[10]; //input of the user
    int maxScreens=3;   //maximum number of screens
    thread screen[maxScreens];  //screens
    string titleScreen[maxScreens]; //saves the title if the video per screen;
    cout<<"************** THREATFLIX **************"<<endl;
    for(;;){
        getline(cin,command);   //get the command
        stringstream line(command);
        string item;
        int i=0;
        while (getline(line, item, ' ')) {  //separate per word
            cmd[i]=item;
            i++;
        }
    //---------------------- Functions -----------------//
        //if watch_time
        if (cmd[0]=="watch_time") {
            cout<<"total watch time: "<<watchTime<<" seconds"<<endl;
        }
        //if start_watching
        else if (cmd[0]=="start_watching"){
            //code to start video in available screen
            //if there are available screens
            if (checkAvailable(titleScreen, maxScreens)<maxScreens){    //check if there is available screen
                int index = checkAvailable(titleScreen, maxScreens);    //get index of open screen
                screen[index]=thread{&play, cmd[1]+".txt", ref(titleScreen[index])};   //create a screen
                screen[index].detach(); //run the thread independently
            }
            else {
                cout<<"No available screens"<<endl;
            }
        }
        //if stop_watching
        else if (cmd[0]=="stop_watching"){
            //code to stop video
            if (findShow(cmd[1]+".txt", titleScreen, maxScreens)<maxScreens){  //check if show is found
                int index = findShow(cmd[1]+".txt", titleScreen, maxScreens);  //get the index of screen where title is
                screen[index]=thread{&stopShow, ref(titleScreen[index])};   //mark screen as open
                screen[index].detach(); //run the thread independently
            }
            else {
                cout<<"Movie not found"<<endl;
            }
        }
        //if check_screens
        else if (cmd[0]=="check_screens"){
            //check if screen is running
            for (int i=0;i<maxScreens;i++){
                cout<<"screen "<<i<<": "<<titleScreen[i]<<endl;
            }
        }
        else {
            cout<<"wrong syntax"<<endl;
        }
        cout<<endl<<endl;
    }
}
