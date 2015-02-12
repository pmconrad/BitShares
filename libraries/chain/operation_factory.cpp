#include <bts/chain/operation_factory.hpp>
#include <bts/chain/account_operations.hpp>
#include <bts/chain/asset_operations.hpp>

namespace bts { namespace chain { 
   const operation_type create_account_operation::type     = create_account_op_type;
   const operation_type create_user_asset_operation::type  = create_user_asset_op_type;

   static bool first_chain = []()->bool{
      bts::chain::operation_factory::instance().register_operation<withdraw_operation>();
      return true;
   }();

   operation_factory& operation_factory::instance()
   {
      static std::unique_ptr<operation_factory> inst( new operation_factory() );
      return *inst;
   }

   void operation_factory::to_variant( const bts::chain::operation& in, fc::variant& output )
   { try {
      auto converter_itr = _converters.find( in.type.value );
      FC_ASSERT( converter_itr != _converters.end() );
      converter_itr->second->to_variant( in, output );
   } FC_RETHROW_EXCEPTIONS( warn, "" ) }

   void operation_factory::from_variant( const fc::variant& in, bts::chain::operation& output )
   { try {
      auto obj = in.get_object();
      output.type = obj["type"].as<operation_type_enum>();

      auto converter_itr = _converters.find( output.type.value );
      FC_ASSERT( converter_itr != _converters.end() );
      converter_itr->second->from_variant( in, output );
   } FC_RETHROW_EXCEPTIONS( warn, "", ("in",in) ) }

} }

namespace fc {
   void to_variant( const bts::chain::operation& var,  variant& vo )
   {
      bts::chain::operation_factory::instance().to_variant( var, vo );
   }

   void from_variant( const variant& var,  bts::chain::operation& vo )
   {
      bts::chain::operation_factory::instance().from_variant( var, vo );
   }
}
