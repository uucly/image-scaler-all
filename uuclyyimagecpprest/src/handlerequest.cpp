/*#include <cpprest/http_listener.h>
#include <cpprest/json.h>
//#pragma comment(lib, "cpprest_2_10")


#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"

std::map<utility::string_t, utility::string_t> dictionary;

void display_json(
    json::value const& jvalue,
    utility::string_t const& prefix)
{
    std::wcout << prefix << jvalue.serialize() << std::endl;
}

void my_handle_get(http_request request)
{
    std::TRACE(L"\nhandle GET\n");

    auto answer = json::value::object();

    for (auto const& p : dictionary)
    {
        answer[p.first] = json::value::string(p.second);
    }

    display_json(json::value::null(), L"R: ");
    display_json(answer, L"S: ");

    request.reply(status_codes::OK, answer);
}*/