/*!
 * \file json_diffcheck.cpp
 * \author ichramm
 *
 * \date January 21, 2014, 10:44 AM
 *
 * json_object Version 2 - Difference Calculation
 */
#include "detail/json_diffcheck.hpp"
#include "json_object.hpp"

#include <algorithm> // min, max

json::json_object json::compute_diff(const json::json_object& o1, const json::json_object& o2)
{
	json_object result;

	if ( o1.m_type != o2.m_type ) { // not having the same type, just return the other
		result = o2;
		result.m_diff_state = JSDS_UPDATED;
		return result;
	}

	if (o1 == o2) { // objects seem to be the same, just return o1
		result = o1;
		result.m_diff_state = JSDS_NONE;
		return result;
	}

	switch (o1.m_type) {
		case JST_OBJ: {
			json_object::const_iterator it1 = o1.begin(), end1 = o1.end(), it2, end2 = o2.end();

			for (; it1 != end1; ++it1) {
				const std::string &key = it1->first;
				it2 = o2.find(key);
				if (it2 != end2) { // object is in both containers, calculate diff
					json_object &diff = result[key] = compute_diff(it1->second, it2->second);
					if (diff.m_diff_state != JSDS_NONE) {
						result.m_diff_state = JSDS_UPDATED;
					}
				} else { // object is in o1 but not in o2, it was deleted
					json_object &diff = result[key] = it1->second;
					detail::set_diff_state_deep(diff, JSDS_DELETED);
					result.m_diff_state = JSDS_UPDATED;
				}
			}
			// now check for new elements on o2
			for (it2 = o2.begin(); it2 != end2; ++it2) {
				const std::string &key = it2->first;
				if (o1.find(key) == end1) {
					detail::set_diff_state_deep(result[key] = it2->second, JSDS_ADDED);
					result.m_diff_state = JSDS_UPDATED;
				}
			}
			break;
		}
		case JST_ARRAY: {
			size_t idx = 0, size1 = o1.size(), size2 = o2.size();
			size_t min_size = std::min(size1, size2);

			result.m_type = JST_ARRAY; // vector is already initialized (and empty)
			result.m_value.arrVal.reserve(std::max(size1, size2)); // avoid preventable reallocations

			for (; idx < min_size; ++idx) {
				json_object diff = compute_diff(o1[idx], o2[idx]);
				if (diff.m_diff_state != JSDS_NONE) {
					result.m_diff_state = JSDS_UPDATED;
				}
				result.push_back(detail::move(diff));
			}

			if (idx < size1) { // more elements in o1 than in o2, so the rest of them were erased
				result.m_diff_state = JSDS_UPDATED;
				for (; idx < size1; ++idx) {
					json_object obj = o1[idx];
					detail::set_diff_state_deep(obj, JSDS_DELETED);
					result.push_back(detail::move(obj));
				}
			} else if (idx < size2) { // more elements in o2, those are new ones
				result.m_diff_state = JSDS_UPDATED;
				for (; idx < size2; ++idx) {
					json_object obj = o2[idx];
					detail::set_diff_state_deep(obj, JSDS_ADDED);
					result.push_back(detail::move(obj));
				}
			}
			break;
		}
	default: // simple object whose underlying value has changed
		result = o2;
		result.m_diff_state = JSDS_UPDATED;
		break;
	}

	return result;
}

json::json_object& json::detail::set_diff_state_deep(json::json_object& obj, diff_state_t diff_state)
{
	switch (obj.m_type) {
		case JST_ARRAY:
			for (json_object::array_iterator it = obj.abegin(), end = obj.aend(); it != end; ++it) {
				set_diff_state_deep(*it, diff_state);
			}
			break;
		case JST_OBJ:
			for (json_object::iterator it = obj.begin(), end = obj.end(); it != end; ++it) {
				set_diff_state_deep(it->second, diff_state);
			}
			break;
		default:
			break;
	}

	obj.m_diff_state = diff_state;
	return obj;
}
