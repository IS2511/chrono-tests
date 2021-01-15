#include <iostream>
#include <string>
#include <fstream>
//#include <filesystem>
#include <chrono>

using namespace std;

const int32_t filesize = 1024*1024;
int32_t filecount = 32*1024;

bool flag_dryRun = false;

void usage(const string& progName) {
    cout << "Usage: " << progName << " [count] [--dry-run]" << endl;
}

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        usage(argv[0]);
        return 1;
    }

//    filesystem::path path = filesystem::path();

    int32_t argCount = argc;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (static_cast<string>(argv[i]) == "--help") {
                usage(argv[0]);
                return 0;
            } else if (static_cast<string>(argv[i]) == "--dry-run") {
                flag_dryRun = true;
                --argCount;
            }
        }
        if (argCount > 1) {
            try {
                filecount = stoi(argv[1]);
            } catch (invalid_argument& err) {
                cerr << "File count must be a number! | ";
                cerr << err.what() << endl;
                usage(argv[0]);
                return 1;
            }
        }
    }

    cout << "Starting write test ("+to_string(filecount)+" files * 1MB)..." << endl;

    char* data = new char[filesize];
    fill(data, data + filesize, 'A');

    ofstream file;

    chrono::steady_clock::time_point timer1 = chrono::steady_clock::now();

    if (!flag_dryRun) {
        for (int i = 0; i < filecount; ++i) {
            try {
                file.open("chrono-tests_"+to_string(i)+".tmp");
                file.write(data, filesize);
                file.close();
            } catch (ofstream::failure& err) {
                cerr << "Error when writing file! | ";
                cerr << err.what() << endl;
                return 1;
            }
        }
    }

    chrono::steady_clock::time_point timer2 = chrono::steady_clock::now();

    chrono::duration<double> timer = chrono::duration_cast<chrono::duration<double>>(timer2 - timer1);

    cout << "Test ended! Results:" << endl;
    cout << "Total time: " << timer.count() << " seconds." << endl;
    cout << "Average time per file: " << (timer.count()/filecount) << " seconds." << endl;

}