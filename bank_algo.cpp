
//===========================================================
// bank_algo.cpp
//-----------------------------------------------------------
//  Created by William McCarthy on 21 Mar 2022.
//===========================================================
//
#include <iostream>
#include <fstream>
#include "vector_.h"
#include "bank.h" 
#include "customer.h"
#include "utils.h"
//  change to use #include <semaphore.h>  if not on macOS
#include <semaphore.h> 
#include <string.h>
#include <stdio.h>
#include <pthread.h>

    // change to use sem_t sem;    if not on macOS
sem_t sem;


void* runner(void* param) {           // thread runner
    Customer* c = (Customer*)param;

    sem_wait(&sem);
    Utils::print_locked("%s%d%s", "<<< Customer thread p#", c->get_id(), " started... >>>\n");
    sem_post(&sem);

    Bank* b = c->get_bank();
                           // TODO...
    int counter = 50;      // for debugging purposes, just stop after 50 iterations
    //                        remove this when algorithm fully working

    while (!c->needs_met() && counter-- > 0) {
        vector_<int> req = c->create_req();
        int idx = c->get_id();
        bool approved = b->req_approved(idx, req);

        if (approved) {
            sem_wait(&sem);
            Utils::print_locked("P%d requesting: %s    ............. request APPROVED!\n", 
                          idx, (req.as_string()).c_str());
            c->alloc_req(req);
            b->withdraw_resources(req);
            b->show();
            sem_post(&sem);

            if (c->needs_met()) {
                sem_wait(&sem);
                b->deposit_resources(c->get_max());
                c->release_all_resources();
                b->show();
                sem_post(&sem);
            }
        }
        else {  
            sem_wait(&sem);
            Utils::print_locked("P%d requesting: %s     --- Request DENIED\n", idx, (req.as_string()).c_str());
            sem_post(&sem);
        }
    }
    sem_wait(&sem);
    Utils::print_locked(">>>>>>>>>>>>>>> Customer thread p#%d shutting down... <<<<<<<<<<<<<<<<<\n\n", c->get_id());
    b->show();
    sem_post(&sem);

    pthread_exit(0);
}

void run_simulation(Bank* bank) {
    Utils::print_locked("\nBanker's algorithm simulation beginning...\n--------------------------------------------\n");
    bank->show();

    // start threads
    pthread_attr_t attr;
    pthread_attr_init(&attr);    /* get the default attributes */

    vector_<Customer*> customers = bank->get_customers();
    for (Customer* c : customers) {
        pthread_create(c->get_threadid(), &attr, runner, c);
    }

    // join threads
    for (Customer* c : customers) {
        pthread_join(*c->get_threadid(), NULL);
    }
    Utils::print_locked("\nBanker's algorithm simulation completed...\n\n");
}

void process_line(char* buf, vector_<int>& values) {    // gets values from one line
  int i = 0;

  Utils::strstrip(buf);  // strip punctuation
  values.clear();

  char* p = strtok(buf, " ");   // split into tokens
  while (p != nullptr) {
    int val = atoi(p);
    values.push_back(val);      // convert to int, add to int array
    p = strtok(nullptr, " ");
    ++i;
  }
}

void process_filestream(std::ifstream& ifs, Bank*& bank) {    // extracts avail for Bank, customers' alloc and max
  char buf[BUFSIZ];
  vector_<int> res;  // resources

  bool finished = false;
  bool first_line = true;
  int idx = 0;
  while (!finished) {
    ifs.getline(buf, BUFSIZ, '\n');
    std::cout << "buf is: " << buf << "\n";

    if (strlen(buf) == 0) { finished = true;  break; }

    process_line(buf, res);
    if (first_line) {    // first line has bank's resources
      bank = new Bank(res);
      first_line = false;
    } else {
      vector_<int> alloc;
      vector_<int> max;
      size_t size = bank->get_avail().size();
      for (size_t i = 0; i < size; ++i) {
        alloc.add(res[i]);            // e.g., for size = 2,  0, 1
        max.add(res[i + size]);       // ditto,               2, 3
      }
      Customer* c = new Customer(idx++, alloc, max, bank);
      bank->add_customer(c);
    }
  }
  ifs.close();
}

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./bank_algo filename\n\n";
        exit(-2);
    }

    std::string filename = argv[1];
    std::ifstream ifs(filename);

    if (!ifs.is_open()) {
      std::cerr << "\n\nWarning, could not open file '" << filename << "'\n\n";
      exit(-1);
    }

    // now we have the filestream open
    Bank* bank = nullptr;
    sem_init(&sem, 0, 1);

    process_filestream(ifs, bank);

    run_simulation(bank);
     
    sem_destroy(&sem);                  // OR use your version of sem_release
    
    std::cout << "\n\t...done.  (all processes should show 0 resources left when finished)\n";
    std::cout << "\t\t... Test with all input files provided...\n";
    std::cout << "\t\t... data/bankers_tinier.txt, data/bankers_tiny.txt, ...\n\n";
    return 0;
}

/*
buf is: 7, 7, 8
buf is: 3, 2, 1,    3, 3, 3
buf is: 1, 2, 1,    2, 2, 2
buf is: 1, 1, 1,    1, 3, 1
buf is: 0, 0, 1,    1, 1, 1
buf is:

Banker's algorithm simulation beginning...
--------------------------------------------

+-----------------------------------------
|     BANK   avail: [7  7  8]
+-----------------------------------------
| P#  0     3  2  1    3  3  3    0  1  2 
| P#  1     1  2  1    2  2  2    1  0  1
| P#  2     1  1  1    1  3  1    0  2  0
| P#  3     0  0  1    1  1  1    1  1  0
+-----------------------------------------
<<< Customer thread p#0 started... >>>
<<< Customer thread p#1 started... >>>
<<< Customer thread p#2 started... >>>
<<< Customer thread p#3 started... >>>
P0 requesting : 0  2  2   --- Request DENIED
P1 requesting : 3  0  3   --- Request DENIED
P2 requesting : 0  2  0   .......... request APPROVED!

+-----------------------------------------
|     BANK   avail: [7  5  8]
+-----------------------------------------
| P#  0     3  2  1    3  3  3    0  1  2 
| P#  1     1  2  1    2  2  2    1  0  1
| P#  2     1  3  1    1  3  1    0  0  0
| P#  3     0  0  1    1  1  1    1  1  0
+-----------------------------------------

+-----------------------------------------
|     BANK   avail: [10 10 10]
+-----------------------------------------
| P#  0     0  0  0    0  0  0    0  0  0 
| P#  1     2  2  2    2  2  2    0  0  0
| P#  2     0  0  0    0  0  0    0  0  0
| P#  3     0  0  0    0  0  0    0  0  0
+-----------------------------------------
Customer p#1 has released all resources and is shutting down

+-----------------------------------------
|     BANK   avail: [12 12 12]
+-----------------------------------------
| P#  0     0  0  0    0  0  0    0  0  0 
| P#  1     0  0  0    0  0  0    0  0  0
| P#  2     0  0  0    0  0  0    0  0  0
| P#  3     0  0  0    0  0  0    0  0  0
+-----------------------------------------
>>>>>>>>>>>>>>> Customer thread p#1 shutting down... >>>>>>>>>>>>>>>

+-----------------------------------------
|     BANK   avail: [12 12 12]
+-----------------------------------------
| P#  0     0  0  0    0  0  0    0  0  0 
| P#  1     0  0  0    0  0  0    0  0  0
| P#  2     0  0  0    0  0  0    0  0  0
| P#  3     0  0  0    0  0  0    0  0  0
+-----------------------------------------

Banker's algorithm simulation completed...

    ...done


*/