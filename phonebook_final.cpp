#include <bits/stdc++.h>
using namespace std;

#define DEFAULT_SAVE "default_save.csv"
#define DEFAULT_LOAD "default_load.csv"
#define SORTED_SAVE "sorted_save.csv"

class Entry{
    public:
        string number;
        string engname;
        string email;
        string gender;
        bool favorited;
        string description;

        Entry(){
            number = "00000000";
            engname = "N/A";
            email = "N/A";
            gender = '-';
            favorited  = false;
            description = "- enter description -";
        }
        Entry(string a, string b, string c, bool d, string e, string f){
            number = a;
            engname = b;
            email = c;
            favorited = d;
            gender = e;
            description = f;
        }
        int ModifyEntries(){
            string a,b,c,e,f;
            cout << setw(20) << "Phone Number: ";
            getline(cin, a);
            // if (a=="\0"){
            //     cout << "Error! Phone number is key field. Please input phone number." << endl << endl;
            //     return 0;
            // } else 
            if (a!= "\0" && a.length() != 8){
                cout << "Error! Enter valid 8-digit HK phone number." << endl << endl;
                return 0;
            }
            number = (a!="\0")?a:number;
            cout << setw(20) << "Full Name: ";
            getline(cin, b);
            engname = (b!="\0")?b:engname;
            cout << setw(20) << "Email Address: ";
            getline(cin, c);
            if(c != "\0" && c.find('@') == string::npos){
               cout << "Error! email must contain '@'!" << endl << endl; 
               return 0;
            } else email = (c!="\0")?c:email;
            cout << setw(20) << "Gender (M/F): ";
            getline(cin, e);
            gender = (e!="\0")?e:gender;
            cout << setw(20) << "Description: ";
            getline(cin, f);
            // f= f.erase(remove(f.begin(), f.end(), ','), f.end)
            description = (f!="\0")?f:description;
            return 1; 
        }
        void PrintEntry(){
            cout<< setw(20) << "Phone Number: " << number << endl
                << setw(20) << "Full Name: " << engname << endl
                << setw(20) << "Email address: " << email << endl 
                << setw(20) << "Gender: " << gender << endl
                << setw(20) << "Description: " << description << endl 
                << setw(20) << (favorited?"Favorited!":"") << endl;

        }
        void Print(){
            cout << setw(20) << number << setw(20) << engname << setw(20) << email << setw(10) << gender <<setw(20) << (favorited?"Favorited!":"") << endl;
        }
        void FoutEntry(fstream& fout){
            fout << number << "," << engname << "," << email << "," << favorited << "," << gender << "," << description <<"\n";
        }
        // bool operator < (const Entry& e) const{
        //     return (number < e.number);
        // }
};

void PrintSelection(vector<Entry> v){
    cout<<"    "<< setw(20) << "Phone Number" << setw(20) << "Full Name"
    << setw(20) << "Email address" << setw(10) << "Gender" << endl;
    int index = 0;
    for(Entry E: v){
        cout << setw(3) << index++ << " ";
        E.Print();
    }
    cout << endl;
    
}

void create(vector<Entry> v, string file){
    fstream fout;
    fout.open(file, fstream::out | fstream::trunc);
    for (Entry E: v){
        E.FoutEntry(fout);
    }
}

void read(vector<Entry>& v, string file){
    fstream fin;
    fin.open(file, fstream::in);
    vector<string> row;
    string line, word; 
    if(fin.is_open()){
		v.clear();
        while(getline(fin, line)){
            row.clear();
            stringstream str(line);
            while (getline(str, word, ',')){
                row.push_back(word);
            }
            // cout << row[0] << row[1] << row[2] << endl;
            v.push_back(Entry(row[0], row[1], row[2], (row[3]=="1"?true:false), row[4], row[5]));
            // cout<<"hi"<<endl; row[0], row[1], row[2]
        }
        fin.close();
    } else cout << "Error occurred in opening file!" << endl;
}

int Sort(vector<Entry>& v, string criteria, string order){
    int option = 0;
    if(criteria == "name"){
        if(order == "asc") option = 3;
        else if (order == "desc") option = 4;
        else {
            cout << "Enter valid ordering method! (asc/desc)"<< endl; 
            return 0;
        }
    } else if (criteria == "number"){
        if(order == "asc") option = 1;
        else if (order == "desc") option = 2;
        else {
            cout << "Enter valid ordering method! (asc/desc)" << endl;
            return 0;
        }
    } else{
        cout << "Enter valid sorting criteria! (name/number)" << endl;
        return 0;
    } 
    switch (option){
        case 1:
            sort(v.begin(), v.end(), [](Entry a, Entry b){return a.number<b.number;});
            break;
        case 2:
            sort(v.begin(), v.end(), [](Entry a, Entry b){return a.number>b.number;});
            break;
        case 3:
            sort(v.begin(), v.end(), [](Entry a, Entry b){return a.engname<b.engname;});
            break;
        case 4:
            sort(v.begin(), v.end(), [](Entry a, Entry b){return a.engname>b.engname;});
            break;
        default:
            cout<<"Operation failed!" << endl;
    }
    return 1;
}

bool Match(Entry E, string criteria){
    bool a,b,c,f = false;
    a = E.engname.find(criteria) != string::npos;
    b = E.number.find(criteria) != string::npos;
    c = E.email.find(criteria) != string::npos;
    f = E.description.find(criteria) != string::npos;
    return a||b||c||f;
}

void Search(vector<Entry> v, string criteria){
    vector<Entry> aux;
    auto it = v.begin();
    while((it = find_if(it, v.end(), [&](Entry const &E){return Match(E, criteria);}))!= v.end()) {
        aux.push_back(v[it-v.begin()]);
        it++;
    }
    PrintSelection(aux);

    int input_length, aux_length = aux.size();;
    string line, word, temp, file;
    vector<string> input = {};

    do{
        input.clear();
        cout << "Currently in Search Menu" << endl << "Choose a command from the following: "<< endl
            << "view, save, sort, exit" << endl;

        getline(cin, line);
        stringstream str(line);
        while(getline(str, word, ' ')){
            input.push_back(word);
        }
        
        input_length = input.size();
        if (input[0] == "view"){
            if (input_length>1) {
                int index = stoi(input[1]);
                if(index < aux_length) aux[index].PrintEntry();
                else cout << "Index out of bounds!";      
            } else cout << "Enter index number after command!" << endl;  
        } else if(input[0] == "save"){
            temp = "";
            if(input_length>1){
                temp = input[1];
            }
            file = (temp!="\0")?temp:SORTED_SAVE;
            create(aux, file);
        } else if (input[0] == "sort"){
            if(input_length>1){
                Sort(aux, input[1], input[2]);
                PrintSelection(aux);
            } else cout<< "Enter criteria (name/number) and sorting order (asc/desc) after command!" << endl;
        }
    }while(input[0] != "exit");

}

int main(){
    system("cls");
    vector<Entry> phonebook;
    vector<string> input = {};
    string line, word, temp, file;
    int index;
    int input_length, phonebook_length;
    cout << "Welcome to Lonz's phonebook!" << endl << endl;
    do{
        input.clear();
        line, word = "";
        cout<< "Choose a command from the following: " << endl
            << "enter, view, viewall, exit, modify, save, retrieve, sort, delete, search, favorite, help" << endl;
        getline(cin, line);
        stringstream str(line);
        while(getline(str, word, ' ')){
            input.push_back(word);
        }
        // for(string w: input) cout << w << endl;
        phonebook_length = phonebook.size();
        input_length = input.size();
        // cout << phonebook_length << "  " << input_length << endl;
        if(input[0] == "enter"){
            phonebook.push_back(Entry());
            if (!phonebook.back().ModifyEntries()) phonebook.pop_back(); // if push is unsuccessful
        } else if (input[0] == "viewall"){
            PrintSelection(phonebook);
        } else if (input[0] == "exit"){
            cout << endl << "Thanks for using Lonz's phonebook!" << endl;
        } else if (input[0] == "view"){
            if (input_length>1) {
                index = stoi(input[1]);
                if(index < phonebook_length) phonebook[index].PrintEntry();
                else cout << "Index out of bounds!";      
            } else cout << "Enter index number after command!" << endl;  
        } else if (input[0] == "modify"){
            if (input_length>1){
                index = stoi(input[1]);
                if(index < phonebook_length) phonebook[index].ModifyEntries();
                else cout << "Index out of bounds!";     
            } else cout << "Enter index number after command!" << endl;   
        } else if (input[0] == "save"){
            temp = "";
            if(input_length>1) temp = input[1];
            file = (temp!="\0")?temp:DEFAULT_SAVE;
            create(phonebook, file);
        } else if (input[0] == "retrieve"){
            temp = "";
            if(input_length>1){
                temp = input[1];
            }
            file = (temp!="\0")?temp:DEFAULT_LOAD;
            read(phonebook, file);
        } else if (input[0] == "sort"){
            if(input_length>1){
                Sort(phonebook, input[1], input[2]);
                PrintSelection(phonebook);
            } else cout << "Enter criteria (name/number) and sorting order (asc/desc) after command!" << endl;
        } else if (input[0] == "delete"){
            if (input_length > 1){
                index = stoi(input[1]);
                if(index < phonebook_length) phonebook.erase(phonebook.begin()+index);
                else cout << "Index out of bounds! "<< endl;
            } else cout << "Enter index number after command!" << endl;
        } else if (input[0] == "search"){
            if(input_length>1){
                Search(phonebook, input[1]);
            } else cout << "Enter search criteria after command!" << endl;
        } else if (input[0] == "favorite"){
            if(input_length>1){
                index = stoi(input[1]);
                if(index < phonebook_length) phonebook[index].favorited^=true;
                else cout << "Index out of bounds!" << endl;
            }else cout << "Enter index number after command!" << endl;

        } else if (input[0] == "help"){
            cout << "Help for commands!" <<endl;
            cout << "enter : add entries to the phonebook" << endl
                 << "view : view a particular entry" << endl
                 << "viewall : display all phonebook entries" << endl
                 << "exit : leave the program (remember to save!)" << endl
                 << "modify [index]: change the information of an entry" << endl
                 << "save [filename] : saves the phonebook to a csv file (default default_save.csv)" << endl
                 << "retrieve [filename] : loads the phonebook from a csv file (default default_load.csv)" << endl
                 << "sort [name/number] [asc/desc]: sorts the phonebook according to a certain criteria" << endl
                 << "search [criteria]: searches for entries that match the criteria" << endl
                 << "delete [index]: deletes an entry" << endl
                 << "favorite [index]: favorites or unfavorites an entry" << endl
                 << "help : shows this help screen" << endl;
        }
        else {
            cout << "Invalid input!" << endl;
        }
        
    }while(input[0] != "exit");
    return 0;
    
}