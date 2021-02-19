#include "pugixml.cpp"
#include <iostream>
#include <random>

using namespace std;
using namespace pugi;

int RandomRangeExcept (int range, int except) { // create a random number in range, except for the given
    int number = except;
    while (number == except) {
        number = rand() % range;
    }
    return number;
}

bool proc_percent_table_check (const int * proc_percent_table, int proc_num){
    int proc_percents[3] = {60, 80, 100}; //suggested top processor workload values
    int check_proc_percents[3] = {0}; // keep track of used values
    for (int i = 0; i < proc_num; i++) {
        if (proc_percent_table[i] == proc_percents[0])
            check_proc_percents[0] = 1;
        if (proc_percent_table[i] == proc_percents[1])
            check_proc_percents[1] = 1;
        if (proc_percent_table[i] == proc_percents[2])
            check_proc_percents[2] = 1;
    }
    return (check_proc_percents[0] && check_proc_percents[1] && check_proc_percents[2]);
    // if all suggested top processor workload values are used
}

bool prog_percent_table_check (const int * prog_percent_table, int proc_num){
    int prog_percents[3] = {5, 10, 20}; // suggested top program workload values
    int check_prog_percents[3] = {0}; // keep track of used values
    for (int i = 0; i < proc_num; i++) {
        if (prog_percent_table[i] == prog_percents[0])
            check_prog_percents[0] = 1;
        if (prog_percent_table[i] == prog_percents[1])
            check_prog_percents[1] = 1;
        if (prog_percent_table[i] == prog_percents[2])
            check_prog_percents[2] = 1;
    }
    return (check_prog_percents[0] && check_prog_percents[1] && check_prog_percents[2]);
    // if all suggested top program workload values are used
}

bool prog_percent_avg_check (const int * prog_percent_table, int prog_num, int proc_num){
    int avg = 0;
    for (int i = 0; i < prog_num; i++) {
        avg+=prog_percent_table[i]; // add up all top program workloads
    }
    return ((avg/proc_num) >= 50);
}

int main(int argc, char ** argv)
{
    if (argc == 4) {
        srand ( time ( 0 ) );
        xml_document file;

        int proc_percents[3] = {60, 80, 100};
        int prog_percents[3] = {5, 10, 20};
        int pair_kb[5] = {-1, 0, 10, 50,
                          100}; // "-1" means programs aren't paired, "0" means that network usage of the pair = 0 Kb/s

        int proc_num = stoi(argv[1]);

        int prog_num = stoi(argv[2]);

        xml_node node_task = file.append_child ( "Task" ); // create root element

        xml_node node_proc_num = node_task.append_child ( "Proc_Num" ); // first sub-element
        node_proc_num.append_attribute ( "num" ) = proc_num; // give it an attribute = number of processors

        xml_node node_proc_workload = node_task.append_child ( "Top_Proc_Workload" ); // second sub-element
        xml_node node_proc_percent; // start of array of processors

        int proc_percent_table[proc_num];
        for ( int i = 0; i < proc_num; i++ ) {
            proc_percent_table[i] = 0;
        }

        while ( !proc_percent_table_check ( proc_percent_table,
                                            proc_num ) ) { // while not all suggested top processor workload values are used
            for ( int i = 0; i < proc_num; i++ ) // fill the proc_percent_table with processor workload values
                proc_percent_table[i] = proc_percents[rand () % 3]; // choose random one of the three
        }

        for ( int i = 0; i < proc_num; i++ ) { // add proc_num new sub-elements with top processor workloads
            node_proc_percent = node_proc_workload.append_child ( "Proc" ); // append sub-element proc to the array
            node_proc_percent.append_attribute (
                    "percent" ) = proc_percent_table[i]; // give its attribute a value from proc_percent_table
        }

        int prog_percent_table[prog_num];
        for ( int i = 0; i < prog_num; i++ ) {
            prog_percent_table[i] = 0;
        }

        random_device rd;
        mt19937 gen ( rd () );
        discrete_distribution<> d ( {95, 3, 2} ); // give probabilities to top program workload values usage

        while ( !prog_percent_table_check ( prog_percent_table, prog_num ) || // if all values are used
                !( prog_percent_avg_check ( prog_percent_table, prog_num,
                                            proc_num ) ) ) { // if in average together they occupy >50% of all processors
            for ( int i = 0; i < prog_num; i++ ) // fill the prog_percent_table with program workload values
                prog_percent_table[i] = prog_percents[d (
                        gen )]; // choose random one of the three (with various chances)
        }

        xml_node node_prog_num = node_task.append_child ( "Prog_Num" ); // third sub-element
        node_prog_num.append_attribute ( "num" ) = prog_num; // give it an attribute = number of programs

        xml_node node_prog_workload = node_task.append_child ( "Top_Prog_Workload" );  // forth sub-element
        xml_node node_prog_percent; // start of array of programs

        for ( int i = 0; i < prog_num; i++ ) { // add prog_num new sub-elements with top program workloads
            node_prog_percent = node_prog_workload.append_child ( "Prog" ); // append sub-element prog to the array
            node_prog_percent.append_attribute (
                    "percent" ) = prog_percent_table[i]; // give its attribute a value from prog_percent_table
        }

        xml_node node_prog_pairs = node_task.append_child ( "Prog_Pairs" ); // fifth sub-element
        xml_node node_paired_with; // start of array of programs
        xml_node node_pair; // start of array of program's each program is paired with

        int pairs_table[prog_num][prog_num];
        for ( int i = 0; i < prog_num; i++ ) { // create matrix with Kb values in program pairs
            for ( int j = 0; j <= prog_num; j++ )
                pairs_table[i][j] = -1; // fill it all with -1 => no pairs
            for ( int j = 0; j < prog_num; j++ ) {
                int k = RandomRangeExcept ( prog_num, i ); // random pair program number except for itself
                pairs_table[i][k] = pair_kb[rand () % 5]; // choose random one of the five
            }
        }

        for ( int i = 0; i < prog_num; i++ )
            for ( int j = 0; j <= i; j++ )
                pairs_table[i][j] = pairs_table[j][i]; // make the matrix diagonal

        for ( int i = 0; i < prog_num; i++ ) {
            node_paired_with = node_prog_pairs.append_child ( "Paired_With" ); // for each program
            for ( int j = 0; j < prog_num; j++ ) {
                if ( pairs_table[i][j] > -1 ) { // if connection between programs exits
                    node_pair = node_paired_with.append_child ( "Pair" ); // for each existing connection
                    node_pair.append_attribute ( "pair" ) = j + 1; // j starts with 0, make it start with 1
                    node_pair.append_attribute ( "Kb" ) = pairs_table[i][j]; // fill attribute with value from matrix
                }
            }
        }

        string path = argv[3];
        bool saveSucceeded = file.save_file ( path.c_str () );
        if ( saveSucceeded )
            cout << "save complete to " << path << endl;
        else
            cout << "error saving the file" << endl;
    } else {
        cout << "wrong number of arguments" << endl;
    }
    return 0;
}
