//
// Created by Bear on 2023/8/15.
//

// modify from darts https://github.com/cs87-dartmouth/darts-2022

#pragma once

#include "bxdf.h"
#include <memory>
#include <set>
#include <string>

namespace beart {
template<typename Object, typename... Args>
class Factory {
 public:
  using Constructor = std::function<Object(Args &&...args)>;

  /**
      Construct an object from the class of the given \p name and pass \p args to the registered constructor.

      \param name
          An internal name that is associated with this class. For darts scenes, this is the
          'type' field found in the scene json file.

      \param args
          Variadic list of arguments to forward to the registered constructor.
   */
  static Object Create(const std::string &name, Args &&...args) {
    if (TypeRegistry().find(name) == TypeRegistry().end()) {
      spdlog::critical("Cannot find a constructor for type '{}' in the factory.", name);
    }
    return TypeRegistry().at(name)(std::forward<Args>(args)...);
  }

  /**
      Register an object constructor with the object factory

      This function is called by the macro #DARTS_REGISTER_CLASS_IN_FACTORY

      \param name
          An internal name that is associated with this class. For darts scenes, this is the
          'type' field found in the scene json file.

      \param constructor
          A function that is able to call the constructor of the class.
     */
  static void RegisterType(const std::string &name, const Constructor constructor) {
    if (TypeRegistry().find(name) != TypeRegistry().end()) {
      spdlog::critical("A constructor for type '{}' has already been registered in this factory.", name);
    }
    TypeRegistry().insert(std::make_pair(name, constructor));
  }

  /// Return a set of all classes registered with this factory (useful for debugging).
  static std::set<std::string> RegisteredTypes() {
    const auto r = TypeRegistry();
    std::set<std::string> keys;
    for (const auto mi : r) {
      keys.insert(mi.first);
    }
    return keys;
  }

 protected:
  /// Singleton pattern that prevents "static initialization order fiasco".
  static std::unordered_map<std::string, Constructor> &TypeRegistry() {
    static std::unordered_map<std::string, Constructor> registry;
    return registry;
  }
};

/**
    A specialization of the #Factory for darts objects read from json

    Additionally, DartsFactory allows storing previously parsed/created instances into a registry with
    #register_instance() and retrieving them with their name using #find().

    \warning If you want to use DartsFactory<T>::find() with more types T, you need to specialize it

    \tparam T This factory will create objects of type shared_ptr<T>.

    \ingroup Parser
*/
template<typename T>
class BeartFactory : public Factory<T *, const nlohmann::json &> {
 public:
  using SharedT = std::shared_ptr<T>; ///< Shared pointer to the created type #T
  using UniqueT = std::unique_ptr<T>; ///< Unique pointer to the created type #T

  using BaseFactory = Factory<T *, const nlohmann::json &>;

  static SharedT CreateShared(const nlohmann::json &j) {
    return CreatePointer<SharedT>(j);
  }

  static UniqueT CreateUnique(const nlohmann::json &j) {
    return CreatePointer<UniqueT>(j);
  }
  template<typename PointerType>
  static PointerType CreatePointer(const nlohmann::json &j) {
    if (!j.contains("type") || !j["type"].is_string())
      spdlog::error("Missing 'type' field in:\n{}", j.dump(4));

    auto type = j.at("type").get<std::string>();

    try {
      auto raw_ptr = BaseFactory::Create(type, j);
      PointerType pointer(raw_ptr);
      return pointer;
    }
    catch (const std::exception &e) {
      spdlog::error("Cannot create a '{}' here:\n{}.\n\t{}", type, j.dump(4), e.what());
      return nullptr;
    }
  }
/**
    Find a previously parsed/created #SharedT, or create a new one.

    Return a #SharedT pointer by parsing a JSON specification. If \p j is a string \p key: "name", then try to find
    a previously parsed #SharedT with name "name". If \p j is a #json object \p key: {}, then create a new #SharedT
    with the specified parameters by calling #create().

    \param jp           The json object for the parent
    \param key          Look for this json field in \p jp
    \return #SharedT    The newly created or retrieved object instance.
 */
  static SharedT Find(const nlohmann::json &jp, const std::string &key) {
    auto it = jp.find(key);
    if (it == jp.end()) {
      spdlog::critical("Cannot find key '{}' here:\n{}", key, jp.dump(4));
    }
    auto j = it.value();
    if (j.is_string()) {
      std::string name = j.get<std::string>();
      // find a pre-declared SharedT
      auto i = InstanceRegistry().find(name);
      if (i != InstanceRegistry().end())
        return i->second;
      else {
        spdlog::critical("Cannot find an object with name '{}' here:\n{}", name, jp.dump(4));
      }
    } else if (j.is_object()) {
      // create a new SharedT instance
      return CreateShared(j);
    } else
      spdlog::critical("Type mismatch: Expecting either a {} definition or {} name here:\n{}", key, key,
                       jp.dump(4));
  }

/**
    Find a previously parsed/created #SharedT, or create a new one using the default key for this type of object.

    \warning This function overload needs to be specialized for new types T.

    \overload

    \param jp           The json object for the parent
    \return #SharedT    The newly created or retrieved object instance.
 */
  static SharedT Find(const nlohmann::json &jp);

/// Associate and store a shared_ptr to an object instance \p o with the name \p name
  static void RegisterInstance(const std::string &name, SharedT o) {
    InstanceRegistry()[name] = o;
  }

/// Global map of #SharedT instances that have been create/parsed
  static std::map<std::string, SharedT> &InstanceRegistry() {
    static std::map<std::string, SharedT> registry;
    return registry;
  }
};

template<>
inline std::shared_ptr<Bxdf> BeartFactory<Bxdf>::Find(const nlohmann::json &jp) {
  return Find(jp, "material");
}
}
namespace beart {
/**
    Macro for registering an object constructor with a #DartsFactory

    \param  T       The object type to register
    \param  cls     The name of the class to create instances of
    \param  name    Associate the keyword `type: "name"` for creating this type of object from a json object

    \ingroup Parser
*/
#define BEART_REGISTER_CLASS_IN_FACTORY(T, cls, name) \
    static struct cls##_FACTORY_HELPER                                                                                 \
    {                                                                                                                  \
        cls##_FACTORY_HELPER()                                                                                         \
        {                                                                                                              \
            beart::BeartFactory<beart::T>::RegisterType(name, [](const nlohmann::json &j) { return new beart::cls(j); });         \
        }                                                                                                              \
    } cls##_FACTORY_HELPER_INSTANCE;
}