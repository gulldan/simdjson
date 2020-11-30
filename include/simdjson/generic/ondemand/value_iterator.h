namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

class document;
class object;
class array;
class value;
class raw_json_string;
class parser;

/**
 * Iterates through a single JSON value at a particular depth.
 * 
 * Does not keep track of the type of value: provides methods for objects, arrays and scalars and expects
 * the caller to call the right ones.
 *
 * @private This is not intended for external use.
 */
class value_iterator {
protected:
  /** The underlying JSON iterator */
  json_iterator *iter{};
  /** The depth of this value */
  depth_t depth{};

public:
  simdjson_really_inline value_iterator() noexcept = default;
  simdjson_really_inline value_iterator(value_iterator &&other) noexcept = default;
  simdjson_really_inline value_iterator &operator=(value_iterator &&other) noexcept = default;
  simdjson_really_inline value_iterator(const value_iterator &other) noexcept = default;
  simdjson_really_inline value_iterator &operator=(const value_iterator &other) noexcept = default;

  /**
   * Denote that we're starting a document.
   */
  simdjson_really_inline void start_document() noexcept;

  /**
   * Skips a JSON value, whether it is a scalar, array or object.
   */
  simdjson_warn_unused simdjson_really_inline error_code skip() noexcept;

  /**
   * Tell whether the iterator is at the EOF mark
   */
  simdjson_really_inline bool at_eof() const noexcept;

  /**
   * Tell whether the value is open--if the value has not been used, or the array/object is still open.
   */
  simdjson_really_inline bool is_open() const noexcept;

  /**
   * Get the child value as a value_iterator.
   */
  simdjson_really_inline value_iterator child_value() const noexcept;

  /**
   * @addtogroup object Object iteration
   * 
   * Methods to iterate and find object fields. These methods generally *assume* the value is
   * actually an object; the caller is responsible for keeping track of that fact.
   * 
   * @{
   */

  /**
   * Start an object iteration.
   *
   * @returns Whether the object had any fields (returns false for empty).
   * @error INCORRECT_TYPE if there is no opening {
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> start_object() noexcept;
  /**
   * Check for an opening { and start an object iteration.
   *
   * @returns Whether the object had any fields (returns false for empty).
   * @error INCORRECT_TYPE if there is no opening {
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> try_start_object() noexcept;

  /**
   * Start an object iteration after the user has already checked and moved past the {.
   *
   * Does not move the iterator.
   *
   * @returns Whether the object had any fields (returns false for empty).
   */
  simdjson_warn_unused simdjson_really_inline bool started_object() noexcept;

  /**
   * Moves to the next field in an object.
   *
   * Looks for , and }. If } is found, the object is finished and the iterator advances past it.
   * Otherwise, it advances to the next value.
   *
   * @return whether there is another field in the object.
   * @error TAPE_ERROR If there is a comma missing between fields.
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> has_next_field() noexcept;

  /**
   * Get the current field's key.
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<raw_json_string> field_key() noexcept;

  /**
   * Pass the : in the field and move to its value.
   */
  simdjson_warn_unused simdjson_really_inline error_code field_value() noexcept;

  /**
   * Find the next field with the given key.
   *
   * Assumes you have called next_field() or otherwise matched the previous value.
   *
   * Key is *raw JSON,* meaning it will be matched against the verbatim JSON without attempting to
   * unescape it. This works well for typical ASCII and UTF-8 keys (almost all of them), but may
   * fail to match some keys with escapes (\u, \n, etc.).
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> find_field_raw(const char *key) noexcept;

  /** @} */

  /**
   * @addtogroup array Array iteration
   * Methods to iterate over array elements. These methods generally *assume* the value is actually
   * an object; the caller is responsible for keeping track of that fact.
   * @{
   */

  /**
   * Check for an opening [ and start an array iteration.
   *
   * @param json A pointer to the potential [.
   * @returns Whether the array had any elements (returns false for empty).
   * @error INCORRECT_TYPE If there is no [.
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> start_array() noexcept;
  /**
   * Check for an opening [ and start an array iteration.
   *
   * @returns Whether the array had any elements (returns false for empty).
   * @error INCORRECT_TYPE If there is no [.
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> try_start_array() noexcept;

  /**
   * Start an array iteration after the user has already checked and moved past the [.
   *
   * Does not move the iterator.
   *
   * @returns Whether the array had any elements (returns false for empty).
   */
  simdjson_warn_unused simdjson_really_inline bool started_array() noexcept;

  /**
   * Moves to the next element in an array.
   *
   * Looks for , and ]. If ] is found, the array is finished and the iterator advances past it.
   * Otherwise, it advances to the next value.
   *
   * @return Whether there is another element in the array.
   * @error TAPE_ERROR If there is a comma missing between elements.
   */
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> has_next_element() noexcept;

  /** @} */

  /**
   * @defgroup scalar Scalar values
   * @addtogroup scalar
   * @{
   */

  simdjson_warn_unused simdjson_really_inline simdjson_result<std::string_view> try_get_string() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<std::string_view> require_string() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<raw_json_string> try_get_raw_json_string() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<raw_json_string> require_raw_json_string() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<uint64_t> try_get_uint64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<uint64_t> require_uint64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<int64_t> try_get_int64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<int64_t> require_int64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<double> try_get_double() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<double> require_double() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> try_get_bool() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> require_bool() noexcept;
  simdjson_really_inline bool require_null(const uint8_t *json) noexcept;
  simdjson_really_inline bool is_null() noexcept;

  simdjson_warn_unused simdjson_really_inline simdjson_result<uint64_t> try_get_root_uint64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<uint64_t> require_root_uint64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<int64_t> try_get_root_int64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<int64_t> require_root_int64() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<double> try_get_root_double() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<double> require_root_double() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> try_get_root_bool() noexcept;
  simdjson_warn_unused simdjson_really_inline simdjson_result<bool> require_root_bool() noexcept;
  simdjson_really_inline bool require_root_null(const uint8_t *json) noexcept;
  simdjson_really_inline bool is_root_null() noexcept;

  /** @} */

protected:
  simdjson_really_inline value_iterator(json_iterator *_iter, uint32_t _depth) noexcept;

  friend class document;
  friend class object;
  friend class array;
  friend class value;
}; // value_iterator

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace simdjson

namespace simdjson {

template<>
struct simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value_iterator> : public SIMDJSON_IMPLEMENTATION::implementation_simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::value_iterator> {
public:
  simdjson_really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::value_iterator &&value) noexcept; ///< @private
  simdjson_really_inline simdjson_result(error_code error) noexcept; ///< @private

  simdjson_really_inline simdjson_result() noexcept = default;
  simdjson_really_inline simdjson_result(simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value_iterator> &&a) noexcept = default;
  simdjson_really_inline ~simdjson_result() noexcept = default; ///< @private
};

} // namespace simdjson