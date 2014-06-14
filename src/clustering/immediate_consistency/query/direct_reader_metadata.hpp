// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_IMMEDIATE_CONSISTENCY_QUERY_DIRECT_READER_METADATA_HPP_
#define CLUSTERING_IMMEDIATE_CONSISTENCY_QUERY_DIRECT_READER_METADATA_HPP_

#include "rdb_protocol/protocol.hpp"
#include "rpc/mailbox/addr.hpp"

/* Each replica exposes a `direct_reader_business_card_t` for each shard that it
is a primary or secondary for. */

class direct_reader_business_card_t {
public:
    typedef mailbox_addr_t< void(
            read_t,
            mailbox_addr_t< void(read_response_t)>
            )> read_addr_t;
    typedef read_addr_t::mailbox_type read_mailbox_t;

    direct_reader_business_card_t() { }
    explicit direct_reader_business_card_t(const read_addr_t &rm) : read_mailbox(rm) { }

    read_addr_t read_mailbox;

    RDB_MAKE_ME_SERIALIZABLE_1(read_mailbox);
};

RDB_MAKE_EQUALITY_COMPARABLE_1(direct_reader_business_card_t, read_mailbox);

#endif /* CLUSTERING_IMMEDIATE_CONSISTENCY_QUERY_DIRECT_READER_METADATA_HPP_ */
