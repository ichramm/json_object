/*!
 * \file Regressions.cpp
 * \author ichramm
 *
 * \date 2017-08-20
 */

#include <fstream>
#include <gtest/gtest.h>

#include <json.hpp>

TEST(JSON_Pointer, Sample1) {
    std::ifstream stream("test/samples/1.glossary.json");
    Json value = json::parse(stream);

    ASSERT_TRUE(value.is_object());
    ASSERT_EQ(1, value.size());
    ASSERT_EQ(1, value[json::Pointer("")].size());
    ASSERT_EQ(1, value[json::Pointer("/")].size());
    ASSERT_EQ(2, value[json::Pointer("/glossary")].size());
    ASSERT_EQ(2, value[json::Pointer("/glossary/GlossDiv")].size());
    ASSERT_EQ(1, value[json::Pointer("/glossary/GlossDiv/GlossList")].size());
    ASSERT_EQ(7, value[json::Pointer("/glossary/GlossDiv/GlossList/GlossEntry")].size());
    ASSERT_EQ(2, value[json::Pointer("/glossary/GlossDiv/GlossList/GlossEntry/GlossDef")].size());
    ASSERT_EQ(2, value[json::Pointer("/glossary/GlossDiv/GlossList/GlossEntry/GlossDef/GlossSeeAlso")].size());
    ASSERT_EQ("GML", value[json::Pointer("/glossary/GlossDiv/GlossList/GlossEntry/GlossDef/GlossSeeAlso/0")].to_string());
    ASSERT_EQ("XML", value[json::Pointer("/glossary/GlossDiv/GlossList/GlossEntry/GlossDef/GlossSeeAlso/1")].to_string());

    ASSERT_TRUE(value.exists(json::Pointer("/glossary")));
    ASSERT_TRUE(value.exists(json::Pointer("/glossary/GlossDiv/GlossList/GlossEntry/GlossDef/GlossSeeAlso")));
    ASSERT_TRUE(value.exists(json::Pointer("/glossary/GlossDiv/GlossList/GlossEntry/GlossDef/GlossSeeAlso/1")));
}

TEST(JSON_Pointer, Sample2) {
    std::ifstream stream("test/samples/2.web_app.json");
    Json value = json::parse(stream);

    ASSERT_EQ("org.cofax.cds.FileServlet",
              value[json::Pointer("/web-app/servlet/servlet-name=fileServlet")]["servlet-class"].to_string());

    ASSERT_EQ(&value["web-app"]["servlet"][0], &value[json::Pointer("/web-app/servlet/0")]);
    ASSERT_EQ(&value["web-app"]["servlet"][1], &value[json::Pointer("/web-app/servlet/1")]);

    ASSERT_EQ(&value["web-app"][json::Pointer("/servlet/0")],
              &value[json::Pointer("/web-app/servlet/0")]);
    ASSERT_EQ(&value["web-app"][json::Pointer("/servlet/1")],
              &value[json::Pointer("/web-app/servlet/1")]);
}

TEST(JSON_Pointer, Tilde) {
    Json json;
    json["root"]["hello~world"].push_back("First");
    json["root"]["hello~world"].push_back("Second");
    ASSERT_THROW(json[json::Pointer("/root/hello~world/0")], json::parse_error);
    ASSERT_EQ("First", json[json::Pointer("/root/hello~0world/0")].to_string());
    ASSERT_EQ("Second", json[json::Pointer("/root/hello~0world/1")].to_string());

    ASSERT_TRUE(json.exists(json::Pointer("/root/hello~0world/0")));
}

TEST(JSON_Pointer, TildeTwice) {
    Json json;
    json["root"]["hello~world"]["goodbye~world"].push_back("First");
    json["root"]["hello~world"]["goodbye~world"].push_back("Second");
    ASSERT_THROW(json[json::Pointer("/root/hello~world/goodbye~world/0")], json::parse_error);
    ASSERT_THROW(json[json::Pointer("/root/hello~0world/goodbye~world/0")], json::parse_error);
    ASSERT_EQ("First", json[json::Pointer("/root/hello~0world/goodbye~0world/0")].to_string());
    ASSERT_EQ("Second", json[json::Pointer("/root/hello~0world/goodbye~0world/1")].to_string());
}

TEST(JSON_Pointer, Slash) {
    Json json;
    json["root"]["hello/world"].push_back("First");
    json["root"]["hello/world"].push_back("Second");
    ASSERT_THROW(json[json::Pointer("/root/hello/world/0")], json::key_not_found);
    ASSERT_EQ("First", json[json::Pointer("/root/hello~1world/0")].to_string());
    ASSERT_EQ("Second", json[json::Pointer("/root/hello~1world/1")].to_string());

    ASSERT_FALSE(json.exists(json::Pointer("/root/hello/world/0")));
    ASSERT_TRUE(json.exists(json::Pointer("/root/hello~1world/0")));
}

TEST(JSON_Pointer, SlashTwice) {
    Json json;
    json["root"]["hello/world"]["goodbye/world"].push_back("First");
    json["root"]["hello/world"]["goodbye/world"].push_back("Second");
    ASSERT_THROW(json[json::Pointer("/root/hello/world/goodbye~world/0")], json::parse_error);
    ASSERT_THROW(json[json::Pointer("/root/hello~1world/goodbye/world/0")], json::key_not_found);
    ASSERT_EQ("First", json[json::Pointer("/root/hello~1world/goodbye~1world/0")].to_string());
    ASSERT_EQ("Second", json[json::Pointer("/root/hello~1world/goodbye~1world/1")].to_string());
}

TEST(JSON_Pointer, Mixed) {
    Json json;
    json["root"]["hello/world"]["goodbye~world"].push_back("First");
    json["root"]["hello/world"]["goodbye~world"].push_back("Second");
    ASSERT_THROW(json[json::Pointer("/root/hello/world/goodbye~world/0")], json::parse_error);
    ASSERT_THROW(json[json::Pointer("/root/hello/world/goodbye~0world/0")], json::key_not_found);
    ASSERT_EQ("First", json[json::Pointer("/root/hello~1world/goodbye~0world/0")].to_string());
    ASSERT_EQ("Second", json[json::Pointer("/root/hello~1world/goodbye~0world/1")].to_string());
}

TEST(JSON_Pointer, WithPropertyInObject) {
    Json json;
    json["name"] = "John Doe";
    json["age"] = 30;
    json["role"] = "Developer";
    json["address"]["number"] = 125;
    json["address"]["street"] = "Maple Avenue";
    json["address"]["city"] = "San Pedro";
    json["address"]["state"] = "California";
    json["address"]["zip_code"] = 90731;
    json["skills"].push_back("C++");
    json["skills"].push_back("Linux");
    json["skills"].push_back("JSON");

    ASSERT_EQ(125, json[json::Pointer("/street=Maple Avenue")]["number"].to_int());
    ASSERT_TRUE(json.exists(json::Pointer("/street=Maple Avenue")));
    ASSERT_TRUE(json.exists(json::Pointer("/street=Maple Avenue/number")));

    ASSERT_FALSE(json.exists(json::Pointer("/UNKNOWN/number")));
    ASSERT_FALSE(json.exists(json::Pointer("/street=UNKNOWN/number")));
    ASSERT_FALSE(json.exists(json::Pointer("/street=Maple Avenue/UNKNOWN")));
}

TEST(JSON_Pointer, WithPropertyInObjectMulti) {
    Json json;
    json["admin"]["name"]="John Doe";
    json["admin"]["age"]=30;
    json["supervisor"]["name"]="Jane Doe";
    json["supervisor"]["age"]=29;
    json["agent"]["name"]="Margaret";
    json["agent"]["age"]=28;

    ASSERT_EQ(30, json[json::Pointer("/name=John Doe")]["age"].to_int());
    ASSERT_EQ(29, json[json::Pointer("/name=Jane Doe")]["age"].to_int());
    ASSERT_EQ(28, json[json::Pointer("/name=Margaret")]["age"].to_int());

    ASSERT_TRUE(json.exists(json::Pointer("/name=John Doe/age")));
}

TEST(JSON_Pointer, WithPropertyInArray) {
    Json json, elem;

    elem["name"] = "John Doe";
    elem["gender"] = "male";
    elem["age"] = 30;
    json["people"].push_back(elem);

    elem["name"] = "Jane Doe";
    elem["gender"] = "female";
    elem["age"] = 29;
    json["people"].push_back(elem);

    elem["name"] = "Margaret";
    elem["gender"] = "female";
    elem["age"] = 29;
    json["people"].push_back(elem);

    ASSERT_EQ("John Doe", json[json::Pointer("/people/gender=male")]["name"].to_string());
    ASSERT_EQ("Jane Doe", json[json::Pointer("/people/gender=female")]["name"].to_string());
}
