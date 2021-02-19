#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main (int argc, char ** argv) {
    if (argc == 4) {
        string time_extra = "/usr/bin/time -f %e -o file.txt ./ASVK_Extra_Task "; // execution time of multithread program command (incomplete)
		string time_main = "/usr/bin/time -f %e -o file.txt ./ASVK_Main_Task "; // execution time of main program command (incomplete)
        string dev_null = " 1 > /dev/null"; // to remove any output other than time
        ofstream out ( "out.txt" );
		time_main.append ( argv[1] ); // /usr/bin/time -f %e -o file.txt ./ASVK_Main_Task (test.xml = argv[1])
        time_extra.append ( argv[1] ); // /usr/bin/time -f %e -o file.txt ./ASVK_Extra_Task (test.xml = argv[1])
        int threads_num = stoi ( argv[2] ); // how many maximum threads to test
        int tests_num = stoi ( argv[3] ); // how many times each program is tested
		time_main.append(dev_null);
		double avg = 0;
		for ( int j = 1; j < tests_num + 1; j++ ) { // for each test
                double tmp_avg;
                system ( time_main.c_str () ); // write result of "time" into file.txt
                ifstream file ( "file.txt" ); // open to read file.txt
                file >> tmp_avg; // read a float from it
                avg += tmp_avg;
                file.close (); // close file
        }
		out << avg / tests_num << endl;
        for ( int i = 1; i < threads_num + 1; i++ ) { // for each thread
			avg = 0;
            string time_extra_num = time_extra + ' ' + to_string (i); // /usr/bin/time -f %e -o file.txt ./ASVK_Extra_Task (test.xml) (amount of threads)
            time_extra_num.append ( dev_null ); // /usr/bin/time -f %e -o file.txt ./ASVK_Extra_Task (test.xml) (amount of threads) 1 > /dev/null
            for ( int j = 1; j < tests_num + 1; j++ ) { // for each test
                double tmp_avg;
                system ( time_extra_num.c_str () ); // write result of "time" into file.txt
                ifstream file ( "file.txt" ); // open to read file.txt
                file >> tmp_avg; // read a float from it
                avg += tmp_avg;
                file.close (); // close file
            }
            out << avg / tests_num << endl; // real average = avg divided by how many tests were run
        }
        out.close();
        ofstream percentfile ( "percent.txt" );
		percentfile << 0 << endl; // время выполнения основного алгоритма берем за 100%, 100 - 100 = 0
        float percent_1 = 0;
        ifstream get_out ( "out.txt" ); // open to read out.txt
        get_out >> percent_1; // get main_algotithm (same as thread algorithm with 1 thread) execution time
        for (int i = 1; i < threads_num + 1; i++) {
            float percent_2 = 0;
            get_out >> percent_2;
            // percentfile << i << " threaded algorithm takes " << percent_2 << " seconds to run, which is " << 100 - percent_2 / percent_1 * 100 << "% less time than " << percent_1 << " seconds of the main (1 threaded) algorithm" << endl << endl; // для красивой записи, следующая строка - просто записывает процент без лишних слов
			percentfile << 100 - percent_2 / percent_1 * 100 << endl;
        }
        get_out.close(); // close out.txt
        percentfile.close(); // close percent.txt
        system ( "rm file.txt" ); // delete file.txt
        system ( "python3 graph.py percent.txt" ); // show diagram using data from out.txt
    } else {
        cout << "not enough arguments" << endl;
    }
    return 0;
}
