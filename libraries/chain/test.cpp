#include <bts/chain/database.hpp>
#include <bts/chain/account_operations.hpp>

using namespace bts::chain;
int main( int argc, char** argv )
{
   try {
      fc::ecc::public_key dans_key;

      auto& db = database::instance();

      signed_block first_block;
      first_block.block_num = 1;

      signed_transaction trx1;
        create_account_operation op;
        op.name = "dan";
        op.owner.required_keys = 1;
        op.owner.keys.push_back( dans_key );
        op.active = op.owner;

      trx1.operations.push_back( op );
      first_block.transactions.push_back( trx1 );

      db.push_block( first_block );

   } 
   catch ( const fc::exception& e )
   {
      elog( "${e}", ("e",e.to_detail_string() ) );
   }
   return 0;
}
