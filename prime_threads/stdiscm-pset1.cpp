#include <iostream>
#include <chrono>
#include <mutex>
#include <queue>
#include <string>
#include "config_reader.h"
#include "A1.h"
#include "A2.h"
#include "B1.h"
#include "B2.h"

using namespace std;

int main() {
    // Reads the config file as soon as program runs
    auto config = read_config("config.txt");
    int X = config["X"];
    int y = config["y"];
    mutex mtx;
    queue<string> results;

    int choice;
    // Program Loop
    do {
        // Command Loop
        do {
			cout << "Number of threads: " << X << endl;
			cout << "Range of numbers: " << y << endl << endl;

            cout << "[1] Immediate Printing, Divided Search Range" << endl;
            cout << "[2] Deferred Printing, Divided Search Range" << endl;
            cout << "[3] Immediate Printing, Linear Search" << endl;
            cout << "[4] Deferred Printing, Linear Search" << endl;
			cout << "[0] Exit" << endl;
            cout << "Select choice: ";
            cin >> choice;

            if (choice < 0 || choice > 4) {
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice < 0 || choice > 4);

        //Print Start Time only if command is not exit
		if (choice >= 1 && choice <= 4) {
            auto start = chrono::system_clock::to_time_t(chrono::system_clock::now());
            char start_time_str[26];
            ctime_s(start_time_str, sizeof(start_time_str), &start);
            cout << "Start Time: " << start_time_str << endl;
		}
        
        // Run command
        switch (choice) {
        case 1:
            cout << "Running [1] Immediate Printing, Divided Search Range..." << endl;
            taskA1(X, y, mtx);
            break;
        case 2:
            cout << "Running [2] Deferred Printing, Divided Search Range..." << endl;
            taskA2(X, y, mtx, results);
            break;
        case 3:
            cout << "Running [3] Immediate Printing, Linear Search..." << endl;
            taskB1(X, y, mtx);
            break;
        case 4:
            cout << "Running [4] Deferred Printing, Linear Search..." << endl;
            taskB2(X, y, mtx, results);
            break;
        default:
            break;
        }

		//Print End Time only if command is not exit
		if (choice >= 1 && choice <= 4) {
			auto end = chrono::system_clock::to_time_t(chrono::system_clock::now());
			char end_time_str[26];
			ctime_s(end_time_str, sizeof(end_time_str), &end);
			cout << "End Time: " << end_time_str << endl;
		}

		// Ask user if they want to run another command
		if (choice != 0) {
            // Retry Loop
            do {
                cout << "Do you wish to run another command?" << endl;
                cout << "[1] Yes" << endl;
                cout << "[0] No" << endl;
                cout << "Select choice: ";
                cin >> choice;

				if (choice < 0 || choice > 1) {
					cout << "Invalid choice. Please try again." << endl;
				}
			} while (choice < 0 || choice > 1);

            if (choice == 1) {
                system("cls");
            }
		}
    } while (choice != 0);

	cout << "Exiting program..." << endl;

    return 0;
}
