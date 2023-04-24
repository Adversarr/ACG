/**
 * @file node.hpp
 * @brief Node system for ACG.
 */

#pragma once

#include <any>
#include <optional>
#include <string>
#include <typeindex>
#include <variant>

namespace acg::node {

struct NodeIO {
  // Name of the node.
  std::string name_;
  // Holds the pointer to data.
  void* value_{nullptr};
  // Relies on `type_info` for type information.
  std::type_index type_;

  template <typename T> explicit NodeIO(T* val = nullptr) : NodeIO("NONAME", val) {}

  template <typename T> explicit NodeIO(std::string_view name, T* val = nullptr)
      : name_(name), value_(val), type_(typeid(T)) {}
};


struct NodeBase {
  void SetInput(std::string node_name, void* data);

  void SetOutput(std::string node_name, void* data);

  virtual ~NodeBase() = 0;
  
  std::vector<NodeIO> inputs_;
  std::vector<NodeIO> outputs_;
};

// A graph containing zero or more nodes is called a `Session`
class Session {
  std::vector<NodeBase* > nodes;
};

}  // namespace acg::node
