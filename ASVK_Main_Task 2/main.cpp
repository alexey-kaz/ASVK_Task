#include "pugixml.cpp"
#include <iostream>
#include <ctime>

using namespace std;
using namespace pugi;

class Proc {
    int proc_num; // number of processors
    int *proc_table; // table with percentages of processor's top workload
public:
    Proc ( int a, int *b ) : proc_num ( a ), proc_table ( b ) { Create (); }

    ~Proc () { Destroy (); }

    void Create () {
        proc_table = new int[proc_num]; // allocate prog_num elements of proc_table
    }

    void Destroy () {
        delete[] proc_table; // free all the elements of proc_table
    }

    int getProcNum () { return proc_num; }

    int element ( int i ) const { return proc_table[i]; }

    void addProcTable ( int i, int workload ) { // put value of i'th processor's top workload in percentages
        proc_table[i] = workload;
    }

    friend ostream &operator<< ( ostream &out, Proc &proc );
};

ostream &operator<< ( ostream &out, Proc &proc ) {
    for ( int i = 0; i < proc.getProcNum (); i++ ) { // output all proc_num elements of proc_table
        out << proc.element ( i );
        cout << " ";
    }
    cout << endl;
    return out;
}

bool isNumber ( string str ) {
    for ( int i = 0; i < str.length (); i++ )
        if ( isdigit ( str[i] ) == 0 )
            return false;
    return true; // if string consists of numbers only or not
}

bool checkProcNum ( xml_node tmp ) {
    return ( ( !tmp.child ( "Proc_Num" ) ) ||
             ( !tmp.child ( "Proc_Num" ).attribute ( "num" ) ) ||
             ( tmp.child ( "Proc_Num" ).attribute ( "num" ).next_attribute () ) );
    // if corrupt path to num or more attributes than needed
}

bool checkTopProcWorkload ( xml_node tmp ) {
    return ( ( tmp.child ( "Top_Proc_Workload" ) ) &&
             ( tmp.child ( "Top_Proc_Workload" ).first_child () ) &&
             ( tmp.child ( "Top_Proc_Workload" ).first_child ().attribute ( "percent" ) ) &&
             ( !tmp.child ( "Top_Proc_Workload" ).first_child ().attribute ( "percent" ).next_attribute () ) );
    // if corrupt path to percent or more attributes than needed
}

bool checkSiblingPercent ( xml_node tmp ) {
    return ( tmp.next_sibling () &&
             ( tmp.attribute ( "percent" ) ) &&
             ( !tmp.attribute ( "percent" ).next_attribute () ) );
    //if corrupt path to sibling's percent or more attributes than needed
}

bool procCheckInput ( char *path ) {
    xml_document file;
    file.load_file ( path ); // loading file because pugixml doesn't support it being used as an argument of a function
    xml_node tmp = file.child ( "Task" );
    if ( ( checkProcNum ( tmp ) ) ||
        ( !isNumber ( tmp.child ( "Proc_Num" ).attribute ( "num" ).as_string () ) ) )
        return false; // if corrupt path to percent or more attributes than needed || attribute "num" value isn't integer
    int proc_num = tmp.child ( "Proc_Num" ).attribute ( "num" ).as_int ();
    if ( proc_num < 0 )
        return false;
    if ( checkTopProcWorkload ( tmp ) ) { // if not corrupt path to percent and needed amount of attributes
        tmp = tmp.child ( "Top_Proc_Workload" ).first_child ();
        if ( !isNumber ( tmp.attribute ( "percent" ).as_string () ) )
            return false; // if attribute "percent" value isn't integer
        for ( int i = 1; i < proc_num; i++ ) { // first child is checked, check the rest
            if ( checkSiblingPercent ( tmp ) ) {
                tmp = tmp.next_sibling ();
                if ( !isNumber ( tmp.attribute ( "percent" ).as_string () ) )
                    return false; // if attribute "percent" value isn't integer
            } else
                return false; //if corrupt path to sibling's percent or more attributes than needed
        }
        if ( tmp.next_sibling () )
            return false; // if more processors specified than stated in proc_num
    } else
        return false; // if processors workloads can't be reached or more attributes than needed
    return true; // all good with proc info
}

Proc createProc ( char *path ) {
    xml_document file;
    file.load_file ( path ); // loading file because pugixml doesn't support it being used as an argument of a function
    if ( !procCheckInput ( path ) ) { // if processor info is faulty
        cout << "error checking processor info" << endl;
        exit ( 0 );
    }
    int proc_num = file.child ( "Task" ).child ( "Proc_Num" ).attribute ( "num" ).as_int ();
    int *proc_table = nullptr;
    Proc proc ( proc_num, proc_table ); // creating object of type Proc
    xml_node tmp = file.child ( "Task" ).child ( "Top_Proc_Workload" ).first_child ();
    for ( int i = 0; i < proc_num; i++ ) { // fill the proc_table with proc_num elements
        proc.addProcTable ( i, tmp.attribute ( "percent" ).as_int () );
        tmp = tmp.next_sibling ();
    }
    return proc; // return created object
}

class Prog {
    int prog_num; // number of programs
    int *prog_table; // // table with percentages of processor's top workload
protected:
    int **prog_pair_table; // matrix with network usage of program pairs in Kb
public:
    Prog ( int a, int *b ) : prog_num ( a ), prog_table ( b ) { Create (); }

    ~Prog () { Destroy (); }

    void Create () {
        prog_table = new int[prog_num]; // allocate prog_num elements of prog_table
        prog_pair_table = new int *[prog_num]; // allocate (prog_num * prog_num) elements of prog_pair_table
        for ( int i = 0; i < prog_num; i++ ) {
            prog_pair_table[i] = new int[prog_num];
        }
        for ( int i = 0; i < prog_num; i++ ) { // fill entire prog_pair_table with 0
            for ( int j = 0; j < prog_num; j++ ) {
                prog_pair_table[i][j] = 0;
            }
        }
    }

    void Destroy () {
        delete[] prog_table; // free all elements of prog_table
        for ( int i = 0; i < prog_num; i++ ) { // free prog_pair table rows
            delete[] prog_pair_table[i];
        }
        delete[] prog_pair_table; // free prog_pair table columns
    }

    int getProgNum () const { return prog_num; }

    int &element ( int i ) const { return prog_table[i]; } // get i'th element of prog_table

    int &elementPair ( int i, int j ) const { return prog_pair_table[i][j]; } // get i,j'th element of pro_pair_table

    void addProgTable ( int i, int workload ) {
        prog_table[i] = workload; // put value of i'th processor's top workload in percentages
    }

    void fillPairTable ( char *path ) {
        xml_document file;
        file.load_file ( path ); // loading file because pugixml doesn't support it being used as an argument of a function
        if ( checkProgPairs ( path ) ) { // if everything ok with path to pairs and pairs themselves
            xml_node tmp = file.child ( "Task" ).child ( "Prog_Pairs" ).first_child ().first_child ();
            for ( int i = 0; i < prog_num; i++ ) {  // loop to fill all network usage in matrix of program pairs
                elementPair ( i, ( tmp.attribute ( "pair" ).as_int () - 1 ) ) = tmp.attribute ( //fill the first specified Kb value in row
                        "pair" ).next_attribute ().as_int ();
                while ( tmp.next_sibling () != nullptr ) { // do the same for the rest of the row
                    tmp = tmp.next_sibling ();
                    elementPair ( i, tmp.attribute ( "pair" ).as_int () - 1 ) = tmp.attribute (
                            "pair" ).next_attribute ().as_int ();
                }
                tmp = tmp.parent ().next_sibling ().first_child (); // move to next row
            }
        } else {
            cout << "Error Prog_Pairs" << endl;
            exit ( 0 );
        }
        for (int i = 0; i < prog_num; i++) // check if matrix isn't symmetrical
            for (int j = 0; j < prog_num; j++)
                if ( elementPair (i, j) != elementPair (j, i) ) {
                    cout << elementPair (i, j) << ' ' << elementPair (j, i) << " input pairs incorrect" << endl;
                    exit (0);
                }
    }

    int maxProgWorkload () { // add all possible program workloads together
        int max = 0;
        for ( int i = 0; i < prog_num; i++ ) {
            for ( int j = 0; j < i; j++ ) {
                max += prog_pair_table[i][j];
            }
        }
        return max;
    }

    static bool checkPairArg ( xml_node tmp );

    static bool checkPair ( xml_node tmp );

    bool checkProgPairs ( char *path ) {
        xml_document file;
        file.load_file ( path );  // loading file because pugixml doesn't support it being used as an argument of a function
        xml_node tmp = file.child ( "Task" );
        if ( tmp.child ( "Prog_Pairs" ) )
            tmp = tmp.child ( "Prog_Pairs" );
        if ( tmp.first_child () )
            tmp = tmp.first_child (); // First Paired_With
        if ( !checkPair ( tmp ) )
            return false; // something's wrong with any pair's arguments or value
        for ( int i = 1; i < prog_num; i++ ) { // first "pairs" is checked, check the rest
            if ( tmp.next_sibling () ) {
                tmp = tmp.next_sibling ();
            } else
                return false; // if less programs specified than stated in prog_num
        }
        if ( tmp.next_sibling () )
            return false; // if more programs specified than stated in prog_num
        return true;
    }

    friend ostream &operator<< ( ostream &out, const Prog &prog );

};

ostream &operator<< ( ostream &out, const Prog &prog ) {
    for ( int i = 0; i < prog.getProgNum (); i++ ) // output all prog_num elements of prog_pair_table
        for ( int j = 0; j < prog.getProgNum (); j++ ) {
            out << prog.elementPair ( i, j );
            if ( ( j + 1 ) % prog.getProgNum () == 0 ) {
                cout << endl;
            } else {
                cout << " ";
            }
        }
    return out;
}

bool Prog::checkPair ( xml_node tmp ) {
    if ( tmp.first_child () )
        tmp = tmp.first_child (); //first pair
    if ( checkPairArg ( tmp ) ) {
        while ( tmp.next_sibling () ) {
            tmp = tmp.next_sibling ();
            if ( !checkPairArg ( tmp ) )
                return false; // if any of the other pairs are faulty
        }
    } else
        return false; // if the first pair is faulty
    return true;
}

bool Prog::checkPairArg ( xml_node tmp ) {
    return ( tmp.attribute ( "pair" ) ) &&
           ( tmp.attribute ( "Kb" ) ) &&
           ( isNumber ( tmp.attribute ( "pair" ).as_string () ) ) &&
           ( isNumber ( tmp.attribute ( "Kb" ).as_string () ) );
    // if both arguments exist and their value is an integer
}

//

bool checkProgNum ( xml_node tmp ) {
    return ( ( !tmp.child ( "Prog_Num" ) ) ||
             ( !tmp.child ( "Prog_Num" ).attribute ( "num" ) ) ||
             ( tmp.child ( "Prog_Num" ).attribute ( "num" ).next_attribute () ) );
    // if corrupt path to num or more attributes than needed
}

bool checkTopProgWorkload ( xml_node tmp ) {
    return ( ( tmp.child ( "Top_Prog_Workload" ) ) &&
             ( tmp.child ( "Top_Prog_Workload" ).first_child () ) &&
             ( tmp.child ( "Top_Prog_Workload" ).first_child ().attribute ( "percent" ) ) &&
             ( !tmp.child ( "Top_Prog_Workload" ).first_child ().attribute ( "percent" ).next_attribute () ) );
    // if corrupt path to percent or more attributes than needed
}

bool progCheckInput ( char *path ) {
    xml_document file;
    file.load_file ( path ); // loading file because pugixml doesn't support it being used as an argument of a function
    xml_node tmp = file.child ( "Task" );
    if ( ( checkProgNum ( tmp ) ) ||
        ( !isNumber ( tmp.child ( "Prog_Num" ).attribute ( "num" ).as_string () ) ) )
        return false; // if corrupt path to percent or more attributes than needed || attribute "num" value isn't integer
    int prog_num = tmp.child ( "Prog_Num" ).attribute ( "num" ).as_int ();
    if ( prog_num < 0 )
        return false;
    if ( checkTopProgWorkload ( tmp ) ) { // if not corrupt path to percent and needed amount of attributes
        tmp = tmp.child ( "Top_Prog_Workload" ).first_child ();
        if ( !isNumber ( tmp.attribute ( "percent" ).as_string () ) )
            return false; // if attribute "percent" value isn't integer
        for ( int i = 1; i < prog_num; i++ ) { // first child is checked, check the rest
            if ( checkSiblingPercent ( tmp ) ) {
                tmp = tmp.next_sibling ();
                if ( !isNumber ( tmp.attribute ( "percent" ).as_string () ) )
                    return false;
            } else
                return false; // if less processors specified than stated in proc_num
        }
        if ( tmp.next_sibling () )
            return false; // if more processors specified than stated in proc_num
    } else
        return false; // if processors workloads can't be reached or more attributes than needed
    return true; // all good with proc info
}

Prog createProg ( char *path ) {
    xml_document file;
    file.load_file ( path ); // loading file because pugixml doesn't support it being used as an argument of a function
    if ( !progCheckInput ( path ) ) { // if program info is faulty
        cout << "error progCheckInput" << endl;
        exit ( 0 );
    }
    int prog_num = file.child ( "Task" ).child ( "Prog_Num" ).attribute ( "num" ).as_int ();
    int prog_table[prog_num];
    Prog prog ( prog_num, prog_table ); // creating object of type Prog
    xml_node tmp = file.child ( "Task" ).child ( "Top_Prog_Workload" ).first_child ();
    for ( int i = 0; i < prog_num; i++ ) { // fill the prog_table
        prog.addProgTable ( i, tmp.attribute ( "percent" ).as_int () );
        tmp = tmp.next_sibling ();
    }
    return prog; // return created object
}

bool checkProcErr ( Proc &processors, Prog &programs, const int *x ) {
    int test_proc_workload[processors.getProcNum ()];
    for ( int l = 0; l < processors.getProcNum (); l++ ) // fill with 0
        test_proc_workload[l] = 0;
    for ( int l = 0; l < processors.getProcNum (); l++ ) {
        for ( int k = 0; k < programs.getProgNum (); k++ ) {
            if ( x[k] == l ) // find process number in vector
                test_proc_workload[l] += programs.element ( k ); // add it's value of proc_workload
        }
        if ( test_proc_workload[l] > processors.element ( l ) )
            return true; // counted workload exceeds limit
    }
    return false;
}

int getFCurr ( Proc &processors, Prog &programs, const int *x ) {
    int F_curr = 0;
    for ( int k = 0; k < programs.getProgNum (); k++ ) { // sum up network usages of programs from different processors
        for ( int l = k; l < programs.getProgNum (); l++ ) { // l = k in order not to divide by 2 later
            if ( ( x[k] != x[l] ) && ( programs.elementPair ( k, l ) > 0 ) ) { // check if processors are the same
                F_curr += programs.elementPair ( k, l );
            }
        }
    }
    return F_curr;
}

int mainAlgorithm ( Proc &processors, Prog &programs, int *x, int &F_best ) {
    srand ( (unsigned) time ( 0 ) ); // to set random base time to get more random results
    int iterations = 0;

    int x_Best[programs.getProgNum ()]; // create best vector and fill with 0
    for ( int i = 0; i < programs.getProgNum (); i++ )
        x_Best[i] = 0;

    if ( F_best == 0 )
        return 0;
    {
        for ( int i = 0; i < 1000; i++ ) {
            for ( int j = 0; j < programs.getProgNum (); j++ ) // randomly generate current vector values
                x[j] = rand () % processors.getProcNum ();
            int F_curr = getFCurr ( processors, programs, x ); // get new network usage with current vector
            if ( checkProcErr ( processors, programs, x ) != 0 ) { // processor usage of vector exceeds limit
                iterations++;
                continue;
            } else if ( F_curr < F_best ) { // new network usage is better that the best result so far
                F_best = F_curr; // remember new best network usage
                for ( int j = 0; j < programs.getProgNum (); j++ ) // remember new best vector
                    x_Best[j] = x[j];
                i = 0; // reset countdown
            }
            iterations++;
        }
    }
    for ( int j = 0; j < programs.getProgNum (); j++ ) // return best vector
        x[j] = x_Best[j];
    return iterations;
}

int main ( int argc, char ** argv ) {

    xml_document file;
    char * path;
    if (argc >= 2) {
        path = argv[1]; // remember the path to test
        xml_parse_result result = file.load_file ( path );
        if ( result && ( file.child ( "Task" ) ) ) { // check if the file loads correctly & first node is Task

            Proc processors = createProc ( path ); // get processors info

            Prog programs = createProg ( path ); // get programs info
            programs.fillPairTable ( path ); // get all program pairs values

            int f_Best = programs.maxProgWorkload (); // get maximum network usage
            int f_Best_First = f_Best; // remember maximum network usage as the first result
            int iterations = 0;

            int x[programs.getProgNum ()]; // create current vector and fill with 0
            for ( int i = 0; i < programs.getProgNum (); i++ )
                x[i] = 0;

            ///

            iterations = mainAlgorithm ( processors, programs, x, f_Best );

            if ( ( f_Best_First <= f_Best ) && ( f_Best != 0 ) ) { // if no better result is found
                for ( int i = 0; i < programs.getProgNum (); i++ ) // turn all vector's values to -1
                    x[i] = -1;
                cout << "failure" << endl << iterations << endl;
            } else {
                cout << "success" << endl << iterations << endl;
                for ( int i = 0; i < programs.getProgNum (); i++ ) // print all best vector's values
                    cout << x[i] << ' ';
                cout << endl << f_Best << endl;
            }
        } else {
            cout << "xml file path error" << endl;
        }
    } else {
        cout << "no input path specified in argv" << endl;
    }
    return 0;
}