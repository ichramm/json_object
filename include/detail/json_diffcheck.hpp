/*!
 * \file json_diffcheck.hpp
 * \author ichramm
 *
 * \date January 21, 2014, 10:44 AM
 *
 * json_object Version 2 - Difference Calculation
 */
#ifndef jsonobject_detail_json_diffcheck_hpp__
#define jsonobject_detail_json_diffcheck_hpp__

namespace json
{
	class json_object; // must not include json_object.hpp

	/*!
	 * Represents the transformation that was applied on
	 * a \c json_object for getting from state "A" to state "B"
	 */
	enum diff_state_t
	{
		/*! Nothing changed */
		JSDS_NONE,
		/*! Object is new */
		JSDS_ADDED,
		/*! Object was removed */
		JSDS_DELETED,
		/*! Object's internal state was updated */
		JSDS_UPDATED
	};

	/*!
	* Computes the difference between two json objects
	*
	* This function assumes that \p o2 is the result of a
	* transformation applied on \p o1.
	*
	* With that in mind, this function will analyze the
	* changes that where performed on \p o1, construct a new
	* json object with the values from \p o2, and finally set
	* the corresponding \c _diff_state attribute.
	*/
	json_object compute_diff(const json_object& o1, const json_object& o2);

	namespace detail
	{
		/*!
		 * Sets the difference state in \p obj, \p obj's children, and the children of
		 * its children to \p diff_state, recursively.
		 *
		 * This function is specially useful for setting states \c JSDS_ADDED and \c JSDS_DELETED
		 */
		json_object& set_diff_state_deep(json_object& obj, diff_state_t diff_state);

	} // namespace detail
} // namespace json

#endif //jsonobject_detail_json_diffcheck_hpp__
