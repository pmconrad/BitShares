#ifndef SHUFFLE_DELEGATES_HPP
#define	SHUFFLE_DELEGATES_HPP

#include <fc/crypto/sha256.hpp>

namespace bts { namespace blockchain {

typedef int64_t delegate_id;

std::vector<delegate_id> next_delegate_order(const std::vector<delegate_id>& prev_delegate_order,
                                             const std::vector<delegate_id>& top_101_delegates_sorted_by_vote,
                                             const fc::sha256& random_seed);
}}

#endif	/* SHUFFLE_DELEGATES_HPP */
