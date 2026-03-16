// Copyright (c) 2025 SignalWire
// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <map>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace signalwire {
namespace rest {

using json = nlohmann::json;

/// Error thrown on non-2xx REST API responses
class SignalWireRestError : public std::runtime_error {
public:
    SignalWireRestError(int status, const std::string& message, const std::string& body = "")
        : std::runtime_error(message), status_(status), body_(body) {}
    int status() const { return status_; }
    const std::string& body() const { return body_; }
private:
    int status_;
    std::string body_;
};

/// HTTP client with Basic Auth support using cpp-httplib
class HttpClient {
public:
    HttpClient(const std::string& base_url,
               const std::string& username,
               const std::string& password);

    /// GET request
    json get(const std::string& path,
             const std::map<std::string, std::string>& params = {}) const;

    /// POST request
    json post(const std::string& path,
              const json& body = json::object()) const;

    /// PUT request
    json put(const std::string& path,
             const json& body = json::object()) const;

    /// PATCH request
    json patch(const std::string& path,
               const json& body = json::object()) const;

    /// DELETE request
    json del(const std::string& path) const;

    /// Set additional default headers
    void set_header(const std::string& key, const std::string& value);

    /// Set request timeout in seconds
    void set_timeout(int seconds);

    const std::string& base_url() const { return base_url_; }

private:
    json handle_response(int status, const std::string& body) const;
    std::string build_query_string(const std::map<std::string, std::string>& params) const;

    std::string base_url_;
    std::string auth_header_;
    std::map<std::string, std::string> headers_;
    int timeout_ = 30;
};

/// Generic CRUD resource for REST API namespaces
class CrudResource {
public:
    CrudResource(const HttpClient& client, const std::string& base_path);

    json list(const std::map<std::string, std::string>& params = {}) const;
    json create(const json& data) const;
    json get(const std::string& id) const;
    json update(const std::string& id, const json& data) const;
    json del(const std::string& id) const;

protected:
    const HttpClient& client_;
    std::string base_path_;
};

} // namespace rest
} // namespace signalwire
