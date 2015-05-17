#ifndef __EBISU_UTILS_EXCEPTION_MAKE_EXCEPTION_HPP__
#define __EBISU_UTILS_EXCEPTION_MAKE_EXCEPTION_HPP__

#include "ebisu/utils/exception/exception_initialization.hpp"
#include "ebisu/utils/exception/exception_logging.hpp"
#include <boost/exception/all.hpp>
#include <exception>

namespace ebisu {
namespace utils {
namespace exception {
// custom <FcbBeginNamespace make_exception>
template <typename EXCEPTION>
inline EXCEPTION make_exception(int line_number, char const* file) {
  return EXCEPTION() << boost::errinfo_at_line(line_number)
                     << boost::errinfo_file_name(file);
}

template <typename EXCEPTION>
inline EXCEPTION make_exception(char const* function_name, int line_number,
                                char const* file) {
  return EXCEPTION() << boost::errinfo_api_function(function_name)
                     << boost::errinfo_at_line(line_number)
                     << boost::errinfo_file_name(file);
}
// end <FcbBeginNamespace make_exception>

}  // namespace exception
}  // namespace utils
}  // namespace ebisu

#endif  // __EBISU_UTILS_EXCEPTION_MAKE_EXCEPTION_HPP__