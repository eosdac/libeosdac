
#include "directory.hpp"

#ifndef EOSDAC_NOTIFY_H
#define EOSDAC_NOTIFY_H

namespace eosdac {
    namespace notify {
        using namespace eosio;

        namespace types {

            struct account_balance_delta {
                name    account;
                asset   balance_delta;
            };

            struct account_weight_delta {
                name    account;
                int64_t weight_delta;
            };

            struct account_stake_delta {
                name    account;
                asset   stake_delta;
            };
        }

        void balanceobsv(vector<types::account_balance_delta> account_balance_deltas, name dac_id);
        using balanceobsv_action = action_wrapper< "balanceobsv"_n, &balanceobsv >;
        void stakeobsv(vector<types::account_stake_delta> account_stake_deltas, name dac_id);
        using stakeobsv_action = action_wrapper< "stakeobsv"_n, &stakeobsv >;
        void weightobsv(vector<types::account_weight_delta> account_weight_deltas, name dac_id);
        using weightobsv_action = action_wrapper< "weightobsv"_n, &weightobsv >;
    }
}
#endif