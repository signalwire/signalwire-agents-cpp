// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
#include "signalwire/skills/skills_http.hpp"

#include "httplib.h"

#include <utility>

namespace signalwire {
namespace skills {

namespace {

// Split a URL of the form `scheme://host[:port]/path?query` into its
// (host_with_scheme_and_port, path_with_query) components. We hand the
// host portion to httplib::Client and keep the path+query for the call.
struct ParsedUrl {
    std::string host;     // e.g. "http://127.0.0.1:1234"
    std::string path;     // e.g. "/api?q=hi" — never empty (defaults to "/")
    bool ok = false;
    std::string error;
};

ParsedUrl parse_url(const std::string& url) {
    ParsedUrl out;
    auto pos = url.find("://");
    if (pos == std::string::npos) {
        out.error = "URL is missing scheme: " + url;
        return out;
    }
    auto rest_start = pos + 3;
    auto path_start = url.find('/', rest_start);
    if (path_start == std::string::npos) {
        out.host = url;
        out.path = "/";
    } else {
        out.host = url.substr(0, path_start);
        out.path = url.substr(path_start);
    }
    if (out.path.empty()) out.path = "/";
    out.ok = true;
    return out;
}

httplib::Headers make_headers(const std::map<std::string, std::string>& m) {
    httplib::Headers hdrs;
    for (const auto& [k, v] : m) hdrs.emplace(k, v);
    return hdrs;
}

}  // namespace

SkillHttpResponse http_get(const std::string& url,
                           const std::map<std::string, std::string>& headers,
                           int timeout_seconds) {
    SkillHttpResponse r;
    auto p = parse_url(url);
    if (!p.ok) {
        r.error = p.error;
        return r;
    }
    httplib::Client cli(p.host);
    cli.set_connection_timeout(timeout_seconds, 0);
    cli.set_read_timeout(timeout_seconds, 0);

    auto res = cli.Get(p.path, make_headers(headers));
    if (!res) {
        r.error = "HTTP GET failed for " + url;
        return r;
    }
    r.status = res->status;
    r.body = res->body;
    return r;
}

SkillHttpResponse http_post(const std::string& url,
                            const std::string& body,
                            const std::string& content_type,
                            const std::map<std::string, std::string>& headers,
                            int timeout_seconds) {
    SkillHttpResponse r;
    auto p = parse_url(url);
    if (!p.ok) {
        r.error = p.error;
        return r;
    }
    httplib::Client cli(p.host);
    cli.set_connection_timeout(timeout_seconds, 0);
    cli.set_read_timeout(timeout_seconds, 0);

    auto res = cli.Post(p.path, make_headers(headers), body, content_type);
    if (!res) {
        r.error = "HTTP POST failed for " + url;
        return r;
    }
    r.status = res->status;
    r.body = res->body;
    return r;
}

}  // namespace skills
}  // namespace signalwire
