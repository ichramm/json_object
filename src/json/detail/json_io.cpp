/*!
 * \file json_io.cpp
 * \author ichramm
 *
 * \date January 20, 2014, 11:33 PM
 *
 * json_object Version 2 - Parser, Printer
 */
#include "json_io.hpp"

#include "json/errors.hpp"

#include "libjson/json.h"

namespace json {
namespace detail {

class IO::parser_context {
public:
    bool                top_level;
    int                 err_code_;
    std::vector<Value*> stack_;
    json_config         config_;
    json_parser         parser_;
    std::string         key_;

public:
    parser_context(Value& obj)
        : top_level(true) {
        stack_.reserve(32);
        stack_.push_back(&obj);

        memset(&config_, 0, sizeof(config_));
        config_.max_nesting = 1024;
        config_.max_data = 1 << 27;
        config_.allow_c_comments = 1;

        err_code_ = json_parser_init(&parser_, &config_, &parser_callback, this);
    }

    ~parser_context() {
        json_parser_free(&parser_);
    }

    int err_code() const {
        return err_code_;
    }

    int parse(const std::string& str, uint32_t& line, uint32_t& column) {
        if (err_code_ != 0) {
            return err_code_;
        }

        uint32_t processed = 0;
        err_code_ = json_parser_string(&parser_,
                                       str.c_str(),
                                       static_cast<uint32_t>(str.size()),
                                       &processed);
        if (err_code_ || !json_parser_is_done(&parser_)) {
            line = 1;
            column = 1;
            for (uint32_t i = 0; i < processed; ++i) {
                if (str[i] == '\n') {
                    ++line;
                    column = 1;
                } else {
                    ++column;
                }
            }
            err_code_ = err_code_ ? err_code_ : JSON_ERROR_UTF8 + 1;
        }

        return err_code_;
    }


    int parse(std::istream& stream, uint32_t &line, uint32_t &column) {
        if (err_code_ != 0) {
            return err_code_;
        }

        char buffer[4096];

        line = 1;
        column = 1;
        for ( ; ; ) {
            stream.read(buffer, sizeof(buffer));
            std::streamsize read = stream.gcount();
            if (read <= 0) {
                break;
            }

            uint32_t processed = 0;
            err_code_ = json_parser_string(&parser_,
                                           buffer,
                                           static_cast<uint32_t>(read),
                                           &processed);

            // check for early return
            if (!err_code_ && json_parser_is_done(&parser_)) {
                return err_code_;
            }

            for (uint32_t i = 0; i < processed; ++i) {
                if (buffer[i] == '\n') {
                    ++line;
                    column = 1;
                } else {
                    ++column;
                }
            }

            if (err_code_) {
                break;
            }
        }

        if (err_code_ || !json_parser_is_done(&parser_)) {
            err_code_ = err_code_ ? err_code_ : JSON_ERROR_UTF8 + 1;
        }

        return err_code_;
    }

private:

    Value& set_json_value(Value& obj, const std::string& key, const Value& val) {
        switch (obj.type()) {
            case Type::Object:
                return (obj.value_.map_val[key] = val);
            case Type::Array:
                obj.value_.arr_val.push_back(val);
                return obj.value_.arr_val.back();
            default:
                throw std::runtime_error("Unexpected json type");
        }
    }

    bool pseudo_parse(const std::string& str, Value& json) {
        if (str == "null") {
            json = Value::Null;
            return true;
        }
        if (str == "true") {
            json = true;
            return true;
        }
        if (str == "false") {
            json = false;
            return true;
        }
        if (str[0] == '\"' && str[str.size() - 1] == '\"') {
            json = str.substr(1, str.size() - 2);
            return true;
        }
        try {
            size_t idx;
            json = detail::stoi64(str, &idx);
            if (str[idx]) { // still data in the string, a double maybe?
                json = detail::stod(str, &idx);
            }
            return !str[idx];
        }
        catch (const std::invalid_argument&) {
            while (false); // avoid compiler warnings, this exception is expected and must be ignored
        }
        return false;
    }

    int parser_callback_impl(int type, const char *data, unsigned int length) {
        Value *obj = stack_.back();

        switch (type) {
            case JSON_OBJECT_BEGIN:
                if (top_level) {
                    top_level = false;
                    *obj = Value();
                } else {
                    stack_.push_back(&set_json_value(*obj, key_, Value()));
                }
                break;
            case JSON_ARRAY_BEGIN:
                if (top_level) {
                    top_level = false;
                    *obj = Value::array_type();
                } else {
                    stack_.push_back(&set_json_value(*obj, key_, Value::array_type()));
                }
                break;
            case JSON_OBJECT_END:	//one level back
            case JSON_ARRAY_END:
                stack_.pop_back();
                break;
            case JSON_KEY:
                key_ = std::string(data, length);
                break;
            case JSON_TRUE:
                set_json_value(*obj, key_, true);
                break;
            case JSON_FALSE:
                set_json_value(*obj, key_, false);
                break;
            case JSON_NULL:
                set_json_value(*obj, key_, Value::Null);
                break;
            case JSON_STRING:
                set_json_value(*obj, key_, std::string(data, length));
                break;
            case JSON_INT:
                set_json_value(*obj, key_, detail::stoi64(data));
                break;
            case JSON_FLOAT:
                set_json_value(*obj, key_, detail::stod(data));
                break;
            default:
                return -1;
        }

        return 0;
    }

    static int parser_callback(void *userdata, int type, const char *data, uint32_t length) {
        return static_cast<parser_context*>(userdata)->parser_callback_impl(type, data, length);
    }
};

class IO::writer_context {
    const Value& obj;
    json_printer printer_;
    std::ostream *stream_;
public:
    writer_context(const Value& obj)
        : obj(obj) {
        json_print_init(&printer_, &printer_callback, this);
    }

    ~writer_context() {
        json_print_free(&printer_);
    }

    void print(std::ostream& os, bool pretty) {
        stream_ = &os;
        deep_print(obj, pretty ? json_print_pretty : json_print_raw);
    }

    static int printer_callback(void* userdata, const char* data, uint32_t length) {
        static_cast<writer_context*>(userdata)->stream_->write(data, length);
        return 0;
    }

    void deep_print(const Value& obj,
                    int(*print_function)(json_printer*, int, const char *, uint32_t)) {
        switch (obj.type_) {
            case Type::Null: {
                print_function(&printer_, JSON_NULL, nullptr, 0);
                break;
            }
            case Type::Boolean: {
                print_function(&printer_, obj.value_.bool_val ? JSON_TRUE : JSON_FALSE, nullptr, 0);
                break;
            }
            case Type::Integer: {
                std::string str = detail::to_string(obj.value_.int_val);
                print_function(&printer_, JSON_INT, str.c_str(), static_cast<uint32_t>(str.size()));
                break;
            }
            case Type::Double:{
                std::string str = detail::to_string(obj.value_.dbl_val);
                print_function(&printer_, JSON_FLOAT, str.c_str(), static_cast<uint32_t>(str.size()));
                break;
            }
            case Type::String: {
                print_function(&printer_, JSON_STRING, obj.value_.str_val.c_str(), static_cast<uint32_t>(obj.value_.str_val.size()));
                break;
            }
            case Type::Array: {
                print_function(&printer_, JSON_ARRAY_BEGIN, nullptr, 0);
                Value::const_array_iterator it = obj.value_.arr_val.begin(), end = obj.value_.arr_val.end();
                for (; it != end; ++it) {
                    deep_print(*it, print_function);
                }
                print_function(&printer_, JSON_ARRAY_END, nullptr, 0);
                break;
            }
            case Type::Object: {
                print_function(&printer_, JSON_OBJECT_BEGIN, nullptr, 0);
                Value::const_iterator it = obj.value_.map_val.begin(), end = obj.value_.map_val.end();
                for (; it != end; ++it) {
                    print_function(&printer_, JSON_KEY, it->first.c_str(), static_cast<uint32_t>(it->first.length()));
                    deep_print(it->second, print_function);
                }
                print_function(&printer_, JSON_OBJECT_END, nullptr, 0);
                break;
            }
        }
    }
};


static const char *error2string(int errcode) {
    static const int MAX_ERROR = JSON_ERROR_UTF8 + 1;
    static const char *error_strings[] = {
        "success",
        "out of memory",
        "bad character (character < 32, except space newline tab)",
        "trying to pop more object/array than pushed on the stack",
        "trying to pop wrong type of mode. popping array in object mode, vice versa",
        "reach nesting limit on stack",
        "reach data limit on buffer",
        "comment are not allowed with current configuration",
        "unexpected char in the current parser context",
        "unicode low surrogate missing after high surrogate",
        "unicode low surrogate missing without previous high surrogate",
        "found a comma not in structure (array/object)",
        "error in a callback (callback returns error)",
        "utf8 stream is invalid",
        "incomplete json" //
    };

    return errcode <= MAX_ERROR ?  error_strings[errcode] : "unknown error";
}

void IO::parse(Value& target, const std::string& spec) {
    uint32_t line = 0, column = 0;
    IO::parser_context context(target);
    if (context.parse(spec, line, column) != 0) {
        throw parse_error(error2string(context.err_code()), line, column);
    }
}

void IO::parse(Value &target, std::istream& stream) {
    uint32_t line = 0, column = 0;
    IO::parser_context context(target);
    if (context.parse(stream, line, column) != 0) {
        throw parse_error(error2string(context.err_code()), line, column);
    }
}

std::ostream& IO::print(const Value& obj, std::ostream& stream, bool pretty) {
    writer_context context(obj);
    context.print(stream, pretty);
    return stream;
}

} // namespace detail
} // namespace json
