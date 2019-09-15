#include "common_utilities.hpp"

#ifndef EOSDAC_DIRECTORY_H
#define EOSDAC_DIRECTORY_H
namespace eosdac {
    namespace directory {
        using namespace eosio;
        using namespace std;

        namespace types {

            enum account_type: uint8_t {
                AUTH = 0,
                TREASURY = 1,
                CUSTODIAN = 2,
                MSIGS = 3,
                SERVICE = 5,
                PROPOSALS = 6,
                ESCROW = 7,
                ROUTER = 8,
                EXTERNAL = 254,
                OTHER = 255
            };

            enum ref_type: uint8_t {
                HOMEPAGE = 0,
                LOGO_URL = 1,
                DESCRIPTION = 2,
                LOGO_NOTEXT_URL = 3,
                BACKGROUND_URL = 4,
                COLORS = 5,
                CLIENT_EXTENSION = 6
            };

            enum dac_state_type: uint8_t {
                dac_state_typeINACTIVE = 0,
                dac_state_typeACTIVE = 1
            };

            struct [[table("dacs"), contract("dacdirectory")]] dac {
                name         owner;
                name         dac_id;
                string         title;
                extended_symbol       symbol;
                map<uint8_t, string> refs;
                map<uint8_t, name> accounts;
                uint8_t      dac_state;

                name account_for_type( uint8_t type) const {
                    print("\ngetting account for type: ", type,"\n");
                    return accounts.at(type);
                }

                uint64_t primary_key() const { return dac_id.value; }
                uint64_t by_owner() const { return owner.value; }
                uint128_t by_symbol() const { return raw_from_extended_symbol(symbol); }
            };
        }

        namespace tables {
            using namespace types;

            typedef multi_index< "dacs"_n,  dac,
                    indexed_by<"byowner"_n, const_mem_fun<dac, uint64_t, &dac::by_owner>>,
            indexed_by<"bysymbol"_n, const_mem_fun<dac, uint128_t, &dac::by_symbol>>
            > dac_table;
        }

        namespace actions {

        }

        const types::dac dac_for_id(name id) {
            tables::dac_table dactable("dacdirectory"_n, "dacdirectory"_n.value);
            return dactable.get(id.value, "ERR::DAC_NOT_FOUND::DAC not found in directory");
        }

        const types::dac dac_for_symbol(extended_symbol sym) {
            tables::dac_table dactable("dacdirectory"_n, "dacdirectory"_n.value);
            auto index = dactable.get_index<"bysymbol"_n>();
            auto dac_idx = index.find(raw_from_extended_symbol(sym));
            print("\ndac_for_symbol: ", sym, "\n");
            check(dac_idx != index.end() && dac_idx->symbol == sym, "ERR::DAC_NOT_FOUND_SYMBOL::DAC not found in directory for the given symbol");
            return *dac_idx;
        }
    }
}
#endif