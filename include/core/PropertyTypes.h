#pragma once

#include <glm/glm.hpp>
#include <string>

enum class PropertyType : std::uint8_t {
  Float,
  Int,
  Bool,
  Vec2,
  Vec3,
  Vec4,
  String,
};

template <typename T>
struct PropertyTypeOf;

template <>
struct PropertyTypeOf<float> {
  static constexpr PropertyType value = PropertyType::Float;
};

template <>
struct PropertyTypeOf<int> {
  static constexpr PropertyType value = PropertyType::Int;
};

template <>
struct PropertyTypeOf<bool> {
  static constexpr PropertyType value = PropertyType::Bool;
};

template <>
struct PropertyTypeOf<glm::vec2> {
  static constexpr PropertyType value = PropertyType::Vec2;
};

template <>
struct PropertyTypeOf<glm::vec3> {
  static constexpr PropertyType value = PropertyType::Vec3;
};

template <>
struct PropertyTypeOf<glm::vec4> {
  static constexpr PropertyType value = PropertyType::Vec4;
};

template <>
struct PropertyTypeOf<std::string> {
  static constexpr PropertyType value = PropertyType::String;
};