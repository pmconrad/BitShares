#include <bts/blockchain/shuffle_delegates.hpp>

//#include <iostream>

namespace bts { namespace blockchain {

class rng {
    public:
        rng( const fc::sha256& seed) {
            state = fc::sha256::hash( seed );
            count = 0;
        }
        unsigned int next_int( unsigned int limit ) {
            if ( count > 3 ) {
                state = fc::sha256::hash( state );
                count = 0;
            }
            // FIXME: this does not create a uniform distribution!
            unsigned int rand = state._hash[count++] % limit;
//            std::cout << "r " << rand << "\n";
            return rand;
        }
    private:
        fc::sha256 state;
        int count;
};


static unsigned int fetch_at( std::set<unsigned int>& free, unsigned int index ){
    std::set<unsigned int>::iterator it = free.begin();
    while ( index > 0 ) { it++; index--; }
    unsigned int fetched = *it;
    free.erase( fetched );
//    std::cout << "i " << fetched << "\n";
    return fetched;
}

static void insert_shuffled( std::vector<delegate_id>& result,
                             delegate_id next,
                             std::set<unsigned int>& free,
                             unsigned int& next_free,
                             std::set<delegate_id>& only_these,
                             rng& prng, bool check ) {
    if ( check && only_these.find( next ) == only_these.end() ) { return; }
    unsigned int next_place = fetch_at( free, prng.next_int(free.size()) );
    result[next_place] = next;
    if ( check ) { only_these.erase( next ); }
    if ( next_free > 0 ) { free.insert( --next_free ); }
}

std::vector<delegate_id> next_delegate_order(const std::vector<delegate_id>& prev_delegate_order,
                                             const std::vector<delegate_id>& top_101_delegates_sorted_by_vote,
                                             const fc::sha256& random_seed) {
    std::set<delegate_id> to_place( top_101_delegates_sorted_by_vote.begin(),
                                    top_101_delegates_sorted_by_vote.end() );
    std::vector<delegate_id> result;
    result.resize(top_101_delegates_sorted_by_vote.size());

    std::set<unsigned int> free;
    unsigned int next_free = top_101_delegates_sorted_by_vote.size();
    while ( next_free > top_101_delegates_sorted_by_vote.size() / 2 ) {
        free.insert( --next_free );
    }

    rng prng( random_seed );

    for ( auto it = prev_delegate_order.rbegin(); it != prev_delegate_order.rend(); it++ ) {
        insert_shuffled( result, *it, free, next_free, to_place, prng, true );
    }

    for ( auto it = to_place.rbegin(); it != to_place.rend(); it++ ) {
        insert_shuffled( result, *it, free, next_free, to_place, prng, false );
    }

    return result;
}

}}
