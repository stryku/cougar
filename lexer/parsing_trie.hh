#pragma once

#include <algorithm>
#include <cassert>
#include <optional>
#include <vector>

namespace Cougar::Lexer {

template <typename PatternStringT, typename TokenT> struct PatternToken {
  PatternStringT pattern;
  TokenT token;
};

template <typename CharT, typename TokenT> class ParsingTrie {
public:
  template <typename PatternTokensIt>
  void build(PatternTokensIt first, PatternTokensIt last);

  template <typename InputIt> std::optional<TokenT> find(InputIt in) const {
    return findInNodes(mRoot, in);
  }

private:
  struct Node;

  using Nodes = std::vector<Node>;

  struct Node {
    CharT c;
    std::optional<TokenT> token;
    Nodes children;
  };

  template <typename PatternIt>
  void add(Nodes &nodes, PatternIt first, PatternIt last, TokenT token);

  template <typename InputIt>
  std::optional<TokenT> findInNodes(const Nodes &nodes, InputIt in) const;

  Nodes mRoot;
};

template <typename CharT, typename TokenT>
template <typename PatternTokensIt>
void ParsingTrie<CharT, TokenT>::build(PatternTokensIt first,
                                       PatternTokensIt last) {

  std::for_each(first, last, [this](auto &pt) {
    add(mRoot, std::begin(pt.pattern), std::end(pt.pattern), pt.token);
  });
}

template <typename CharT, typename TokenT>
template <typename PatternIt>
void ParsingTrie<CharT, TokenT>::add(Nodes &nodes, PatternIt first,
                                     PatternIt last, TokenT token) {

  assert(first != last);
  CharT c = CharT(*first); // this could be char->rune conversion

  auto it = std::lower_bound(nodes.begin(), nodes.end(), c,
                             [&](const Node &a, CharT b) { return a.c < b; });

  if (it == nodes.end() || it->c != c) {
    // insert new node
    Node &node = *nodes.emplace(it);
    node.c = c;
    first++;
    if (first == last) {
      node.token = token;
      return;
    } else
      add(node.children, first, last, token);
  } else {
    // node exists
    Node &node = *it;
    first++;
    if (first == last) {
      if (node.token)
        throw std::logic_error("Duplicate pattern");
      node.token = token;
      return;
    } else {
      add(node.children, first, last, token);
    }
  }
}

template <typename CharT, typename TokenT>
template <typename InputIt>
std::optional<TokenT>
ParsingTrie<CharT, TokenT>::findInNodes(const Nodes &nodes, InputIt in) const {

  CharT c = *in;
  auto it = std::lower_bound(nodes.begin(), nodes.end(), c,
                             [&](const Node &a, CharT b) { return a.c < b; });
  if (it == nodes.end() || it->c != c)
    return std::nullopt;

  // ok, we've found a matchinng letter
  const Node &node = *it;
  if (node.children.empty()) {
    if (node.token)
      ++in;
    return node.token;
  } else {
    ++in;
    auto t = findInNodes(node.children, in);
    if (t)
      return t;
    else
      return node.token;
  }
}

} // namespace Cougar::Lexer