#include <bts/blockchain/shuffle_delegates.hpp>

#include <fc/io/raw.hpp>

#include <iostream>

static void print_delegates( const std::vector<bts::blockchain::delegate_id>& delegates ) {
    for ( auto delegate : delegates ) {
        std::cout << delegate << "\t";
    }
    std::cout << "\n";
}

int main(int argc, char** argv) {
    std::vector<bts::blockchain::delegate_id> voted;
    std::vector<bts::blockchain::delegate_id> previous;
    for ( int i = 1; i <= 23; i++ ) {
        voted.push_back(i);
        previous.push_back(24 - i);
    }

    fc::sha256 seed1 = fc::sha256::hash("test");
//    std::cout << std::string(seed1) << "\n";
    print_delegates(bts::blockchain::next_delegate_order(previous, voted, seed1));
//    std::cout << std::string(seed1) << "\n";
    print_delegates(bts::blockchain::next_delegate_order(previous, voted, seed1));

    std::cout << "------\n";

    fc::sha256 seed2 = fc::sha256::hash("testtest");
//    std::cout << std::string(seed2) << "\n";
    print_delegates(bts::blockchain::next_delegate_order(previous, voted, seed2));
//    std::cout << std::string(seed2) << "\n";
    print_delegates(bts::blockchain::next_delegate_order(previous, voted, seed2));

    std::cout << "------\n";

    for ( int i = 24; i <= 27; i++ ) {
        voted.push_back(i + 19);
        previous.push_back(i);
    }
    print_delegates(bts::blockchain::next_delegate_order(previous, voted, seed1));
    print_delegates(bts::blockchain::next_delegate_order(previous, voted, seed2));

    return 0;
}

