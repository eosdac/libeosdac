
#include <eosio/eosio.hpp>
#include <eosio/symbol.hpp>

namespace eosdac {
    namespace token {
        using namespace eosio;
        using std::string;

        namespace types {

            struct [[eosio::table]] member_type {
                name     sender;
                uint64_t agreedtermsversion;

                uint64_t primary_key() const { return sender.value; }
            };


            struct [[eosio::table]] termsinfo_type {
                string   terms;
                string   hash;
                uint64_t version;

                uint64_t primary_key() const { return version; }
                uint64_t by_latest_version() const { return UINT64_MAX - version; }
            };


            // Standard token contract tables
            struct [[eosio::table]] account_type {
                asset    balance;

                uint64_t primary_key()const { return balance.symbol.code().raw(); }
            };

            struct [[eosio::table]] stat_type {
                asset    supply;
                asset    max_supply;
                name     issuer;
                bool     transfer_locked = false;

                uint64_t primary_key()const { return supply.symbol.code().raw(); }
            };

        }

        namespace tables {
            using namespace types;

            typedef multi_index<"members"_n, member_type> member_table;

            typedef multi_index<"memberterms"_n, termsinfo_type,
                    indexed_by<"bylatestver"_n, const_mem_fun<termsinfo_type, uint64_t, &termsinfo_type::by_latest_version> >
            > member_terms_table;

            typedef multi_index< "accounts"_n, account_type > account_table;
            typedef multi_index< "stat"_n, stat_type > stat_table;
        }

        namespace actions {

        }

        static void assert_valid_member(name member, name dac_id) {
            name member_terms_account;

            member_terms_account = directory::dac_for_id(dac_id).symbol.get_contract(); // Need this line without the temp block
            tables::member_table reg_members(member_terms_account, dac_id.value);
            tables::member_terms_table memberterms(member_terms_account, dac_id.value);

            const auto &regmem = reg_members.get(member.value, "ERR::GENERAL_REG_MEMBER_NOT_FOUND::Account is not registered with members.");
            check((regmem.agreedtermsversion != 0), "ERR::GENERAL_MEMBER_HAS_NOT_AGREED_TO_ANY_TERMS::Account has not agreed to any terms");
            auto latest_member_terms = (--memberterms.end());
            check(latest_member_terms->version == regmem.agreedtermsversion, "ERR::GENERAL_MEMBER_HAS_NOT_AGREED_TO_LATEST_TERMS::Agreed terms isn't the latest.");
        }
    }
}
