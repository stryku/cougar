#include "diagnostics.hh"
namespace Cougar::Utils {

void Diagnostics::print() {

  // print errors
  for (const Message &msg : mErrors) {
    fmt::print("{}:{}: ERROR: {}\n", msg.path, msg.location, msg.message);
  }
}

} // namespace Cougar::Utils