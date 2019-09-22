
#include <eosio/eosio.hpp>
#include <eosio/symbol.hpp>

namespace eosdac {
    namespace custodian {
        using namespace eosio;
        using std::string;

        namespace types {

            struct [[eosio::table]] custodian_type {
                name     cust_name;
                asset    requestedpay;
                uint64_t total_votes;

                uint64_t primary_key() const { return cust_name.value; }

                uint64_t by_votes_rank() const { return static_cast<uint64_t>(UINT64_MAX - total_votes); }

                uint64_t by_requested_pay() const { return static_cast<uint64_t>(requestedpay.amount); }
            };


            struct [[eosio::table]] candidate_type {
                name           candidate_name;
                asset          requestedpay;
                asset          locked_tokens;
                uint64_t       total_votes;
                uint8_t        is_active;
                time_point_sec custodian_end_time_stamp;

                uint64_t primary_key() const { return candidate_name.value; }
                uint64_t by_number_votes() const { return static_cast<uint64_t>(total_votes); }
                uint64_t by_votes_rank() const { return static_cast<uint64_t>(UINT64_MAX - total_votes); }
                uint64_t by_requested_pay() const { return static_cast<uint64_t>(requestedpay.amount); }
            };


            struct [[eosio::table]] vote_weight_type {
                name     voter;
                uint64_t weight;

                uint64_t primary_key()const { return voter.value; }
            };
        }

        namespace tables {
            using namespace types;

            typedef eosio::multi_index<"custodians"_n, custodian_type,
                    eosio::indexed_by<"byvotesrank"_n, eosio::const_mem_fun<custodian_type, uint64_t, &custodian_type::by_votes_rank> >,
                    eosio::indexed_by<"byreqpay"_n, eosio::const_mem_fun<custodian_type, uint64_t, &custodian_type::by_requested_pay> >
            > custodian_table;


            typedef eosio::multi_index<"candidates"_n, candidate_type,
                    eosio::indexed_by<"bycandidate"_n, eosio::const_mem_fun<candidate_type, uint64_t, &candidate_type::primary_key> >,
                    eosio::indexed_by<"byvotes"_n, eosio::const_mem_fun<candidate_type, uint64_t, &candidate_type::by_number_votes> >,
                    eosio::indexed_by<"byvotesrank"_n, eosio::const_mem_fun<candidate_type, uint64_t, &candidate_type::by_votes_rank> >,
                    eosio::indexed_by<"byreqpay"_n, eosio::const_mem_fun<candidate_type, uint64_t, &candidate_type::by_requested_pay> >
            > candidates_table;

            typedef eosio::multi_index< "weights"_n, vote_weight_type > weight_table;
        }

        namespace actions {

        }
    }
}
