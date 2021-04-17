#pragma once

#include "list.hh"
#include "source_location.hh"
#include "zone_allocator.hh"

#include <fmt/format.h>

namespace Cougar::Utils {

class Diagnostics {
public:
  struct Message {
    SourceLocation location;
    std::string_view path;
    std::string_view message;
  };

  Diagnostics() = default;

  void setPath(std::string_view path) { mPath = path; }

  void print();

  bool hasErrors() const { return !mErrors.empty(); }

  template <typename... Args>
  void error(const SourceLocation &loc, std::string_view fmt,
             const Args &...args);

private:
  std::string_view mPath;

  List<Message> mErrors;
  List<Message> mWarnings;
};

template <typename... Args>
void Diagnostics::error(const SourceLocation &loc, std::string_view fmt,
                        const Args &...args) {

  std::string_view text = Zone::format(fmt, args...);

  mErrors.emplace_back(Message{loc, mPath, text});
}

} // namespace Cougar::Utils