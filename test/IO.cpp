/*!
 * \file IO.cpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#include <fstream>
#include <gtest/gtest.h>

#include <json.hpp>

TEST(JSON_Parser, Parse_Inner) {
    Json obj;
    obj["hello"] = "world";
    obj["test"].parse("{\"bye\":\"world\"}");
    ASSERT_TRUE(obj["test"].is_object());
    ASSERT_TRUE(obj["test"]["bye"].is_string());
    ASSERT_EQ("world", obj["test"]["bye"].to_string());
}

TEST(JSON_Parser, Int_Array) {
    std::string str_input = "[10,20,30,40]";

    for (int i = 0; i < 2; ++i) {
        Json value;
        if (i == 0) {
            value = Json(json::option_parse, str_input);
        } else {
            std::istringstream iss(str_input);
            iss >> value;
        }

        ASSERT_TRUE(value.is_array());
        ASSERT_EQ(4, value.size());
        ASSERT_EQ(10, value[0].to_int());
        ASSERT_EQ(20, value[1].to_int());
        ASSERT_EQ(30, value[2].to_int());
        ASSERT_EQ(40, value[3].to_int());

        std::string str_output = value.stringify();
        ASSERT_EQ(str_input, str_output);
        std::ostringstream oss;
        oss << value;
        ASSERT_EQ(str_input, oss.str());
    }
}

TEST(JSON_Parser, Object_Of_Array_Of_Object) {
    std::string str_input = "{\"data\":[{\"age\":10},{\"age\":20},{\"age\":30},{\"age\":40}]}";

    for (int i = 0; i < 2; ++i) {
        Json value;
        if (i == 0) {
            value = Json(json::option_parse, str_input);
        } else {
            std::istringstream iss(str_input);
            iss >> value;
        }

        ASSERT_TRUE(value["data"].is_array());
        ASSERT_EQ(4, value["data"].size());
        ASSERT_EQ(10, value["data"][0]["age"].to_int());
        ASSERT_EQ(20, value["data"][1]["age"].to_int());
        ASSERT_EQ(30, value["data"][2]["age"].to_int());

        std::string str_output = value.stringify();
        ASSERT_EQ(str_input, str_output);
        std::ostringstream oss;
        oss << value;
        ASSERT_EQ(str_input, oss.str());
        ASSERT_EQ(40, value["data"][3]["age"].to_int());
    }
}

TEST(JSON_Parser, Parse_Char_Pointer_Short) {
    std::string str_input = "{\"data\":[{\"age\":10},{\"age\":20},{\"age\":30},{\"age\":40}]}";

    Json value = json::parse(&str_input[0], str_input.size());

    ASSERT_TRUE(value["data"].is_array());
    ASSERT_EQ(4, value["data"].size());
    ASSERT_EQ(10, value["data"][0]["age"].to_int());
    ASSERT_EQ(20, value["data"][1]["age"].to_int());
    ASSERT_EQ(30, value["data"][2]["age"].to_int());

    std::string str_output = value.stringify();
    ASSERT_EQ(str_input, str_output);
    std::ostringstream oss;
    oss << value;
    ASSERT_EQ(str_input, oss.str());
    ASSERT_EQ(40, value["data"][3]["age"].to_int());
}

TEST(JSON_Parser, Object_Array) {
    std::string str_input = "[{\"age\":10}, {\"age\":20}, {\"age\":30}, {\"age\":40}]";
    for (int i = 0; i < 2; ++i) {
        Json value;
        if (i == 0) {
            value = Json(json::option_parse, str_input);
        } else {
            std::istringstream iss(str_input);
            iss >> value;
        }

        ASSERT_TRUE(value.is_array());
        ASSERT_EQ(4, value.size());
        ASSERT_EQ(10, value[0]["age"].to_int());
        ASSERT_EQ(20, value[1]["age"].to_int());
        ASSERT_EQ(30, value[2]["age"].to_int());
        ASSERT_EQ(40, value[3]["age"].to_int());
    }
}

TEST(JSON_Parser, Heterogeneous_Array_Simple) {
    std::string str_input = "[10, 20, 30, 40, true, false, 2.5, \"String\" ]";
    for (int i = 0; i < 2; ++i) {
        Json value;
        if (i == 0) {
            value = Json(json::option_parse, str_input);
        } else {
            std::istringstream iss(str_input);
            iss >> value;
        }

        ASSERT_TRUE(value.is_array());
        ASSERT_EQ(8, value.size());
        ASSERT_EQ(10, value[0].to_int());
        ASSERT_EQ(20, value[1].to_int());
        ASSERT_EQ(30, value[2].to_int());
        ASSERT_EQ(40, value[3].to_int());
        ASSERT_TRUE(value[4].to_bool());
        ASSERT_FALSE(value[5].to_bool());
        ASSERT_DOUBLE_EQ(2.5, value[6].to_double());
        ASSERT_STREQ("String", value[7].to_string().c_str());
    }
}

TEST(JSON_Parser, Heterogeneous_Array_With_Object_At_End) {
    std::string str_input = "[10,2.5,{\"age\":40}]";
    for (int i = 0; i < 2; ++i) {
        Json value;
        if (i == 0) {
            value = Json(json::option_parse, str_input);
        } else {
            std::istringstream iss(str_input);
            iss >> value;
        }

        ASSERT_TRUE(value.is_array());
        ASSERT_EQ(3, value.size());
        ASSERT_TRUE(value[0].is_integer());
        ASSERT_TRUE(value[1].is_double());
        ASSERT_TRUE(value[2].is_object());
        ASSERT_EQ(1, value[2].size());

        std::string str_output = value.stringify();
        ASSERT_EQ(str_input, str_output);
        std::ostringstream oss;
        oss << value;
        ASSERT_EQ(str_input, oss.str());
    }
}

TEST(JSON_Parser, Heterogeneous_Array_With_Object_At_Begin) {
    std::string str_input = "[{\"age\":40,\"name\":\"Miguel\"},100,true,null]";
    for (int i = 0; i < 2; ++i) {
        Json value;
        if (i == 0) {
            value = Json(json::option_parse, str_input);
        } else {
            std::istringstream iss(str_input);
            iss >> value;
        }

        ASSERT_TRUE(value.is_array());
        ASSERT_EQ(4, value.size());
        ASSERT_TRUE(value[0].is_object());
        ASSERT_TRUE(value[1].is_integer());
        ASSERT_TRUE(value[2].is_boolean());
        ASSERT_TRUE(value[3].is_null());

        std::string str_output = value.stringify();
        ASSERT_EQ(str_input, str_output);
        std::ostringstream oss;
        oss << value;
        ASSERT_EQ(str_input, oss.str());
    }
}


// we used to have a problem with inner quotes so here is a test for it
TEST(JSON_Parser, Inner_Quotes) {
    std::string str_input = "{\"age\":40,\"name\":\"\\\"Miguel Angel Gonzalez Zuniga\\\"\"}";
    for (int i = 0; i < 2; ++i) {
        Json value;
        if (i == 0) {
            value = Json(json::option_parse, str_input);
        } else {
            std::istringstream iss(str_input);
            iss >> value;
        }

        ASSERT_TRUE(value.is_object());
        ASSERT_TRUE(value.exists("name"));
        ASSERT_TRUE(value["name"].is_string());
        ASSERT_TRUE(value.exists("age"));
        ASSERT_TRUE(value["age"].is_integer());
        ASSERT_EQ(40, value["age"].to_int());

        std::string str_output = value.stringify();
        ASSERT_EQ(str_input, str_output);
        std::ostringstream oss;
        oss << value;
        ASSERT_EQ(str_input, oss.str());
    }
}

TEST(JSON_Parser, Sample1) {
    std::ifstream stream("test/samples/1.glossary.json");
    Json value = json::parse(stream);

    ASSERT_TRUE(value.is_object());
    ASSERT_EQ(1, value.size());
    ASSERT_EQ(2, value["glossary"].size());
    ASSERT_EQ(2, value["glossary"]["GlossDiv"].size());
    ASSERT_EQ(1, value["glossary"]["GlossDiv"]["GlossList"].size());
    ASSERT_EQ(7, value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"].size());
    ASSERT_EQ(2, value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"].size());
    ASSERT_EQ(2, value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"].size());
    ASSERT_EQ("GML", value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"][0].to_string());
    ASSERT_EQ("XML", value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"][1].to_string());
}

TEST(JSON_Parser, Parse_Char_Pointer_Sample1) {
    std::ifstream stream("test/samples/1.glossary.json");
    std::istreambuf_iterator<char> endit;
    std::string str(std::istreambuf_iterator<char>(stream), endit);

    Json value = json::parse(&str[0], str.size());

    ASSERT_TRUE(value.is_object());
    ASSERT_EQ(1, value.size());
    ASSERT_EQ(2, value["glossary"].size());
    ASSERT_EQ(2, value["glossary"]["GlossDiv"].size());
    ASSERT_EQ(1, value["glossary"]["GlossDiv"]["GlossList"].size());
    ASSERT_EQ(7, value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"].size());
    ASSERT_EQ(2, value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"].size());
    ASSERT_EQ(2, value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"].size());
    ASSERT_EQ("GML", value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"][0].to_string());
    ASSERT_EQ("XML", value["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"][1].to_string());
}

TEST(JSON_Parser, Sample1_Bad) {
    {
        std::ifstream stream("test/samples/1.glossary.bad.json");
        ASSERT_THROW(json::parse(stream), json::parse_error);
    }
    {
        std::ifstream stream("test/samples/1.glossary.bad.json");
        try {
            json::parse(stream);
        } catch (const json::parse_error& e) {
            ASSERT_EQ(11, e.line());
            ASSERT_EQ(25, e.column());
        }
    }
}

TEST(JSON_Parser, Sample2) {
    std::ifstream stream("test/samples/2.web_app.json");
    Json value(stream);

    ASSERT_TRUE(value.is_object());
    ASSERT_EQ(1, value.size());
    ASSERT_EQ(3, value["web-app"].size());
    ASSERT_TRUE(value["web-app"]["servlet"].is_array());
    ASSERT_EQ(5, value["web-app"]["servlet"].size());
    ASSERT_TRUE(value["web-app"]["servlet-mapping"].is_object());
    ASSERT_TRUE(value["web-app"]["taglib"].is_object());
}

TEST(JSON_Parser, Sample2_Bad) {
    {
        std::ifstream stream("test/samples/2.web_app.bad.json");
        ASSERT_THROW(json::parse(stream), json::parse_error);
    }
    {
        std::ifstream stream("test/samples/2.web_app.bad.json");
        try {
            json::parse(stream);
        } catch (const json::parse_error& e) {
            ASSERT_EQ(63, e.line());
            ASSERT_EQ(23, e.column());
        }
    }
}
