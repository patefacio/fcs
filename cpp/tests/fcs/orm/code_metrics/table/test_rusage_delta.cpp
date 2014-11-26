#include "fcs/orm/code_metrics/table/rusage_delta.hpp"
#include "fcs/utils/streamers/random.hpp"
#include <boost/test/included/unit_test.hpp>


using namespace fcs::orm::code_metrics::table;
using namespace fcs::utils::streamers;
using fcs::utils::streamers::operator<<;

namespace fcs {
namespace utils {
namespace streamers {
  // random row generation

  template< >
  inline Random_source & operator>>
    (Random_source &source, Code_packages_pkey &obj) {
    source >> obj.id;
    return source;
  }

  template< >
  inline Random_source & operator>>
    (Random_source &source, Code_packages_value &obj) {
    source >> obj.name
      >> obj.descr;
    return source;
  }


  template< >
  inline Random_source & operator>>
    (Random_source &source, Code_packages<>::Row_t &row) {
    source >> row.first >> row.second;
    return source;
  }

  template< >
  inline Random_source & operator>>
    (Random_source &source, Code_locations_pkey &obj) {
    source >> obj.id;
    return source;
  }

  template< >
  inline Random_source & operator>>
    (Random_source &source, Code_locations_value &obj) {
    source >> obj.code_packages_id
      >> obj.label
      >> obj.file_name
      >> obj.line_number
      >> obj.git_commit;
    return source;
  }


  template< >
  inline Random_source & operator>>
    (Random_source &source, Code_locations<>::Row_t &row) {
    source >> row.first >> row.second;
    return source;
  }

  template< >
  inline Random_source & operator>>
    (Random_source &source, Rusage_delta_pkey &obj) {
    source >> obj.id;
    return source;
  }

  template< >
  inline Random_source & operator>>
    (Random_source &source, Rusage_delta_value &obj) {
    source >> obj.code_locations_id
      >> obj.created
      >> obj.start_processor
      >> obj.end_processor
      >> obj.cpu_mhz
      >> obj.debug
      >> obj.user_time_sec
      >> obj.user_time_usec
      >> obj.system_time_sec
      >> obj.system_time_usec
      >> obj.ru_maxrss
      >> obj.ru_ixrss
      >> obj.ru_idrss
      >> obj.ru_isrss
      >> obj.ru_minflt
      >> obj.ru_majflt
      >> obj.ru_nswap
      >> obj.ru_inblock
      >> obj.ru_oublock
      >> obj.ru_msgsnd
      >> obj.ru_msgrcv
      >> obj.ru_nsignals
      >> obj.ru_nvcsw
      >> obj.ru_nivcsw;
    return source;
  }


  template< >
  inline Random_source & operator>>
    (Random_source &source, Rusage_delta<>::Row_t &row) {
    source >> row.first >> row.second;
    return source;
  }


}
}
}

namespace fcs {
namespace orm {
namespace code_metrics {
namespace table {
  void test_insert_update_delete_rows() {
    // testing insertion and deletion
    auto code_packages_gw = Code_packages<>::instance();
    Code_packages<>::Row_list_t code_packages_rows;
    {
      code_packages_gw.delete_all_rows();
      auto rows = code_packages_gw.select_all_rows();
      BOOST_REQUIRE(rows.empty());
    }
    auto code_locations_gw = Code_locations<>::instance();
    Code_locations<>::Row_list_t code_locations_rows;
    {
      code_locations_gw.delete_all_rows();
      auto rows = code_locations_gw.select_all_rows();
      BOOST_REQUIRE(rows.empty());
    }
    auto rusage_delta_gw = Rusage_delta<>::instance();
    Rusage_delta<>::Row_list_t rusage_delta_rows;
    {
      rusage_delta_gw.delete_all_rows();
      auto rows = rusage_delta_gw.select_all_rows();
      BOOST_REQUIRE(rows.empty());
    }

    // create some records with random data
    int const num_rows = 20;
    Random_source random_source;

    for(int i=0; i<num_rows; ++i) {

      // Declare all rows
      Code_packages<>::Row_t code_packages_row;
      Code_locations<>::Row_t code_locations_row;
      Rusage_delta<>::Row_t rusage_delta_row;

      // Generate random data for all rows
      random_source >> code_packages_row;
      random_source >> code_locations_row;
      random_source >> rusage_delta_row;


      // Link up reference ids
      link_rows(code_locations_row, code_packages_row);
      link_rows(rusage_delta_row, code_locations_row);

      // Push related records
      code_packages_rows.push_back(code_packages_row);
      code_locations_rows.push_back(code_locations_row);
      rusage_delta_rows.push_back(rusage_delta_row);

    }

    // insert those records, select back and validate
    code_packages_gw.insert(code_packages_rows);
    auto post_insert_code_packages_rows =
      code_packages_gw.select_all_rows();
    BOOST_REQUIRE(post_insert_code_packages_rows.size() == num_rows);
    code_locations_gw.insert(code_locations_rows);
    auto post_insert_code_locations_rows =
      code_locations_gw.select_all_rows();
    BOOST_REQUIRE(post_insert_code_locations_rows.size() == num_rows);
    rusage_delta_gw.insert(rusage_delta_rows);
    auto post_insert_rusage_delta_rows =
      rusage_delta_gw.select_all_rows();
    BOOST_REQUIRE(post_insert_rusage_delta_rows.size() == num_rows);

    for(size_t i=0; i<num_rows; i++) {
      BOOST_REQUIRE(code_packages_rows[i].second ==
                    post_insert_code_packages_rows[i].second);
      std::swap(code_packages_rows, post_insert_code_packages_rows);
      BOOST_REQUIRE(code_locations_rows[i].second ==
                    post_insert_code_locations_rows[i].second);
      std::swap(code_locations_rows, post_insert_code_locations_rows);
      BOOST_REQUIRE(rusage_delta_rows[i].second ==
                    post_insert_rusage_delta_rows[i].second);
      std::swap(rusage_delta_rows, post_insert_rusage_delta_rows);
    }

    // now update all values in memory with new random data
    auto updated_code_packages_rows = code_packages_rows;
    auto updated_code_locations_rows = code_locations_rows;
    auto updated_rusage_delta_rows = rusage_delta_rows;
    for(size_t i=0; i<num_rows; i++) {
      random_source >> updated_code_packages_rows[i].second;
      BOOST_REQUIRE(updated_code_packages_rows[i].second !=
                    code_packages_rows[i].second);
      random_source >> updated_code_locations_rows[i].second;
      BOOST_REQUIRE(updated_code_locations_rows[i].second !=
                    code_locations_rows[i].second);
      random_source >> updated_rusage_delta_rows[i].second;
      BOOST_REQUIRE(updated_rusage_delta_rows[i].second !=
                    rusage_delta_rows[i].second);

    }

    if(false) {
      Code_packages<>::print_recordset_as_table(
        updated_code_packages_rows, std::cout);
      Code_locations<>::print_recordset_as_table(
        updated_code_locations_rows, std::cout);
      Rusage_delta<>::print_recordset_as_table(
        updated_rusage_delta_rows, std::cout);
    }

    // push updates to database via update
    code_packages_gw.update(updated_code_packages_rows);
    code_locations_gw.update(updated_code_locations_rows);
    rusage_delta_gw.update(updated_rusage_delta_rows);

    // verify the updates
    for(size_t i=0; i<num_rows; i++) {
      {
        Code_packages<>::Value_t value;
        bool found = code_packages_gw.find_row_by_key(
          updated_code_packages_rows[i].first, value);
        BOOST_REQUIRE(found);
        BOOST_REQUIRE(value == updated_code_packages_rows[i].second);
      }
      {
        Code_locations<>::Value_t value;
        bool found = code_locations_gw.find_row_by_key(
          updated_code_locations_rows[i].first, value);
        BOOST_REQUIRE(found);
        BOOST_REQUIRE(value == updated_code_locations_rows[i].second);
      }
      {
        Rusage_delta<>::Value_t value;
        bool found = rusage_delta_gw.find_row_by_key(
          updated_rusage_delta_rows[i].first, value);
        BOOST_REQUIRE(found);
        BOOST_REQUIRE(value == updated_rusage_delta_rows[i].second);
      }
    }

  }

} // namespace table
} // namespace code_metrics
} // namespace orm
} // namespace fcs

boost::unit_test::test_suite* init_unit_test_suite(int , char*[]) {
  using namespace fcs::orm::code_metrics::table;
  using namespace boost::unit_test;
  test_suite* test= BOOST_TEST_SUITE( "<rusage_delta>" );
  test->add( BOOST_TEST_CASE( &test_insert_update_delete_rows ) );
  return test;
}