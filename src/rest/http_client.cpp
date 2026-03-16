// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
#include "signalwire/rest/http_client.hpp"
#include "signalwire/common.hpp"
#include "httplib.h"

namespace signalwire {
namespace rest {

HttpClient::HttpClient(const std::string& base_url,
                       const std::string& username,
                       const std::string& password)
    : base_url_(base_url) {
    auth_header_ = "Basic " + signalwire::base64_encode(username + ":" + password);
    headers_["Content-Type"] = "application/json";
    headers_["Accept"] = "application/json";
}

void HttpClient::set_header(const std::string& key, const std::string& value) {
    headers_[key] = value;
}

void HttpClient::set_timeout(int seconds) {
    timeout_ = seconds;
}

std::string HttpClient::build_query_string(const std::map<std::string, std::string>& params) const {
    if (params.empty()) return "";
    std::string qs = "?";
    bool first = true;
    for (const auto& [k, v] : params) {
        if (!first) qs += "&";
        qs += signalwire::url_encode(k) + "=" + signalwire::url_encode(v);
        first = false;
    }
    return qs;
}

json HttpClient::handle_response(int status, const std::string& body) const {
    if (status == 204 || body.empty()) {
        return json::object();
    }

    if (status >= 200 && status < 300) {
        try {
            return json::parse(body);
        } catch (...) {
            return json::object({{"raw", body}});
        }
    }

    std::string msg = "HTTP " + std::to_string(status);
    try {
        json err = json::parse(body);
        if (err.contains("message")) msg += ": " + err["message"].get<std::string>();
        else if (err.contains("error")) msg += ": " + err["error"].get<std::string>();
    } catch (...) {
        if (!body.empty()) msg += ": " + body;
    }

    throw SignalWireRestError(status, msg, body);
}

// Helper to extract scheme and host from base_url
static std::pair<std::string, std::string> parse_url(const std::string& base_url) {
    std::string scheme = "http";
    std::string host = base_url;
    auto pos = host.find("://");
    if (pos != std::string::npos) {
        scheme = host.substr(0, pos);
        host = host.substr(pos + 3);
    }
    if (!host.empty() && host.back() == '/') host.pop_back();
    return {scheme, host};
}

static httplib::Headers make_headers(const std::string& auth,
                                      const std::map<std::string, std::string>& extra) {
    httplib::Headers hdrs;
    hdrs.emplace("Authorization", auth);
    for (const auto& [k, v] : extra) hdrs.emplace(k, v);
    return hdrs;
}

json HttpClient::get(const std::string& path,
                     const std::map<std::string, std::string>& params) const {
    auto [scheme, host] = parse_url(base_url_);
    std::string full_path = path + build_query_string(params);
    auto hdrs = make_headers(auth_header_, headers_);

    httplib::Client cli(scheme + "://" + host);
    cli.set_connection_timeout(timeout_, 0);
    cli.set_read_timeout(timeout_, 0);

    auto res = cli.Get(full_path, hdrs);
    if (!res) throw SignalWireRestError(0, "Connection failed to " + host);
    return handle_response(res->status, res->body);
}

json HttpClient::post(const std::string& path, const json& body) const {
    auto [scheme, host] = parse_url(base_url_);
    auto hdrs = make_headers(auth_header_, headers_);
    std::string body_str = body.dump();

    httplib::Client cli(scheme + "://" + host);
    cli.set_connection_timeout(timeout_, 0);
    cli.set_read_timeout(timeout_, 0);

    auto res = cli.Post(path, hdrs, body_str, "application/json");
    if (!res) throw SignalWireRestError(0, "Connection failed");
    return handle_response(res->status, res->body);
}

json HttpClient::put(const std::string& path, const json& body) const {
    auto [scheme, host] = parse_url(base_url_);
    auto hdrs = make_headers(auth_header_, headers_);
    std::string body_str = body.dump();

    httplib::Client cli(scheme + "://" + host);
    cli.set_connection_timeout(timeout_, 0);

    auto res = cli.Put(path, hdrs, body_str, "application/json");
    if (!res) throw SignalWireRestError(0, "Connection failed");
    return handle_response(res->status, res->body);
}

json HttpClient::patch(const std::string& path, const json& body) const {
    auto [scheme, host] = parse_url(base_url_);
    auto hdrs = make_headers(auth_header_, headers_);
    std::string body_str = body.dump();

    httplib::Client cli(scheme + "://" + host);
    cli.set_connection_timeout(timeout_, 0);

    auto res = cli.Patch(path, hdrs, body_str, "application/json");
    if (!res) throw SignalWireRestError(0, "Connection failed");
    return handle_response(res->status, res->body);
}

json HttpClient::del(const std::string& path) const {
    auto [scheme, host] = parse_url(base_url_);
    auto hdrs = make_headers(auth_header_, headers_);

    httplib::Client cli(scheme + "://" + host);
    cli.set_connection_timeout(timeout_, 0);

    auto res = cli.Delete(path, hdrs);
    if (!res) throw SignalWireRestError(0, "Connection failed");
    return handle_response(res->status, res->body);
}

// ============================================================================
// CrudResource
// ============================================================================

CrudResource::CrudResource(const HttpClient& client, const std::string& base_path)
    : client_(client), base_path_(base_path) {}

json CrudResource::list(const std::map<std::string, std::string>& params) const {
    return client_.get(base_path_, params);
}

json CrudResource::create(const json& data) const {
    return client_.post(base_path_, data);
}

json CrudResource::get(const std::string& id) const {
    return client_.get(base_path_ + "/" + id);
}

json CrudResource::update(const std::string& id, const json& data) const {
    return client_.put(base_path_ + "/" + id, data);
}

json CrudResource::del(const std::string& id) const {
    return client_.del(base_path_ + "/" + id);
}

} // namespace rest
} // namespace signalwire
