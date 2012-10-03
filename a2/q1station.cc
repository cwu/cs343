#include "q1station.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
#include <cstdlib>

int Station::numPendingRequests = 0;

Station::Station(unsigned int id) : id_(id), next_(NULL) {}

void Station::setup( Station *nexthop ) {
    next_ = nexthop;
}

void Station::sendreq( unsigned int round, unsigned int dst, unsigned int prio ) {
    SendRequest request = { round, dst, prio };
    sendRequests_.push_back(request);
    Station::numPendingRequests++;
}

void Station::start() {
    Frame frame = { Frame::Token, 0, 0, 0 };
    data(frame);
}

void Station::data( Frame frame ) {
    this->frame = frame;
    resume();
}

bool Station::hasPendingRequest( unsigned int round ) {
    return !sendRequests_.empty() && round >= sendRequests_.front().round;
}

bool Station::keepPassingFrame() {
    return id_ != 0 || numPendingRequests > 0 || frame.type != Frame::Token;
}

void Station::main() {
    unsigned int round = 0;
    unsigned int savedPrio = 0;
    while (keepPassingFrame()) {
        switch (frame.type) {
            case Frame::Token:
                // See if we have a request to send and send it if the priority
                // is high enough.
                if (hasPendingRequest(round)) {
                    SendRequest request = sendRequests_.front();
                    if (frame.prio <= request.prio) {
                        frame.type = Frame::Data;
                        frame.src = id_;
                        frame.dst = request.dst;
                        frame.prio = savedPrio;
                        sendRequests_.pop_front();
                        savedPrio = 0;
                        Station::numPendingRequests--;
                    }
                }
                break;
            case Frame::Data:
                // check if the data has reached its destination
                if (frame.dst == id_) {
                    frame.type = Frame::Ack;
                    frame.dst = frame.src;
                    frame.src = id_;
                }

                // swap priorities if nessesary
                if (hasPendingRequest(round)) {
                    SendRequest& request = sendRequests_.front();
                    if (frame.prio < request.prio) {
                        savedPrio = frame.prio;
                        frame.prio = request.prio;
                    }
                }
                break;
            case Frame::Ack:
                // if the ack has reached its destination, pass a frame
                if (frame.dst == id_) {
                    frame.type = Frame::Token;
                } else if (hasPendingRequest(round)) {
                    // swap priorities if nessesary
                    SendRequest& request = sendRequests_.front();
                    if (frame.prio < request.prio) {
                        savedPrio = frame.prio;
                        frame.prio = request.prio;
                    }
                }
                break;

        } // switch

        // output the sending state
        cout << "round " << round << " station " << id_ << " sends with prio "
             << frame.prio << " ";
        if (frame.type == Frame::Token) {
            cout << "token" << endl;
        } else if (frame.type == Frame::Data) {
            cout << "data from " << frame.src << " to " << frame.dst << endl;
        } else {
            cout << "ack from " << frame.src << " to " << frame.dst << endl;
        }

        // send the frame
        next_->data(frame);

        // resume and increase the round
        round++;
    }
}

/**
 * Prints the usage and exits the program.
 *
 * argv - the program argument values
 */
static void usage(char **argv) {
    cerr << "Usage: " << argv[0] << " <stations (1-100)> [input-file]" << endl;
    exit(EXIT_FAILURE);
}

void uMain::main() {
    istream *input = &cin;

    // invalid number of arguments
    if (argc < 2 || argc > 3) {
        usage(argv);
    } // if

    // check for valid number of stations
    unsigned int numStations = atoi(argv[1]);
    if (numStations <= 0) {
        usage(argv);
    } // if

    if (argc == 3) {
        // try opening the specified file
        try {
            input = new ifstream(argv[2]);
        } catch (uFile::Failure &e) {
            cerr << "Error! Could not open file \"" << argv[2] << "\"" << endl;
            usage(argv);
        }
    } // if

    // setup the stations
    vector<Station*> stations;
    stations.push_back(new Station(0));
    for (unsigned int s = 1; s < numStations; s++) {
        stations.push_back(new Station(s));
        stations[s - 1]->setup(stations[s]);
    }
    stations.back()->setup(stations[0]);

    // read in all of the instructions
    unsigned int round, src, dst, prio;
    while (*input >> round >> src >> dst >> prio) {
        if (src < stations.size() && dst < stations.size()) {
            stations[src]->sendreq(round, dst, prio);
        }
    }

    // start the simulation
    stations[0]->start();


    // cleanup stations
    for (unsigned int s = 0; s < stations.size(); s++) {
        delete stations[s];
    }

    // don't delete cin
    if (input != &cin) {
        delete input;
    }
}
